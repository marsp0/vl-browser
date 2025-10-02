#include "runner.h"

#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "test_utils.h"
#include "html/test_tokenizer.h"

#include "dom/node.h"
#include "dom/document.h"
#include "dom/element.h"
#include "dom/comment.h"
#include "dom/attribute.h"
#include "dom/text.h"
#include "util/utf8.h"

typedef enum
{
    STATE_DESCRIPTION,
    STATE_DATA,
    STATE_ERRORS,
    STATE_OUTPUT,
    STATE_STATES
} test_state_e;

static FILE* file = NULL;
static bool file_done = false;
static unsigned char file_buffer[4096] = { 0 };
static uint32_t file_buffer_cursor = 0;
static uint32_t file_buffer_size = 0;
static unsigned char prev = 0;

static unsigned char line[2048] = { 0 };
static uint32_t line_size = 0;
static uint32_t is_eof = false;
static uint32_t line_num = 0;
static uint32_t test_line = 0;

// test data
static unsigned char test_data[2048] = { 0 };
static uint32_t test_data_size = 0;

static html_token_t tokens[50] = { 0 };
static uint32_t tokens_size = 0;
static uint32_t last_element = 0;

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


static void parse_token()
{
    // todo make all this strings static const chars / defines
    if (strncmp("Character", line, 9) == 0)
    {
        uint32_t start = 10;
        uint32_t size = line_size - start;

        html_token_t* token = &tokens[tokens_size++];
        token->is_valid = true;
        token->type = HTML_CHARACTER_TOKEN;
        memcpy(token->data, &line[start], size);
        token->data_size = size;
    }
    else if (strncmp("Comment", line, 7) == 0)
    {
        uint32_t start = 8;
        uint32_t size = line_size - start;

        html_token_t* token = &tokens[tokens_size++];
        token->is_valid = true;
        token->type = HTML_COMMENT_TOKEN;
        memcpy(token->data, &line[start], size);
        token->data_size = size;
    }
    else if (strncmp("StartTag", line, 8) == 0)
    {
        uint32_t name_start = 10;
        uint32_t name_end = name_start;

        for (uint32_t i = name_start; i < line_size; i++)
        {
            if (line[i] == '"') { name_end = i; break; }
        }
        uint32_t name_size = name_end - name_start;

        html_token_t* token = &tokens[tokens_size];
        token->is_valid = true;
        token->type = HTML_START_TOKEN;
        memcpy(token->name, &line[name_start], name_size);
        token->name_size = name_size;
        last_element = tokens_size;
        tokens_size++;

        uint32_t self_close = line_size - name_end;
        if (self_close > 1) { token->self_closing = true; }
    }
    else if (strncmp("EndTag", line, 6) == 0)
    {
        uint32_t start = 8;
        uint32_t size = line_size - 1 - start;

        html_token_t* token = &tokens[tokens_size];
        token->is_valid = true;
        token->type = HTML_END_TOKEN;
        memcpy(token->name, &line[start], size);
        token->name_size = size;
        last_element = tokens_size;
        tokens_size++;
    }
    else if (strncmp("Attr", line, 4) == 0)
    {
        uint32_t name_start = 6;
        uint32_t name_end = name_start;

        for (uint32_t i = name_start; i < line_size; i++)
        {
            if (line[i] == '"' && line[i-1] != '\\') { name_end = i; break; }
        }

        uint32_t val_start = name_end + 3;
        uint32_t val_end = name_end;

        for (uint32_t i = val_start; i < line_size; i++)
        {
            if (line[i] == '"') { val_end = i; break; }
        }

        html_token_t* token = &tokens[last_element];
        html_token_attribute_t* attr = &token->attributes[token->attributes_size++];

        attr->name_size = name_end - name_start;
        uint32_t i = 0;
        while(name_start < name_end)
        {
            if (line[name_start - 1] == '\\' && line[name_start] == '"')
            {
                attr->name[i - 1] = '"';
                attr->name_size--;
            }
            else
            {
                attr->name[i] = line[name_start];
                i++;
            }

            name_start++;
        }
        // memcpy(attr->name, &line[name_start], name_end - name_start);
        // attr->name_size = name_end - name_start;

        memcpy(attr->value, &line[val_start], val_end - val_start);
        attr->value_size = val_end - val_start;
    }
    else if (strncmp("DOCTYPE", line, 7) == 0)
    {
        uint32_t name_start = 9;
        uint32_t name_end = name_start;

        for (uint32_t i = name_start; i < line_size; i++)
        {
            if (line[i] == '"') { name_end = i; break; }
        }
        uint32_t name_size = name_end - name_start;

        // public id
        uint32_t public_start = name_end + 3;
        uint32_t public_end = public_start;

        for (uint32_t i = public_start; i < line_size; i++)
        {
            if (line[i] == '"') { public_end = i; break; }
        }
        uint32_t public_size = public_end - public_start;

        // system id
        uint32_t system_start = public_end + 3;
        uint32_t system_end = system_start;

        for (uint32_t i = system_start; i < line_size; i++)
        {
            if (line[i] == '"') { system_end = i; break; }
        }
        uint32_t system_size = system_end - system_start;

        html_token_t* token = &tokens[tokens_size++];
        token->is_valid = true;
        token->type = HTML_DOCTYPE_TOKEN;

        memcpy(token->name, &line[name_start], name_size);
        token->name_size = name_size;

        memcpy(token->public_id, &line[public_start], public_size);
        token->public_id_size = public_size;

        memcpy(token->system_id, &line[system_start], system_size);
        token->system_id_size = system_size;

        // printf("name: %u %u %u\n", name_start, name_end, name_size);
        // printf("public: %u %u %u\n", public_start, public_end, public_size);
        // printf("system: %u %u %u\n", system_start, system_end, system_size);

        uint32_t quirks_start = system_end + 2;
        uint32_t quirks_size = line_size - quirks_start;
        // printf("quirks: %u %u %u\n", quirks_start, line_size, quirks_size);
        // printf("\"%s\" - %u\n", line, line_size);
        token->force_quirks = false;

        if (quirks_size == 5)
        {
            token->force_quirks = true;
        }
    }
    else
    {
        assert(false);
    }

    assert(tokens_size < 50);
}


