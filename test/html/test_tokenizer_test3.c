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

void test_html_tokenizer_test3()
{
    TEST_CASE(test_empty_buffer);
    TEST_CASE(test_empty_buffer_cdata_section_state);
    TEST_CASE(test_tab_character);
    TEST_CASE(test_tab_character_cdata_section);
    TEST_CASE(test_newline_character);
    TEST_CASE(test_newline_character_cdata_section);
}













// {"description":"\\u000B",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":"\u000B",
// "output":[["Character", "\u000B"]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 1 }
// ]},

// {"description":"\\u000B",
// "initialStates":["CDATA section state"],
// "input":"\u000B",
// "output":[["Character", "\u000B"]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 1 },
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":"\\u000C",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":"\u000C",
// "output":[["Character", "\u000C"]]},

// {"description":"\\u000C",
// "initialStates":["CDATA section state"],
// "input":"\u000C",
// "output":[["Character", "\u000C"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":" ",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":" ",
// "output":[["Character", " "]]},

// {"description":" ",
// "initialStates":["CDATA section state"],
// "input":" ",
// "output":[["Character", " "]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":"!",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":"!",
// "output":[["Character", "!"]]},

// {"description":"!",
// "initialStates":["CDATA section state"],
// "input":"!",
// "output":[["Character", "!"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":"\"",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":"\"",
// "output":[["Character", "\""]]},

// {"description":"\"",
// "initialStates":["CDATA section state"],
// "input":"\"",
// "output":[["Character", "\""]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":"%",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":"%",
// "output":[["Character", "%"]]},

// {"description":"%",
// "initialStates":["CDATA section state"],
// "input":"%",
// "output":[["Character", "%"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":"&",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":"&",
// "output":[["Character", "&"]]},

// {"description":"&",
// "initialStates":["CDATA section state"],
// "input":"&",
// "output":[["Character", "&"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":"'",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":"'",
// "output":[["Character", "'"]]},

// {"description":"'",
// "initialStates":["CDATA section state"],
// "input":"'",
// "output":[["Character", "'"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":",",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":",",
// "output":[["Character", ","]]},

// {"description":",",
// "initialStates":["CDATA section state"],
// "input":",",
// "output":[["Character", ","]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":"-",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":"-",
// "output":[["Character", "-"]]},

// {"description":"-",
// "initialStates":["CDATA section state"],
// "input":"-",
// "output":[["Character", "-"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":".",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":".",
// "output":[["Character", "."]]},

// {"description":".",
// "initialStates":["CDATA section state"],
// "input":".",
// "output":[["Character", "."]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":"/",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":"/",
// "output":[["Character", "/"]]},

// {"description":"/",
// "initialStates":["CDATA section state"],
// "input":"/",
// "output":[["Character", "/"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":"0",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":"0",
// "output":[["Character", "0"]]},

// {"description":"0",
// "initialStates":["CDATA section state"],
// "input":"0",
// "output":[["Character", "0"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":"1",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":"1",
// "output":[["Character", "1"]]},

// {"description":"1",
// "initialStates":["CDATA section state"],
// "input":"1",
// "output":[["Character", "1"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":"9",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":"9",
// "output":[["Character", "9"]]},

// {"description":"9",
// "initialStates":["CDATA section state"],
// "input":"9",
// "output":[["Character", "9"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":";",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";",
// "output":[["Character", ";"]]},

// {"description":";",
// "initialStates":["CDATA section state"],
// "input":";",
// "output":[["Character", ";"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 2 }
// ]},

// {"description":";=",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";=",
// "output":[["Character", ";="]]},

// {"description":";=",
// "initialStates":["CDATA section state"],
// "input":";=",
// "output":[["Character", ";="]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";>",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";>",
// "output":[["Character", ";>"]]},

// {"description":";>",
// "initialStates":["CDATA section state"],
// "input":";>",
// "output":[["Character", ";>"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";?",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";?",
// "output":[["Character", ";?"]]},

// {"description":";?",
// "initialStates":["CDATA section state"],
// "input":";?",
// "output":[["Character", ";?"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";@",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";@",
// "output":[["Character", ";@"]]},

// {"description":";@",
// "initialStates":["CDATA section state"],
// "input":";@",
// "output":[["Character", ";@"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";A",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";A",
// "output":[["Character", ";A"]]},

// {"description":";A",
// "initialStates":["CDATA section state"],
// "input":";A",
// "output":[["Character", ";A"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";B",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";B",
// "output":[["Character", ";B"]]},

// {"description":";B",
// "initialStates":["CDATA section state"],
// "input":";B",
// "output":[["Character", ";B"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";Y",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";Y",
// "output":[["Character", ";Y"]]},

// {"description":";Y",
// "initialStates":["CDATA section state"],
// "input":";Y",
// "output":[["Character", ";Y"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";Z",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";Z",
// "output":[["Character", ";Z"]]},

// {"description":";Z",
// "initialStates":["CDATA section state"],
// "input":";Z",
// "output":[["Character", ";Z"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";`",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";`",
// "output":[["Character", ";`"]]},

// {"description":";`",
// "initialStates":["CDATA section state"],
// "input":";`",
// "output":[["Character", ";`"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";a",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";a",
// "output":[["Character", ";a"]]},

// {"description":";a",
// "initialStates":["CDATA section state"],
// "input":";a",
// "output":[["Character", ";a"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";b",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";b",
// "output":[["Character", ";b"]]},

// {"description":";b",
// "initialStates":["CDATA section state"],
// "input":";b",
// "output":[["Character", ";b"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";y",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";y",
// "output":[["Character", ";y"]]},

// {"description":";y",
// "initialStates":["CDATA section state"],
// "input":";y",
// "output":[["Character", ";y"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";z",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";z",
// "output":[["Character", ";z"]]},

// {"description":";z",
// "initialStates":["CDATA section state"],
// "input":";z",
// "output":[["Character", ";z"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";{",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";{",
// "output":[["Character", ";{"]]},

// {"description":";{",
// "initialStates":["CDATA section state"],
// "input":";{",
// "output":[["Character", ";{"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 3 }
// ]},

// {"description":";\\uDBC0\\uDC00",
// "initialStates":["Data state", "PLAINTEXT state", "RCDATA state", "RAWTEXT state", "Script data state"],
// "input":";\uDBC0\uDC00",
// "output":[["Character", ";\uDBC0\uDC00"]]},

// {"description":";\\uDBC0\\uDC00",
// "initialStates":["CDATA section state"],
// "input":";\uDBC0\uDC00",
// "output":[["Character", ";\uDBC0\uDC00"]],
// "errors":[
//     { "code": "eof-in-cdata", "line": 1, "col": 4 }
// ]},

// {"description":"<",
// "input":"<",
// "output":[["Character", "<"]],
// "errors":[
//     { "code": "eof-before-tag-name", "line": 1, "col": 2 }
// ]},

// {"description":"<\\u0000",
// "input":"<\u0000",
// "output":[["Character", "<\u0000"]],
// "errors":[
//     { "code": "invalid-first-character-of-tag-name", "line": 1, "col": 2 },
//     { "code": "unexpected-null-character", "line": 1, "col": 2 }
// ]},

// {"description":"<\\u0009",
// "input":"<\u0009",
// "output":[["Character", "<\u0009"]],
// "errors":[
//     { "code": "invalid-first-character-of-tag-name", "line": 1, "col": 2 }
// ]},

// {"description":"<\\u000A",
// "input":"<\u000A",
// "output":[["Character", "<\u000A"]],
// "errors":[
//     { "code": "invalid-first-character-of-tag-name", "line": 1, "col": 2 }
// ]},

// {"description":"<\\u000B",
// "input":"<\u000B",
// "output":[["Character", "<\u000B"]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 2 },
//     { "code": "invalid-first-character-of-tag-name", "line": 1, "col": 2 }
// ]},

// {"description":"<\\u000C",
// "input":"<\u000C",
// "output":[["Character", "<\u000C"]],
// "errors":[
//     { "code": "invalid-first-character-of-tag-name", "line": 1, "col": 2 }
// ]},

// {"description":"< ",
// "input":"< ",
// "output":[["Character", "< "]],
// "errors":[
//     { "code": "invalid-first-character-of-tag-name", "line": 1, "col": 2 }
// ]},

// {"description":"<!",
// "input":"<!",
// "output":[["Comment", ""]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!\\u0000",
// "input":"<!\u0000",
// "output":[["Comment", "\uFFFD"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 },
//     { "code": "unexpected-null-character", "line": 1, "col": 3 }
// ]},

