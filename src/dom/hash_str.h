#pragma once

#include <stdint.h>

typedef uint32_t hash_str_t;

void                    hash_str_pool_new();
hash_str_t              hash_str_new(const unsigned char* str, const uint32_t str_size);
hash_str_t              hash_str_compute(const unsigned char* str, const uint32_t str_size);
const unsigned char*    hash_str_get(hash_str_t hash);
uint32_t                hash_str_get_size(hash_str_t hash);
void                    hash_str_print(hash_str_t hash);
void                    hash_str_pool_stats();
void                    hash_str_pool_free();