static void run_test()
{
    tokens_size = 0;
    states_size = 0;
    memset(tokens, 0, sizeof(tokens));
    memset(states, 0, sizeof(states));

    memset(test_data, 0, 2048);
    test_data_size = 0;

    // #description
    read_line();
    assert(strncmp("#description", line, 12) == 0);
    test_line = line_num;

    // line containing test description
    read_line();

    // next header
    read_line();
    test_state_e state              = STATE_DESCRIPTION;

    while (line_size > 0)
    {
        if (strncmp(line, "#errors", 7) == 0)
        {
            state = STATE_ERRORS;
        }
        else if (strncmp(line, "#data", 5) == 0)
        {
            state = STATE_DATA;
        }
        else if (strncmp(line, "#output", 7) == 0)
        {
            state = STATE_OUTPUT;
        }
        else if (strncmp(line, "#states", 7) == 0)
        {
            state = STATE_STATES;
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
            // printf("\"%s\" - %u - %u\n", test_data, line_size, test_line);
            test_data_size = line_size;
        }
        else if (state == STATE_OUTPUT)
        {
            parse_token();
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
                html_token_t token_a = tokens_a[i];
                html_token_t token_e = tokens[current_token++];
                if (token_a.type == HTML_EOF_TOKEN)
                {
                    run = false;
                    break;
                }

                ASSERT_TOKEN(token_a, token_e);
            }

            if (!TEST_SUCCEEDED())
            {
                printf("\n========== Test %u ==========\n", test_line);
                printf("%s\n", test_data);
            }

            html_tokenizer_free();
        }
    }
}


void html_tokenizer_test()
{
    const unsigned char* files[] = {
                                    // "./test/html/tokenizer/data/debug.data",
                                    "./test/html/tokenizer/data/test1.data",
                                    "./test/html/tokenizer/data/test2.data",
                                    "./test/html/tokenizer/data/test3.data",
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

        while (!is_eof) { TEST_CASE(run_test) }
    }
}