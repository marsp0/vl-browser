#pragma once

#include <stdbool.h>

#include "css/rule.h"

typedef struct
{
    css_rule_t rule;
    css_rule_t rules[10];

} css_group_rule_t;


void                css_group_rule_init(css_group_rule_t* rule, uint32_t type);
css_group_rule_t*   css_group_rule_new();
bool                css_rule_is_group(css_rule_t* rule);
css_rule_t*         css_rule_from_group_rule(css_group_rule_t* rule);
css_group_rule_t*   css_group_rule_from_rule(css_rule_t* rule);
void                css_group_rule_add(css_rule_t* rule);
void                css_group_rule_remove(css_rule_t* rule);
void                css_group_rule_free(css_rule_t* rule);