#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "dom/hash_str.h"

#define CSS_TOKEN_MAX_DATA_SIZE 64


typedef enum
{
    CSS_TOKEN_HASH_UNRESTRICTED,
    CSS_TOKEN_HASH_ID
} css_token_hash_type_e;


typedef enum
{
    CSS_TOKENIZER_DATA_STATE,
    CSS_TOKENIZER_STRING_STATE,
    CSS_TOKENIZER_STRING_REVERSE_SOLIDUS_STATE,
    CSS_TOKENIZER_POUND_SIGN_STATE,
    CSS_TOKENIZER_POUND_SIGN_REVERSE_SOLIDUS_STATE,
    CSS_TOKENIZER_ID_SEQ_STATE,
    CSS_TOKENIZER_NUMERIC_STATE,
} css_tokenizer_state_e;


typedef enum
{
    CSS_TOKEN_DELIM,
    CSS_TOKEN_IDENT,
    CSS_TOKEN_FUNCTION,
    CSS_TOKEN_AT_KEYWORD,
    CSS_TOKEN_HASH,
    CSS_TOKEN_STRING,
    CSS_TOKEN_BAD_STRING,
    CSS_TOKEN_PERCENTAGE,
    CSS_TOKEN_DIMENSION,
    CSS_TOKEN_NUMBER,
    CSS_TOKEN_WHITESPACE,
    CSS_TOKEN_CDO,
    CSS_TOKEN_CDC,
    CSS_TOKEN_COLON,
    CSS_TOKEN_SEMICOLON,
    CSS_TOKEN_COMMA,
    CSS_TOKEN_OPEN_BRACKET,
    CSS_TOKEN_CLOSED_BRACKET,
    CSS_TOKEN_OPEN_PARENTHESIS,
    CSS_TOKEN_CLOSED_PARENTHESIS,
    CSS_TOKEN_OPEN_BRACE,
    CSS_TOKEN_CLOSED_BRACE,
    CSS_TOKEN_EOF
} css_token_type_e;


typedef struct
{
    css_token_type_e        type;
    unsigned char           data[CSS_TOKEN_MAX_DATA_SIZE];
    uint32_t                data_size;
    int32_t                 integer;
    float                   real;
    css_token_hash_type_e   hash_type;
} css_token_t;


void        css_tokenizer_init(unsigned char* buffer, uint32_t buffer_size);
css_token_t css_tokenizer_next();
void        css_tokenizer_free();