// {"description":"<!\\u0009",
// "input":"<!\u0009",
// "output":[["Comment", "\u0009"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!\\u000A",
// "input":"<!\u000A",
// "output":[["Comment", "\u000A"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!\\u000B",
// "input":"<!\u000B",
// "output":[["Comment", "\u000B"]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 3 },
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!\\u000C",
// "input":"<!\u000C",
// "output":[["Comment", "\u000C"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<! ",
// "input":"<! ",
// "output":[["Comment", " "]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<! \\u0000",
// "input":"<! \u0000",
// "output":[["Comment", " \uFFFD"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 },
//     { "code": "unexpected-null-character", "line": 1, "col": 4 }
// ]},

// {"description":"<!!",
// "input":"<!!",
// "output":[["Comment", "!"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!\"",
// "input":"<!\"",
// "output":[["Comment", "\""]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!&",
// "input":"<!&",
// "output":[["Comment", "&"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!'",
// "input":"<!'",
// "output":[["Comment", "'"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!-",
// "input":"<!-",
// "output":[["Comment", "-"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!--",
// "input":"<!--",
// "output":[["Comment", ""]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 5 }
// ]},

// {"description":"<!--\\u0000",
// "input":"<!--\u0000",
// "output":[["Comment", "\uFFFD"]],
// "errors":[
//     { "code": "unexpected-null-character", "line": 1, "col": 5 },
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--\\u0009",
// "input":"<!--\u0009",
// "output":[["Comment", "\u0009"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--\\u000A",
// "input":"<!--\u000A",
// "output":[["Comment", "\u000A"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 2, "col": 1 }
// ]},

// {"description":"<!--\\u000B",
// "input":"<!--\u000B",
// "output":[["Comment", "\u000B"]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 5 },
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--\\u000C",
// "input":"<!--\u000C",
// "output":[["Comment", "\u000C"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!-- ",
// "input":"<!-- ",
// "output":[["Comment", " "]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!-- \\u0000",
// "input":"<!-- \u0000",
// "output":[["Comment", " \uFFFD"]],
// "errors":[
//     { "code": "unexpected-null-character", "line": 1, "col": 6 },
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- \\u0009",
// "input":"<!-- \u0009",
// "output":[["Comment", " \u0009"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- \\u000A",
// "input":"<!-- \u000A",
// "output":[["Comment", " \u000A"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 2, "col": 1 }
// ]},

// {"description":"<!-- \\u000B",
// "input":"<!-- \u000B",
// "output":[["Comment", " \u000B"]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 6 },
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- \\u000C",
// "input":"<!-- \u000C",
// "output":[["Comment", " \u000C"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!--  ",
// "input":"<!--  ",
// "output":[["Comment", "  "]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- !",
// "input":"<!-- !",
// "output":[["Comment", " !"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- \"",
// "input":"<!-- \"",
// "output":[["Comment", " \""]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- &",
// "input":"<!-- &",
// "output":[["Comment", " &"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- '",
// "input":"<!-- '",
// "output":[["Comment", " '"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- ,",
// "input":"<!-- ,",
// "output":[["Comment", " ,"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- -",
// "input":"<!-- -",
// "output":[["Comment", " "]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- -\\u0000",
// "input":"<!-- -\u0000",
// "output":[["Comment", " -\uFFFD"]],
// "errors":[
//     { "code": "unexpected-null-character", "line": 1, "col": 7 },
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -\\u0009",
// "input":"<!-- -\u0009",
// "output":[["Comment", " -\u0009"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -\\u000A",
// "input":"<!-- -\u000A",
// "output":[["Comment", " -\u000A"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 2, "col": 1 }
// ]},

// {"description":"<!-- -\\u000B",
// "input":"<!-- -\u000B",
// "output":[["Comment", " -\u000B"]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 7 },
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -\\u000C",
// "input":"<!-- -\u000C",
// "output":[["Comment", " -\u000C"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- - ",
// "input":"<!-- - ",
// "output":[["Comment", " - "]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -!",
// "input":"<!-- -!",
// "output":[["Comment", " -!"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -\"",
// "input":"<!-- -\"",
// "output":[["Comment", " -\""]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -&",
// "input":"<!-- -&",
// "output":[["Comment", " -&"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -'",
// "input":"<!-- -'",
// "output":[["Comment", " -'"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -,",
// "input":"<!-- -,",
// "output":[["Comment", " -,"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- --",
// "input":"<!-- --",
// "output":[["Comment", " "]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -.",
// "input":"<!-- -.",
// "output":[["Comment", " -."]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -/",
// "input":"<!-- -/",
// "output":[["Comment", " -/"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -0",
// "input":"<!-- -0",
// "output":[["Comment", " -0"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -1",
// "input":"<!-- -1",
// "output":[["Comment", " -1"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -9",
// "input":"<!-- -9",
// "output":[["Comment", " -9"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -<",
// "input":"<!-- -<",
// "output":[["Comment", " -<"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -=",
// "input":"<!-- -=",
// "output":[["Comment", " -="]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- ->",
// "input":"<!-- ->",
// "output":[["Comment", " ->"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -?",
// "input":"<!-- -?",
// "output":[["Comment", " -?"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -@",
// "input":"<!-- -@",
// "output":[["Comment", " -@"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -A",
// "input":"<!-- -A",
// "output":[["Comment", " -A"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -B",
// "input":"<!-- -B",
// "output":[["Comment", " -B"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -Y",
// "input":"<!-- -Y",
// "output":[["Comment", " -Y"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -Z",
// "input":"<!-- -Z",
// "output":[["Comment", " -Z"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -`",
// "input":"<!-- -`",
// "output":[["Comment", " -`"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -a",
// "input":"<!-- -a",
// "output":[["Comment", " -a"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -b",
// "input":"<!-- -b",
// "output":[["Comment", " -b"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -y",
// "input":"<!-- -y",
// "output":[["Comment", " -y"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -z",
// "input":"<!-- -z",
// "output":[["Comment", " -z"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -{",
// "input":"<!-- -{",
// "output":[["Comment", " -{"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-- -\\uDBC0\\uDC00",
// "input":"<!-- -\uDBC0\uDC00",
// "output":[["Comment", " -\uDBC0\uDC00"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 9 }
// ]},

// {"description":"<!-- .",
// "input":"<!-- .",
// "output":[["Comment", " ."]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- /",
// "input":"<!-- /",
// "output":[["Comment", " /"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- 0",
// "input":"<!-- 0",
// "output":[["Comment", " 0"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- 1",
// "input":"<!-- 1",
// "output":[["Comment", " 1"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- 9",
// "input":"<!-- 9",
// "output":[["Comment", " 9"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- <",
// "input":"<!-- <",
// "output":[["Comment", " <"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- =",
// "input":"<!-- =",
// "output":[["Comment", " ="]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- >",
// "input":"<!-- >",
// "output":[["Comment", " >"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- ?",
// "input":"<!-- ?",
// "output":[["Comment", " ?"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- @",
// "input":"<!-- @",
// "output":[["Comment", " @"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- A",
// "input":"<!-- A",
// "output":[["Comment", " A"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- B",
// "input":"<!-- B",
// "output":[["Comment", " B"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- Y",
// "input":"<!-- Y",
// "output":[["Comment", " Y"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- Z",
// "input":"<!-- Z",
// "output":[["Comment", " Z"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- `",
// "input":"<!-- `",
// "output":[["Comment", " `"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- a",
// "input":"<!-- a",
// "output":[["Comment", " a"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- b",
// "input":"<!-- b",
// "output":[["Comment", " b"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- y",
// "input":"<!-- y",
// "output":[["Comment", " y"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- z",
// "input":"<!-- z",
// "output":[["Comment", " z"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- {",
// "input":"<!-- {",
// "output":[["Comment", " {"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!-- \\uDBC0\\uDC00",
// "input":"<!-- \uDBC0\uDC00",
// "output":[["Comment", " \uDBC0\uDC00"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!--!",
// "input":"<!--!",
// "output":[["Comment", "!"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--\"",
// "input":"<!--\"",
// "output":[["Comment", "\""]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--&",
// "input":"<!--&",
// "output":[["Comment", "&"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--'",
// "input":"<!--'",
// "output":[["Comment", "'"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--,",
// "input":"<!--,",
// "output":[["Comment", ","]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!---",
// "input":"<!---",
// "output":[["Comment", ""]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!---\\u0000",
// "input":"<!---\u0000",
// "output":[["Comment", "-\uFFFD"]],
// "errors":[
//     { "code": "unexpected-null-character", "line": 1, "col": 6 },
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---\\u0009",
// "input":"<!---\u0009",
// "output":[["Comment", "-\u0009"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---\\u000A",
// "input":"<!---\u000A",
// "output":[["Comment", "-\u000A"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 2, "col": 1 }
// ]},

