#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    unsigned char*  data;
    uint32_t        size;   // size in bytes
    uint32_t        len;    // amount of code points
} string_t;

string_t    string_new(unsigned char* data, uint32_t size);
bool        string_compare(const string_t first, const string_t second);
bool        string_compare_buffer(const string_t first, unsigned char* buffer, uint32_t buffer_size);
void        string_free(string_t str);