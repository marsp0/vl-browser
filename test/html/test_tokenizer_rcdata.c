#include "test_tokenizer.h"

static void test_1()
{
    const char buffer[]                         = "</!";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_RCDATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '/' } } },
                                                    { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

void test_html_tokenizer_rcdata()
{
    TEST_CASE(test_1);
}