// {"description":"<!---\\u000B",
// "input":"<!---\u000B",
// "output":[["Comment", "-\u000B"]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 6 },
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---\\u000C",
// "input":"<!---\u000C",
// "output":[["Comment", "-\u000C"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!--- ",
// "input":"<!--- ",
// "output":[["Comment", "- "]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---!",
// "input":"<!---!",
// "output":[["Comment", "-!"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---\"",
// "input":"<!---\"",
// "output":[["Comment", "-\""]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---&",
// "input":"<!---&",
// "output":[["Comment", "-&"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---'",
// "input":"<!---'",
// "output":[["Comment", "-'"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---,",
// "input":"<!---,",
// "output":[["Comment", "-,"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!----",
// "input":"<!----",
// "output":[["Comment", ""]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!----\\u0000",
// "input":"<!----\u0000",
// "output":[["Comment", "--\uFFFD"]],
// "errors":[
//     { "code": "unexpected-null-character", "line": 1, "col": 7 },
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----\\u0009",
// "input":"<!----\u0009",
// "output":[["Comment", "--\u0009"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----\\u000A",
// "input":"<!----\u000A",
// "output":[["Comment", "--\u000A"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 2, "col": 1 }
// ]},

// {"description":"<!----\\u000B",
// "input":"<!----\u000B",
// "output":[["Comment", "--\u000B"]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 7 },
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----\\u000C",
// "input":"<!----\u000C",
// "output":[["Comment", "--\u000C"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!---- ",
// "input":"<!---- ",
// "output":[["Comment", "-- "]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!---- -",
// "input":"<!---- -",
// "output":[["Comment", "-- "]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 9 }
// ]},


// {"description":"<!---- --",
// "input":"<!---- --",
// "output":[["Comment", "-- "]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 10 }
// ]},

// {"description":"<!---- -->",
// "input":"<!---- -->",
// "output":[["Comment", "-- "]]},

// {"description":"<!----  -->",
// "input":"<!----  -->",
// "output":[["Comment", "--  "]]},

// {"description":"<!---- a-->",
// "input":"<!---- a-->",
// "output":[["Comment", "-- a"]]},

// {"description":"<!----!",
// "input":"<!----!",
// "output":[["Comment", ""]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----!>",
// "input":"<!----!>",
// "output":[["Comment", ""]],
// "errors":[
//     { "code": "incorrectly-closed-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----! >",
// "input":"<!----! >",
// "output":[["Comment", "--! >"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 10 }
// ]},

// {"description":"<!----!LF>",
// "input":"<!----!\n>",
// "output":[["Comment", "--!\n>"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 2, "col": 2 }
// ]},

// {"description":"<!----!CR>",
// "input":"<!----!\r>",
// "output":[["Comment", "--!\n>"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 2, "col": 2 }
// ]},

// {"description":"<!----!CRLF>",
// "input":"<!----!\r\n>",
// "output":[["Comment", "--!\n>"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 2, "col": 2 }
// ]},

// {"description":"<!----!a",
// "input":"<!----!a",
// "output":[["Comment", "--!a"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 9 }
// ]},

// {"description":"<!----!a-",
// "input":"<!----!a-",
// "output":[["Comment", "--!a"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 10 }
// ]},

// {"description":"<!----!a--",
// "input":"<!----!a--",
// "output":[["Comment", "--!a"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 11 }
// ]},

// {"description":"<!----!a-->",
// "input":"<!----!a-->",
// "output":[["Comment", "--!a"]]},

// {"description":"<!----!-",
// "input":"<!----!-",
// "output":[["Comment", "--!"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 9 }
// ]},

// {"description":"<!----!--",
// "input":"<!----!--",
// "output":[["Comment", "--!"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 10 }
// ]},

// {"description":"<!----!-->",
// "input":"<!----!-->",
// "output":[["Comment", "--!"]]},

// {"description":"<!----\"",
// "input":"<!----\"",
// "output":[["Comment", "--\""]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----&",
// "input":"<!----&",
// "output":[["Comment", "--&"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----'",
// "input":"<!----'",
// "output":[["Comment", "--'"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----,",
// "input":"<!----,",
// "output":[["Comment", "--,"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!-----",
// "input":"<!-----",
// "output":[["Comment", "-"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----.",
// "input":"<!----.",
// "output":[["Comment", "--."]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----/",
// "input":"<!----/",
// "output":[["Comment", "--/"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----0",
// "input":"<!----0",
// "output":[["Comment", "--0"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----1",
// "input":"<!----1",
// "output":[["Comment", "--1"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----9",
// "input":"<!----9",
// "output":[["Comment", "--9"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----<",
// "input":"<!----<",
// "output":[["Comment", "--<"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----=",
// "input":"<!----=",
// "output":[["Comment", "--="]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!---->",
// "input":"<!---->",
// "output":[["Comment", ""]]},

// {"description":"<!----?",
// "input":"<!----?",
// "output":[["Comment", "--?"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----@",
// "input":"<!----@",
// "output":[["Comment", "--@"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----A",
// "input":"<!----A",
// "output":[["Comment", "--A"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----B",
// "input":"<!----B",
// "output":[["Comment", "--B"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----Y",
// "input":"<!----Y",
// "output":[["Comment", "--Y"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----Z",
// "input":"<!----Z",
// "output":[["Comment", "--Z"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----`",
// "input":"<!----`",
// "output":[["Comment", "--`"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----a",
// "input":"<!----a",
// "output":[["Comment", "--a"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----b",
// "input":"<!----b",
// "output":[["Comment", "--b"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----y",
// "input":"<!----y",
// "output":[["Comment", "--y"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----z",
// "input":"<!----z",
// "output":[["Comment", "--z"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----{",
// "input":"<!----{",
// "output":[["Comment", "--{"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!----\\uDBC0\\uDC00",
// "input":"<!----\uDBC0\uDC00",
// "output":[["Comment", "--\uDBC0\uDC00"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 9 }
// ]},

// {"description":"<!---.",
// "input":"<!---.",
// "output":[["Comment", "-."]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---/",
// "input":"<!---/",
// "output":[["Comment", "-/"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---0",
// "input":"<!---0",
// "output":[["Comment", "-0"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---1",
// "input":"<!---1",
// "output":[["Comment", "-1"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---9",
// "input":"<!---9",
// "output":[["Comment", "-9"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---<",
// "input":"<!---<",
// "output":[["Comment", "-<"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---=",
// "input":"<!---=",
// "output":[["Comment", "-="]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---?",
// "input":"<!---?",
// "output":[["Comment", "-?"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---@",
// "input":"<!---@",
// "output":[["Comment", "-@"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---A",
// "input":"<!---A",
// "output":[["Comment", "-A"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---B",
// "input":"<!---B",
// "output":[["Comment", "-B"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---Y",
// "input":"<!---Y",
// "output":[["Comment", "-Y"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---Z",
// "input":"<!---Z",
// "output":[["Comment", "-Z"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---`",
// "input":"<!---`",
// "output":[["Comment", "-`"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---a",
// "input":"<!---a",
// "output":[["Comment", "-a"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---b",
// "input":"<!---b",
// "output":[["Comment", "-b"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---y",
// "input":"<!---y",
// "output":[["Comment", "-y"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---z",
// "input":"<!---z",
// "output":[["Comment", "-z"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---{",
// "input":"<!---{",
// "output":[["Comment", "-{"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!---\\uDBC0\\uDC00",
// "input":"<!---\uDBC0\uDC00",
// "output":[["Comment", "-\uDBC0\uDC00"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 8 }
// ]},

// {"description":"<!--.",
// "input":"<!--.",
// "output":[["Comment", "."]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--/",
// "input":"<!--/",
// "output":[["Comment", "/"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--0",
// "input":"<!--0",
// "output":[["Comment", "0"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--1",
// "input":"<!--1",
// "output":[["Comment", "1"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--9",
// "input":"<!--9",
// "output":[["Comment", "9"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--<",
// "input":"<!--<",
// "output":[["Comment", "<"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--=",
// "input":"<!--=",
// "output":[["Comment", "="]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--?",
// "input":"<!--?",
// "output":[["Comment", "?"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--@",
// "input":"<!--@",
// "output":[["Comment", "@"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--A",
// "input":"<!--A",
// "output":[["Comment", "A"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--B",
// "input":"<!--B",
// "output":[["Comment", "B"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--Y",
// "input":"<!--Y",
// "output":[["Comment", "Y"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--Z",
// "input":"<!--Z",
// "output":[["Comment", "Z"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--`",
// "input":"<!--`",
// "output":[["Comment", "`"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--a",
// "input":"<!--a",
// "output":[["Comment", "a"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--b",
// "input":"<!--b",
// "output":[["Comment", "b"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--y",
// "input":"<!--y",
// "output":[["Comment", "y"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--z",
// "input":"<!--z",
// "output":[["Comment", "z"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--{",
// "input":"<!--{",
// "output":[["Comment", "{"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 6 }
// ]},

