#pragma once

#include <stdint.h>

typedef struct css_parser_node_t css_parser_node_t;

void print_css_parse_tree(css_parser_node_t* node, uint32_t level);
void ASSERT_CSS_PARSER_NODE(css_parser_node_t* a, css_parser_node_t* e);