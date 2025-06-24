#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    const unsigned char* data;
    uint32_t index;
    uint32_t size;
} utf8_view_t;

// utf_view_t  utf8_new_view(const unsigned char* buffer, const uint32_t size);
bool        utf8_validate(const unsigned char* buffer, const uint32_t size);
int32_t     utf8_encode(uint32_t code_point, unsigned char* buf);
int32_t     utf8_decode(const unsigned char* buf, const uint32_t size, const uint32_t cur, uint32_t* val);
// note: should below be taking code_point as input???
bool        utf8_is_upper_alpha(uint32_t code_point);
bool        utf8_is_lower_alpha(uint32_t code_point);
bool        utf8_is_alpha(uint32_t code_point);
bool        utf8_is_digit(uint32_t code_point);
bool        utf8_is_alphanumeric(uint32_t code_point);
bool        utf8_is_lower_hex(uint32_t code_point);
bool        utf8_is_upper_hex(uint32_t code_point);
bool        utf8_is_hex(uint32_t code_point);
bool        utf8_is_control(uint32_t code_point);
bool        utf8_is_whitespace(uint32_t code_point);