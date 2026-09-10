#pragma once

#include <stdint.h>

#include "dom/hash_str.h"

typedef struct css_token_t css_token_t;

typedef enum
{
    CSS_PARSER_NODE_TYPE_STYLESHEET,
    CSS_PARSER_NODE_TYPE_AT_RULE,
    CSS_PARSER_NODE_TYPE_Q_RULE,
    CSS_PARSER_NODE_TYPE_DECL,
    CSS_PARSER_NODE_TYPE_BLOCK,
    CSS_PARSER_NODE_TYPE_TOKEN,
    CSS_PARSER_NODE_TYPE_FUNCTION
} css_parser_node_type_e;


typedef struct css_parser_node_t
{
    hash_str_t                  name;           // used by at-rules, declarations, function
    css_parser_node_type_e      type;

    struct css_parser_node_t*   comp_vals;      // used by 
                                                //      - at-rules (prelude)
                                                //      - qualified rules (prelude)
                                                //      - declaration (component values)
                                                //      - simple block (component values)
                                                //      - function (component values)

    struct css_parser_node_t*   decls;          // used by
                                                //      - at-rules (decls)
                                                //      - qualified rules

    struct css_parser_node_t*   rules;          // used by 
                                                //      - stylesheet        (list of rules)
                                                //      - at-rules          (list of child rules)
                                                //      - qualified rules   (list of child rules)

    css_token_t*                token;          // used by component values and blocks

    struct css_parser_node_t*   parent;
    struct css_parser_node_t*   next;
    struct css_parser_node_t*   prev;
    
} css_parser_node_t;

css_parser_node_t*          css_parser_node_new(hash_str_t name, css_parser_node_type_e type);
void                        css_parser_node_add_sibling(css_parser_node_t* node, css_parser_node_t* sibling);
void                        css_parser_node_add_comp_val(css_parser_node_t* node, css_parser_node_t* child);
void                        css_parser_node_add_decl(css_parser_node_t* node, css_parser_node_t* child);
void                        css_parser_node_add_rule(css_parser_node_t* node, css_parser_node_t* child);

void                        css_parser_types_init();
void                        css_parser_types_reset();
void                        css_parser_types_free();