#include "test_tokenizer.h"

static void test_1()
{
    const char buffer[]                         = "</!";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '/' } } },
                                                    { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_3()
{
    const char buffer[]                         = "</a >";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1};
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_END_TOKEN, .name_size = 1, .name = { [0] = 'a' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_4()
{
    const char buffer[]                         = "\0";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1};
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_5()
{
    const char buffer[]                         = "<";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1};
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_6()
{
    const char buffer[]                         = "</Aa!";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
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

static void test_7()
{
    const char buffer[]                         = "<!---\0";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER,
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_8()
{
    const char buffer[]                         = "<!--\0-<";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_9()
{
    const char buffer[]                         = "<!--\0-\0";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER,
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_10()
{
    const char buffer[]                         = "<!--\0-!";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_11()
{
    const char buffer[]                         = "<!--\0\0";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER,
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_char_in_script_data_double_escaped_state()
{
    const char buffer[]                         = "<!--<scripT>\0";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER,
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'c' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'r' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'i' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'p' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'T' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void non_alpha_in_script_data_double_escape_state()
{
    const char buffer[]                         = "<!--<scripT!>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'c' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'r' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'i' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'p' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'T' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_in_script_data_double_escaped_dash_state()
{
    const char buffer[]                         = "<!--<script>-\0";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER,
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'c' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'r' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'i' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'p' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void triangle_in_script_data_double_escaped_dash_dash_state()
{
    const char buffer[]                         = "<!--<script>-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'c' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'r' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'i' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'p' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_in_script_data_double_escaped_dash_dash_state()
{
    const char buffer[]                         = "<!--<script>--\0";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER,
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'c' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'r' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'i' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'p' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void non_solidus_in_data_double_escaped_less_than_state()
{
    const char buffer[]                         = "<!--<script>-<script>-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK};
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'c' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'r' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'i' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'p' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'c' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'r' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'i' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'p' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void non_script_word_in_script_data_double_escape_end_state()
{
    const char buffer[]                         = "<!--<script>-</scrOpt!>-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT};
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'c' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'r' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'i' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'p' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '/' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'c' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'r' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'O' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'p' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

void test_html_tokenizer_script_data()
{
    TEST_CASE(test_1);
    TEST_CASE(test_3);
    TEST_CASE(test_4);
    TEST_CASE(test_5);
    TEST_CASE(test_6);
    TEST_CASE(test_7);
    TEST_CASE(test_8);
    TEST_CASE(test_9);
    TEST_CASE(test_10);
    TEST_CASE(test_11);
    TEST_CASE(null_char_in_script_data_double_escaped_state);
    TEST_CASE(non_alpha_in_script_data_double_escape_state);
    TEST_CASE(null_in_script_data_double_escaped_dash_state);
    TEST_CASE(triangle_in_script_data_double_escaped_dash_dash_state);
    TEST_CASE(null_in_script_data_double_escaped_dash_dash_state);
    TEST_CASE(non_solidus_in_data_double_escaped_less_than_state);
    TEST_CASE(non_script_word_in_script_data_double_escape_end_state);
}