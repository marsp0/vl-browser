#pragma once

#include <stdbool.h>

typedef struct css_decl_t css_decl_t;
typedef struct css_rule_t css_rule_t;
typedef struct dom_node_t dom_node_t;

typedef struct css_decl_block_t
{
    css_decl_t*     decls;
    css_rule_t*     parent;
    dom_node_t*     owner;
    bool            updating;
    bool            computed;
    bool            read_only;

} css_decl_block_t;

css_decl_block_t*   css_decl_block_new();
void                css_decl_block_free(css_decl_block_t* block);
