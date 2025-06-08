#include "test_tokenizer.h"

#include "test_utils.h"

#include "html/tokenizer.h"

#define SIZE_TEN 10
#define ASSERT_TOKENS_SIZE(expected, max_size)  do \
                                                { \
                                                    uint32_t actual = 0; \
                                                    for (uint32_t i = 0; i < max_size; i++) \
                                                    { \
                                                        if (tokens[i].is_valid) { actual++; } \
                                                    } \
                                                    ASSERT_EQUAL(actual, expected); \
                                                } while (0);

static void test_when_state_is_data_state_and_next_token_is_ampersand()
{
    
}

static void test_when_state_is_data_state_and_next_token_is_less_than()
{
    
}

static void test_when_state_is_data_state_and_next_token_is_eof()
{
    html_token_t tokens[SIZE_TEN] = { 0 };
    const unsigned char* buffer = "<html></html>";
    html_tokenizer_init(buffer, 0, tokens, SIZE_TEN);                     // passing size of 0 to trigger eof token

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_tokenizer_status_e state = html_tokenizer_next();

    ASSERT_EQUAL(state, HTML_TOKENIZER_OK);
    ASSERT_TRUE(tokens[0].is_valid);
    ASSERT_EQUAL(tokens[0].type, HTML_EOF_TOKEN);
}

static void test_when_state_is_data_state_and_next_token_is_null()
{
    html_token_t tokens[SIZE_TEN] = { 0 };
    const unsigned char buffer[] = { 0x00 };
    html_tokenizer_init(buffer, sizeof(buffer), tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_tokenizer_status_e state = html_tokenizer_next();

    ASSERT_TOKENS_SIZE(1, SIZE_TEN);

    ASSERT_EQUAL(state, HTML_TOKENIZER_OK);
    ASSERT_EQUAL(tokens[0].type, HTML_CHARACTER_TOKEN);
    ASSERT_EQUAL(tokens[0].data_size, 1);
    ASSERT_EQUAL((char)(tokens[0].data[0]), 0);
}

static void test_when_state_is_data_state_and_next_token_is_anything_else()
{
    html_token_t tokens[SIZE_TEN] = { 0 };
    const unsigned char* buffer = "-123-";
    html_tokenizer_init(buffer, sizeof(buffer), tokens, SIZE_TEN);                     // passing size of 0 to trigger eof token

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_tokenizer_status_e state   = html_tokenizer_next();
    unsigned char expected_data[1]       = { '-' };

    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(state, HTML_TOKENIZER_OK);
    ASSERT_EQUAL(tokens[0].type, HTML_CHARACTER_TOKEN);
    ASSERT_EQUAL(tokens[0].data_size, 1);
    ASSERT_STRING((char)tokens[0].data, (char)expected_data, 1);
}

void test_html_tokenizer()
{
    TEST_CASE(test_when_state_is_data_state_and_next_token_is_ampersand);
    TEST_CASE(test_when_state_is_data_state_and_next_token_is_less_than);
    TEST_CASE(test_when_state_is_data_state_and_next_token_is_eof);
    TEST_CASE(test_when_state_is_data_state_and_next_token_is_null);
    TEST_CASE(test_when_state_is_data_state_and_next_token_is_anything_else);
}