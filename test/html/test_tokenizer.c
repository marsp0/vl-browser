#include "test_tokenizer.h"

#include "test_utils.h"

#include "html/tokenizer.h"

#define SIZE_TEN 10
#define ASSERT_TOKENS_SIZE(expected, max_size)  do                                              \
                                                {                                               \
                                                    uint32_t actual = 0;                        \
                                                    for (uint32_t k = 0; k < max_size; k++)     \
                                                    {                                           \
                                                        if (tokens[k].is_valid) { actual++; }   \
                                                    }                                           \
                                                    ASSERT_EQUAL(actual, expected);             \
                                                } while (0);

#define ASSERT_DOCTYPE(a, b)    do                                                                          \
                                {                                                                           \
                                    ASSERT_TRUE(a.is_valid);                                                \
                                    ASSERT_EQUAL(a.is_valid, b.is_valid);                                   \
                                    ASSERT_EQUAL(a.type, b.type);                                           \
                                    ASSERT_EQUAL(a.name_size, b.name_size);                                 \
                                    ASSERT_STRING((char)a.name, (char)b.name, a.name_size);                 \
                                    ASSERT_EQUAL(a.public_id_size, b.public_id_size);                       \
                                    ASSERT_STRING((char)a.public_id, (char)b.public_id, a.public_id_size);  \
                                    ASSERT_EQUAL(a.system_id_size, b.system_id_size);                       \
                                    ASSERT_STRING((char)a.system_id, (char)b.system_id, a.system_id_size);  \
                                    ASSERT_EQUAL(a.force_quirks, b.force_quirks);                           \
                                } while (0);

#define ASSERT_COMMENT(a, b)    do                                                              \
                                {                                                               \
                                    ASSERT_TRUE(a.is_valid);                                    \
                                    ASSERT_EQUAL(a.is_valid, b.is_valid);                       \
                                    ASSERT_EQUAL(a.type, b.type);                               \
                                    ASSERT_EQUAL(a.data_size, b.data_size);                     \
                                    ASSERT_STRING((char)a.data, (char)b.data, a.data_size);     \
                                } while (0);

#define ASSERT_TAG(a, b)        do                                                              \
                                {                                                               \
                                    ASSERT_TRUE(a.is_valid);                                    \
                                    ASSERT_EQUAL(a.is_valid, b.is_valid);                       \
                                    ASSERT_EQUAL(a.type, b.type);                               \
                                    ASSERT_EQUAL(a.name_size, b.name_size);                     \
                                    ASSERT_STRING((char)a.name, (char)b.name, a.name_size);     \
                                    ASSERT_EQUAL(a.attributes_size, b.attributes_size);         \
                                    for (uint32_t k = 0; k < a.attributes_size; k++)            \
                                    {                                                           \
                                        html_token_attribute_t a_attr = a.attributes[k];        \
                                        html_token_attribute_t b_attr = b.attributes[k];        \
                                        ASSERT_EQUAL(a_attr.name_size, b_attr.name_size);       \
                                        ASSERT_EQUAL(a_attr.value_size, b_attr.value_size);     \
                                        ASSERT_STRING((char)a_attr.name, (char)b_attr.name, a_attr.name_size); \
                                        ASSERT_STRING((char)a_attr.value, (char)b_attr.value, a_attr.value_size); \
                                    }                                                           \
                                } while (0);

#define ASSERT_EOF(a, b)        do                                                              \
                                {                                                               \
                                    ASSERT_TRUE(a.is_valid);                                    \
                                    ASSERT_EQUAL(a.is_valid, b.is_valid);                       \
                                    ASSERT_EQUAL(a.type, b.type);                               \
                                } while (0);

