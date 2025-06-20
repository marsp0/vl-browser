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
}