// {"description":"<!--\\uDBC0\\uDC00",
// "input":"<!--\uDBC0\uDC00",
// "output":[["Comment", "\uDBC0\uDC00"]],
// "errors":[
//     { "code": "eof-in-comment", "line": 1, "col": 7 }
// ]},

// {"description":"<!/",
// "input":"<!/",
// "output":[["Comment", "/"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!0",
// "input":"<!0",
// "output":[["Comment", "0"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!1",
// "input":"<!1",
// "output":[["Comment", "1"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!9",
// "input":"<!9",
// "output":[["Comment", "9"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!<",
// "input":"<!<",
// "output":[["Comment", "<"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!=",
// "input":"<!=",
// "output":[["Comment", "="]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!>",
// "input":"<!>",
// "output":[["Comment", ""]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!?",
// "input":"<!?",
// "output":[["Comment", "?"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!@",
// "input":"<!@",
// "output":[["Comment", "@"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!A",
// "input":"<!A",
// "output":[["Comment", "A"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!B",
// "input":"<!B",
// "output":[["Comment", "B"]],
// "errors":[
//     { "code": "incorrectly-opened-comment", "line": 1, "col": 3 }
// ]},

// {"description":"<!DOCTYPE",
// "input":"<!DOCTYPE",
// "output":[["DOCTYPE", null, null, null, false]],
// "errors":[
//     { "code": "eof-in-doctype", "line": 1, "col": 10 }
// ]},

// {"description":"<!DOCTYPE\\u0000",
// "input":"<!DOCTYPE\u0000",
// "output":[["DOCTYPE", "\uFFFD", null, null, false]],
// "errors":[
//     { "code": "missing-whitespace-before-doctype-name", "line": 1, "col": 10 },
//     { "code": "unexpected-null-character", "line": 1, "col": 10 },
//     { "code": "eof-in-doctype", "line": 1, "col": 11 }
// ]},

// {"description":"<!DOCTYPE\\u0008",
// "input":"<!DOCTYPE\u0008",
// "output":[["DOCTYPE", "\u0008", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 10 },
//     { "code": "missing-whitespace-before-doctype-name", "line": 1, "col": 10 },
//     { "code": "eof-in-doctype", "line": 1, "col": 11 }
// ]},

// {"description":"<!DOCTYPE\\u0009",
// "input":"<!DOCTYPE\u0009",
// "output":[["DOCTYPE", null, null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 11 }
// ]},

// {"description":"<!DOCTYPE\\u000A",
// "input":"<!DOCTYPE\u000A",
// "output":[["DOCTYPE", null, null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 2, "col": 1 }
// ]},

// {"description":"<!DOCTYPE\\u000B",
// "input":"<!DOCTYPE\u000B",
// "output":[["DOCTYPE", "\u000B", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 10 },
//     { "code": "missing-whitespace-before-doctype-name", "line": 1, "col": 10 },
//     { "code": "eof-in-doctype", "line": 1, "col": 11 }
// ]},

// {"description":"<!DOCTYPE\\u000C",
// "input":"<!DOCTYPE\u000C",
// "output":[["DOCTYPE", null, null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 11 }
// ]},

// {"description":"<!DOCTYPE\\u000D",
// "input":"<!DOCTYPE\u000D",
// "output":[["DOCTYPE", null, null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 2, "col": 1 }
// ]},

// {"description":"<!DOCTYPE\\u001F",
// "input":"<!DOCTYPE\u001F",
// "output":[["DOCTYPE", "\u001F", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 10 },
//     { "code": "missing-whitespace-before-doctype-name", "line": 1, "col": 10 },
//     { "code": "eof-in-doctype", "line": 1, "col": 11 }
// ]},

// {"description":"<!DOCTYPE ",
// "input":"<!DOCTYPE ",
// "output":[["DOCTYPE", null, null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 11 }
// ]},

// {"description":"<!DOCTYPE \\u0000",
// "input":"<!DOCTYPE \u0000",
// "output":[["DOCTYPE", "\uFFFD", null, null, false]],
// "errors":[
//    { "code": "unexpected-null-character", "line": 1, "col": 11 },
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE \\u0008",
// "input":"<!DOCTYPE \u0008",
// "output":[["DOCTYPE", "\u0008", null, null, false]],
// "errors":[
//    { "code": "control-character-in-input-stream", "line": 1, "col": 11 },
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE \\u0009",
// "input":"<!DOCTYPE \u0009",
// "output":[["DOCTYPE", null, null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE \\u000A",
// "input":"<!DOCTYPE \u000A",
// "output":[["DOCTYPE", null, null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 2, "col": 1 }
// ]},

// {"description":"<!DOCTYPE \\u000B",
// "input":"<!DOCTYPE \u000B",
// "output":[["DOCTYPE", "\u000B", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 11 },
//     { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE \\u000C",
// "input":"<!DOCTYPE \u000C",
// "output":[["DOCTYPE", null, null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE \\u000D",
// "input":"<!DOCTYPE \u000D",
// "output":[["DOCTYPE", null, null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 2, "col": 1 }
// ]},

// {"description":"<!DOCTYPE \\u001F",
// "input":"<!DOCTYPE \u001F",
// "output":[["DOCTYPE", "\u001F", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 11 },
//     { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE  ",
// "input":"<!DOCTYPE  ",
// "output":[["DOCTYPE", null, null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE !",
// "input":"<!DOCTYPE !",
// "output":[["DOCTYPE", "!", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE \"",
// "input":"<!DOCTYPE \"",
// "output":[["DOCTYPE", "\"", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE &",
// "input":"<!DOCTYPE &",
// "output":[["DOCTYPE", "&", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE '",
// "input":"<!DOCTYPE '",
// "output":[["DOCTYPE", "'", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE -",
// "input":"<!DOCTYPE -",
// "output":[["DOCTYPE", "-", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE /",
// "input":"<!DOCTYPE /",
// "output":[["DOCTYPE", "/", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE 0",
// "input":"<!DOCTYPE 0",
// "output":[["DOCTYPE", "0", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE 1",
// "input":"<!DOCTYPE 1",
// "output":[["DOCTYPE", "1", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE 9",
// "input":"<!DOCTYPE 9",
// "output":[["DOCTYPE", "9", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE <",
// "input":"<!DOCTYPE <",
// "output":[["DOCTYPE", "<", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE =",
// "input":"<!DOCTYPE =",
// "output":[["DOCTYPE", "=", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE >",
// "input":"<!DOCTYPE >",
// "output":[["DOCTYPE", null, null, null, false]],
// "errors":[
//     { "code": "missing-doctype-name", "line": 1, "col": 11 }
// ]},

// {"description":"<!DOCTYPE ?",
// "input":"<!DOCTYPE ?",
// "output":[["DOCTYPE", "?", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE @",
// "input":"<!DOCTYPE @",
// "output":[["DOCTYPE", "@", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE A",
// "input":"<!DOCTYPE A",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE B",
// "input":"<!DOCTYPE B",
// "output":[["DOCTYPE", "b", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE Y",
// "input":"<!DOCTYPE Y",
// "output":[["DOCTYPE", "y", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE Z",
// "input":"<!DOCTYPE Z",
// "output":[["DOCTYPE", "z", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE [",
// "input":"<!DOCTYPE [",
// "output":[["DOCTYPE", "[", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE `",
// "input":"<!DOCTYPE `",
// "output":[["DOCTYPE", "`", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE a",
// "input":"<!DOCTYPE a",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//    { "code": "eof-in-doctype", "line": 1, "col": 12 }
// ]},