#define ASSERT_CHARACTER(a, b)  do                                                              \
                                {                                                               \
                                    ASSERT_TRUE(a.is_valid);                                    \
                                    ASSERT_EQUAL(a.is_valid, b.is_valid);                       \
                                    ASSERT_EQUAL(a.type, b.type);                               \
                                    ASSERT_EQUAL(a.data_size, b.data_size);                     \
                                    ASSERT_STRING((char)a.data, (char)b.data, a.data_size);     \
                                } while (0);

static const html_token_t eof_token = {.is_valid = true, .type = HTML_EOF_TOKEN };

static void correct_doctype_lowercase()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    html_tokenizer_error_e status;
    const char buffer[] = "<!DOCTYPE html>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t doctype_token = {  .is_valid = true, 
                                    .type = HTML_DOCTYPE_TOKEN,
                                    .name_size = 4,
                                    .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' },
                                    .force_quirks = false };

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_DOCTYPE(tokens[0], doctype_token);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void correct_doctype_uppercase()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    html_tokenizer_error_e status;
    const char buffer[] = "<!DOCTYPE HTML>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t doctype = { .is_valid = true, 
                              .type = HTML_DOCTYPE_TOKEN,
                              .name_size = 4,
                              .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' }, 
                              .force_quirks = false };

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_DOCTYPE(tokens[0], doctype);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void correct_doctype_mixedcase()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    html_tokenizer_error_e status;
    const char buffer[] = "<!DOCTYPE HtMl>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t expected = { .is_valid = true, 
                              .type = HTML_DOCTYPE_TOKEN,
                              .name_size = 4,
                              .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' }, 
                              .force_quirks = false };

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_DOCTYPE(tokens[0], expected);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void correct_doctype_with_eof()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    html_tokenizer_error_e status;
    const char buffer[] = "<!DOCTYPE html";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t e_doctype = {  .is_valid = true, 
                                .type = HTML_DOCTYPE_TOKEN,
                                .name_size = 4,
                                .name = { [0] = 'h', [1] = 't', [2] = 'm', [3] = 'l' }, 
                                .force_quirks = true };


    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(2, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_EOF_IN_DOCTYPE);
    ASSERT_DOCTYPE(tokens[0], e_doctype);
    ASSERT_EOF(tokens[1], eof_token);
}

static void truncated_doctype_start()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!DOC>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t e_comment = {  .is_valid = true, 
                                .type = HTML_COMMENT_TOKEN,
                                .data_size = 3,
                                .data = { [0] = 'D', [1] = 'O', [2] = 'C'} };


    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_INCORRECTLY_OPENED_COMMENT);
    ASSERT_COMMENT(tokens[0], e_comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void doctype_in_error()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!DOCTYPE foo>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t e_doctype = {  .is_valid = true, 
                                .type = HTML_DOCTYPE_TOKEN,
                                .name_size = 3,
                                .name = { [0] = 'f', [1] = 'o', [2] = 'o' }, 
                                .force_quirks = false };


    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_DOCTYPE(tokens[0], e_doctype);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void single_start_tag()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<h>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t expected = {   .is_valid = true, 
                                .type = HTML_START_TOKEN,
                                .name_size = 1,
                                .name = { [0] = 'h' } };


    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_TAG(tokens[0], expected);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void empty_end_tag()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "</>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t expected = {   .is_valid = true, 
                                .type = HTML_EOF_TOKEN };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_MISSING_END_TAG_NAME);
    ASSERT_EOF(tokens[0], expected);
}

static void empty_start_tag()
{
    // output differs

    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t expected = {   .is_valid = true, 
                                .type = HTML_CHARACTER_TOKEN,
                                .data_size = 1,
                                .data = { [0] = '<' } };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_INVALID_FIRST_CHARACTER_OF_TAG_NAME);
    ASSERT_CHARACTER(tokens[0], expected);

    status = html_tokenizer_next();
    expected.data[0] = '>';

    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_CHARACTER(tokens[0], expected);
}

