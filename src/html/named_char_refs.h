#pragma once

#include <stdint.h>

#include "dom/hash_str.h"

void html_named_char_ref_map_init();
void html_named_char_ref_map_free();
uint32_t html_get_named_char_ref(hash_str_t key);
