#include "runner.h"

#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "test_utils.h"

#include "css/util.h"
#include "css/tokenizer.h"
#include "util/utf8.h"

typedef enum
{
    STATE_DATA,
    STATE_TYPE,
    STATE_VALUE
} state_e;

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
static state_e state = STATE_DATA;

// test data
static unsigned char test_data[2048] = { 0 };
static uint32_t test_data_size = 0;

static css_token_t tokens[50] = { 0 };
static uint32_t tokens_size = 0;
static uint32_t current = 0;
static unsigned char* type_map_keys[] = { 
                                            "at-keyword-token",
                                            "whitespace-token",
                                            "delim-token",
                                            "ident-token",
                                            "number-token"
                                        };
static css_token_type_e type_map_vals[] = { 
                                            CSS_TOKEN_AT_KEYWORD,
                                            CSS_TOKEN_WHITESPACE,
                                            CSS_TOKEN_DELIM,
                                            CSS_TOKEN_IDENT,
                                            CSS_TOKEN_NUMBER
                                          };

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


static void run_css_tokenizer_test()
{
    current = 0;
    bool first = true;
    tokens_size = 0;
    test_data_size = 0;
    memset(tokens, 0, sizeof(tokens));
    memset(test_data, 0, 2048);

    do
    {
        if (is_eof) { return; }
        read_line();
    } while (strncmp(line, "#data", 5) != 0);

    
    state = STATE_DATA;

    while (true)
    {
        if (strncmp(line, "#data", 5) == 0)
        {
            state = STATE_DATA;
            test_line = line_num;
        }
        else if (strncmp(line, "#token-type", 11) == 0)
        {
            if (!first)
            {
                current++;
            }
            
            state = STATE_TYPE;
            first = false;
        }
        else if (strncmp(line, "#token-value", 12) == 0)
        {
            state = STATE_VALUE;
        }
        else if (strncmp(line, "#end-test", 9) == 0)
        {
            break;
        }
        else if (state == STATE_DATA)
        {
            memcpy(test_data, line, line_size);
            test_data_size = line_size;
        }
        else if (state == STATE_TYPE)
        {
            uint32_t max_size = sizeof(type_map_vals) / sizeof(css_token_type_e);
            css_token_type_e type = CSS_TOKEN_EOF;

            for (uint32_t i = 0; i < max_size; i++)
            {
                if (strcmp(line, type_map_keys[i]) == 0)
                {
                    type = type_map_vals[i];
                    break;
                }
            }

            tokens[current].type = type;
        }
        else if (state == STATE_VALUE)
        {
            if (tokens[current].type == CSS_TOKEN_NUMBER)
            {
                uint32_t repr[32] = { 0 };
                uint32_t repr_i = 0;
                uint32_t cur_line_size = 0;
                while (cur_line_size < line_size)
                {
                    uint32_t cp = 0;
                    int32_t cp_len = -1;
                    cp_len = utf8_decode(line, line_size, cur_line_size, &cp);

                    if (cp_len <= 0) { break; }

                    repr[repr_i] = cp;
                    cur_line_size += (uint32_t)cp_len;
                    repr_i += 1;
                }

                double res = css_convert_buf_to_num(repr, 32);
                tokens[current].real = (float)res;
                tokens[current].integer = (int32_t)res;
            }
            else
            {
                memcpy(tokens[current].data, line, line_size);
                tokens[current].data_size = line_size;
            }
        }

        read_line();
    }

    css_tokenizer_init(test_data, test_data_size);

    if (strncmp(test_data, "10--custom-px", 13) == 0)
    {
        printf("here\n");
    }

    for (uint32_t i = 0; i < current; i++)
    {
        css_token_t e = tokens[i];
        css_token_t a = css_tokenizer_next();

        ASSERT_EQUAL(a.type, e.type);
        ASSERT_EQUAL(a.data_size, e.data_size);
        if (a.data_size == e.data_size)
        {
            ASSERT_STRING((char)a.data, (char)e.data, a.data_size);
        }
        ASSERT_EQUAL(a.integer, e.integer);
        ASSERT_EQUAL(a.real, e.real);
        // ASSERT_EQUAL(a.hash, e.hash);
    }

    if (!TEST_SUCCEEDED())
    {
        printf("\n========== Test %u ==========\n", test_line);
        printf("FILE: %s\n", test_file);
        printf("INPUT: %s\n", test_data);
    }
}


void css_tokenizer_test()
{
    const unsigned char* files[] = {
                                    "./test/css/tokenizer/data/at-keyword.txt",
                                    // "./test/css/tokenizer/data/bad-string.txt",
                                    // "./test/css/tokenizer/data/bad-url.txt",
                                    // "./test/css/tokenizer/data/colon.txt",
                                    // "./test/css/tokenizer/data/comma.txt",
                                    // "./test/css/tokenizer/data/comment.txt",
                                    // "./test/css/tokenizer/data/digit.txt",
                                    // "./test/css/tokenizer/data/dimension.txt",
                                    // "./test/css/tokenizer/data/escaped-code-point.txt",
                                    // "./test/css/tokenizer/data/full-stop.txt",
                                    // "./test/css/tokenizer/data/fuzz.txt",
                                    // "./test/css/tokenizer/data/hash.txt",
                                    // "./test/css/tokenizer/data/hyphen-minus.txt",
                                    // "./test/css/tokenizer/data/ident-like.txt",
                                    // "./test/css/tokenizer/data/ident.txt",
                                    // "./test/css/tokenizer/data/left-curly-bracket.txt",
                                    // "./test/css/tokenizer/data/left-parenthesis.txt",
                                    // "./test/css/tokenizer/data/left-square-bracket.txt",
                                    // "./test/css/tokenizer/data/less-than.txt",
                                    // "./test/css/tokenizer/data/number.txt",
                                    // "./test/css/tokenizer/data/numeric.txt",
                                    // "./test/css/tokenizer/data/plus.txt",
                                    // "./test/css/tokenizer/data/reverse-solidus.txt",
                                    // "./test/css/tokenizer/data/right-curly-bracket.txt",
                                    // "./test/css/tokenizer/data/right-parenthesis.txt",
                                    // "./test/css/tokenizer/data/right-square-bracket.txt",
                                    // "./test/css/tokenizer/data/semi-colon.txt",
                                    // "./test/css/tokenizer/data/string.txt",
                                    // "./test/css/tokenizer/data/url.txt",
                                    // "./test/css/tokenizer/data/whitespace.txt"
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

        while (!is_eof) { TEST_CASE(run_css_tokenizer_test) }
    }
}