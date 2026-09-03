#pragma once

typedef struct css_parser_node_t css_parser_node_t;
typedef struct css_value_t css_value_t;

css_value_t* css_value_parse_percentage(css_parser_node_t* node);