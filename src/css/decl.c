/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "decl.h"

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

css_decl_t* css_decl_new()
{
    css_decl_t* decl = malloc(sizeof(css_decl_t));

    memset(decl, 0, sizeof(css_decl_t));

    // decl->prop       = { 0 };
    // decl->value      = { 0 };
    // decl->important  = false;
    // decl->case_sens  = false;

    return decl;
}


void css_decl_free(css_decl_t* decl)
{
    free(decl);
}