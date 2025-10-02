#pragma once

#include "test_utils.h"
#include "html/tokenizer.h"

#define MAX_TOKENS 10

#define ASSERT_DOCTYPE(a, e)                                                \
do                                                                          \
{                                                                           \
    ASSERT_TRUE(a.is_valid);                                                \
    ASSERT_EQUAL(a.is_valid, e.is_valid);                                   \
    ASSERT_EQUAL(a.type, e.type);                                           \
    ASSERT_EQUAL(a.name_size, e.name_size);                                 \
    ASSERT_STRING((char)a.name, (char)e.name, a.name_size);                 \
    ASSERT_EQUAL(a.public_id_size, e.public_id_size);                       \
    ASSERT_STRING((char)a.public_id, (char)e.public_id, a.public_id_size);  \
    ASSERT_EQUAL(a.system_id_size, e.system_id_size);                       \
    ASSERT_STRING((char)a.system_id, (char)e.system_id, a.system_id_size);  \
    ASSERT_EQUAL(a.force_quirks, e.force_quirks);                           \
} while (0);


#define ASSERT_COMMENT(a, e)                                    \
do                                                              \
{                                                               \
    ASSERT_TRUE(a.is_valid);                                    \
    ASSERT_EQUAL(a.is_valid, e.is_valid);                       \
    ASSERT_EQUAL(a.type, e.type);                               \
    ASSERT_EQUAL(a.data_size, e.data_size);                     \
    ASSERT_STRING((char)a.data, (char)e.data, a.data_size);     \
} while (0);


#define ASSERT_TAG(a, e)                                        \
do                                                              \
{                                                               \
    ASSERT_TRUE(a.is_valid);                                    \
    ASSERT_EQUAL(a.is_valid, e.is_valid);                       \
    ASSERT_EQUAL(a.type, e.type);                               \
    ASSERT_EQUAL(a.name_size, e.name_size);                     \
    ASSERT_STRING((char)a.name, (char)e.name, a.name_size);     \
    ASSERT_EQUAL(a.attributes_size, e.attributes_size);         \
    ASSERT_EQUAL(a.self_closing, e.self_closing);               \
    for (uint32_t k = 0; k < a.attributes_size; k++)            \
    {                                                           \
        html_token_attribute_t a_attr = a.attributes[k];        \
        html_token_attribute_t e_attr = e.attributes[k];        \
        ASSERT_EQUAL(a_attr.name_size, e_attr.name_size);       \
        ASSERT_EQUAL(a_attr.value_size, e_attr.value_size);     \
        ASSERT_STRING((char)a_attr.name, (char)e_attr.name, a_attr.name_size); \
        ASSERT_STRING((char)a_attr.value, (char)e_attr.value, a_attr.value_size); \
    }                                                           \
} while (0);


#define ASSERT_EOF(a, e)                                        \
do                                                              \
{                                                               \
    ASSERT_TRUE(a.is_valid);                                    \
    ASSERT_EQUAL(a.is_valid, e.is_valid);                       \
    ASSERT_EQUAL(a.type, e.type);                               \
} while (0);


#define ASSERT_CHARACTER(a, e)                                  \
do                                                              \
{                                                               \
    ASSERT_TRUE(a.is_valid);                                    \
    ASSERT_EQUAL(a.is_valid, e.is_valid);                       \
    ASSERT_EQUAL(a.type, e.type);                               \
    ASSERT_EQUAL(a.data_size, e.data_size);                     \
    ASSERT_STRING((char)a.data, (char)e.data, a.data_size);     \
} while (0);


#define ASSERT_TOKEN(a, e)                                          \
do                                                                  \
{                                                                   \
    if (a.type == HTML_DOCTYPE_TOKEN)   { ASSERT_DOCTYPE(a, e); }   \
    if (a.type == HTML_START_TOKEN)     { ASSERT_TAG(a, e); }       \
    if (a.type == HTML_END_TOKEN)       { ASSERT_TAG(a, e); }       \
    if (a.type == HTML_COMMENT_TOKEN)   { ASSERT_COMMENT(a, e); }   \
    if (a.type == HTML_CHARACTER_TOKEN) { ASSERT_CHARACTER(a, e); } \
    if (a.type == HTML_EOF_TOKEN)       { ASSERT_EOF(a, e); }       \
} while (0);                                                        \


#define RUN_TEST_AND_ASSERT_TOKENS(buffer, states, sizes, errors, tokens_e)             \
do                                                                                      \
{                                                                                       \
    html_token_t tokens[MAX_TOKENS] = { 0 };                                            \
                                                                                        \
    const uint32_t buffer_size      = sizeof(buffer) - 1;                               \
    const uint32_t states_size      = sizeof(states) / sizeof(html_tokenizer_state_e);  \
    const uint32_t sizes_size       = sizeof(sizes) / sizeof(uint32_t);                 \
    const uint32_t errors_size      = sizeof(errors) / sizeof(html_tokenizer_error_e);  \
    const uint32_t tokens_e_size    = sizeof(tokens_e) / MAX_TOKENS / sizeof(html_token_t);          \
                                                                                        \
    for (uint32_t s = 0; s < states_size; s++)                                          \
    {                                                                                   \
        memset(tokens, 0, sizeof(tokens));                                              \
        html_tokenizer_init(buffer, buffer_size, tokens, MAX_TOKENS);                   \
        html_tokenizer_set_state(states[s]);                                            \
                                                                                        \
        for (uint32_t i = 0; i < MAX_TOKENS; i++) { ASSERT_FALSE(tokens[i].is_valid); } \
                                                                                        \
        ASSERT_EQUAL(sizes_size, errors_size);                                          \
        ASSERT_EQUAL(tokens_e_size, errors_size);                                       \
                                                                                        \
        for (uint32_t i = 0; i < sizes_size; i++)                                       \
        {                                                                               \
            html_tokenizer_error_e err_e    = errors[i];                                \
            html_tokenizer_error_e err_a    = html_tokenizer_next();                    \
            ASSERT_EQUAL(err_a, err_e);                                                 \
                                                                                        \
            uint32_t size_e = sizes[i];                                                 \
            uint32_t size_a = 0;                                                        \
            for (uint32_t k = 0; k < MAX_TOKENS; k++)                                   \
            {                                                                           \
                if (tokens[k].is_valid) { size_a++; }                                   \
            }                                                                           \
            ASSERT_EQUAL(size_a, size_e);                                               \
                                                                                        \
            for (uint32_t j = 0; j < size_e; j++)                                       \
            {                                                                           \
                html_token_t token_e = tokens_e[i][j];                                  \
                ASSERT_TOKEN(tokens[j], token_e);                                       \
            }                                                                           \
        }                                                                               \
    }                                                                                   \
} while (0);                                                                            \

void test_html_tokenizer_test1();
void test_html_tokenizer_test2();
void test_html_tokenizer_test3();
void test_html_tokenizer_rcdata();
void test_html_tokenizer_rawtext();
void test_html_tokenizer_script_data();