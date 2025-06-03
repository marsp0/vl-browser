#pragma once

#include <stdint.h>
#include <stdbool.h>

bool    utf8_validate(const unsigned char* buffer, const uint32_t size);
int32_t utf8_code_point(const unsigned char* buf, const uint32_t size, const uint32_t cur, uint32_t* val);
bool    utf8_is_upper_alpha(uint32_t code_point);
bool    utf8_is_lower_alpha(uint32_t code_point);
bool    utf8_is_alpha(uint32_t code_point);