// {"description":"<!DOCTYPE a\\u0000",
// "input":"<!DOCTYPE a\u0000",
// "output":[["DOCTYPE", "a\uFFFD", null, null, false]],
// "errors":[
//     { "code": "unexpected-null-character", "line": 1, "col": 12 },
//     { "code": "eof-in-doctype", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a\\u0008",
// "input":"<!DOCTYPE a\u0008",
// "output":[["DOCTYPE", "a\u0008", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 12 },
//     { "code": "eof-in-doctype", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a\\u0009",
// "input":"<!DOCTYPE a\u0009",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "eof-in-doctype", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a\\u000A",
// "input":"<!DOCTYPE a\u000A",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "eof-in-doctype", "line": 2, "col": 1 }
// ]},

// {"description":"<!DOCTYPE a\\u000B",
// "input":"<!DOCTYPE a\u000B",
// "output":[["DOCTYPE", "a\u000B", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 12 },
//     { "code": "eof-in-doctype", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a\\u000C",
// "input":"<!DOCTYPE a\u000C",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "eof-in-doctype", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a\\u000D",
// "input":"<!DOCTYPE a\u000D",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "eof-in-doctype", "line": 2, "col": 1 }
// ]},

// {"description":"<!DOCTYPE a\\u001F",
// "input":"<!DOCTYPE a\u001F",
// "output":[["DOCTYPE", "a\u001F", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 12 },
//     { "code": "eof-in-doctype", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a ",
// "input":"<!DOCTYPE a ",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "eof-in-doctype", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a \\u0000",
// "input":"<!DOCTYPE a \u0000",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 },
//     { "code": "unexpected-null-character", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a \\u0008",
// "input":"<!DOCTYPE a \u0008",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 13 },
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a \\u0009",
// "input":"<!DOCTYPE a \u0009",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "eof-in-doctype", "line": 1, "col": 14 }
// ]},

// {"description":"<!DOCTYPE a \\u000A",
// "input":"<!DOCTYPE a \u000A",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "eof-in-doctype", "line": 2, "col": 1 }
// ]},

// {"description":"<!DOCTYPE a \\u000B",
// "input":"<!DOCTYPE a \u000B",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 13 },
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a \\u000C",
// "input":"<!DOCTYPE a \u000C",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "eof-in-doctype", "line": 1, "col": 14 }
// ]},

// {"description":"<!DOCTYPE a \\u000D",
// "input":"<!DOCTYPE a \u000D",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "eof-in-doctype", "line": 2, "col": 1 }
// ]},

// {"description":"<!DOCTYPE a \\u001F",
// "input":"<!DOCTYPE a \u001F",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 13 },
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a  ",
// "input":"<!DOCTYPE a  ",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "eof-in-doctype", "line": 1, "col": 14 }
// ]},

// {"description":"<!DOCTYPE a !",
// "input":"<!DOCTYPE a !",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a \"",
// "input":"<!DOCTYPE a \"",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a &",
// "input":"<!DOCTYPE a &",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a '",
// "input":"<!DOCTYPE a '",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a -",
// "input":"<!DOCTYPE a -",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a /",
// "input":"<!DOCTYPE a /",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a 0",
// "input":"<!DOCTYPE a 0",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a 1",
// "input":"<!DOCTYPE a 1",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a 9",
// "input":"<!DOCTYPE a 9",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a <",
// "input":"<!DOCTYPE a <",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a =",
// "input":"<!DOCTYPE a =",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a >",
// "input":"<!DOCTYPE a >",
// "output":[["DOCTYPE", "a", null, null, true]]},

// {"description":"<!DOCTYPE a ?",
// "input":"<!DOCTYPE a ?",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a @",
// "input":"<!DOCTYPE a @",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a A",
// "input":"<!DOCTYPE a A",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a B",
// "input":"<!DOCTYPE a B",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "invalid-character-sequence-after-doctype-name", "line": 1, "col": 13 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC",
// "input":"<!DOCTYPE a PUBLIC",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "eof-in-doctype", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\\u0000",
// "input":"<!DOCTYPE a PUBLIC\u0000",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 },
//     { "code": "unexpected-null-character", "line": 1, "col": 19 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\\u0008",
// "input":"<!DOCTYPE a PUBLIC\u0008",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 19 },
//     { "code": "missing-quote-before-doctype-public-identifier", "line": 1, "col": 19 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\\u0009",
// "input":"<!DOCTYPE a PUBLIC\u0009",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "eof-in-doctype", "col": 20, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\\u000A",
// "input":"<!DOCTYPE a PUBLIC\u000A",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "eof-in-doctype", "col": 1, "line": 2 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\\u000B",
// "input":"<!DOCTYPE a PUBLIC\u000B",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 19 },
//     { "code": "missing-quote-before-doctype-public-identifier", "line": 1, "col": 19 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\\u000C",
// "input":"<!DOCTYPE a PUBLIC\u000C",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "eof-in-doctype", "col": 20, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\\u000D",
// "input":"<!DOCTYPE a PUBLIC\u000D",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "eof-in-doctype", "col": 1, "line": 2 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\\u001F",
// "input":"<!DOCTYPE a PUBLIC\u001F",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 19 },
//     { "code": "missing-quote-before-doctype-public-identifier", "line": 1, "col": 19 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC ",
// "input":"<!DOCTYPE a PUBLIC ",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "eof-in-doctype", "col": 20, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC!",
// "input":"<!DOCTYPE a PUBLIC!",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"",
// "input":"<!DOCTYPE a PUBLIC\"",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 20, "line": 1 }

// ]},

