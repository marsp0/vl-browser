#include "css/rule.h"

#include <stdbool.h>

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

void css_rule_init(css_rule_t* rule, uint32_t type)
{
    rule->type = type;
}


void css_rule_free(css_rule_t* rule)
{
    if (css_rule_is_group(rule)) { css_group_rule_free(rule); }
}