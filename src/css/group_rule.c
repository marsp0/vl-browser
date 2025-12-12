#include "group_rule.h"

#include <stdlib.h>
#include <assert.h>

#include "css/types.h"
#include "css/rule.h"


void css_group_rule_init(css_group_rule_t* rule, uint32_t type)
{
    css_rule_init((css_rule_t*)rule, type | CSS_RULE_GROUP);
}


css_group_rule_t* css_group_rule_new()
{
    css_group_rule_t* g_rule = malloc(sizeof(css_group_rule_t));

    css_group_rule_init(g_rule, CSS_RULE_GROUP);

    return g_rule;
}


bool css_rule_is_group(css_rule_t* rule)
{
    return rule->type & CSS_RULE_GROUP;
}


css_rule_t* css_rule_from_group_rule(css_group_rule_t* rule)
{
    return (css_rule_t*)rule;
}


css_group_rule_t* css_group_rule_from_rule(css_rule_t* rule)
{
    assert(css_rule_is_group(rule));

    return (css_group_rule_t*)rule;
}


void css_group_rule_free(css_rule_t* rule)
{
    css_group_rule_t* g_rule = css_group_rule_from_rule(rule);
    free(g_rule);
}