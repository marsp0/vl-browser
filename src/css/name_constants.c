/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "css/name_constants.h"

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/

hash_str_t inherit = 0;
hash_str_t initial = 0;
hash_str_t unset = 0;


/********************/
/* static functions */
/********************/


/********************/
/* public functions */
/********************/

void css_name_constants_init()
{
    inherit = hash_str_new("inherit", 7);
    initial = hash_str_new("initial", 7);
    unset = hash_str_new("unset", 5);
}


hash_str_t css_name_inherit()
{
    return inherit;
}


hash_str_t css_name_initial()
{
    return initial;
}


hash_str_t css_name_unset()
{
    return unset;
}