static void start_tag_with_attribute()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<h a='b'>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t expected1 = {  .is_valid = true, 
                                .type = HTML_START_TOKEN,
                                .name_size = 1,
                                .name = { [0] = 'h' },
                                .attributes_size = 1,
                                .attributes = { [0] = { .name = { [0] = 'a' }, 
                                                        .name_size = 1,
                                                        .value = { [0] = 'b' },
                                                        .value_size = 1 } } };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_TAG(tokens[0], expected1);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void start_tag_with_attribute_no_quotes()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<h a=b>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t expected1 = {  .is_valid = true, 
                                .type = HTML_START_TOKEN,
                                .name_size = 1,
                                .name = { [0] = 'h' },
                                .attributes_size = 1,
                                .attributes = { [0] = { .name = { [0] = 'a' }, 
                                                        .name_size = 1,
                                                        .value = { [0] = 'b' },
                                                        .value_size = 1 } } };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_TAG(tokens[0], expected1);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void start_and_end_tag()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<h></h>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t start = {.is_valid = true, 
                          .type = HTML_START_TOKEN,
                          .name_size = 1,
                          .name = { [0] = 'h' },
                          .attributes_size = 0 };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_TAG(tokens[0], start);
    
    html_token_t end = {.is_valid = true, 
                        .type = HTML_END_TOKEN,
                        .name_size = 1,
                        .name = { [0] = 'h' },
                        .attributes_size = 0 };

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_TAG(tokens[0], end);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void two_unclosed_start_tags()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<p>One<p>Two";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t start = {.is_valid = true, 
                          .type = HTML_START_TOKEN,
                          .name_size = 1,
                          .name = { [0] = 'p' },
                          .attributes_size = 0 };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_TAG(tokens[0], start);

    html_token_t char_1 = {.is_valid = true, 
                           .type = HTML_CHARACTER_TOKEN,
                           .data_size = 1,
                           .data = { [0] = 'O' } };

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_CHARACTER(tokens[0], char_1);

    html_token_t char_2 = {.is_valid = true, 
                           .type = HTML_CHARACTER_TOKEN,
                           .data_size = 1,
                           .data = { [0] = 'n' } };

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_CHARACTER(tokens[0], char_2);

    html_token_t char_3 = {.is_valid = true, 
                           .type = HTML_CHARACTER_TOKEN,
                           .data_size = 1,
                           .data = { [0] = 'e' } };

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_CHARACTER(tokens[0], char_3);

    html_token_t start2 = {.is_valid = true, 
                          .type = HTML_START_TOKEN,
                          .name_size = 1,
                          .name = { [0] = 'p' },
                          .attributes_size = 0 };

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_TAG(tokens[0], start2);

    html_token_t char_4 = {.is_valid = true, 
                           .type = HTML_CHARACTER_TOKEN,
                           .data_size = 1,
                           .data = { [0] = 'T' } };

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_CHARACTER(tokens[0], char_4);

    html_token_t char_5 = {.is_valid = true, 
                           .type = HTML_CHARACTER_TOKEN,
                           .data_size = 1,
                           .data = { [0] = 'w' } };

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_CHARACTER(tokens[0], char_5);

    html_token_t char_6 = {.is_valid = true, 
                           .type = HTML_CHARACTER_TOKEN,
                           .data_size = 1,
                           .data = { [0] = 'o' } };

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_CHARACTER(tokens[0], char_6);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void end_tag_with_attribute()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<h></h a='b'>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t start = {.is_valid = true, 
                          .type = HTML_START_TOKEN,
                          .name_size = 1,
                          .name = { [0] = 'h' },
                          .attributes_size = 0 };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_TAG(tokens[0], start);
    
    html_token_t end = {.is_valid = true, 
                        .type = HTML_END_TOKEN,
                        .name_size = 1,
                        .name = { [0] = 'h' },
                        .attributes_size = 1,
                        .attributes = { {.name = { [0] = 'a'}, .name_size = 1, .value = { [0] = 'b' }, .value_size = 1 } } };

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_TAG(tokens[0], end);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void multiple_attributes()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<h a='b' c='d'>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t start = {.is_valid = true, 
                          .type = HTML_START_TOKEN,
                          .name_size = 1,
                          .name = { [0] = 'h' },
                          .attributes_size = 2,
                          .attributes = { {.name = { [0] = 'a'}, .name_size = 1, .value = { [0] = 'b' }, .value_size = 1 },
                                          {.name = { [0] = 'c'}, .name_size = 1, .value = { [0] = 'd' }, .value_size = 1 } } };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_TAG(tokens[0], start);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void multiple_attributes_no_space()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<h a='b'c='d'>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t start = {.is_valid = true, 
                          .type = HTML_START_TOKEN,
                          .name_size = 1,
                          .name = { [0] = 'h' },
                          .attributes_size = 2,
                          .attributes = { {.name = { [0] = 'a'}, .name_size = 1, .value = { [0] = 'b' }, .value_size = 1 },
                                          {.name = { [0] = 'c'}, .name_size = 1, .value = { [0] = 'd' }, .value_size = 1 } } };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_MISSING_WHITESPACE_BETWEEN_ATTRIBUTES);
    ASSERT_TAG(tokens[0], start);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void repeated_attribute()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<h a='b' a='d'>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t start = {.is_valid = true, 
                          .type = HTML_START_TOKEN,
                          .name_size = 1,
                          .name = { [0] = 'h' },
                          .attributes_size = 1,
                          .attributes = { {.name = { [0] = 'a'}, .name_size = 1, .value = { [0] = 'b' }, .value_size = 1 } } };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_TAG(tokens[0], start);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void simple_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!--comment-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 7,
                            .data = { [0] = 'c', [1] = 'o', [2] = 'm', [3] = 'm', [4] = 'e', [5] = 'n', [6] = 't' } };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_COMMENT(tokens[0], comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void comment_central_dash_no_space()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!----->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 1,
                            .data = { [0] = '-'} };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_COMMENT(tokens[0], comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void comment_two_central_dashes()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-- --comment -->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 11,
                            .data = { [0] = ' ', [1] = '-', [2] = '-', [3] = 'c', [4] = 'o', [5] = 'm', [6] = 'm', [7] = 'e', [8] = 'n', [9] = 't', [10] = ' ' } };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_COMMENT(tokens[0], comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void comment_central_less_than_bang()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!--<!-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 2,
                            .data = { [0] = '<', [1] = '!' } };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_COMMENT(tokens[0], comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void unfinished_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!--comment";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 7,
                            .data = { [0] = 'c', [1] = 'o', [2] = 'm', [3] = 'm', [4] = 'e', [5] = 'n', [6] = 't' } };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(2, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_EOF_IN_COMMENT);
    ASSERT_COMMENT(tokens[0], comment);
    ASSERT_EOF(tokens[1], eof_token);
}

