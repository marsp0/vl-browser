#include "runner.h"

#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "test_utils.h"

#include "dom/node.h"
#include "dom/document.h"
#include "dom/element.h"
#include "dom/comment.h"
#include "dom/attribute.h"
#include "dom/text.h"
#include "util/utf8.h"
#include "html/tokenizer.h"

#define MAX_TOKENS 33

typedef enum
{
    STATE_DESCRIPTION,
    STATE_DATA,
    STATE_ERRORS,
    STATE_STATES,
    STATE_DOCTYPE,
    STATE_DOCTYPE_PUBLIC,
    STATE_DOCTYPE_SYSTEM,
    STATE_DOCTYPE_QUIRKS,
    STATE_COMMENT,
    STATE_CHARACTER,
    STATE_END_TAG,
    STATE_START_TAG,
    STATE_START_TAG_SELF_CLOSE,
    STATE_ATTR_NAME,
    STATE_ATTR_VALUE,
    STATE_LAST_START_TAG,
} test_state_e;

static const unsigned char* test_file = NULL;
static FILE* file = NULL;
static bool file_done = false;
static unsigned char file_buffer[4096] = { 0 };
static uint32_t file_buffer_cursor = 0;
static uint32_t file_buffer_size = 0;
static unsigned char prev = 0;

static unsigned char line[2048] = { 0 };
static uint32_t line_cursor = 0;
static uint32_t line_size = 0;
static uint32_t is_eof = false;
static uint32_t line_num = 0;
static uint32_t test_line = 0;
static unsigned char description[2048] = { 0 };

// test data
static unsigned char test_data[2048] = { 0 };
static uint32_t test_data_size = 0;
static hash_str_t last_start_tag = 0;

static html_token_t tokens[50] = { 0 };
static uint32_t tokens_size = 0;
static uint32_t current = 0;

static html_tokenizer_state_e states[10] = { 0 };
static uint32_t states_size = 0;

static int32_t get_char()
{
    if (file_buffer_cursor > 0) { prev = file_buffer[file_buffer_cursor - 1]; }

    if (file_done && file_buffer_cursor == file_buffer_size)
    {
        return -1;
    }

    if (file_buffer_cursor >= file_buffer_size)
    {
        file_buffer_size = (uint32_t)fread(file_buffer, 1, sizeof(file_buffer), file);
        file_buffer_cursor = 0;

        if (file_buffer_size < sizeof(file_buffer)) { file_done = true; }
    }

    int32_t c = file_buffer[file_buffer_cursor++];

    return c;
}

static void read_line()
{
    memset(line, 0, 2048);
    line_size = 0;
    line_cursor = 0;
    int32_t c = 0;

    while (c != '\n')
    {
        c = get_char(file);
        if (c == -1)
        {
            is_eof = true;
            return;
        }

        if (prev == '\\' && c == 'n')
        {
            line[line_size - 1] = '\n';
        }
        else
        {
            line[line_size++] = (unsigned char)c;
        }
    }

    line[--line_size] = '\0';
    line_num++;
}


