#include "css/rule.h"

#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

/*
 * Notes
 * 
 */

/********************/
/*      defines     */
/********************/

void css_group_rule_free(css_rule_t* rule);
bool css_rule_is_group(css_rule_t* rule);

/********************/
/* static variables */
/********************/


/********************/
/* static functions */
/********************/


/********************/
/* public functions */
/********************/


css_rule_t* css_rule_new()
{
    css_rule_t* rule = malloc(sizeof(css_rule_t));

    memset(rule, 0, sizeof(css_rule_t));

    return rule;
}


void css_rule_init(css_rule_t* rule, uint32_t type)
{
    rule->type = type;
}


void css_rule_add_sibling(css_rule_t* rule, css_rule_t* sibling)
{
    assert(rule);

    if(!sibling)
    {
        return;
    }

    while (rule->next)
    {
        rule = rule->next;
    }

    rule->next      = sibling;
    sibling->prev   = rule;
    sibling->parent = rule->parent;
}


void css_rule_free(css_rule_t* rule)
{
    if (css_rule_is_group(rule)) { css_group_rule_free(rule); }
}