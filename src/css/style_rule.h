#pragma once

#include "css/group_rule.h"

typedef struct
{
    css_group_rule_t rule;
} css_style_rule_t;

css_style_rule_t*   css_style_rule_new();
bool                css_is_style_rule(css_rule_t* rule);
css_rule_t*         css_rule_from_style_rule(css_style_rule_t* rule);
css_style_rule_t*   css_style_rule_from_rule(css_rule_t* rule);
void                css_style_rule_free(css_rule_t* rule);