/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "css/rule.h"

#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "css/decl.h"

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

css_rule_t* css_rule_new(css_rule_type_e type)
{
    css_rule_t* rule = malloc(sizeof(css_rule_t));

    memset(rule, 0, sizeof(css_rule_t));
    rule->type = type;

    return rule;
}


void css_rule_init(css_rule_t* rule, uint32_t type)
{
    rule->type = type;
}


void css_rule_add_decl(css_rule_t* rule, css_decl_t* decl)
{
    if (!decl) { return; }

    if (!rule->decls)
    {
        rule->decls = decl;
    }
    else
    {
        css_decl_t* child = rule->decls;
        while (child->next)
        {
            child = child->next;
        }

        child->next = decl;
        decl->prev = child;
    }
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
    free(rule);
}