// {"description":"<!DOCTYPE a PUBLIC\"\\u0000",
// "input":"<!DOCTYPE a PUBLIC\"\u0000",
// "output":[["DOCTYPE", "a", "\uFFFD", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "unexpected-null-character", "col": 20, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"\\u0009",
// "input":"<!DOCTYPE a PUBLIC\"\u0009",
// "output":[["DOCTYPE", "a", "\u0009", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"\\u000A",
// "input":"<!DOCTYPE a PUBLIC\"\u000A",
// "output":[["DOCTYPE", "a", "\u000A", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 1, "line": 2 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"\\u000B",
// "input":"<!DOCTYPE a PUBLIC\"\u000B",
// "output":[["DOCTYPE", "a", "\u000B", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "control-character-in-input-stream", "line": 1, "col": 20 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"\\u000C",
// "input":"<!DOCTYPE a PUBLIC\"\u000C",
// "output":[["DOCTYPE", "a", "\u000C", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\" ",
// "input":"<!DOCTYPE a PUBLIC\" ",
// "output":[["DOCTYPE", "a", " ", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"!",
// "input":"<!DOCTYPE a PUBLIC\"!",
// "output":[["DOCTYPE", "a", "!", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"\"",
// "input":"<!DOCTYPE a PUBLIC\"\"",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"\"\\u0000",
// "input":"<!DOCTYPE a PUBLIC\"\"\u0000",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 },
//     { "code": "unexpected-null-character", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"\" \\u0000",
// "input":"<!DOCTYPE a PUBLIC\"\" \u0000",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 22, "line": 1 },
//     { "code": "unexpected-null-character", "col": 22, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"#",
// "input":"<!DOCTYPE a PUBLIC\"#",
// "output":[["DOCTYPE", "a", "#", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"&",
// "input":"<!DOCTYPE a PUBLIC\"&",
// "output":[["DOCTYPE", "a", "&", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"'",
// "input":"<!DOCTYPE a PUBLIC\"'",
// "output":[["DOCTYPE", "a", "'", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"-",
// "input":"<!DOCTYPE a PUBLIC\"-",
// "output":[["DOCTYPE", "a", "-", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"/",
// "input":"<!DOCTYPE a PUBLIC\"/",
// "output":[["DOCTYPE", "a", "/", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"0",
// "input":"<!DOCTYPE a PUBLIC\"0",
// "output":[["DOCTYPE", "a", "0", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"1",
// "input":"<!DOCTYPE a PUBLIC\"1",
// "output":[["DOCTYPE", "a", "1", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"9",
// "input":"<!DOCTYPE a PUBLIC\"9",
// "output":[["DOCTYPE", "a", "9", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"<",
// "input":"<!DOCTYPE a PUBLIC\"<",
// "output":[["DOCTYPE", "a", "<", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"=",
// "input":"<!DOCTYPE a PUBLIC\"=",
// "output":[["DOCTYPE", "a", "=", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\">",
// "input":"<!DOCTYPE a PUBLIC\">",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "abrupt-doctype-public-identifier", "col": 20, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"?",
// "input":"<!DOCTYPE a PUBLIC\"?",
// "output":[["DOCTYPE", "a", "?", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"@",
// "input":"<!DOCTYPE a PUBLIC\"@",
// "output":[["DOCTYPE", "a", "@", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"A",
// "input":"<!DOCTYPE a PUBLIC\"A",
// "output":[["DOCTYPE", "a", "A", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"B",
// "input":"<!DOCTYPE a PUBLIC\"B",
// "output":[["DOCTYPE", "a", "B", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"Y",
// "input":"<!DOCTYPE a PUBLIC\"Y",
// "output":[["DOCTYPE", "a", "Y", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"Z",
// "input":"<!DOCTYPE a PUBLIC\"Z",
// "output":[["DOCTYPE", "a", "Z", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"`",
// "input":"<!DOCTYPE a PUBLIC\"`",
// "output":[["DOCTYPE", "a", "`", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"a",
// "input":"<!DOCTYPE a PUBLIC\"a",
// "output":[["DOCTYPE", "a", "a", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"b",
// "input":"<!DOCTYPE a PUBLIC\"b",
// "output":[["DOCTYPE", "a", "b", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"y",
// "input":"<!DOCTYPE a PUBLIC\"y",
// "output":[["DOCTYPE", "a", "y", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"z",
// "input":"<!DOCTYPE a PUBLIC\"z",
// "output":[["DOCTYPE", "a", "z", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"{",
// "input":"<!DOCTYPE a PUBLIC\"{",
// "output":[["DOCTYPE", "a", "{", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\"\\uDBC0\\uDC00",
// "input":"<!DOCTYPE a PUBLIC\"\uDBC0\uDC00",
// "output":[["DOCTYPE", "a", "\uDBC0\uDC00", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 22, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC#",
// "input":"<!DOCTYPE a PUBLIC#",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC&",
// "input":"<!DOCTYPE a PUBLIC&",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'",
// "input":"<!DOCTYPE a PUBLIC'",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 20, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'\\u0000",
// "input":"<!DOCTYPE a PUBLIC'\u0000",
// "output":[["DOCTYPE", "a", "\uFFFD", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "unexpected-null-character", "col": 20, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'\\u0009",
// "input":"<!DOCTYPE a PUBLIC'\u0009",
// "output":[["DOCTYPE", "a", "\u0009", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'\\u000A",
// "input":"<!DOCTYPE a PUBLIC'\u000A",
// "output":[["DOCTYPE", "a", "\u000A", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 1, "line": 2 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'\\u000B",
// "input":"<!DOCTYPE a PUBLIC'\u000B",
// "output":[["DOCTYPE", "a", "\u000B", null, false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "control-character-in-input-stream", "line": 1, "col": 20 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'\\u000C",
// "input":"<!DOCTYPE a PUBLIC'\u000C",
// "output":[["DOCTYPE", "a", "\u000C", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC' ",
// "input":"<!DOCTYPE a PUBLIC' ",
// "output":[["DOCTYPE", "a", " ", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'!",
// "input":"<!DOCTYPE a PUBLIC'!",
// "output":[["DOCTYPE", "a", "!", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'\"",
// "input":"<!DOCTYPE a PUBLIC'\"",
// "output":[["DOCTYPE", "a", "\"", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'&",
// "input":"<!DOCTYPE a PUBLIC'&",
// "output":[["DOCTYPE", "a", "&", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''",
// "input":"<!DOCTYPE a PUBLIC''",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''\\u0000",
// "input":"<!DOCTYPE a PUBLIC''\u0000",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 },
//     { "code": "unexpected-null-character", "line": 1, "col": 21 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''\\u0008",
// "input":"<!DOCTYPE a PUBLIC''\u0008",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "control-character-in-input-stream", "line": 1, "col": 21 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''\\u0009",
// "input":"<!DOCTYPE a PUBLIC''\u0009",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 22, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''\\u000A",
// "input":"<!DOCTYPE a PUBLIC''\u000A",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 1, "line": 2 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''\\u000B",
// "input":"<!DOCTYPE a PUBLIC''\u000B",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "control-character-in-input-stream", "line": 1, "col": 21 },
//     { "code" : "missing-quote-before-doctype-system-identifier", "col": 21, "line" : 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''\\u000C",
// "input":"<!DOCTYPE a PUBLIC''\u000C",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 22, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''\\u000D",
// "input":"<!DOCTYPE a PUBLIC''\u000D",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 1, "line": 2 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''\\u001F",
// "input":"<!DOCTYPE a PUBLIC''\u001F",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "control-character-in-input-stream", "line": 1, "col": 21 },
//     { "code" : "missing-quote-before-doctype-system-identifier", "col": 21, "line" : 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'' ",
// "input":"<!DOCTYPE a PUBLIC'' ",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 22, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''!",
// "input":"<!DOCTYPE a PUBLIC''!",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''\"",
// "input":"<!DOCTYPE a PUBLIC''\"",
// "output":[["DOCTYPE", "a", "", "", false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-whitespace-between-doctype-public-and-system-identifiers", "col": 21, "line": 1 },
//     { "code": "eof-in-doctype", "col": 22, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''#",
// "input":"<!DOCTYPE a PUBLIC''#",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''&",
// "input":"<!DOCTYPE a PUBLIC''&",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'''",
// "input":"<!DOCTYPE a PUBLIC'''",
// "output":[["DOCTYPE", "a", "", "", false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-whitespace-between-doctype-public-and-system-identifiers", "col": 21, "line": 1 },
//     { "code": "eof-in-doctype", "col": 22, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''''\\u0000",
// "input":"<!DOCTYPE a PUBLIC''''\u0000",
// "output":[["DOCTYPE", "a", "", "", true]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-whitespace-between-doctype-public-and-system-identifiers", "col": 21, "line": 1 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "line": 1, "col": 23 },
//     { "code": "unexpected-null-character", "line": 1, "col": 23 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''''x\\u0000",
// "input":"<!DOCTYPE a PUBLIC''''x\u0000",
// "output":[["DOCTYPE", "a", "", "", true]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-whitespace-between-doctype-public-and-system-identifiers", "col": 21, "line": 1 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "line": 1, "col": 23 },
//     { "code": "unexpected-null-character", "line": 1, "col": 24 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'''' \\u0000",
// "input":"<!DOCTYPE a PUBLIC'''' \u0000",
// "output":[["DOCTYPE", "a", "", "", true]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-whitespace-between-doctype-public-and-system-identifiers", "col": 21, "line": 1 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "line": 1, "col": 24 },
//     { "code": "unexpected-null-character", "line": 1, "col": 24 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'''' x\\u0000",
// "input":"<!DOCTYPE a PUBLIC'''' x\u0000",
// "output":[["DOCTYPE", "a", "", "", true]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-whitespace-between-doctype-public-and-system-identifiers", "col": 21, "line": 1 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "line": 1, "col": 24 },
//     { "code": "unexpected-null-character", "line": 1, "col": 25 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''(",
// "input":"<!DOCTYPE a PUBLIC''(",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''-",
// "input":"<!DOCTYPE a PUBLIC''-",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''/",
// "input":"<!DOCTYPE a PUBLIC''/",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''0",
// "input":"<!DOCTYPE a PUBLIC''0",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''1",
// "input":"<!DOCTYPE a PUBLIC''1",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''9",
// "input":"<!DOCTYPE a PUBLIC''9",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''<",
// "input":"<!DOCTYPE a PUBLIC''<",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''=",
// "input":"<!DOCTYPE a PUBLIC''=",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''>",
// "input":"<!DOCTYPE a PUBLIC''>",
// "output":[["DOCTYPE", "a", "", null, true]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''?",
// "input":"<!DOCTYPE a PUBLIC''?",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''@",
// "input":"<!DOCTYPE a PUBLIC''@",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''A",
// "input":"<!DOCTYPE a PUBLIC''A",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''B",
// "input":"<!DOCTYPE a PUBLIC''B",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''Y",
// "input":"<!DOCTYPE a PUBLIC''Y",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''Z",
// "input":"<!DOCTYPE a PUBLIC''Z",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''`",
// "input":"<!DOCTYPE a PUBLIC''`",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''a",
// "input":"<!DOCTYPE a PUBLIC''a",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''b",
// "input":"<!DOCTYPE a PUBLIC''b",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''y",
// "input":"<!DOCTYPE a PUBLIC''y",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''z",
// "input":"<!DOCTYPE a PUBLIC''z",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''{",
// "input":"<!DOCTYPE a PUBLIC''{",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC''\\uDBC0\\uDC00",
// "input":"<!DOCTYPE a PUBLIC''\uDBC0\uDC00",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'(",
// "input":"<!DOCTYPE a PUBLIC'(",
// "output":[["DOCTYPE", "a", "(", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'-",
// "input":"<!DOCTYPE a PUBLIC'-",
// "output":[["DOCTYPE", "a", "-", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'/",
// "input":"<!DOCTYPE a PUBLIC'/",
// "output":[["DOCTYPE", "a", "/", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'0",
// "input":"<!DOCTYPE a PUBLIC'0",
// "output":[["DOCTYPE", "a", "0", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'1",
// "input":"<!DOCTYPE a PUBLIC'1",
// "output":[["DOCTYPE", "a", "1", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'9",
// "input":"<!DOCTYPE a PUBLIC'9",
// "output":[["DOCTYPE", "a", "9", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'<",
// "input":"<!DOCTYPE a PUBLIC'<",
// "output":[["DOCTYPE", "a", "<", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'=",
// "input":"<!DOCTYPE a PUBLIC'=",
// "output":[["DOCTYPE", "a", "=", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'>",
// "input":"<!DOCTYPE a PUBLIC'>",
// "output":[["DOCTYPE", "a", "", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "abrupt-doctype-public-identifier", "col": 20, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'?",
// "input":"<!DOCTYPE a PUBLIC'?",
// "output":[["DOCTYPE", "a", "?", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'@",
// "input":"<!DOCTYPE a PUBLIC'@",
// "output":[["DOCTYPE", "a", "@", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'A",
// "input":"<!DOCTYPE a PUBLIC'A",
// "output":[["DOCTYPE", "a", "A", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'B",
// "input":"<!DOCTYPE a PUBLIC'B",
// "output":[["DOCTYPE", "a", "B", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'Y",
// "input":"<!DOCTYPE a PUBLIC'Y",
// "output":[["DOCTYPE", "a", "Y", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'Z",
// "input":"<!DOCTYPE a PUBLIC'Z",
// "output":[["DOCTYPE", "a", "Z", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'`",
// "input":"<!DOCTYPE a PUBLIC'`",
// "output":[["DOCTYPE", "a", "`", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'a",
// "input":"<!DOCTYPE a PUBLIC'a",
// "output":[["DOCTYPE", "a", "a", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'b",
// "input":"<!DOCTYPE a PUBLIC'b",
// "output":[["DOCTYPE", "a", "b", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'y",
// "input":"<!DOCTYPE a PUBLIC'y",
// "output":[["DOCTYPE", "a", "y", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'z",
// "input":"<!DOCTYPE a PUBLIC'z",
// "output":[["DOCTYPE", "a", "z", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'{",
// "input":"<!DOCTYPE a PUBLIC'{",
// "output":[["DOCTYPE", "a", "{", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC'\\uDBC0\\uDC00",
// "input":"<!DOCTYPE a PUBLIC'\uDBC0\uDC00",
// "output":[["DOCTYPE", "a", "\uDBC0\uDC00", null, false]],
// "errors": [
//     { "code": "missing-whitespace-after-doctype-public-keyword", "col": 19, "line": 1 },
//     { "code": "eof-in-doctype", "col": 22, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC(",
// "input":"<!DOCTYPE a PUBLIC(",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC-",
// "input":"<!DOCTYPE a PUBLIC-",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC/",
// "input":"<!DOCTYPE a PUBLIC/",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC0",
// "input":"<!DOCTYPE a PUBLIC0",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC1",
// "input":"<!DOCTYPE a PUBLIC1",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC9",
// "input":"<!DOCTYPE a PUBLIC9",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC<",
// "input":"<!DOCTYPE a PUBLIC<",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC=",
// "input":"<!DOCTYPE a PUBLIC=",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC>",
// "input":"<!DOCTYPE a PUBLIC>",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC?",
// "input":"<!DOCTYPE a PUBLIC?",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC@",
// "input":"<!DOCTYPE a PUBLIC@",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLICA",
// "input":"<!DOCTYPE a PUBLICA",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLICB",
// "input":"<!DOCTYPE a PUBLICB",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLICY",
// "input":"<!DOCTYPE a PUBLICY",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLICZ",
// "input":"<!DOCTYPE a PUBLICZ",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC`",
// "input":"<!DOCTYPE a PUBLIC`",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLICa",
// "input":"<!DOCTYPE a PUBLICa",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLICb",
// "input":"<!DOCTYPE a PUBLICb",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLICy",
// "input":"<!DOCTYPE a PUBLICy",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLICz",
// "input":"<!DOCTYPE a PUBLICz",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC{",
// "input":"<!DOCTYPE a PUBLIC{",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a PUBLIC\\uDBC0\\uDC00",
// "input":"<!DOCTYPE a PUBLIC\uDBC0\uDC00",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-public-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM",
// "input":"<!DOCTYPE a SYSTEM",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "eof-in-doctype", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\\u0000",
// "input":"<!DOCTYPE a SYSTEM\u0000",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 19, "line": 1 },
//     { "code": "unexpected-null-character", "line": 1, "col": 19 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM \\u0000",
// "input":"<!DOCTYPE a SYSTEM \u0000",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 20, "line": 1 },
//     { "code": "unexpected-null-character", "line": 1, "col": 20 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM x\\u0000",
// "input":"<!DOCTYPE a SYSTEM \u0000",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 20, "line": 1 },
//     { "code": "unexpected-null-character", "line": 1, "col": 20 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\\u0008",
// "input":"<!DOCTYPE a SYSTEM\u0008",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 19 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\\u0009",
// "input":"<!DOCTYPE a SYSTEM\u0009",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "eof-in-doctype", "col": 20, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\\u000A",
// "input":"<!DOCTYPE a SYSTEM\u000A",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "eof-in-doctype", "col": 1, "line": 2 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\\u000B",
// "input":"<!DOCTYPE a SYSTEM\u000B",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 19 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\\u000C",
// "input":"<!DOCTYPE a SYSTEM\u000C",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "eof-in-doctype", "col": 20, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\\u000D",
// "input":"<!DOCTYPE a SYSTEM\u000D",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "eof-in-doctype", "col": 1, "line": 2 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\\u001F",
// "input":"<!DOCTYPE a SYSTEM\u001F",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "control-character-in-input-stream", "line": 1, "col": 19 },
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM ",
// "input":"<!DOCTYPE a SYSTEM ",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors": [
//     { "code": "eof-in-doctype", "col": 20, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM!",
// "input":"<!DOCTYPE a SYSTEM!",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "missing-quote-before-doctype-system-identifier", "col": 19, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"",
// "input":"<!DOCTYPE a SYSTEM\"",
// "output":[["DOCTYPE", "a", null, "", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 20, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"\\u0000",
// "input":"<!DOCTYPE a SYSTEM\"\u0000",
// "output":[["DOCTYPE", "a", null, "\uFFFD", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-null-character", "col": 20, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"\\u0009",
// "input":"<!DOCTYPE a SYSTEM\"\u0009",
// "output":[["DOCTYPE", "a", null, "\u0009", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"\\u000A",
// "input":"<!DOCTYPE a SYSTEM\"\u000A",
// "output":[["DOCTYPE", "a", null, "\u000A", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 1, "line": 2 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"\\u000B",
// "input":"<!DOCTYPE a SYSTEM\"\u000B",
// "output":[["DOCTYPE", "a", null, "\u000B", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "control-character-in-input-stream", "line": 1, "col": 20 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"\\u000C",
// "input":"<!DOCTYPE a SYSTEM\"\u000C",
// "output":[["DOCTYPE", "a", null, "\u000C", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\" ",
// "input":"<!DOCTYPE a SYSTEM\" ",
// "output":[["DOCTYPE", "a", null, " ", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"!",
// "input":"<!DOCTYPE a SYSTEM\"!",
// "output":[["DOCTYPE", "a", null, "!", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"\"",
// "input":"<!DOCTYPE a SYSTEM\"\"",
// "output":[["DOCTYPE", "a", null, "", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"#",
// "input":"<!DOCTYPE a SYSTEM\"#",
// "output":[["DOCTYPE", "a", null, "#", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"&",
// "input":"<!DOCTYPE a SYSTEM\"&",
// "output":[["DOCTYPE", "a", null, "&", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"'",
// "input":"<!DOCTYPE a SYSTEM\"'",
// "output":[["DOCTYPE", "a", null, "'", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"-",
// "input":"<!DOCTYPE a SYSTEM\"-",
// "output":[["DOCTYPE", "a", null, "-", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"/",
// "input":"<!DOCTYPE a SYSTEM\"/",
// "output":[["DOCTYPE", "a", null, "/", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"0",
// "input":"<!DOCTYPE a SYSTEM\"0",
// "output":[["DOCTYPE", "a", null, "0", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"1",
// "input":"<!DOCTYPE a SYSTEM\"1",
// "output":[["DOCTYPE", "a", null, "1", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"9",
// "input":"<!DOCTYPE a SYSTEM\"9",
// "output":[["DOCTYPE", "a", null, "9", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"<",
// "input":"<!DOCTYPE a SYSTEM\"<",
// "output":[["DOCTYPE", "a", null, "<", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"=",
// "input":"<!DOCTYPE a SYSTEM\"=",
// "output":[["DOCTYPE", "a", null, "=", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\">",
// "input":"<!DOCTYPE a SYSTEM\">",
// "output":[["DOCTYPE", "a", null, "", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "abrupt-doctype-system-identifier", "col": 20, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"?",
// "input":"<!DOCTYPE a SYSTEM\"?",
// "output":[["DOCTYPE", "a", null, "?", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"@",
// "input":"<!DOCTYPE a SYSTEM\"@",
// "output":[["DOCTYPE", "a", null, "@", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"A",
// "input":"<!DOCTYPE a SYSTEM\"A",
// "output":[["DOCTYPE", "a", null, "A", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"B",
// "input":"<!DOCTYPE a SYSTEM\"B",
// "output":[["DOCTYPE", "a", null, "B", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"Y",
// "input":"<!DOCTYPE a SYSTEM\"Y",
// "output":[["DOCTYPE", "a", null, "Y", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"Z",
// "input":"<!DOCTYPE a SYSTEM\"Z",
// "output":[["DOCTYPE", "a", null, "Z", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"`",
// "input":"<!DOCTYPE a SYSTEM\"`",
// "output":[["DOCTYPE", "a", null, "`", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"a",
// "input":"<!DOCTYPE a SYSTEM\"a",
// "output":[["DOCTYPE", "a", null, "a", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"b",
// "input":"<!DOCTYPE a SYSTEM\"b",
// "output":[["DOCTYPE", "a", null, "b", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"y",
// "input":"<!DOCTYPE a SYSTEM\"y",
// "output":[["DOCTYPE", "a", null, "y", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"z",
// "input":"<!DOCTYPE a SYSTEM\"z",
// "output":[["DOCTYPE", "a", null, "z", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"{",
// "input":"<!DOCTYPE a SYSTEM\"{",
// "output":[["DOCTYPE", "a", null, "{", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM\"\\uDBC0\\uDC00",
// "input":"<!DOCTYPE a SYSTEM\"\uDBC0\uDC00",
// "output":[["DOCTYPE", "a", null, "\uDBC0\uDC00", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 22, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM#",
// "input":"<!DOCTYPE a SYSTEM#",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "missing-quote-before-doctype-system-identifier", "line": 1, "col": 19 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM&",
// "input":"<!DOCTYPE a SYSTEM&",
// "output":[["DOCTYPE", "a", null, null, false]],
// "errors":[
//     { "code": "missing-quote-before-doctype-system-identifier", "line": 1, "col": 19 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM'",
// "input":"<!DOCTYPE a SYSTEM'",
// "output":[["DOCTYPE", "a", null, "", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 20, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM'\\u0000",
// "input":"<!DOCTYPE a SYSTEM'\u0000",
// "output":[["DOCTYPE", "a", null, "\uFFFD", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-null-character", "col": 20, "line": 1 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM'\\u0009",
// "input":"<!DOCTYPE a SYSTEM'\u0009",
// "output":[["DOCTYPE", "a", null, "\u0009", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM'\\u000A",
// "input":"<!DOCTYPE a SYSTEM'\u000A",
// "output":[["DOCTYPE", "a", null, "\u000A", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 1, "line": 2 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM'\\u000B",
// "input":"<!DOCTYPE a SYSTEM'\u000B",
// "output":[["DOCTYPE", "a", null, "\u000B", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "control-character-in-input-stream", "line": 1, "col": 20 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM'\\u000C",
// "input":"<!DOCTYPE a SYSTEM'\u000C",
// "output":[["DOCTYPE", "a", null, "\u000C", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM' ",
// "input":"<!DOCTYPE a SYSTEM' ",
// "output":[["DOCTYPE", "a", null, " ", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM'!",
// "input":"<!DOCTYPE a SYSTEM'!",
// "output":[["DOCTYPE", "a", null, "!", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM'\"",
// "input":"<!DOCTYPE a SYSTEM'\"",
// "output":[["DOCTYPE", "a", null, "\"", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM'&",
// "input":"<!DOCTYPE a SYSTEM'&",
// "output":[["DOCTYPE", "a", null, "&", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''",
// "input":"<!DOCTYPE a SYSTEM''",
// "output":[["DOCTYPE", "a", null, "", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''\\u0000",
// "input":"<!DOCTYPE a SYSTEM''\u0000",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 },
//     { "code": "unexpected-null-character", "line": 1, "col": 21 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''\\u0008",
// "input":"<!DOCTYPE a SYSTEM''\u0008",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "control-character-in-input-stream", "line": 1, "col": 21 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "line": 1, "col": 21 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''\\u0009",
// "input":"<!DOCTYPE a SYSTEM''\u0009",
// "output":[["DOCTYPE", "a", null, "", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 22, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''\\u000A",
// "input":"<!DOCTYPE a SYSTEM''\u000A",
// "output":[["DOCTYPE", "a", null, "", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 1, "line": 2 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''\\u000B",
// "input":"<!DOCTYPE a SYSTEM''\u000B",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "control-character-in-input-stream", "line": 1, "col": 21 },
//     { "code" : "unexpected-character-after-doctype-system-identifier", "line": 1, "col": 21}
// ]},

