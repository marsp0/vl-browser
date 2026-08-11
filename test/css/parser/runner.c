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
    STATE_VALUE,
    STATE_UNIT,
    STATE_HASH_TYPE
} state_e;

static const unsigned char* test_file = NULL;
static FILE* file = NULL;
static bool file_done = false;
static unsigned char file_buffer[4096] = { 0 };
static uint32_t file_buffer_cursor = 0;
static uint32_t file_buffer_size = 0;
static unsigned char prev = 0;
static unsigned char description[2048] = { 0 };

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


static void run_css_parser_test()
{
    current = 0;
    tokens_size = 0;
    test_data_size = 0;
    memset(tokens, 0, sizeof(tokens));
    memset(test_data, 0, 2048);

    do
    {
        if (is_eof) { return; }
        read_line();
    } while (strncmp(line, "#test", 5) != 0);

    
    state = STATE_DATA;

    while (true)
    {
        printf("%s\n", line);

        if (is_eof) { break; }
        read_line();
    }

    if (!TEST_SUCCEEDED())
    {
        printf("\n========== Test %u ==========\n", test_line);
        printf("FILE: %s\n", test_file);
        printf("TEST: %s\n", description);
        printf("INPUT: %s\n", test_data);
    }
}


void css_parser_test()
{
    const unsigned char* files[] = {
                                    "./test/css/parser/data/test1.data",
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

        while (!is_eof) { TEST_CASE(run_css_parser_test) }
    }
}