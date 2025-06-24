#include "test_tokenizer.h"

static void doctype_without_name()
{
    // {"description":"DOCTYPE without name",
    // "input":"<!DOCTYPE>",
    // "output":[["DOCTYPE", null, null, null, false]],
    // "errors":[
    //     { "code": "missing-doctype-name", "line": 1, "col": 10 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_MISSING_DOCTYPE_NAME, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void doctype_without_space_before_name()
{
    // {"description":"DOCTYPE without space before name",
    // "input":"<!DOCTYPEhtml>",
    // "output":[["DOCTYPE", "html", null, null, true]],
    // "errors":[
    //     { "code": "missing-whitespace-before-doctype-name", "line": 1, "col": 10 }
    // ]},

    const char buffer[]                         = "<!DOCTYPEhtml>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_MISSING_WHITESPACE_BEFORE_DOCTYPE_NAME, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = false, .name_size = 4,
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void incorrect_doctype_without_space_before_name()
{
    // {"description":"Incorrect DOCTYPE without a space before name",
    // "input":"<!DOCTYPEfoo>",
    // "output":[["DOCTYPE", "foo", null, null, true]],
    // "errors":[
    //     { "code": "missing-whitespace-before-doctype-name", "line": 1, "col": 10 }
    // ]},

    const char buffer[]                         = "<!DOCTYPEfoo>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_MISSING_WHITESPACE_BEFORE_DOCTYPE_NAME, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = false, .name_size = 3,
                                                        .name = { [0] = 'f', [1] = 'o', [2] = 'o' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void doctype_with_public_id()
{
    // {"description":"DOCTYPE with publicId",
    // "input":"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML Transitional 4.01//EN\">",
    // "output":[["DOCTYPE", "html", "-//W3C//DTD HTML Transitional 4.01//EN", null, true]]},

    const char buffer[]                         = "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML Transitional 4.01//EN\">";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = false, .name_size = 4,
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' },
                                                        .public_id_size =  38,
                                                        .public_id = { [0] = '-', [1] = '/', [2] = '/', [3] = 'W', [4] = '3', [5] = 'C', [6] = '/', [7] = '/',
                                                                       [8] = 'D', [9] = 'T', [10] = 'D', [11] = ' ', [12] = 'H', [13] = 'T', [14] = 'M', [15] = 'L',
                                                                       [16] = ' ', [17] = 'T', [18] = 'r', [19] = 'a', [20] = 'n', [21] = 's', [22] = 'i', [23] = 't',
                                                                       [24] = 'i', [25] = 'o', [26] = 'n', [27] = 'a', [28] = 'l', [29] = ' ', [30] = '4', [31] = '.',
                                                                       [32] = '0', [33] = '1', [34] = '/', [35] = '/', [36] = 'E', [37] = 'N' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void doctype_with_eof_after_public()
{
    // {"description":"DOCTYPE with EOF after PUBLIC",
    // "input":"<!DOCTYPE html PUBLIC",
    // "output":[["DOCTYPE", "html", null, null, false]],
    // "errors": [
    //     { "code": "eof-in-doctype", "col": 22, "line": 1 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE html PUBLIC";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true, .name_size = 4,
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } },
                                                    {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void doctype_with_eof_after_public_two()
{
    // {"description":"DOCTYPE with EOF after PUBLIC '",
    // "input":"<!DOCTYPE html PUBLIC '",
    // "output":[["DOCTYPE", "html", "", null, false]],
    // "errors": [
    //     { "code": "eof-in-doctype", "col": 24, "line": 1 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE html PUBLIC '";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true, .name_size = 4,
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } },
                                                    {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void doctype_with_eof_after_public_three()
{
    // {"description":"DOCTYPE with EOF after PUBLIC 'x",
    // "input":"<!DOCTYPE html PUBLIC 'x",
    // "output":[["DOCTYPE", "html", "x", null, false]],
    // "errors": [
    //     { "code": "eof-in-doctype", "col": 25, "line": 1 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE html PUBLIC 'x";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true, .name_size = 4,
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' },
                                                        .public_id_size = 1,
                                                        .public_id = { [0] = 'x' } },
                                                    {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void doctype_with_system_id()
{
    // {"description":"DOCTYPE with systemId",
    // "input":"<!DOCTYPE html SYSTEM \"-//W3C//DTD HTML Transitional 4.01//EN\">",
    // "output":[["DOCTYPE", "html", null, "-//W3C//DTD HTML Transitional 4.01//EN", true]]},

    const char buffer[]                         = "<!DOCTYPE html SYSTEM \"-//W3C//DTD HTML Transitional 4.01//EN\">";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = false, .name_size = 4,
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' },
                                                        .system_id_size =  38,
                                                        .system_id = {  [0] = '-', [1] = '/', [2] = '/', [3] = 'W', [4] = '3', [5] = 'C', [6] = '/', [7] = '/',
                                                                        [8] = 'D', [9] = 'T', [10] = 'D', [11] = ' ', [12] = 'H', [13] = 'T', [14] = 'M', [15] = 'L',
                                                                        [16] = ' ', [17] = 'T', [18] = 'r', [19] = 'a', [20] = 'n', [21] = 's', [22] = 'i', [23] = 't',
                                                                        [24] = 'i', [25] = 'o', [26] = 'n', [27] = 'a', [28] = 'l', [29] = ' ', [30] = '4', [31] = '.',
                                                                        [32] = '0', [33] = '1', [34] = '/', [35] = '/', [36] = 'E', [37] = 'N' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void doctype_with_system_id_single_quoted()
{
    // {"description":"DOCTYPE with single-quoted systemId",
    // "input":"<!DOCTYPE html SYSTEM '-//W3C//DTD HTML Transitional 4.01//EN'>",
    // "output":[["DOCTYPE", "html", null, "-//W3C//DTD HTML Transitional 4.01//EN", true]]},

    const char buffer[]                         = "<!DOCTYPE html SYSTEM '-//W3C//DTD HTML Transitional 4.01//EN'>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = false, .name_size = 4,
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' },
                                                        .system_id_size =  38,
                                                        .system_id = {  [0] = '-', [1] = '/', [2] = '/', [3] = 'W', [4] = '3', [5] = 'C', [6] = '/', [7] = '/',
                                                                        [8] = 'D', [9] = 'T', [10] = 'D', [11] = ' ', [12] = 'H', [13] = 'T', [14] = 'M', [15] = 'L',
                                                                        [16] = ' ', [17] = 'T', [18] = 'r', [19] = 'a', [20] = 'n', [21] = 's', [22] = 'i', [23] = 't',
                                                                        [24] = 'i', [25] = 'o', [26] = 'n', [27] = 'a', [28] = 'l', [29] = ' ', [30] = '4', [31] = '.',
                                                                        [32] = '0', [33] = '1', [34] = '/', [35] = '/', [36] = 'E', [37] = 'N' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void doctype_with_system_id_and_public_id()
{
    // {"description":"DOCTYPE with publicId and systemId",
    // "input":"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML Transitional 4.01//EN\" \"-//W3C//DTD HTML Transitional 4.01//EN\">",
    // "output":[["DOCTYPE", "html", "-//W3C//DTD HTML Transitional 4.01//EN", "-//W3C//DTD HTML Transitional 4.01//EN", true]]},

    const char buffer[]                         = "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML Transitional 4.01//EN\" \"-//W3C//DTD HTML Transitional 4.01//EN\">";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = false, .name_size = 4,
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' },
                                                        .system_id_size =  38,
                                                        .system_id = {  [0] = '-', [1] = '/', [2] = '/', [3] = 'W', [4] = '3', [5] = 'C', [6] = '/', [7] = '/',
                                                                        [8] = 'D', [9] = 'T', [10] = 'D', [11] = ' ', [12] = 'H', [13] = 'T', [14] = 'M', [15] = 'L',
                                                                        [16] = ' ', [17] = 'T', [18] = 'r', [19] = 'a', [20] = 'n', [21] = 's', [22] = 'i', [23] = 't',
                                                                        [24] = 'i', [25] = 'o', [26] = 'n', [27] = 'a', [28] = 'l', [29] = ' ', [30] = '4', [31] = '.',
                                                                        [32] = '0', [33] = '1', [34] = '/', [35] = '/', [36] = 'E', [37] = 'N' },
                                                        .public_id_size =  38,
                                                        .public_id = {  [0] = '-', [1] = '/', [2] = '/', [3] = 'W', [4] = '3', [5] = 'C', [6] = '/', [7] = '/',
                                                                        [8] = 'D', [9] = 'T', [10] = 'D', [11] = ' ', [12] = 'H', [13] = 'T', [14] = 'M', [15] = 'L',
                                                                        [16] = ' ', [17] = 'T', [18] = 'r', [19] = 'a', [20] = 'n', [21] = 's', [22] = 'i', [23] = 't',
                                                                        [24] = 'i', [25] = 'o', [26] = 'n', [27] = 'a', [28] = 'l', [29] = ' ', [30] = '4', [31] = '.',
                                                                        [32] = '0', [33] = '1', [34] = '/', [35] = '/', [36] = 'E', [37] = 'N' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void angle_bracket_in_doctype_public_id()
{
    // {"description":"DOCTYPE with > in double-quoted publicId",
    // "input":"<!DOCTYPE html PUBLIC \">x",
    // "output":[["DOCTYPE", "html", "", null, false], ["Character", "x"]],
    // "errors": [
    //     { "code": "abrupt-doctype-public-identifier", "col": 24, "line": 1 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE html PUBLIC \">x";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_ABRUPT_DOCTYPE_PUBLIC_IDENTIFIER,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true, .name_size = 4,
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'x' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void angle_bracket_in_doctype_public_id_single_quoted()
{
    // {"description":"DOCTYPE with > in single-quoted publicId",
    // "input":"<!DOCTYPE html PUBLIC '>x",
    // "output":[["DOCTYPE", "html", "", null, false], ["Character", "x"]],
    // "errors": [
    //     { "code": "abrupt-doctype-public-identifier", "col": 24, "line": 1 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE html PUBLIC '>x";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_ABRUPT_DOCTYPE_PUBLIC_IDENTIFIER,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true, .name_size = 4,
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'x' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void angle_bracket_in_doctype_system_id_double_quoted()
{
    // {"description":"DOCTYPE with > in double-quoted systemId",
    // "input":"<!DOCTYPE html PUBLIC \"foo\" \">x",
    // "output":[["DOCTYPE", "html", "foo", "", false], ["Character", "x"]],
    // "errors": [
    //     { "code": "abrupt-doctype-system-identifier", "col": 30, "line": 1 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE html PUBLIC \"foo\" \">x";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_ABRUPT_DOCTYPE_SYSTEM_IDENTIFIER,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true, .name_size = 4,
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' },
                                                        .public_id_size = 3,
                                                        .public_id = { [0] = 'f', [1] = 'o', [2] = 'o' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'x' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void angle_bracket_in_doctype_system_id_single_quoted()
{
    // {"description":"DOCTYPE with > in single-quoted systemId",
    // "input":"<!DOCTYPE html PUBLIC 'foo' '>x",
    // "output":[["DOCTYPE", "html", "foo", "", false], ["Character", "x"]],
    // "errors": [
    //     { "code": "abrupt-doctype-system-identifier", "col": 30, "line": 1 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE html PUBLIC 'foo' '>x";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_ABRUPT_DOCTYPE_SYSTEM_IDENTIFIER,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true, .name_size = 4,
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' },
                                                        .public_id_size = 3,
                                                        .public_id = { [0] = 'f', [1] = 'o', [2] = 'o' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'x' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void incomplete_doctype()
{
    // {"description":"Incomplete doctype",
    // "input":"<!DOCTYPE html ",
    // "output":[["DOCTYPE", "html", null, null, false]],
    // "errors":[
    //     { "code": "eof-in-doctype", "line": 1, "col": 16 }
    // ]},

    const char buffer[]                         = "<!DOCTYPE html ";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_DOCTYPE };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_DOCTYPE_TOKEN, .force_quirks = true, .name_size = 4,
                                                        .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' } },
                                                      {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void numeric_entity_representing_the_null_character()
{
    // {"description":"Numeric entity representing the NUL character",
    // "input":"&#0000;",
    // "output":[["Character", "\uFFFD"]],
    // "errors":[
    //     { "code": "null-character-reference", "line": 1, "col": 8 }
    // ]},

    const char buffer[]                         = "&#0000;";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_NULL_CHARACTER_REFERENCE,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void hexadecimal_entity_representing_the_null_character()
{
    // {"description":"Hexadecimal entity representing the NUL character",
    // "input":"&#x0000;",
    // "output":[["Character", "\uFFFD"]],
    // "errors":[
    //     { "code": "null-character-reference", "line": 1, "col": 9 }
    // ]},

    const char buffer[]                         = "&#x0000;";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_NULL_CHARACTER_REFERENCE,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void numeric_entity_represeting_10FFFF()
{
    // {"description":"Numeric entity representing a codepoint after 1114111 (U+10FFFF)",
    // "input":"&#2225222;",
    // "output":[["Character", "\uFFFD"]],
    // "errors":[
    //     { "code": "character-reference-outside-unicode-range", "line": 1, "col": 11 }
    // ]},

    const char buffer[]                         = "&#2225222;";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_CHARACTER_REFERENCE_OUTSIDE_UNICODE_RANGE,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void hexadecimal_entity_represeting_10FFFF()
{
    // {"description":"Hexadecimal entity representing a codepoint after 1114111 (U+10FFFF)",
    // "input":"&#x1010FFFF;",
    // "output":[["Character", "\uFFFD"]],
    // "errors":[
    //     { "code": "character-reference-outside-unicode-range", "line": 1, "col": 13 }
    // ]},

    const char buffer[]                         = "&#x1010FFFF;";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_CHARACTER_REFERENCE_OUTSIDE_UNICODE_RANGE,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void hexadecimal_entity_pair_representing_surrogate_pair()
{
    // {"description":"Hexadecimal entity pair representing a surrogate pair",
    // "input":"&#xD869;&#xDED6;",
    // "output":[["Character", "\uFFFD\uFFFD"]],
    // "errors":[
    //     { "code": "surrogate-character-reference", "line": 1, "col": 9 },
    //     { "code": "surrogate-character-reference", "line": 1, "col": 17 }
    // ]},

    const char buffer[]                         = "&#xD869;&#xDED6;";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_SURROGATE_CHARACTER_REFERENCE,
                                                    HTML_TOKENIZER_SURROGATE_CHARACTER_REFERENCE,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xef, [1] = 0xbf, [2] = 0xbd } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void hexadecimal_entity_with_mixed_case()
{
    // {"description":"Hexadecimal entity with mixed uppercase and lowercase",
    // "input":"&#xaBcD;",
    // "output":[["Character", "\uABCD"]]},

    const char buffer[]                         = "&#xaBcD;";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 3, .data = { [0] = 0xea, [1] = 0xaf, [2] = 0x8d } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void entity_without_name()
{
    // {"description":"Entity without a name",
    // "input":"&;",
    // "output":[["Character", "&;"]]},

    const char buffer[]                         = "&;";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ';' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void unescaped_ampersand_in_attribute_value()
{
    // {"description":"Unescaped ampersand in attribute value",
    // "input":"<h a='&'>",
    // "output":[["StartTag", "h", { "a":"&" }]]},

    const char buffer[]                         = "<h a='&'>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { { .is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' }, .attributes_size = 1,
                                                        .attributes = { [0] = { .name = { [0] = 'a' }, .name_size = 1, .value = { [0] = '&' }, .value_size = 1 } } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void start_tag_containing_less_than()
{
    // {"description":"StartTag containing <",
    // "input":"<a<b>",
    // "output":[["StartTag", "a<b", { }]]},

    const char buffer[]                         = "<a<b>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {  .is_valid = true, .type = HTML_START_TOKEN, .name_size = 3, .name = { [0] = 'a', [1] = '<', [2] = 'b' }, } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void non_void_element_containing_trailing_forward_slash()
{
    // {"description":"Non-void element containing trailing /",
    // "input":"<h/>",
    // "output":[["StartTag","h",{},true]]},

    const char buffer[]                         = "<h/>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {  .is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' }, .self_closing = true } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void void_element_with_permitted_slash()
{
    // {"description":"Void element with permitted slash",
    // "input":"<br/>",
    // "output":[["StartTag","br",{},true]]},

    const char buffer[]                         = "<br/>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {  .is_valid = true, .type = HTML_START_TOKEN, .name_size = 2, .name = { [0] = 'b', [1] = 'r' }, .self_closing = true } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void void_element_with_permitted_slash_and_attribute()
{
    // {"description":"Void element with permitted slash (with attribute)",
    // "input":"<br foo='bar'/>",
    // "output":[["StartTag","br",{"foo":"bar"},true]]},

    const char buffer[]                         = "<br foo='bar'/>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_START_TOKEN, .name_size = 2, .name = { [0] = 'b', [1] = 'r' }, .self_closing = true,
                                                        .attributes_size = 1,
                                                        .attributes = { [0] = { .name = { [0] = 'f', [1] = 'o', [2] = 'o' }, .name_size = 3,
                                                                                .value = { [0] = 'b', [1] = 'a', [2] = 'r' }, .value_size = 3 } } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void start_tag_with_forward_slash()
{
    // {"description":"StartTag containing /",
    // "input":"<h/a='b'>",
    // "output":[["StartTag", "h", { "a":"b" }]],
    // "errors":[
    //     { "code": "unexpected-solidus-in-tag", "line": 1, "col": 4 }
    // ]},

    const char buffer[]                         = "<h/a='b'>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_SOLIDUS_IN_TAG,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' },
                                                        .attributes_size = 1,
                                                        .attributes = { [0] = { .name = { [0] = 'a' }, .name_size = 1, .value = { [0] = 'b' }, .value_size = 1 } } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_in_single_quoted_attribute_value()
{
    const char buffer[]                         = "<h/a='b";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_TAG };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_in_single_quoted_attribute_value()
{
    const char buffer[]                         = "<h/a='b\0'>";
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

static void double_quoted_attribute_value()
{
    // {"description":"Double-quoted attribute value",
    // "input":"<h a=\"b\">",
    // "output":[["StartTag", "h", { "a":"b" }]]},

    const char buffer[]                         = "<h a=\"b\">";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' },
                                                        .attributes_size = 1,
                                                        .attributes = { [0] = { .name = { [0] = 'a' }, .name_size = 1, .value = { [0] = 'b' }, .value_size = 1 } } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_double_quoted_attribute_value()
{
    const char buffer[]                         = "<h a=\"b";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_TAG };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void numeric_char_ref_in_double_quoted_attribute_value()
{
    const char buffer[]                         = "<h a=\"b&#0000;\">";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_NULL_CHARACTER_REFERENCE, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' },
                                                        .attributes_size = 1,
                                                        .attributes = { [0] = { .name = { [0] = 'a' }, .name_size = 1, 
                                                                                .value = { [0] = 'b', [1] = 0xef, [2] = 0xbf, [3] = 0xbd }, 
                                                                                .value_size = 4 } } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_char_ref_in_double_quoted_attribute_value()
{
    const char buffer[]                         = "<h a=\"b\0\">";
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

static void unescaped_forward_slash()
{
    // {"description":"Unescaped </",
    // "input":"</",
    // "output":[["Character", "</"]],
    // "errors":[
    //     { "code": "eof-before-tag-name", "line": 1, "col": 3 }
    // ]},

    const char buffer[]                         = "</";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 3 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_BEFORE_TAG_NAME };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                                      {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '/' } },
                                                      {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void illegal_end_tag_name()
{
    // {"description":"Illegal end tag name",
    // "input":"</1>",
    // "output":[["Comment", "1"]],
    // "errors":[
    //     { "code": "invalid-first-character-of-tag-name", "line": 1, "col": 3 }
    // ]},

    const char buffer[]                         = "</1>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_INVALID_FIRST_CHARACTER_OF_TAG_NAME, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 1, .data = { [0] = '1' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void simili_processing_instruction()
{
    // {"description":"Simili processing instruction",
    // "input":"<?namespace>",
    // "output":[["Comment", "?namespace"]],
    // "errors":[
    //     { "code": "unexpected-question-mark-instead-of-tag-name", "line": 1, "col": 2 }
    // ]},

    const char buffer[]                         = "<?namespace>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_QUOESTION_MARK_INSTEAD_OF_TAG_NAME, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 10, 
                                                        .data = {   [0] = '?', [1] = 'n', [2] = 'a', [3] = 'm', [4] = 'e', [5] = 's', [6] = 'p',
                                                                    [7] = 'a', [8] = 'c', [9] = 'e' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void bogus_comment_stops_at_triangle_bracket_even_if_preceded_by_dashes()
{
    // {"description":"A bogus comment stops at >, even if preceded by two dashes",
    // "input":"<?foo-->",
    // "output":[["Comment", "?foo--"]],
    // "errors":[
    //     { "code": "unexpected-question-mark-instead-of-tag-name", "line": 1, "col": 2 }
    // ]},

    const char buffer[]                         = "<?foo-->";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_QUOESTION_MARK_INSTEAD_OF_TAG_NAME, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 6, 
                                                        .data = { [0] = '?', [1] = 'f', [2] = 'o', [3] = 'o', [4] = '-', [5] = '-' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void unexpected_triangle_bracket()
{
    // {"description":"Unescaped <",
    // "input":"foo < bar",
    // "output":[["Character", "foo < bar"]],
    // "errors":[
    //     { "code": "invalid-first-character-of-tag-name", "line": 1, "col": 6 }
    // ]},

    const char buffer[]                         = "foo < bar";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_INVALID_FIRST_CHARACTER_OF_TAG_NAME,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'f', } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'o', } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'o', } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ', } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<', } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ', } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'b', } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'a', } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'r', } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_byte_replacement()
{
    // {"description":"Null Byte Replacement",
    // "input":"\u0000",
    // "output":[["Character", "\u0000"]],
    // "errors":[
    //     { "code": "unexpected-null-character", "line": 1, "col": 1 }
    // ]},

    const char buffer[]                         = "\0";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '\0', } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void comment_with_dash()
{
    // {"description":"Comment with dash",
    // "input":"<!---x",
    // "output":[["Comment", "-x"]],
    // "errors":[
    //     { "code": "eof-in-comment", "line": 1, "col": 7 }
    // ]},

    const char buffer[]                         = "<!---x";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 2 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_COMMENT };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 2, .data = { [0] = '-', [1] = 'x' } },
                                                      {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

// static void named_entity_with_newline()
// {
//     // {"description":"Entity + newline",
//     // "input":"\nx\n&gt;\n",
//     // "output":[["Character","\nx\n>\n"]]},
// }

static void start_tag_with_no_attributes_but_with_space_before_end()
{
    // {"description":"Start tag with no attributes but space before the greater-than sign",
    // "input":"<h >",
    // "output":[["StartTag", "h", {}]]},

    const char buffer[]                         = "<h >";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void eof_after_attribute_name_state()
{
    const char buffer[]                         = "<h ";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_EOF_IN_TAG };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void empty_attribute_followed_by_uppercase_attribute()
{
    // {"description":"Empty attribute followed by uppercase attribute",
    // "input":"<h a B=''>",
    // "output":[["StartTag", "h", {"a":"", "b":""}]]},

    const char buffer[]                         = "<h a B=''>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' },
                                                        .attributes_size = 2,
                                                        .attributes = { [0] = { .name_size = 1, .name = { [0] = 'a' } },
                                                                        [1] = { .name_size = 1, .name = { [0] = 'b' } } } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void null_char_in_attribute_name()
{
    const char buffer[]                         = "<h a\0 B=''>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' },
                                                        .attributes_size = 2,
                                                        .attributes = { [0] = { .name_size = 4, .name = { [0] = 'a', [1] = 0xef, [2] = 0xbf, [3] = 0xbd } },
                                                                        [1] = { .name_size = 1, .name = { [0] = 'b' } } } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void double_quote_after_attribute_name()
{
    // {"description":"Double-quote after attribute name",
    // "input":"<h a \">",
    // "output":[["StartTag", "h", {"a":"", "\"":""}]],
    // "errors":[
    //     { "code": "unexpected-character-in-attribute-name", "line": 1, "col": 6 }
    // ]},

    const char buffer[]                         = "<h a \">";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' },
                                                        .attributes_size = 2,
                                                        .attributes = { [0] = { .name_size = 1, .name = { [0] = 'a' } },
                                                                        [1] = { .name_size = 1, .name = { [0] = '"' } } } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void single_quote_after_attribute_name()
{
    // {"description":"Single-quote after attribute name",
    // "input":"<h a '>",
    // "output":[["StartTag", "h", {"a":"", "'":""}]],
    // "errors":[
    //     { "code": "unexpected-character-in-attribute-name", "line": 1, "col": 6 }
    // ]},

    const char buffer[]                         = "<h a '>";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME, HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'h' },
                                                        .attributes_size = 2,
                                                        .attributes = { [0] = { .name_size = 1, .name = { [0] = 'a' } },
                                                                        [1] = { .name_size = 1, .name = { [0] = '\'' } } } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void empty_end_tag_with_following_characters()
{
    // {"description":"Empty end tag with following characters",
    // "input":"a</>bc",
    // "output":[["Character", "abc"]],
    // "errors":[
    //     { "code": "missing-end-tag-name", "line": 1, "col": 4 }
    // ]},

    const char buffer[]                         = "a</>bc";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_MISSING_END_TAG_NAME,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'a' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'b' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'c' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void empty_end_tag_with_following_tag()
{
    // {"description":"Empty end tag with following tag",
    // "input":"a</><b>c",
    // "output":[["Character", "a"], ["StartTag", "b", {}], ["Character", "c"]],
    // "errors":[
    //     { "code": "missing-end-tag-name", "line": 1, "col": 4 }
    // ]},

    const char buffer[]                         = "a</><b>c";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_MISSING_END_TAG_NAME,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'a' } } },
                                                    { {.is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'b' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'c' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void empty_end_tag_with_following_comment()
{
    // {"description":"Empty end tag with following comment",
    // "input":"a</><!--b-->c",
    // "output":[["Character", "a"], ["Comment", "b"], ["Character", "c"]],
    // "errors":[
    //     { "code": "missing-end-tag-name", "line": 1, "col": 4 }
    // ]},

    const char buffer[]                         = "a</><!--b-->c";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_MISSING_END_TAG_NAME,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'a' } } },
                                                    { {.is_valid = true, .type = HTML_COMMENT_TOKEN, .data_size = 1, .data = { [0] = 'b' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'c' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

static void empty_end_tag_with_following_end_tag()
{
    // {"description":"Empty end tag with following end tag",
    // "input":"a</></b>c",
    // "output":[["Character", "a"], ["EndTag", "b"], ["Character", "c"]],
    // "errors":[
    //     { "code": "missing-end-tag-name", "line": 1, "col": 4 }
    // ]}

    const char buffer[]                         = "a</></b>c";
    const html_tokenizer_state_e states[]       = { HTML_TOKENIZER_DATA_STATE };
    const uint32_t sizes[]                      = { 1, 1, 1, 1 };
    const html_tokenizer_error_e errors[]       = { HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_MISSING_END_TAG_NAME,
                                                    HTML_TOKENIZER_OK,
                                                    HTML_TOKENIZER_OK };
    const html_token_t tokens_e[][MAX_TOKENS]   = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'a' } } },
                                                    { {.is_valid = true, .type = HTML_END_TOKEN, .name_size = 1, .name = { [0] = 'b' } } },
                                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'c' } } },
                                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };
    RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e);
}

void test_html_tokenizer_test2()
{
    TEST_CASE(doctype_without_name);
    TEST_CASE(doctype_without_space_before_name);
    TEST_CASE(incorrect_doctype_without_space_before_name);
    TEST_CASE(doctype_with_public_id);
    TEST_CASE(doctype_with_eof_after_public);
    TEST_CASE(doctype_with_eof_after_public_two);
    TEST_CASE(doctype_with_eof_after_public_three);
    TEST_CASE(doctype_with_system_id);
    TEST_CASE(doctype_with_system_id_single_quoted);
    TEST_CASE(doctype_with_system_id_and_public_id);
    TEST_CASE(angle_bracket_in_doctype_public_id);
    TEST_CASE(angle_bracket_in_doctype_public_id_single_quoted);
    TEST_CASE(angle_bracket_in_doctype_system_id_double_quoted);
    TEST_CASE(angle_bracket_in_doctype_system_id_single_quoted);
    TEST_CASE(incomplete_doctype);
    TEST_CASE(numeric_entity_representing_the_null_character);
    TEST_CASE(hexadecimal_entity_representing_the_null_character);
    TEST_CASE(numeric_entity_represeting_10FFFF);
    TEST_CASE(hexadecimal_entity_represeting_10FFFF);
    TEST_CASE(hexadecimal_entity_pair_representing_surrogate_pair);
    TEST_CASE(hexadecimal_entity_with_mixed_case);
    TEST_CASE(entity_without_name);
    TEST_CASE(unescaped_ampersand_in_attribute_value);
    TEST_CASE(start_tag_containing_less_than);
    TEST_CASE(non_void_element_containing_trailing_forward_slash);
    TEST_CASE(void_element_with_permitted_slash);
    TEST_CASE(void_element_with_permitted_slash_and_attribute);
    TEST_CASE(start_tag_with_forward_slash);
    TEST_CASE(eof_in_single_quoted_attribute_value);
    TEST_CASE(null_in_single_quoted_attribute_value);
    TEST_CASE(double_quoted_attribute_value);
    TEST_CASE(eof_double_quoted_attribute_value);
    TEST_CASE(numeric_char_ref_in_double_quoted_attribute_value);
    TEST_CASE(null_char_ref_in_double_quoted_attribute_value);
    TEST_CASE(unescaped_forward_slash);
    TEST_CASE(illegal_end_tag_name);
    TEST_CASE(simili_processing_instruction);
    TEST_CASE(bogus_comment_stops_at_triangle_bracket_even_if_preceded_by_dashes);
    TEST_CASE(unexpected_triangle_bracket);
    TEST_CASE(null_byte_replacement);
    TEST_CASE(comment_with_dash);
    // TEST_CASE(named_entity_with_newline);
    TEST_CASE(start_tag_with_no_attributes_but_with_space_before_end);
    TEST_CASE(eof_after_attribute_name_state);
    TEST_CASE(empty_attribute_followed_by_uppercase_attribute);
    TEST_CASE(null_char_in_attribute_name);
    TEST_CASE(double_quote_after_attribute_name);
    TEST_CASE(single_quote_after_attribute_name);
    TEST_CASE(empty_end_tag_with_following_characters);
    TEST_CASE(empty_end_tag_with_following_tag);
    TEST_CASE(empty_end_tag_with_following_comment);
    TEST_CASE(empty_end_tag_with_following_end_tag);
}