// {"description":"<!DOCTYPE a SYSTEM''\\u000C",
// "input":"<!DOCTYPE a SYSTEM''\u000C",
// "output":[["DOCTYPE", "a", null, "", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 22, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''\\u000D",
// "input":"<!DOCTYPE a SYSTEM''\u000D",
// "output":[["DOCTYPE", "a", null, "", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 1, "line": 2 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''\\u001F",
// "input":"<!DOCTYPE a SYSTEM''\u001F",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "control-character-in-input-stream", "line": 1, "col": 21 },
//     { "code" : "unexpected-character-after-doctype-system-identifier", "line": 1, "col": 21}
// ]},

// {"description":"<!DOCTYPE a SYSTEM'' ",
// "input":"<!DOCTYPE a SYSTEM'' ",
// "output":[["DOCTYPE", "a", null, "", false]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "eof-in-doctype", "col": 22, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM'' \\u0000",
// "input":"<!DOCTYPE a SYSTEM'' \u0000",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 22, "line": 1 },
//     { "code": "unexpected-null-character", "line": 1, "col": 22 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM'' x\\u0000",
// "input":"<!DOCTYPE a SYSTEM'' x\u0000",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 22, "line": 1 },
//     { "code": "unexpected-null-character", "line": 1, "col": 23 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''!",
// "input":"<!DOCTYPE a SYSTEM''!",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''\"",
// "input":"<!DOCTYPE a SYSTEM''\"",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''&",
// "input":"<!DOCTYPE a SYSTEM''&",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM'''",
// "input":"<!DOCTYPE a SYSTEM'''",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''-",
// "input":"<!DOCTYPE a SYSTEM''-",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''/",
// "input":"<!DOCTYPE a SYSTEM''/",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''0",
// "input":"<!DOCTYPE a SYSTEM''0",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''1",
// "input":"<!DOCTYPE a SYSTEM''1",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''9",
// "input":"<!DOCTYPE a SYSTEM''9",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''<",
// "input":"<!DOCTYPE a SYSTEM''<",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''=",
// "input":"<!DOCTYPE a SYSTEM''=",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''>",
// "input":"<!DOCTYPE a SYSTEM''>",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''?",
// "input":"<!DOCTYPE a SYSTEM''?",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''@",
// "input":"<!DOCTYPE a SYSTEM''@",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''A",
// "input":"<!DOCTYPE a SYSTEM''A",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''B",
// "input":"<!DOCTYPE a SYSTEM''B",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''Y",
// "input":"<!DOCTYPE a SYSTEM''Y",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''Z",
// "input":"<!DOCTYPE a SYSTEM''Z",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''`",
// "input":"<!DOCTYPE a SYSTEM''`",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},

// {"description":"<!DOCTYPE a SYSTEM''a",
// "input":"<!DOCTYPE a SYSTEM''a",
// "output":[["DOCTYPE", "a", null, "", true]],
// "errors":[
//     { "code": "missing-whitespace-after-doctype-system-keyword", "line": 1, "col": 19 },
//     { "code": "unexpected-character-after-doctype-system-identifier", "col": 21, "line": 1 }
// ]},