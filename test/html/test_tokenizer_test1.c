#include "test_tokenizer.h"

static void correct_doctype_lowercase()
{
    const char buffer[]                         = "<!DOCTYPE html>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN, 
                                                        .name_size = 4, 
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' }, 
                                                        .force_quirks = false } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void correct_doctype_uppercase()
{
    const char buffer[]                         = "<!DOCTYPE HTML>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN, 
                                                        .name_size = 4, 
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' }, 
                                                        .force_quirks = false } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void correct_doctype_mixedcase()
{
    const char buffer[]                         = "<!DOCTYPE HtMl>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN, 
                                                        .name_size = 4, 
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' }, 
                                                        .force_quirks = false } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void correct_doctype_with_eof()
{
    const char buffer[]                         = "<!DOCTYPE html";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN, 
                                                        .name_size = 4, 
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' }, 
                                                        .force_quirks = true },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void truncated_doctype_start()
{
    const char buffer[]                         = "<!DOC>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_INCORRECTLY_OPENED_COMMENT, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_COMMENT_TOKEN, 
                                                        .data_size = 3,
                                                        .data = { [0] = 'D', [1] = 'O', [2] = 'C'}, 
                                                        .force_quirks = true } },
                                                      { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void doctype_in_error()
{
    const char buffer[]                         = "<!DOCTYPE foo>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN,
                                                        .name_size = 3,
                                                        .name = { [0] = 'f', [1] = 'o', [2] = 'o' }, 
                                                        .force_quirks = false } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void single_start_tag()
{
    const char buffer[]                         = "<h>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_START_TOKEN,
                                                        .name_size = 1,
                                                        .name = { [0] = 'h' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void empty_end_tag()
{
    const char buffer[]                         = "</>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_MISSING_END_TAG_NAME };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void empty_start_tag()
{
    // output differs

    const char buffer[]                         = "<>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_INVALID_FIRST_CHARACTER_OF_TAG_NAME, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void start_tag_with_attribute()
{
    const char buffer[]                         = "<h a='b'>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_START_TOKEN,
                                                        .name_size = 1,
                                                        .name = { [0] = 'h' },
                                                        .attributes_size = 1,
                                                        .attributes = { [0] = { .name = { [0] = 'a' }, 
                                                                                .name_size = 1,
                                                                                .value = { [0] = 'b' },
                                                                                .value_size = 1 } } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void start_tag_with_attribute_no_quotes()
{
    const char buffer[]                         = "<h a=b>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_START_TOKEN,
                                                        .name_size = 1,
                                                        .name = { [0] = 'h' },
                                                        .attributes_size = 1,
                                                        .attributes = { [0] = { .name = { [0] = 'a' }, 
                                                                                .name_size = 1,
                                                                                .value = { [0] = 'b' },
                                                                                .value_size = 1 } } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void start_and_end_tag()
{
    const char buffer[]                         = "<h></h>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' }, .attributes_size = 0 } },
                                                    { { .is_valid = true, .type = HTML_END_TOKEN, .name_size = 1, .name = { [0] = 'h' }, .attributes_size = 0 } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void two_unclosed_start_tags()
{
    const char buffer[]                         = "<p>One<p>Two";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'p' }, .attributes_size = 0 } },
                                                    { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'O' } } },
                                                    { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'n' } } },
                                                    { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'e' } } },
                                                    { { .is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'p' } } },
                                                    { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'T' } } },
                                                    { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'w' } } },
                                                    { { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'o' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void end_tag_with_attribute()
{
    const char buffer[]                         = "<h></h a='b'>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' } } },
                                                    { { .is_valid = true, .type = HTML_END_TOKEN, .name_size = 1, .name = { [0] = 'h' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void multiple_attributes()
{
    const char buffer[]                         = "<h a='b' c='d'>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' }, .attributes_size = 2,
                                                        .attributes = { {.name = { [0] = 'a'}, .name_size = 1, .value = { [0] = 'b' }, .value_size = 1 },
                                                                        {.name = { [0] = 'c'}, .name_size = 1, .value = { [0] = 'd' }, .value_size = 1 } } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void multiple_attributes_no_space()
{
    const char buffer[]                         = "<h a='b'c='d'>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_MISSING_WHITESPACE_BETWEEN_ATTRIBUTES, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' }, .attributes_size = 2,
                                                        .attributes = { {.name = { [0] = 'a'}, .name_size = 1, .value = { [0] = 'b' }, .value_size = 1 },
                                                                        {.name = { [0] = 'c'}, .name_size = 1, .value = { [0] = 'd' }, .value_size = 1 } } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void repeated_attribute()
{
    const char buffer[]                         = "<h a='b' a='d'>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' }, .attributes_size = 1,
                                                        .attributes = { {.name = { [0] = 'a'}, .name_size = 1, .value = { [0] = 'b' }, .value_size = 1 } } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void simple_comment()
{
    const char buffer[]                         = "<!--comment-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 7,
                                                        .data = { [0] = 'c', [1] = 'o', [2] = 'm', [3] = 'm', [4] = 'e', [5] = 'n', [6] = 't' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void comment_central_dash_no_space()
{
    const char buffer[]                         = "<!----->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 1, .data = { [0] = '-'} } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void comment_two_central_dashes()
{
    const char buffer[]                         = "<!-- --comment -->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 11,
                                                        .data = { [0] = ' ', [1] = '-', [2] = '-', [3] = 'c', [4] = 'o', [5] = 'm', [6] = 'm', 
                                                                  [7] = 'e', [8] = 'n', [9] = 't', [10] = ' ' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void comment_central_less_than_bang()
{
    const char buffer[]                         = "<!--<!-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 2, .data = { [0] = '<', [1] = '!' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void unfinished_comment()
{
    const char buffer[]                         = "<!--comment";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_COMMENT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 7,
                                                        .data = { [0] = 'c', [1] = 'o', [2] = 'm', [3] = 'm', [4] = 'e', [5] = 'n', [6] = 't' } },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void unfinished_comment_after_start_of_nested_comment()
{
    const char buffer[]                         = "<!-- <!--";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_COMMENT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 3, .data = { [0] = ' ', [1] = '<', [2] = '!'} },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void start_of_comment()
{
    const char buffer[]                         = "<!-";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_INCORRECTLY_OPENED_COMMENT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 1, .data = { [0] = '-'} },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void short_comment()
{
    const char buffer[]                         = "<!-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_ABRUPT_CLOSING_OF_EMPTY_COMMENT, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 0 } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void short_comment_two()
{
    const char buffer[]                         = "<!--->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_ABRUPT_CLOSING_OF_EMPTY_COMMENT, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 0 } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void short_comment_three()
{
    const char buffer[]                         = "<!---->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 0 } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void less_than_in_comment()
{
    const char buffer[]                         = "<!-- <test-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 6,
                                                        .data = { [0] = ' ', [1] = '<', [2] = 't', [3] = 'e', [4] = 's', [5] = 't'} } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void double_less_than_in_comment()
{
    const char buffer[]                         = "<!-- <<test-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 7,
                                                        .data = { [0] = ' ', [1] = '<', [2] = '<', [3] = 't', [4] = 'e', [5] = 's', [6] = 't'} } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void less_than_bang_in_comment()
{
    const char buffer[]                         = "<!-- <!test-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 7,
                                                        .data = { [0] = ' ', [1] = '<', [2] = '!', [3] = 't', [4] = 'e', [5] = 's', [6] = 't'} } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void less_than_bang_dash_in_comment()
{
    const char buffer[]                         = "<!-- <!-est-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 7,
                                                        .data = { [0] = ' ', [1] = '<', [2] = '!', [3] = '-', [4] = 'e', [5] = 's', [6] = 't'} } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void nested_comment()
{
    const char buffer[]                         = "<!-- <!--test-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_NESTED_COMMENT, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 9,
                                                        .data = { [0] = ' ', [1] = '<', [2] = '!', [3] = '-', [4] = '-', [5] = 't', [6] = 'e', [7] = 's', [8] = 't'} } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void nested_comment_with_extra_less_than_sign()
{
    const char buffer[]                         = "<!-- <<!--test-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_NESTED_COMMENT, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 10,
                                                        .data = { [0] = ' ', [1] = '<', [2] = '<', [3] = '!', [4] = '-', [5] = '-', 
                                                                  [6] = 't', [7] = 'e', [8] = 's', [9] = 't'} } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void less_than_in_script_data()
{
    const char buffer[]                         = "<test-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'e' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void less_than_bang_in_script_data()
{
    const char buffer[]                         = "<!test-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
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
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'e' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void less_than_bang_dash_in_script_data()
{
    const char buffer[]                         = "<!-test-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
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
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'e' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void escaped_script_data()
{
    const char buffer[]                         = "<!--test-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
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
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'e' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void less_than_in_script_html_comment()
{
    const char buffer[]                         = "<!-- < test -->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
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
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'e' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void less_than_solidus_in_script_html_comment()
{
    const char buffer[]                         = "<!-- </ test -->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
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
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '/' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'e' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void start_tag_in_script_html_comment()
{
    const char buffer[]                         = "<!-- <test> -->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
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
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'e' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void end_tag_in_script_html_comment()
{
    const char buffer[]                         = "<!-- </test> -->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1 };
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
                                                    HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '/' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'e' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void dash_in_script_html_comment_double_escaped()
{
    const char buffer[]                         = "<!--<script>-</script>-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
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

static void dash_dash_in_script_html_comment_double_escaped()
{
    const char buffer[]                         = "<!--<script>--</script>-->";
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
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '/' } } },
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

static void dash_dash_dash_in_script_html_comment_double_escaped()
{
    const char buffer[]                         = "<!--<script>---</script>-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
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
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '/' } } },
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

static void dash_spaced_in_script_html_comment_double_escaped()
{
    const char buffer[]                         = "<!--<script> - </script>-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
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
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '/' } } },
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

static void dash_dash_spaced_in_script_html_comment_double_escaped()
{
    const char buffer[]                         = "<!--<script> -- </script>-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
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
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '/' } } },
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

static void ampersand_eof()
{
    const char buffer[]                         = "&";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK};
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void ampersand_ampersand_eof()
{
    const char buffer[]                         = "&&";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK};
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void ampersand_space_eof()
{
    const char buffer[]                         = "& ";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK};
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

// static void unfinished_enitity()
// {
    
// }

static void ampersand_numbersign()
{
    const char buffer[]                         = "&#";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_ABSENCE_OF_DIGITS_IN_NUMERIC_CHARACTER_REFERENCE,
                                                    HTML_TOKENIZER_OK};
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '#' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void unfinished_numeric_entity()
{
    const char buffer[]                         = "&#x";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 3, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_ABSENCE_OF_DIGITS_IN_NUMERIC_CHARACTER_REFERENCE,
                                                    HTML_TOKENIZER_OK};
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '#' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'x' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

// static void entity_with_trailing_semicolon_one()
// {
//     {"description":"Entity with trailing semicolon (1)",
//     "input":"I'm &not;it",
//     "output":[["Character","I'm \u00ACit"]]},
// }

// static void entity_with_trailing_semicolon_two()
// {
//     {"description":"Entity with trailing semicolon (2)",
//     "input":"I'm &notin;",
//     "output":[["Character","I'm \u2209"]]},
// }

// static void entity_without_trailing_semicolon_one()
// {
//     {"description":"Entity without trailing semicolon (1)",
//     "input":"I'm &notit",
//     "output":[["Character","I'm \u00ACit"]],
//     "errors": [
//         {"code" : "missing-semicolon-after-character-reference", "line": 1, "col": 9 }
//     ]},
// }

// static void entity_without_trailing_semicolon_two()
// {
//     {"description":"Entity without trailing semicolon (2)",
//     "input":"I'm &notin",
//     "output":[["Character","I'm \u00ACin"]],
//     "errors": [
//         {"code" : "missing-semicolon-after-character-reference", "line": 1, "col": 9 }
//     ]},
// }

// static void partial_entity_match_at_eof()
// {
//     {"description":"Partial entity match at end of file",
//     "input":"I'm &no",
//     "output":[["Character","I'm &no"]]},
// }

// static void non_ascii_char_ref_name()
// {
//     {"description":"Non-ASCII character reference name",
//     "input":"&\u00AC;",
//     "output":[["Character", "&\u00AC;"]]},
// }

// static void ascii_decimal_entity()
// {
//     {"description":"ASCII decimal entity",
//     "input":"&#0036;",
//     "output":[["Character","$"]]},
// }

// static void ascii_hexadecimal_entity()
// {
//     {"description":"ASCII hexadecimal entity",
//     "input":"&#x3f;",
//     "output":[["Character","?"]]},
// }

// static void hexadecimal_entity_in_attribute()
// {
    
//     {"description":"Hexadecimal entity in attribute",
//     "input":"<h a='&#x3f;'></h>",
//     "output":[["StartTag", "h", {"a":"?"}], ["EndTag", "h"]]},
// }

// static void entity_in_attribute_without_semicolon_ending_in_x()
// {
//     {"description":"Entity in attribute without semicolon ending in x",
//     "input":"<h a='&notx'>",
//     "output":[["StartTag", "h", {"a":"&notx"}]]},
// }

// static void entity_in_attribute_without_semicolon_ending_in_one()
// {
//     {"description":"Entity in attribute without semicolon ending in 1",
//     "input":"<h a='&not1'>",
//     "output":[["StartTag", "h", {"a":"&not1"}]]},
// }

// static void entity_in_attribute_without_semicolon_ending_in_i()
// {
    
//     {"description":"Entity in attribute without semicolon ending in i",
//     "input":"<h a='&noti'>",
//     "output":[["StartTag", "h", {"a":"&noti"}]]},
// }

// static void entity_in_attribute_without_semicolon()
// {
//     {"description":"Entity in attribute without semicolon",
//     "input":"<h a='&COPY'>",
//     "output":[["StartTag", "h", {"a":"\u00A9"}]],
//     "errors": [
//         {"code" : "missing-semicolon-after-character-reference", "line": 1, "col": 12 }
//     ]},
// }

static void unquoted_attribute_ending_in_ampersand()
{
    const char buffer[]                         = "<s o=& t>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK};
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 's' }, .attributes_size = 2, 
                                                        .attributes = { [0] = { .name = { [0] = 'o' }, .name_size = 1, .value = { [0] = '&' }, .value_size = 1 },
                                                                        [1] = { .name = { [0] = 't' }, .name_size = 1 } } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void unquoted_attribute_at_end_of_tag_with_final_char_ampersand()
{
    // {"description":"Unquoted attribute at end of tag with final character of &, with tag followed by characters",
    // "input":"<a a=a&>foo",
    // "output":[["StartTag", "a", {"a":"a&"}], ["Character", "foo"]]},

    const char buffer[]                         = "<a a=a&>foo";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'a' }, .attributes_size = 1, 
                                                        .attributes = { [0] = { .name = { [0] = 'a' }, .name_size = 1, .value = { [0] = 'a', [1] = '&' }, .value_size = 2 } } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'f' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'o' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'o' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void plaintext_element()
{
    
    // {"description":"plaintext element",
    //  "input":"<plaintext>foobar",
    //  "output":[["StartTag","plaintext",{}], ["Character","foobar"]]},

    const char buffer[]                         = "<plaintext>foobar";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1, 1, 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_START_TOKEN, .name_size = 9, 
                                                        .name = { [0] = 'p', [1] = 'l', [2] = 'a', [3] = 'i', [4] = 'n', [5] = 't', [6] = 'e', [7] = 'x', [8] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'f' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'o' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'o' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'b' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'a' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'r' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void open_angle_bracked_in_attribute_value()
{
    // {"description":"Open angled bracket in unquoted attribute value state",
    //  "input":"<a a=f<>",
    //  "output":[["StartTag", "a", {"a":"f<"}]],
    //  "errors":[
    //     { "code": "unexpected-character-in-unquoted-attribute-value", "line": 1, "col": 7 }
    // ]}

    const char buffer[]                         = "<a a=f<>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_CHARACTER_IN_UNQUOTED_ATTRIBUTE_VALUE,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'a' }, .attributes_size = 1,
                                                        .attributes = { [0] = { .name = { [0] = 'a' }, .name_size = 1, .value = { [0] = 'f', [1] = '<' }, .value_size = 2 } } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_in_attribute_value_unquoted()
{
    const char buffer[]                         = "<a a=f";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_TAG };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_in_unquoted_quoted_attribute_value()
{
    const char buffer[]                         = "<h a=b\0>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' },
                                                        .attributes_size = 1,
                                                        .attributes = { [0] = { .name = { [0] = 'a' }, .name_size = 1, 
                                                                                .value = { [0] = 'b', [1] = 0xef, [2] = 0xbf, [3] = 0xbd }, 
                                                                                .value_size = 4 } } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_after_attribute_value_quoted_state()
{
    const char buffer[]                         = "<h a='b'";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_TAG };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_char_in_doctype_name()
{
    // {"description":"<!DOCTYPE \\u0000",
    // "input":"<!DOCTYPE \u0000",
    // "output":[["DOCTYPE", "\uFFFD", null, null, false]],
    // "errors":[
    //    { "code": "unexpected-null-character", "line": 1, "col": 11 },
    //    { "code": "eof-in-doctype", "line": 1, "col": 12 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE \0";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN,
                                                        .name_size = 3,
                                                        .name = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd }, 
                                                        .force_quirks = true },
                                                      {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_char_in_doctype_name_two()
{
    // {"description":"<!DOCTYPE a\\u0000",
    // "input":"<!DOCTYPE a\u0000",
    // "output":[["DOCTYPE", "a\uFFFD", null, null, false]],
    // "errors":[
    //     { "code": "unexpected-null-character", "line": 1, "col": 12 },
    //     { "code": "eof-in-doctype", "line": 1, "col": 13 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE a\0";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN,
                                                        .name_size = 4,
                                                        .name = { [0] = 'a', [1] = 0xef, [2] = 0xbf, [3] = 0xbd }, 
                                                        .force_quirks = true },
                                                      {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void doctype_tag_with_space_between_name_and_bracket()
{
    const char buffer[]                         = "<!DOCTYPE a >";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN,
                                                        .name_size = 1,
                                                        .name = { [0] = 'a' }, 
                                                        .force_quirks = false } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_char_in_doctype_public_identifier_double_quoted_state()
{
    const char buffer[]                         = "<!DOCTYPE a PUBLIC \"a\0\">";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN,
                                                        .name_size = 1,
                                                        .name = { [0] = 'a' },
                                                        .public_id_size = 4,
                                                        .public_id = { [0] = 'a' , [1] = 0xef, [2] = 0xbf, [3] = 0xbd }, 
                                                        .force_quirks = false } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_char_in_doctype_public_identifier_single_quoted_state()
{
    const char buffer[]                         = "<!DOCTYPE a PUBLIC 'a\0'>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN,
                                                        .name_size = 1,
                                                        .name = { [0] = 'a' },
                                                        .public_id_size = 4,
                                                        .public_id = { [0] = 'a' , [1] = 0xef, [2] = 0xbf, [3] = 0xbd }, 
                                                        .force_quirks = false } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void end_doctype_token_when_between_public_and_system_identifiers()
{
    const char buffer[]                         = "<!DOCTYPE a PUBLIC 'a' >";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN,
                                                        .name_size = 1,
                                                        .name = { [0] = 'a' },
                                                        .public_id_size = 1,
                                                        .public_id = { [0] = 'a'}, 
                                                        .force_quirks = false } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_character_in_double_quoted_system_identifier()
{
    const char buffer[]                         = "<!DOCTYPE a SYSTEM \"a\0\" >";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN,
                                                        .name_size = 1,
                                                        .name = { [0] = 'a' },
                                                        .system_id_size = 4,
                                                        .system_id = { [0] = 'a' , [1] = 0xef, [2] = 0xbf, [3] = 0xbd }, 
                                                        .force_quirks = false } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_character_in_single_quoted_system_identifier()
{
    const char buffer[]                         = "<!DOCTYPE a SYSTEM 'a\0' >";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN,
                                                        .name_size = 1,
                                                        .name = { [0] = 'a' },
                                                        .system_id_size = 4,
                                                        .system_id = { [0] = 'a' , [1] = 0xef, [2] = 0xbf, [3] = 0xbd }, 
                                                        .force_quirks = false } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_character_in_bogus_doctype_state()
{
    const char buffer[]                         = "<!DOCTYPE a SYSTEM 'a' !\0>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, 
                                                        .type = HTML_DOCTYPE_TOKEN,
                                                        .name_size = 1,
                                                        .name = { [0] = 'a' },
                                                        .system_id_size = 1,
                                                        .system_id = { [0] = 'a' }, 
                                                        .force_quirks = false } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

void test_html_tokenizer_test1()
{
    TEST_CASE(correct_doctype_lowercase);
    TEST_CASE(correct_doctype_uppercase);
    TEST_CASE(correct_doctype_mixedcase);
    TEST_CASE(correct_doctype_with_eof);
    TEST_CASE(truncated_doctype_start);
    TEST_CASE(doctype_in_error);
    TEST_CASE(single_start_tag);
    TEST_CASE(empty_end_tag);
    TEST_CASE(empty_start_tag);
    TEST_CASE(start_tag_with_attribute);
    TEST_CASE(start_tag_with_attribute_no_quotes);
    TEST_CASE(start_and_end_tag);
    TEST_CASE(two_unclosed_start_tags);
    TEST_CASE(end_tag_with_attribute);
    TEST_CASE(multiple_attributes);
    TEST_CASE(multiple_attributes_no_space);
    TEST_CASE(repeated_attribute);
    TEST_CASE(simple_comment);
    TEST_CASE(comment_central_dash_no_space);
    TEST_CASE(comment_two_central_dashes);
    TEST_CASE(comment_central_less_than_bang);
    TEST_CASE(unfinished_comment);
    TEST_CASE(unfinished_comment_after_start_of_nested_comment);
    TEST_CASE(start_of_comment);
    TEST_CASE(short_comment);
    TEST_CASE(short_comment_two);
    TEST_CASE(short_comment_three);
    TEST_CASE(less_than_in_comment);
    TEST_CASE(double_less_than_in_comment);
    TEST_CASE(less_than_bang_in_comment);
    TEST_CASE(less_than_bang_dash_in_comment);
    TEST_CASE(nested_comment);
    TEST_CASE(nested_comment_with_extra_less_than_sign);
    TEST_CASE(less_than_in_script_data);
    TEST_CASE(less_than_bang_in_script_data);
    TEST_CASE(less_than_bang_dash_in_script_data);
    TEST_CASE(escaped_script_data);
    TEST_CASE(less_than_in_script_html_comment);
    TEST_CASE(less_than_solidus_in_script_html_comment);
    TEST_CASE(start_tag_in_script_html_comment);
    TEST_CASE(end_tag_in_script_html_comment);
    TEST_CASE(dash_in_script_html_comment_double_escaped);
    TEST_CASE(dash_dash_in_script_html_comment_double_escaped);
    TEST_CASE(dash_dash_dash_in_script_html_comment_double_escaped);
    TEST_CASE(dash_spaced_in_script_html_comment_double_escaped);
    TEST_CASE(dash_dash_spaced_in_script_html_comment_double_escaped);
    TEST_CASE(ampersand_eof);
    TEST_CASE(ampersand_ampersand_eof);
    TEST_CASE(ampersand_space_eof);
    // TEST_CASE(unfinished_enitity);
    TEST_CASE(ampersand_numbersign);
    TEST_CASE(unfinished_numeric_entity);
    // TEST_CASE(entity_with_trailing_semicolon_one);
    // TEST_CASE(entity_with_trailing_semicolon_two);
    // TEST_CASE(entity_without_trailing_semicolon_one);
    // TEST_CASE(entity_without_trailing_semicolon_two);
    // TEST_CASE(partial_entity_match_at_eof);
    // TEST_CASE(non_ascii_char_ref_name);
    // TEST_CASE(ascii_decimal_entity);
    // TEST_CASE(ascii_hexadecimal_entity);
    // TEST_CASE(hexadecimal_entity_in_attribute);
    // TEST_CASE(entity_in_attribute_without_semicolon_ending_in_x);
    // TEST_CASE(entity_in_attribute_without_semicolon_ending_in_one);
    // TEST_CASE(entity_in_attribute_without_semicolon_ending_in_i);
    // TEST_CASE(entity_in_attribute_without_semicolon);
    TEST_CASE(unquoted_attribute_ending_in_ampersand);
    TEST_CASE(unquoted_attribute_at_end_of_tag_with_final_char_ampersand);
    TEST_CASE(plaintext_element);
    TEST_CASE(open_angle_bracked_in_attribute_value);
    TEST_CASE(eof_in_attribute_value_unquoted);
    TEST_CASE(null_in_unquoted_quoted_attribute_value);
    TEST_CASE(eof_after_attribute_value_quoted_state);
    TEST_CASE(null_char_in_doctype_name);
    TEST_CASE(null_char_in_doctype_name_two);
    TEST_CASE(doctype_tag_with_space_between_name_and_bracket);
    TEST_CASE(null_char_in_doctype_public_identifier_double_quoted_state);
    TEST_CASE(null_char_in_doctype_public_identifier_single_quoted_state);
    TEST_CASE(end_doctype_token_when_between_public_and_system_identifiers);
    TEST_CASE(null_character_in_double_quoted_system_identifier);
    TEST_CASE(null_character_in_single_quoted_system_identifier);
    TEST_CASE(null_character_in_bogus_doctype_state);
}
