#include "test_tokenizer.h"

static void test_empty_buffer()
{
    // {"description":"[empty]",
    // "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
    // "input":"",
    // "output":[]},

    const char buffer[]                         = "";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE, 
                                                    HTML_TOKENIZER_PLAINTEXT_STATE, 
                                                    HTML_TOKENIZER_RCDATA_STATE,
                                                    HTML_TOKENIZER_RAWTEXT_STATE,
                                                    HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_empty_buffer_cdata_section_state()
{
    // {"description":"[empty]",
    // "initialStates":["CDATA section state"],
    // "input":"",
    // "output":[],
    // "errors":[
    //     { "code": "eof-in-cdata", "line": 1, "col": 1 }
    // ]},

    const char buffer[]                         = "";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_CDATA_SECTION_STATE };
    const uint32_t sizes[]                      = { 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_CDATA };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_tab_character()
{
    // {"description":"\\u0009",
    // "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
    // "input":"\u0009",
    // "output":[["Character", "\u0009"]]},

    const char buffer[]                         = "\t";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE, 
                                                    HTML_TOKENIZER_PLAINTEXT_STATE, 
                                                    HTML_TOKENIZER_RCDATA_STATE,
                                                    HTML_TOKENIZER_RAWTEXT_STATE,
                                                    HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { {   { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '\t' } } }, 
                                                    {   { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_tab_character_cdata_section()
{
    // {"description":"\\u0009",
    // "initialStates":["CDATA section state"],
    // "input":"\u0009",
    // "output":[["Character", "\u0009"]],
    // "errors":[
    //     { "code": "eof-in-cdata", "line": 1, "col": 2 }
    // ]},

    const char buffer[]                         = "\t";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_CDATA_SECTION_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_EOF_IN_CDATA };
    const html_token_t tokens_e[][MAX_TOKENS]   = { {   { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '\t' } } }, 
                                                    {   { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_newline_character()
{
    // {"description":"\\u000A",
    // "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
    // "input":"\u000A",
    // "output":[["Character", "\u000A"]]},

    const char buffer[]                         = "\n";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE, 
                                                    HTML_TOKENIZER_PLAINTEXT_STATE, 
                                                    HTML_TOKENIZER_RCDATA_STATE,
                                                    HTML_TOKENIZER_RAWTEXT_STATE,
                                                    HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { {   { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '\n' } } }, 
                                                    {   { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void test_newline_character_cdata_section()
{
    // {"description":"\\u000A",
    // "initialStates":["CDATA section state"],
    // "input":"\u000A",
    // "output":[["Character", "\u000A"]],
    // "errors":[
    //     { "code": "eof-in-cdata", "line": 2, "col": 1 }
    // ]},

    const char buffer[]                         = "\n";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_CDATA_SECTION_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_EOF_IN_CDATA };
    const html_token_t tokens_e[][MAX_TOKENS]   = { {   { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '\n' } } }, 
                                                    {   { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void cr_as_numeric_entity()
{
    // {"description": "CR as numeric entity",
    // "input":"&#013;",
    // "output": [["Character", "\r"]],
    // "errors":[
    //     { "code": "control-character-reference", "line": 1, "col": 7 }
    // ]},

    const char buffer[]                         = "&#013;";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_CONTROL_CHARACTER_REFERENCE, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { {   { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '\r' } } }, 
                                                    {   { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_before_tag_name_in_opening_tag()
{
    // {"description":"<",
    // "input":"<",
    // "output":[["Character", "<"]],
    // "errors":[
    //     { "code": "eof-before-tag-name", "line": 1, "col": 2 }
    // ]},

    const char buffer[]                         = "<";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_BEFORE_TAG_NAME };
    const html_token_t tokens_e[][MAX_TOKENS]   = { {   { .is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } }, 
                                                        { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_in_comment_start_dash_state()
{
    // {"description":"<!--",
    // "input":"<!--",
    // "output":[["Comment", ""]],
    // "errors":[
    //     { "code": "eof-in-comment", "line": 1, "col": 5 }
    // ]},

    const char buffer[]                         = "<!--";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_COMMENT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { {   { .is_valid = true, .type = HTML_COMMENT_TOKEN }, 
                                                        { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_in_comment_start_end_dash_state()
{
    // {"description":"<!-- -",
    // "input":"<!--",
    // "output":[["Comment", ""]],
    // "errors":[
    //     { "code": "eof-in-comment", "line": 1, "col": 5 }
    // ]},

    const char buffer[]                         = "<!-- -";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_COMMENT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { {   { .is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 1, .data = { [0] = ' ' } }, 
                                                        { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_in_comment_start_end_bang_state()
{
    const char buffer[]                         = "<!----!";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_COMMENT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { {   { .is_valid = true, .type = HTML_COMMENT_TOKEN }, 
                                                        { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_in_doctype_state()
{
    // {"description":"<!DOCTYPE",
    // "input":"<!DOCTYPE",
    // "output":[["DOCTYPE", null, null, null, false]],
    // "errors":[
    //     { "code": "eof-in-doctype", "line": 1, "col": 10 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_in_before_doctype_name_state()
{
    // {"description":"<!DOCTYPE ",
    // "input":"<!DOCTYPE",
    // "output":[["DOCTYPE", null, null, null, false]],
    // "errors":[
    //     { "code": "eof-in-doctype", "line": 1, "col": 10 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE ";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_before_doctype_public_identifier_state()
{
    const char buffer[]                         = "<!DOCTYPE html PUBLIC ";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_doctype_public_identifier_double_quoted_state()
{
    const char buffer[]                         = "<!DOCTYPE html PUBLIC \"";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_doctype_public_identifier_single_quoted_state()
{
    const char buffer[]                         = "<!DOCTYPE html PUBLIC '";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_after_doctype_public_identifier_state()
{
    const char buffer[]                         = "<!DOCTYPE html PUBLIC 'dsa'";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' },
                                                        .public_id_size = 3,
                                                        .public_id = { [0] = 'd', [1] = 's', [2] = 'a' } },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_between_public_and_system_identifier()
{
    const char buffer[]                         = "<!DOCTYPE html PUBLIC 'dsa' ";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' },
                                                        .public_id_size = 3,
                                                        .public_id = { [0] = 'd', [1] = 's', [2] = 'a' } },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_after_system_keyword()
{
    const char buffer[]                         = "<!DOCTYPE html SYSTEM";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_before_system_identifier()
{
    const char buffer[]                         = "<!DOCTYPE html SYSTEM ";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_system_identifier_single_quoted()
{
    const char buffer[]                         = "<!DOCTYPE html SYSTEM '";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_system_identifier_double_quoted()
{
    const char buffer[]                         = "<!DOCTYPE html SYSTEM \"";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_after_system_identifier()
{
    const char buffer[]                         = "<!DOCTYPE html SYSTEM \"d\"";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' },
                                                        .system_id_size = 1,
                                                        .system_id = { [0] = 'd' } },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_in_script_data_escaped_dash_state()
{
    // {
    //    "description":"EOF in script HTML comment after dash",
    //    "initialStates":["Script data state"],
    //    "input":"<!--test-",
    //    "output":[["Character", "<!--test-"]],
    //    "errors":[
    //        { "code": "eof-in-script-html-comment-like-text", "line": 1, "col": 10 }
    //    ]
    // },

    const char buffer[]                         = "<!--test-";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1};
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
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
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_in_script_data_escaped_dash_dash_state()
{
    // {
    //    "description":"EOF in script HTML comment after dash dash",
    //    "initialStates":["Script data state"],
    //    "input":"<!--test--",
    //    "output":[["Character", "<!--test--"]],
    //    "errors":[
    //        { "code": "eof-in-script-html-comment-like-text", "line": 1, "col": 11 }
    //    ]
    // },

    const char buffer[]                         = "<!--test--";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
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
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_in_script_data_double_escaped_dash_state()
{
    // {
    //    "description":"EOF in script HTML comment double escaped after dash",
    //    "initialStates":["Script data state"],
    //    "input":"<!--<script>-",
    //    "output":[["Character", "<!--<script>-"]],
    //    "errors":[
    //        { "code": "eof-in-script-html-comment-like-text", "line": 1, "col": 14 }
    //    ]
    // },

    const char buffer[]                         = "<!--<script>-";
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
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_in_script_data_double_escaped_dash_dash_state()
{
    // {
    //    "description":"EOF in script HTML comment double escaped after dash dash",
    //    "initialStates":["Script data state"],
    //    "input":"<!--<script>--",
    //    "output":[["Character", "<!--<script>--"]],
    //    "errors":[
    //        { "code": "eof-in-script-html-comment-like-text", "line": 1, "col": 15 }
    //    ]
    // },

    const char buffer[]                         = "<!--<script>--";
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
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_in_script_data_double_escaped_state()
{
    // {
    //    "description":"EOF in script HTML comment - double escaped",
    //    "initialStates":["Script data state"],
    //    "input":"<!--<script>",
    //    "output":[["Character", "<!--<script>"]],
    //    "errors":[
    //        { "code": "eof-in-script-html-comment-like-text", "line": 1, "col": 13 }
    //    ]
    // },

    const char buffer[]                         = "<!--<script>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_SCRIPT_DATA_STATE };
    const uint32_t sizes[]                      = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
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
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void cr_eof_in_tag_name()
{
    // {"description":"CR EOF in tag name",
    // "input":"<z\r",
    // "output":[],
    // "errors":[
    //     { "code": "eof-in-tag", "line": 2, "col": 1 }
    // ]},

    const char buffer[]                         = "<z\r";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_TAG };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void slash_eof_in_tag_name()
{
    // {"description":"Slash EOF in tag name",
    // "input":"<z/",
    // "output":[],
    // "errors":[
    //     { "code": "eof-in-tag", "line": 1, "col": 4 }
    // ]},

    const char buffer[]                         = "<z/";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_TAG };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void incorrectly_closed_comment()
{
    const char buffer[]                         = "<!----!>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_INCORRECTLY_CLOSED_COMMENT, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_COMMENT_TOKEN } }, 
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void invalid_chars_after_doctype_name()
{
    // {"description":"Doctype publik",
    // "input":"<!DOCTYPE html PUBLIK \"AbC\" \"XyZ\">",
    // "output":[["DOCTYPE", "html", null, null, false]],
    // "errors":[
    //     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 16 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE html PUBLIK \"AbC\" \"XyZ\">";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_INVALID_CHARACTER_SEQUENCE_AFTER_DOCTYPE_NAME, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void missing_attribute_value_error()
{
    // {"description":"<a a =>",
    // "input":"<a a =>",
    // "output":[["StartTag", "a", {"a":""}]],
    // "errors":[
    //     { "code": "missing-attribute-value", "line": 1, "col": 7 }
    // ]},

    const char buffer[]                         = "<a a =>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_MISSING_ATTRIBUTE_VALUE, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'a' },
                                                        .attributes_size = 1,
                                                        .attributes = { {   .name_size = 1,
                                                                            .name = { [0] = 'a' } } } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void missing_doctype_public_identifier_after_doctype_public_keyword()
{
    // {"description":"<!DOCTYPE html PUBLIC>",
    // "input":"<!DOCTYPE a PUBLIC>",
    // "output":[["DOCTYPE", "a", null, null, false]],
    // "errors": [
    //     { "code": "missing-doctype-public-identifier", "col": 19, "line": 1 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE html PUBLIC>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_MISSING_DOCTYPE_PUBLIC_IDENTIFIER, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void missing_doctype_public_identifier_before_doctype_public_identifier()
{
    // {"description":"<!DOCTYPE html PUBLIC>",
    // "input":"<!DOCTYPE a PUBLIC>",
    // "output":[["DOCTYPE", "a", null, null, false]],
    // "errors": [
    //     { "code": "missing-doctype-public-identifier", "col": 19, "line": 1 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE html PUBLIC >";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_MISSING_DOCTYPE_PUBLIC_IDENTIFIER, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void missing_doctype_system_identifier_after_doctype_public_keyword()
{
    // {"description":"<!DOCTYPE html SYSTEM>",
    // "input":"<!DOCTYPE a SYSTEM>",
    // "output":[["DOCTYPE", "a", null, null, false]],
    // "errors": [
    //     { "code": "missing-doctype-system-identifier", "col": 19, "line": 1 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE html SYSTEM>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_MISSING_DOCTYPE_SYSTEM_IDENTIFIER, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void missing_doctype_system_identifier_before_doctype_public_identifier()
{
    // {"description":"<!DOCTYPE html SYSTEM>",
    // "input":"<!DOCTYPE a SYSTEM>",
    // "output":[["DOCTYPE", "a", null, null, false]],
    // "errors": [
    //     { "code": "missing-doctype-system-identifier", "col": 19, "line": 1 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE html SYSTEM >";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_MISSING_DOCTYPE_SYSTEM_IDENTIFIER, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 4, .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } } },
                                                    { { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void missing_quote_after_doctype_public_keyword()
{
    // {"description":"<!DOCTYPE a PUBLIC!",
    // "input":"<!DOCTYPE a PUBLIC!",
    // "output":[["DOCTYPE", "a", null, null, false]],
    // "errors": [
    //     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE a PUBLIC!";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_MISSING_QUOTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER  };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 1, .name = { [0] = 'a' } },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void missing_quote_before_doctype_public_identifier()
{
    // {"description":"<!DOCTYPE a PUBLIC!",
    // "input":"<!DOCTYPE a PUBLIC!",
    // "output":[["DOCTYPE", "a", null, null, false]],
    // "errors": [
    //     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE a PUBLIC !";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_MISSING_QUOTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER  };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true,
                                                        .name_size = 1, .name = { [0] = 'a' } },
                                                      { .is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

void test_html_tokenizer_test3()
{
    TEST_CASE(test_empty_buffer);
    TEST_CASE(test_empty_buffer_cdata_section_state);
    TEST_CASE(test_tab_character);
    TEST_CASE(test_tab_character_cdata_section);
    TEST_CASE(test_newline_character);
    TEST_CASE(test_newline_character_cdata_section);
    TEST_CASE(cr_as_numeric_entity);
    TEST_CASE(eof_before_tag_name_in_opening_tag);
    TEST_CASE(eof_in_comment_start_dash_state);
    TEST_CASE(eof_in_comment_start_end_dash_state);
    TEST_CASE(eof_in_comment_start_end_bang_state);
    TEST_CASE(eof_in_doctype_state);
    TEST_CASE(eof_in_before_doctype_name_state);
    TEST_CASE(eof_before_doctype_public_identifier_state);
    TEST_CASE(eof_doctype_public_identifier_double_quoted_state);
    TEST_CASE(eof_doctype_public_identifier_single_quoted_state);
    TEST_CASE(eof_after_doctype_public_identifier_state);
    TEST_CASE(eof_between_public_and_system_identifier);
    TEST_CASE(eof_after_system_keyword);
    TEST_CASE(eof_before_system_identifier);
    TEST_CASE(eof_system_identifier_single_quoted);
    TEST_CASE(eof_system_identifier_double_quoted);
    TEST_CASE(eof_after_system_identifier);
    TEST_CASE(eof_in_script_data_escaped_dash_state);
    TEST_CASE(eof_in_script_data_escaped_dash_dash_state);
    TEST_CASE(eof_in_script_data_double_escaped_dash_state);
    TEST_CASE(eof_in_script_data_double_escaped_dash_dash_state);
    TEST_CASE(eof_in_script_data_double_escaped_state);
    TEST_CASE(cr_eof_in_tag_name);
    TEST_CASE(slash_eof_in_tag_name);
    TEST_CASE(incorrectly_closed_comment);
    TEST_CASE(invalid_chars_after_doctype_name);
    TEST_CASE(missing_attribute_value_error);
    TEST_CASE(missing_doctype_public_identifier_after_doctype_public_keyword);
    TEST_CASE(missing_doctype_public_identifier_before_doctype_public_identifier);
    TEST_CASE(missing_doctype_system_identifier_after_doctype_public_keyword);
    TEST_CASE(missing_doctype_system_identifier_before_doctype_public_identifier);
    TEST_CASE(missing_quote_after_doctype_public_keyword);
    TEST_CASE(missing_quote_before_doctype_public_identifier);
}
