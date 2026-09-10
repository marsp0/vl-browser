#include "css/parser/types.h"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

/*
 * Notes
 * 
 */

/********************/
/*      defines     */
/********************/

#define BLOCK_SIZE 20
#define BLOCKS_SIZE 50

/********************/
/* static variables */
/********************/

typedef struct css_parser_node_block_t
{
    css_parser_node_t nodes[BLOCK_SIZE];
    struct css_parser_node_block_t* next;
    struct css_parser_node_block_t* prev;

} css_parser_node_block_t;

static css_parser_node_block_t first    = { 0 };
static css_parser_node_block_t* current = &first;
static uint32_t block_idx               = 0;
static uint32_t count                   = 0;
static uint32_t max_count               = 0;

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

void css_parser_types_init()
{
    
}


css_parser_node_t* css_parser_node_new(hash_str_t name, css_parser_node_type_e type)
{
    if (block_idx == BLOCK_SIZE)
    {
        css_parser_node_block_t* block =  malloc(sizeof(css_parser_node_block_t));
        memset(block, 0, sizeof(css_parser_node_block_t));

        current->next   = block;
        block->prev     = current;
        current         = block;
        block_idx       = 0;
    }

    css_parser_node_t* node = &current->nodes[block_idx];
    node->name              = name;
    node->type              = type;

    block_idx++;
    count++;

    return node;
}


void css_parser_node_add_sibling(css_parser_node_t* node, css_parser_node_t* sibling)
{
    if (!sibling) { return; }

    css_parser_node_t* tmp = node;
    while (tmp->next) { tmp = tmp->next; }

    tmp->next = sibling;
    sibling->prev = tmp;
    sibling->parent = tmp->parent;
}


void css_parser_node_add_comp_val(css_parser_node_t* node, css_parser_node_t* child)
{
    if (!child) { return; }

    child->parent = node;

    if (!node->comp_vals)
    {
        node->comp_vals = child;
    }
    else
    {
        css_parser_node_add_sibling(node->comp_vals, child);
    }
}


void css_parser_node_add_decl(css_parser_node_t* node, css_parser_node_t* child)
{
    if (!child) { return; }

    child->parent = node;

    if (!node->decls)
    {
        node->decls = child;
    }
    else
    {
        css_parser_node_add_sibling(node->decls, child);
    }
}


void css_parser_node_add_rule(css_parser_node_t* node, css_parser_node_t* child)
{
    if (!child) { return; }

    child->parent = node;

    if (!node->rules)
    {
        node->rules = child;
    }
    else
    {
        css_parser_node_add_sibling(node->rules, child);
    }
}


void css_parser_types_reset()
{
    css_parser_node_block_t* tmp = current;
    while (tmp->prev)
    {
        tmp = tmp->prev;
        memset(tmp->next, 0, sizeof(css_parser_node_block_t));
    }

    memset(tmp, 0, sizeof(css_parser_node_block_t));

    current = &first;
    block_idx = 0;

    if (count > max_count) { max_count = count; }
    count = 0;
}


void css_parser_types_free()
{
    printf("css_parser_node max count: %u\n", max_count);

    css_parser_node_block_t* prev = current;

    while (&first != prev)
    {
        prev = prev->prev;
        free(prev->next);
    }
}