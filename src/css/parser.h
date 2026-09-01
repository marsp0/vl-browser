#pragma once

#include <stdint.h>

typedef struct css_parser_node_t css_parser_node_t;

void                css_parser_init(const unsigned char* buf, uint32_t buf_size);
css_parser_node_t*  css_parser_parse_stylesheet();
void                css_parser_free();