static void unfinished_comment_after_start_of_nested_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-- <!--";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 3,
                            .data = { [0] = ' ', [1] = '<', [2] = '!'} };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(2, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_EOF_IN_COMMENT);
    ASSERT_COMMENT(tokens[0], comment);
    ASSERT_EOF(tokens[1], eof_token);
}

static void start_of_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 1,
                            .data = { [0] = '-'} };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(2, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_INCORRECTLY_OPENED_COMMENT);
    ASSERT_COMMENT(tokens[0], comment);
    ASSERT_EOF(tokens[1], eof_token);   
}

static void short_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 0 };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_ABRUPT_CLOSING_OF_EMPTY_COMMENT);
    ASSERT_COMMENT(tokens[0], comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void short_comment_two()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!--->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 0 };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_ABRUPT_CLOSING_OF_EMPTY_COMMENT);
    ASSERT_COMMENT(tokens[0], comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void short_comment_three()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!---->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 0 };

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_COMMENT(tokens[0], comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void less_than_in_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-- <test-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 6,
                            .data = { [0] = ' ', [1] = '<', [2] = 't', [3] = 'e', [4] = 's', [5] = 't'}};

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_COMMENT(tokens[0], comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void double_less_than_in_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-- <<test-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 7,
                            .data = { [0] = ' ', [1] = '<', [2] = '<', [3] = 't', [4] = 'e', [5] = 's', [6] = 't'}};

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_COMMENT(tokens[0], comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void less_than_bang_in_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-- <!test-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 7,
                            .data = { [0] = ' ', [1] = '<', [2] = '!', [3] = 't', [4] = 'e', [5] = 's', [6] = 't'}};

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_COMMENT(tokens[0], comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void less_than_bang_dash_in_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-- <!-est-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 7,
                            .data = { [0] = ' ', [1] = '<', [2] = '!', [3] = '-', [4] = 'e', [5] = 's', [6] = 't'}};

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_COMMENT(tokens[0], comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void nested_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-- <!--test-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 9,
                            .data = { [0] = ' ', [1] = '<', [2] = '!', [3] = '-', [4] = '-', [5] = 't', [6] = 'e', [7] = 's', [8] = 't'}};

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_NESTED_COMMENT);
    ASSERT_COMMENT(tokens[0], comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void nested_comment_with_extra_less_than_sign()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-- <<!--test-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    html_token_t comment = {.is_valid = true, 
                            .type = HTML_COMMENT_TOKEN,
                            .data_size = 10,
                            .data = { [0] = ' ', [1] = '<', [2] = '<', [3] = '!', [4] = '-', [5] = '-', [6] = 't', [7] = 'e', [8] = 's', [9] = 't'}};

    html_tokenizer_error_e status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_NESTED_COMMENT);
    ASSERT_COMMENT(tokens[0], comment);

    status = html_tokenizer_next();
    ASSERT_TOKENS_SIZE(1, SIZE_TEN);
    ASSERT_EQUAL(status, HTML_TOKENIZER_OK);
    ASSERT_EOF(tokens[0], eof_token);
}

