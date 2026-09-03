/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "properties.h"

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/

hash_str_t color                = 0;
hash_str_t padding_left         = 0;

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


hash_str_t css_prop_padding_left()
{
    return padding_left;
}


void css_properties_init()
{
    color               = hash_str_new("color", 5);
    padding_left        = hash_str_new("padding-left", 12);
}