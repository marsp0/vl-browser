#pragma once

#include <stdint.h>
#include <stdbool.h>

#define HTML_TOKEN_MAX_NAME_LEN     64
#define HTML_TOKEN_MAX_ATTRIBUTES   5

typedef enum
{
    HTML_TOKENIZER_OK,
    HTML_TOKENIZER_DONE,
    HTML_TOKENIZER_ERROR

} html_tokenizer_status_e;

// https://html.spec.whatwg.org/multipage/parsing.html#tokenization
typedef enum
{
    HTML_DOCTYPE_TOKEN,
    HTML_START_TOKEN,
    HTML_END_TOKEN,
    HTML_COMMENT_TOKEN,
    HTML_CHARACTER_TOKEN,
    HTML_EOF_TOKEN

} html_token_type_e;

typedef struct
{
    unsigned char   name[HTML_TOKEN_MAX_NAME_LEN];
    uint32_t        name_size;
    unsigned char   value[HTML_TOKEN_MAX_NAME_LEN];
    uint32_t        value_size;

} html_token_attribute_t;

typedef struct
{
    bool                        is_valid;
    html_token_type_e           type;

    unsigned char               name[HTML_TOKEN_MAX_NAME_LEN];
    uint32_t                    name_size;

    // DOCTYPE
    unsigned char               public_id[HTML_TOKEN_MAX_NAME_LEN];
    uint32_t                    public_id_size;
    unsigned char               system_id[HTML_TOKEN_MAX_NAME_LEN];
    uint32_t                    system_id_size;
    bool                        force_quirks;

    // start/end tags
    html_token_attribute_t      attributes[HTML_TOKEN_MAX_ATTRIBUTES];
    uint32_t                    attributes_size;
    bool                        self_closing;

    // comments and character tokens
    unsigned char               data[HTML_TOKEN_MAX_NAME_LEN];
    uint32_t                    data_size;

} html_token_t;

void                    html_tokenizer_init(const unsigned char* new_buffer, const uint32_t new_size, html_token_t* new_tokens, const uint32_t new_max_tokens);
html_tokenizer_status_e html_tokenizer_next();
void                    html_tokenizer_free();
