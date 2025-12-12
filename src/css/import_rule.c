#include "import_rule.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "css/types.h"

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


css_import_rule_t* css_import_rule_new()
{
    css_import_rule_t* i_rule = malloc(sizeof(css_import_rule_t));

    css_rule_init((css_rule_t*)i_rule, CSS_RULE_IMPORT);

    return i_rule;
}


bool css_rule_is_import(css_rule_t* rule)
{
    return rule->type & CSS_RULE_IMPORT;
}


css_rule_t* css_rule_from_import_rule(css_import_rule_t* rule)
{
    return (css_rule_t*)rule;
}


css_import_rule_t* css_import_rule_from_rule(css_rule_t* rule)
{
    assert(css_rule_is_import(rule));

    return (css_import_rule_t*)rule;
}


void css_import_rule_add(css_rule_t* rule)
{
    assert(rule);
}


void css_import_rule_remove(css_rule_t* rule)
{
    assert(rule);
}


void css_import_rule_free(css_rule_t* rule)
{
    css_import_rule_t* i_rule = css_import_rule_from_rule(rule);

    free(i_rule);
}
