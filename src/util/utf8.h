#pragma once

#include <stdint.h>
#include <stdbool.h>

bool        utf8_validate(const unsigned char* buffer, const uint32_t size);
int32_t     utf8_encode(uint32_t code_point, unsigned char* buf);
int32_t     utf8_decode(const unsigned char* buf, const uint32_t size, const uint32_t cur, uint32_t* val);
uint32_t    utf8_get_len(const unsigned char* buffer, const uint32_t size);
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