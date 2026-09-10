#include "css/tokenizer/types.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Notes
 * 
 */

/********************/
/*      defines     */
/********************/

#define BLOCK_SIZE 40

/********************/
/* static variables */
/********************/

typedef struct css_token_block_t
{
    css_token_t block[BLOCK_SIZE];

    struct css_token_block_t* next;
    struct css_token_block_t* prev;

} css_token_block_t;

static css_token_block_t first      = { 0 };
static css_token_block_t* current   = &first;
static uint32_t block_idx           = 0;

static uint32_t count               = 0;
static uint32_t max_count           = 0;

/********************/
/* static functions */
/********************/

static void css_tokenizer_block_allocate()
{
    css_token_block_t* block = malloc(sizeof(css_token_block_t));

    current->next = block;
    block->prev = current;
    block_idx = 0;
    current = block;
}

/********************/
/* public functions */
/********************/


void css_tokenizer_types_init()
{
    
}


css_token_t* css_token_new()
{
    if (block_idx == BLOCK_SIZE) { css_tokenizer_block_allocate(); }

    css_token_t* token = &(current->block[block_idx]);
    block_idx++;

    count++;

    return token;
}


void css_tokenizer_types_reset()
{
    css_token_block_t* tmp = current;
    while (tmp->prev)
    {
        tmp = tmp->prev;
        memset(tmp->next, 0, sizeof(css_token_block_t));
    }

    memset(tmp, 0, sizeof(css_token_block_t));

    current = &first;
    block_idx = 0;

    if (count > max_count) { max_count = count; }
    count = 0;
}


void css_tokenizer_types_free()
{
    printf("css_token_t max count: %u\n", max_count);

    css_token_block_t* prev = current;

    while (&first != prev)
    {
        prev = prev->prev;
        free(prev->next);
    }
}
