#pragma once

typedef struct css_rule_t css_rule_t;
typedef struct css_decl_t css_decl_t;
typedef struct css_parser_node_t css_parser_node_t;

css_rule_t* css_parser_rule_to_om_rule(css_parser_node_t* node);
css_decl_t* css_parser_decl_to_om_decl(css_parser_node_t* node);