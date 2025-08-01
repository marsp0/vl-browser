#include "test_tokenizer.h"

static void test_1()
{
    const char buffer[]                         = "<\0";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_RAWTEXT_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_2()
{
    const char buffer[]                         = "</Aa!";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_RAWTEXT_STATE };
    const uint32_t sizes[]                      = { 4, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '/' } },
                                                      { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'A' } },
                                                      { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'a' } } },
                                                    { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_3()
{
    const char buffer[]                         = "</!";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_RAWTEXT_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '/' } } },
                                                    { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

void test_html_tokenizer_rawtext()
{
    TEST_CASE(test_1);
    TEST_CASE(test_2);
    TEST_CASE(test_3);
}