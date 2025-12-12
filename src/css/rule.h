#pragma once

#include <stdint.h>

#define MAX_CSS_RULE_FIELD_SIZE 64

struct css_style_sheet_t;

typedef struct css_rule_t
{
    uint32_t                    type;
    unsigned char               text[MAX_CSS_RULE_FIELD_SIZE];
    struct css_rule_t*          parent_rule;
    struct css_style_sheet_t*   parent_sheet;
} css_rule_t;

void css_rule_init(css_rule_t* rule, uint32_t type);
void css_rule_free(css_rule_t* rule);