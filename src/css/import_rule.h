#pragma once

#include <stdbool.h>

#include "css/rule.h"

struct css_style_sheet_t;

typedef struct
{
    css_rule_t                  rule;

    unsigned char               href[MAX_CSS_RULE_FIELD_SIZE];
    unsigned char               layer[MAX_CSS_RULE_FIELD_SIZE];
    unsigned char               supports_text[MAX_CSS_RULE_FIELD_SIZE];
    uint32_t*                   media_list;
    struct css_style_sheet_t*   style_sheet;

} css_import_rule_t;

css_import_rule_t*  css_import_rule_new();
bool                css_rule_is_import(css_rule_t* rule);
css_rule_t*         css_rule_from_import_rule(css_import_rule_t* rule);
css_import_rule_t*  css_import_rule_from_rule(css_rule_t* rule);
void                css_import_rule_add(css_rule_t* rule);
void                css_import_rule_remove(css_rule_t* rule);
void                css_import_rule_free(css_rule_t* rule);