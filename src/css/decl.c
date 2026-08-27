/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "decl.h"

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

css_decl_t* css_decl_new(hash_str_t name, hash_str_t val)
{
    css_decl_t decl = malloc(sizeof(css_decl_t));

    memset(decl, 0, sizeof(css_decl_t));

    decl.name       = name;
    decl.value      = value;
    decl.important  = false;
    decl.case_sens  = false;

    return decl;
}


void css_decl_free(css_decl_t* decl)
{
    free(decl);
}