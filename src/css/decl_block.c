/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "decl_block.h"

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

css_decl_block_t* css_decl_block_new()
{
    css_decl_block_t* block = malloc(sizeof(css_decl_block_t));

    memset(block, 0, sizeof(css_decl_block_t));

    return block;
}


void css_decl_block_free(css_decl_block_t* block)
{
    free(block);
}
