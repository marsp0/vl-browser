#include "style_rule.h"

#include <stdlib.h>
#include <assert.h>

#include "css/types.h"
#include "css/group_rule.h"

/*
 * Notes
 * 
 */

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/


/********************/
/* static functions */
/********************/


/********************/
/* public functions */
/********************/

css_style_rule_t* css_style_rule_new()
{
    css_style_rule_t* s_rule = malloc(sizeof(css_style_rule_t));

    css_group_rule_init((css_group_rule_t*)s_rule, CSS_RULE_STYLE);

    return s_rule;
}


bool css_is_style_rule(css_rule_t* rule)
{
    return rule->type & CSS_RULE_STYLE;
}


css_rule_t* css_rule_from_style_rule(css_style_rule_t* rule)
{
    return (css_rule_t*)rule;
}


css_style_rule_t* css_style_rule_from_rule(css_rule_t* rule)
{
    assert(css_is_style_rule(rule));

    return (css_style_rule_t*)rule;
}


void css_style_rule_free(css_rule_t* rule)
{
    css_style_rule_t* s_rule = css_style_rule_from_rule(rule);

    free(s_rule);
}