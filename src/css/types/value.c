/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "value.h"

#include <stdlib.h>
#include <string.h>

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

css_value_t* css_value_new(css_value_type_e type, css_value_type_unit_e unit)
{
    css_value_t* val = malloc(sizeof(css_value_t));
    memset(val, 0, sizeof(css_value_t));

    val->type = type;
    val->unit = unit;

    return val;
}


void css_value_free(css_value_t* val)
{
    free(val);
}