static void less_than_in_script_data()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<test-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);
    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK};

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'e' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                   { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void less_than_bang_in_script_data()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!test-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);
    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 2, 1, 1, 1, 1, 1, 1, 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK};

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
                                     {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '!' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'e' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 's' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 't' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '-' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '>' } } },
                                   { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void less_than_bang_dash_in_script_data()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-test-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);
    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK };

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
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

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void escaped_script_data()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!--test-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);
    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
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

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
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

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void less_than_in_script_html_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-- < test -->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);
    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
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

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
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

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void less_than_solidus_in_script_html_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-- </ test -->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);
    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
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

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
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

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void start_tag_in_script_html_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-- <test> -->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);
    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
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

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
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

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void end_tag_in_script_html_comment()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!-- </test> -->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);
    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 2, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
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

    html_token_t tokens_e[][7] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
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

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 7;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void dash_in_script_html_comment_double_escaped()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!--<script>-</script>-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);
    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
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
                                        HTML_TOKENIZER_OK};

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
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

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void dash_dash_in_script_html_comment_double_escaped()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!--<script>--</script>-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);
    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
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
                                        HTML_TOKENIZER_OK};

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
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

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void dash_dash_dash_in_script_html_comment_double_escaped()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!--<script>---</script>-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);
    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
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
                                        HTML_TOKENIZER_OK};

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
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

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void dash_spaced_in_script_html_comment_double_escaped()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!--<script> - </script>-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);
    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
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
                                        HTML_TOKENIZER_OK};

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
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

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void dash_dash_spaced_in_script_html_comment_double_escaped()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<!--<script> -- </script>-->";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);
    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
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
                                        HTML_TOKENIZER_OK};

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '<' } },
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

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void ampersand_eof()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "&";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } } },
                                   { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void ampersand_ampersand_eof()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "&&";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 1, 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } } },
                                   { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void ampersand_space_eof()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "& ";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 1, 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = ' ' } } },
                                   { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

// static void unfinished_enitity()
// {
//     html_token_t tokens[SIZE_TEN] = { 0 };

//     const char buffer[] = "&f";
//     const uint32_t buffer_size = sizeof(buffer) - 1;
//     html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

//     for (uint32_t i = 0; i < SIZE_TEN; i++)
//     {
//         ASSERT_FALSE(tokens[i].is_valid);
//     }

//     uint32_t return_sizes[]         = { 1, 1, 1 };
//     html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };

//     html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } } },
//                                    { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'f' } } },
//                                    { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

//     uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
//     uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
//     uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
//     ASSERT_EQUAL(return_sizes_len, errors_len);
//     ASSERT_EQUAL(tokens_e_len, errors_len);

//     uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
//     for (uint32_t i = 0; i < tests; i++)
//     {
//         uint32_t size_e = return_sizes[i];
//         html_tokenizer_error_e err_e    = errors[i];
//         html_tokenizer_error_e err_a    = html_tokenizer_next();

//         ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
//         ASSERT_EQUAL(err_a, err_e);

//         for (uint32_t j = 0; j < size_e; j++)
//         {
//             html_token_t token_e = tokens_e[i][j];
//             if (!token_e.is_valid) { continue; }
//             if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
//             if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
//         }
//     }
// }

static void ampersand_numbersign()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "&#";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 2, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_ABSENCE_OF_DIGITS_IN_NUMERIC_CHARACTER_REFERENCE, HTML_TOKENIZER_OK };

    html_token_t tokens_e[][2] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } },
                                     {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '#' } } },
                                   { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 2;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
}

