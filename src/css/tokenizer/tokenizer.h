#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "css/tokenizer/types.h"
#include "dom/hash_str.h"

void        css_tokenizer_global_init();
void        css_tokenizer_init(const unsigned char* buffer, uint32_t buffer_size);
css_token_t css_tokenizer_next();
void        css_tokenizer_free();
void        css_tokenizer_global_free();