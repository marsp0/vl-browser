#pragma once

#include <stdint.h>

#define MAX_CSS_RULE_FIELD_SIZE 64

typedef struct css_style_sheet_t    css_style_sheet_t;
typedef struct css_decl_t           css_decl_t;


typedef enum
{
    CSS_RULE_TYPE_STYLE
} css_rule_type_e;


typedef struct css_rule_t
{
    css_rule_type_e         type;
    // unsigned char               text[MAX_CSS_RULE_FIELD_SIZE];

    // style rule
    css_decl_t*             decls;

    struct css_rule_t*      parent;
    css_style_sheet_t*      sheet;

    struct css_rule_t*      next;
    struct css_rule_t*      prev;
} css_rule_t;

css_rule_t* css_rule_new(css_rule_type_e type);
void        css_rule_init(css_rule_t* rule, uint32_t type);
void        css_rule_add_decl(css_rule_t* rule, css_decl_t* decl);
void        css_rule_add_sibling(css_rule_t* rule, css_rule_t* sibling);
void        css_rule_free(css_rule_t* rule);