static void run_tokenizer_test()
{
    tokens_size = 0;
    states_size = 0;
    memset(tokens, 0, sizeof(tokens));
    memset(states, 0, sizeof(states));

    memset(description, 0, 2048);
    memset(test_data, 0, 2048);
    test_data_size = 0;

    // #description
    read_line();
    assert(strncmp("#description", line, 12) == 0);
    test_line = line_num;

    // line containing test description
    read_line();
    memcpy(description, line, line_size);

    // next header
    read_line();
    test_state_e state = STATE_DESCRIPTION;
    bool should_continue = true;

    while (should_continue)
    {
        if (strncmp(line, "#errors", 7) == 0)
        {
            state = STATE_ERRORS;
        }
        else if (strncmp(line, "#data", 5) == 0)
        {
            state = STATE_DATA;
        }
        else if (strncmp(line, "#states", 7) == 0)
        {
            state = STATE_STATES;
        }
        else if (strncmp(line, "#doctype-public", 15) == 0)
        {
            state = STATE_DOCTYPE_PUBLIC;
        }
        else if (strncmp(line, "#doctype-system", 15) == 0)
        {
            state = STATE_DOCTYPE_SYSTEM;
        }
        else if (strncmp(line, "#doctype-quirks", 15) == 0)
        {
            state = STATE_DOCTYPE_QUIRKS;
        }
        else if (strncmp(line, "#doctype", 8) == 0)
        {
            state = STATE_DOCTYPE;
        }
        else if (strncmp(line, "#comment", 8) == 0)
        {
            state = STATE_COMMENT;
        }
        else if (strncmp(line, "#character", 10) == 0)
        {
            state = STATE_CHARACTER;
        }
        else if (strncmp(line, "#end-tag", 8) == 0)
        {
            state = STATE_END_TAG;
        }
        else if (strncmp(line, "#start-tag-self-close", 21) == 0)
        {
            state = STATE_START_TAG_SELF_CLOSE;
        }
        else if (strncmp(line, "#start-tag", 10) == 0)
        {
            state = STATE_START_TAG;
        }
        else if (strncmp(line, "#attr-name", 10) == 0)
        {
            state = STATE_ATTR_NAME;
        }
        else if (strncmp(line, "#attr-value", 11) == 0)
        {
            state = STATE_ATTR_VALUE;
        }
        else if (strncmp(line, "#last-start-tag", 11) == 0)
        {
            state = STATE_LAST_START_TAG;
        }
        else if (strncmp(line, "#end-test", 9) == 0)
        {
            should_continue = false;
        }
        else if (state == STATE_ERRORS)
        {
            // pass - not sure yet what to do with these errors
        }
        else if (state == STATE_STATES)
        {
            if (strncmp(line, "Data state", 10) == 0)       { states[states_size++] = HTML_TOKENIZER_DATA_STATE; }
            if (strncmp(line, "PLAINTEXT state", 15) == 0)  { states[states_size++] = HTML_TOKENIZER_PLAINTEXT_STATE; }
            if (strncmp(line, "RCDATA state", 12) == 0)     { states[states_size++] = HTML_TOKENIZER_RCDATA_STATE; }
            if (strncmp(line, "RAWTEXT state", 13) == 0)    { states[states_size++] = HTML_TOKENIZER_RAWTEXT_STATE; }
            if (strncmp(line, "Script data state", 17) == 0){ states[states_size++] = HTML_TOKENIZER_SCRIPT_DATA_STATE; }
        }
        else if (state == STATE_DATA)
        {
            memcpy(test_data, line, line_size);
            test_data_size = line_size;
        }
        else if (state == STATE_DOCTYPE)
        {
            current = tokens_size;
            tokens_size++;

            html_token_t* token = &tokens[current];
            token->is_valid = true;
            token->type = HTML_DOCTYPE_TOKEN;

            memcpy(token->name, line, line_size);
            token->name_size = line_size;
        }
        else if (state == STATE_DOCTYPE_PUBLIC)
        {
            html_token_t* token = &tokens[current];
            memcpy(token->public_id, line, line_size);
            token->public_id_size = line_size;
        }
        else if (state == STATE_DOCTYPE_SYSTEM)
        {
            html_token_t* token = &tokens[current];
            memcpy(token->system_id, line, line_size);
            token->system_id_size = line_size;
        }
        else if (state == STATE_DOCTYPE_QUIRKS)
        {
            html_token_t* token = &tokens[current];
            if (strncmp(line, "true", 4) == 0)
            {
                token->force_quirks = false;
            }
            else
            {
                token->force_quirks = true;
            }
        }
        else if (state == STATE_COMMENT)
        {
            current = tokens_size;
            tokens_size++;

            html_token_t* token = &tokens[current];
            token->is_valid = true;
            token->type = HTML_COMMENT_TOKEN;

            memcpy(token->data, line, line_size);
            token->data_size = line_size;
        }
        else if (state == STATE_CHARACTER)
        {
            current = tokens_size;
            tokens_size++;

            html_token_t* token = &tokens[current];
            token->is_valid = true;
            token->type = HTML_CHARACTER_TOKEN;

            memcpy(token->data, line, line_size);
            token->data_size = line_size;
        }
        else if (state == STATE_END_TAG)
        {
            current = tokens_size;
            tokens_size++;

            html_token_t* token = &tokens[current];
            token->is_valid = true;
            token->type = HTML_END_TOKEN;
            memcpy(token->name, line, line_size);
            token->name_size = line_size;
        }
        else if (state == STATE_START_TAG)
        {
            current = tokens_size;
            tokens_size++;

            html_token_t* token = &tokens[current];
            token->is_valid = true;
            token->type = HTML_START_TOKEN;
            memcpy(token->name, line, line_size);
            token->name_size = line_size;
        }
        else if (state == STATE_START_TAG_SELF_CLOSE)
        {
            html_token_t* token = &tokens[current];

            if (strncmp(line, "true", 4) == 0)
            {
                token->self_closing = true;
            }
            else
            {
                token->self_closing = false;
            }
        }
        else if (state == STATE_ATTR_NAME)
        {
            html_token_t* token = &tokens[current];
            html_token_attribute_t* attr = &token->attributes[token->attributes_size];

            memcpy(attr->name, line, line_size);
            attr->name_size = line_size;
        }
        else if (state == STATE_ATTR_VALUE)
        {
            html_token_t* token = &tokens[current];
            html_token_attribute_t* attr = &token->attributes[token->attributes_size];

            memcpy(attr->value, line, line_size);
            attr->value_size = line_size;
            token->attributes_size++;
        }
        else if (state == STATE_LAST_START_TAG)
        {
            last_start_tag = hash_str_new(line, line_size);
        }
        else
        {
            printf("unhandled state\n");
            assert(false);
        }

        read_line();
    }

    if (states_size == 0) { states[states_size++] = HTML_TOKENIZER_DATA_STATE; }

    html_token_t tokens_a[MAX_TOKENS] = { 0 };

    for (uint32_t s = 0; s < states_size; s++)
    {
        uint32_t current_token = 0;
        memset(tokens_a, 0, sizeof(tokens_a));
        html_tokenizer_init(test_data, test_data_size, tokens_a, MAX_TOKENS);
        html_tokenizer_set_state(states[s]);
        if (last_start_tag > 0)
        {
            html_tokenizer_set_last_emitted_start_tag(last_start_tag);
        }

        bool run = true;
        while (run)
        {
            html_tokenizer_next();

            uint32_t size = 0;
            for (uint32_t k = 0; k < MAX_TOKENS; k++)
            {
                if (tokens_a[k].is_valid)
                {
                    size++;
                }
            }

            for (uint32_t i = 0; i < size; i++)
            {
                html_token_t a = tokens_a[i];
                html_token_t e = tokens[current_token++];
                if (a.type == HTML_EOF_TOKEN)
                {
                    run = false;
                    break;
                }

                ASSERT_TRUE(a.is_valid);
                ASSERT_EQUAL(a.type, e.type);

                ASSERT_EQUAL(a.name_size, e.name_size);
                if (a.name_size == e.name_size)             { ASSERT_STRING((char)a.name, (char)e.name, a.name_size); }

                ASSERT_EQUAL(a.public_id_size, e.public_id_size);
                if (a.public_id_size == e.public_id_size)   { ASSERT_STRING((char)a.public_id, (char)e.public_id, a.public_id_size); }

                ASSERT_EQUAL(a.system_id_size, e.system_id_size);
                if (a.system_id_size == e.system_id_size)   { ASSERT_STRING((char)a.system_id, (char)e.system_id, a.system_id_size); }

                ASSERT_EQUAL(a.data_size, e.data_size);
                if (a.data_size == e.data_size)             {  ASSERT_STRING((char)a.data, (char)e.data, a.data_size); }

                ASSERT_EQUAL(a.force_quirks, e.force_quirks);
                ASSERT_EQUAL(a.self_closing, e.self_closing);

                ASSERT_EQUAL(a.attributes_size, e.attributes_size);

                for (uint32_t k = 0; k < a.attributes_size; k++)
                {
                    html_token_attribute_t a_attr = a.attributes[k];
                    html_token_attribute_t e_attr = e.attributes[k];

                    ASSERT_EQUAL(a_attr.name_size, e_attr.name_size);
                    if (a_attr.name_size == e_attr.name_size)   { ASSERT_STRING((char)a_attr.name, (char)e_attr.name, a_attr.name_size); }

                    ASSERT_EQUAL(a_attr.value_size, e_attr.value_size);
                    if (a_attr.value_size == e_attr.value_size)   { ASSERT_STRING((char)a_attr.value, (char)e_attr.value, a_attr.value_size); }
                }
            }
        }

        if (!TEST_SUCCEEDED())
        {
            printf("\n========== Test %u ==========\n", test_line);
            printf("FILE: %s\n", test_file);
            printf("TEST: %s\n", description);
            printf("INPUT: %s\n", test_data);
        }

        html_tokenizer_free();
    }
}