static void unfinished_numeric_entity()
{
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "&#x";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 3, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_ABSENCE_OF_DIGITS_IN_NUMERIC_CHARACTER_REFERENCE, HTML_TOKENIZER_OK };

    html_token_t tokens_e[][3] = { { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '&' } },
                                     {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = '#' } },
                                     {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'x' } } },
                                   { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 3;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
        }
    }
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
    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<s o=& t>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK, HTML_TOKENIZER_OK };

    html_token_t tokens_e[][1] = { { {.is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 's' }, .attributes_size = 2, 
                                      .attributes = { [0] = { .name = { [0] = 'o' }, 
                                                              .name_size = 1, 
                                                              .value = { [0] = '&' }, 
                                                              .value_size = 1 },
                                                      [1] = { .name = { [0] = 't' },
                                                              .name_size = 1 } } } },
                                   { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 1;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
            if (token_e.type == HTML_START_TOKEN)       { ASSERT_TAG(tokens[j], token_e); }
        }
    }
}

static void unquoted_attribute_at_end_of_tag_with_final_char_ampersand()
{
    // {"description":"Unquoted attribute at end of tag with final character of &, with tag followed by characters",
    // "input":"<a a=a&>foo",
    // "output":[["StartTag", "a", {"a":"a&"}], ["Character", "foo"]]},

    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<a a=a&>foo";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 1, 1, 1, 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK };

    html_token_t tokens_e[][1] = { { {.is_valid = true, .type = HTML_START_TOKEN, .name_size = 1, .name = { [0] = 'a' }, .attributes_size = 1, 
                                      .attributes = { [0] = { .name = { [0] = 'a' }, 
                                                              .name_size = 1, 
                                                              .value = { [0] = 'a', [1] = '&' }, 
                                                              .value_size = 2 } } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'f' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'o' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'o' } } },
                                   { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 1;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
            if (token_e.type == HTML_START_TOKEN)       { ASSERT_TAG(tokens[j], token_e); }
        }
    }
}

