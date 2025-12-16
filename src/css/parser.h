#pragma once

#include <stdint.h>

#include "core/op_result.h"

struct css_parser_stylesheet_t;

typedef struct
{
    op_result_e                     result;
    struct css_parser_stylesheet_t* sheet;
} css_parser_result_t;

void                css_parser_init(const unsigned char* buf, uint32_t buf_size);
css_parser_result_t css_parser_run();
void                css_parser_free();