void html_tokenizer_test()
{
    const unsigned char* files[] = {
                                    "./test/html/tokenizer/data/debug.data",
                                    "./test/html/tokenizer/data/test1.data",
                                    "./test/html/tokenizer/data/test2.data",
                                    "./test/html/tokenizer/data/test3.data",
                                    "./test/html/tokenizer/data/numericEntities.data",
                                    "./test/html/tokenizer/data/test4.data",
                                    "./test/html/tokenizer/data/namedEntities.data",
                                    "./test/html/tokenizer/data/unicodeChars.data",
                                    "./test/html/tokenizer/data/unicodeCharsProblematic.data",
                                    "./test/html/tokenizer/data/pendingSpecChanges.data",
                                    "./test/html/tokenizer/data/escapeFlag.data",
                                    "./test/html/tokenizer/data/entities.data",
                                    "./test/html/tokenizer/data/test5.data",
                                    };
    uint32_t len = sizeof(files) / sizeof(char*);

    for (uint32_t i = 0; i < len; i++)
    {
        test_line = 0;
        line_num = 0;
        tokens_size = 0;
        file_buffer_cursor = 0;
        file_buffer_size = 0;
        file_done = false;
        is_eof = false;

        file = fopen(files[i], "r");
        if (!file)
        {
            printf("Cannot open file %s\n", files[i]);
            return;
        }

        test_file = files[i];

        while (!is_eof) { TEST_CASE(run_tokenizer_test) }
    }
}