static void plaintext_element()
{
    
    // {"description":"plaintext element",
    //  "input":"<plaintext>foobar",
    //  "output":[["StartTag","plaintext",{}], ["Character","foobar"]]},

    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<plaintext>foobar";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 1, 1, 1, 1, 1, 1, 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK,
                                        HTML_TOKENIZER_OK };

    html_token_t tokens_e[][1] = { { {.is_valid = true, 
                                      .type = HTML_START_TOKEN, 
                                      .name_size = 9, 
                                      .name = { [0] = 'p', [1] = 'l', [2] = 'a', [3] = 'i', [4] = 'n', [5] = 't', [6] = 'e', [7] = 'x', [8] = 't' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'f' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'o' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'o' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'b' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'a' } } },
                                   { {.is_valid = true, .type = HTML_CHARACTER_TOKEN, .data_size = 1, .data = { [0] = 'r' } } },
                                   { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 1;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
            if (token_e.type == HTML_START_TOKEN)       { ASSERT_TAG(tokens[j], token_e); }
        }
    }
}

static void open_angle_bracked_in_attribute_value()
{
    // {"description":"Open angled bracket in unquoted attribute value state",
    //  "input":"<a a=f<>",
    //  "output":[["StartTag", "a", {"a":"f<"}]],
    //  "errors":[
    //     { "code": "unexpected-character-in-unquoted-attribute-value", "line": 1, "col": 7 }
    // ]}

    html_token_t tokens[SIZE_TEN] = { 0 };

    const char buffer[] = "<a a=f<>";
    const uint32_t buffer_size = sizeof(buffer) - 1;
    html_tokenizer_init(buffer, buffer_size, tokens, SIZE_TEN);

    for (uint32_t i = 0; i < SIZE_TEN; i++)
    {
        ASSERT_FALSE(tokens[i].is_valid);
    }

    uint32_t return_sizes[]         = { 1, 1 };
    html_tokenizer_error_e errors[] = { HTML_TOKENIZER_UNEXPECTED_CHARACTER_IN_UNQUOTED_ATTRIBUTE_VALUE,
                                        HTML_TOKENIZER_OK };

    html_token_t tokens_e[][1] = { { {.is_valid = true, 
                                      .type = HTML_START_TOKEN, 
                                      .name_size = 1, 
                                      .name = { [0] = 'a' },
                                      .attributes_size = 1,
                                      .attributes = { [0] = { .name = { [0] = 'a' },
                                                              .name_size = 1,
                                                              .value = { [0] = 'f', [1] = '<' },
                                                              .value_size = 2 } } } },
                                   { {.is_valid = true, .type = HTML_EOF_TOKEN } } };

    uint32_t return_sizes_len = sizeof(return_sizes) / sizeof(uint32_t);
    uint32_t errors_len = sizeof(errors) / sizeof(html_tokenizer_error_e);
    uint32_t tokens_e_len = sizeof(tokens_e) / sizeof(html_token_t) / 1;
    ASSERT_EQUAL(return_sizes_len, errors_len);
    ASSERT_EQUAL(tokens_e_len, errors_len);

    uint32_t tests = sizeof(return_sizes) / sizeof(uint32_t);
    for (uint32_t i = 0; i < tests; i++)
    {
        uint32_t size_e = return_sizes[i];
        html_tokenizer_error_e err_e    = errors[i];
        html_tokenizer_error_e err_a    = html_tokenizer_next();

        ASSERT_TOKENS_SIZE(size_e, SIZE_TEN);
        ASSERT_EQUAL(err_a, err_e);

        for (uint32_t j = 0; j < size_e; j++)
        {
            html_token_t token_e = tokens_e[i][j];
            if (!token_e.is_valid) { continue; }
            if (token_e.type == HTML_CHARACTER_TOKEN)   { ASSERT_CHARACTER(tokens[j], token_e); }
            if (token_e.type == HTML_EOF_TOKEN)         { ASSERT_EOF(tokens[j], token_e); }
            if (token_e.type == HTML_START_TOKEN)       { ASSERT_TAG(tokens[j], token_e); }
        }
    }
}

void test_html_tokenizer()
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

}