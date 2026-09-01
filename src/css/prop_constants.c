/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "prop_constants.h"

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/

hash_str_t color = 0;

/********************/
/* static functions */
/********************/


/********************/
/* public functions */
/********************/


hash_str_t css_prop_color()
{
    return color;
}


void css_prop_names_init()
{
    color = hash_str_new("color", 5);
}