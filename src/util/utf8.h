#pragma once

#include <stdint.h>
#include <stdbool.h>

bool    utf8_validate(unsigned char* buffer, uint32_t size);
int32_t utf8_code_point(unsigned char* buffer, uint32_t size, uint32_t cursor, uint32_t* value);