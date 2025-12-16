#include "parser_types.h"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

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

static css_parser_comp_val_t* comp_val_blocks[BLOCKS_SIZE]      = { 0 };
static uint32_t comp_val_block                                  = 0;
static uint32_t comp_val_block_idx                              = 0;

static css_parser_decl_t* decl_blocks[BLOCKS_SIZE]              = { 0 };
static uint32_t decl_block                                      = 0;
static uint32_t decl_block_idx                                  = 0;

static css_parser_rule_t* rule_blocks[BLOCKS_SIZE]              = { 0 };
static uint32_t rule_block                                      = 0;
static uint32_t rule_block_idx                                  = 0;

static css_parser_stylesheet_t* stylesheet_blocks[BLOCKS_SIZE]  = { 0 };
static uint32_t stylesheet_block                                = 0;
static uint32_t stylesheet_block_idx                            = 0;

/********************/
/* static functions */
/********************/


/********************/
/* public functions */
/********************/

void css_parser_types_init()
{
    comp_val_blocks[comp_val_block] = malloc(sizeof(css_parser_comp_val_t) * BLOCK_SIZE);
}


css_parser_comp_val_t* css_parser_comp_val_new()
{
    css_parser_comp_val_t* val = &comp_val_blocks[comp_val_block][comp_val_block_idx];
    comp_val_block_idx++;

    if (comp_val_block_idx < BLOCK_SIZE) { return val; }

    comp_val_block++;
    comp_val_block_idx = 0;

    assert(comp_val_block < BLOCKS_SIZE);

    if (comp_val_blocks[comp_val_block] != NULL) { return val; }

    comp_val_blocks[comp_val_block] = malloc(sizeof(css_parser_comp_val_t) * BLOCK_SIZE);

    return val;
}


css_parser_decl_t* css_parser_decl_new()
{
    css_parser_decl_t* val = &decl_blocks[decl_block][decl_block_idx];
    decl_block_idx++;

    if (decl_block_idx < BLOCK_SIZE) { return val; }

    decl_block++;
    decl_block_idx = 0;

    assert(decl_block < BLOCKS_SIZE);

    if (decl_blocks[decl_block] != NULL) { return val; }

    decl_blocks[decl_block] = malloc(sizeof(css_parser_decl_t) * BLOCK_SIZE);

    return val;
}


css_parser_rule_t* css_parser_rule_new()
{
    css_parser_rule_t* val = &rule_blocks[rule_block][rule_block_idx];
    rule_block_idx++;

    if (rule_block_idx < BLOCK_SIZE) { return val; }

    rule_block++;
    rule_block_idx = 0;

    assert(rule_block < BLOCKS_SIZE);

    if (rule_blocks[rule_block] != NULL) { return val; }

    rule_blocks[rule_block] = malloc(sizeof(css_parser_rule_t) * BLOCK_SIZE);

    return val;
}


css_parser_stylesheet_t* css_parser_stylesheet_new()
{
    css_parser_stylesheet_t* val = &stylesheet_blocks[stylesheet_block][stylesheet_block_idx];
    stylesheet_block_idx++;

    if (stylesheet_block_idx < BLOCK_SIZE) { return val; }

    stylesheet_block++;
    stylesheet_block_idx = 0;

    assert(stylesheet_block < BLOCKS_SIZE);

    if (stylesheet_blocks[stylesheet_block] != NULL) { return val; }

    stylesheet_blocks[stylesheet_block] = malloc(sizeof(css_parser_stylesheet_t) * BLOCK_SIZE);

    return val;
}


void css_parser_rule_add_comp_val(css_parser_rule_t* rule, css_parser_comp_val_t* c_val)
{
    if (!rule->comp_vals)
    {
        rule->comp_vals = c_val;
        rule->comp_vals_size++;
    }
    else
    {
        css_parser_comp_val_t* child = rule->comp_vals;

        while (child->next)
        {
            child = child->next;
        }

        child->next = c_val;
        rule->comp_vals_size++;
    }
}


void css_parser_rule_add_decl(css_parser_rule_t* rule, css_parser_decl_t* decl)
{
    if (!rule->decls)
    {
        rule->decls = decl;
        rule->decls_size++;
    }
    else
    {
        css_parser_decl_t* child = rule->decls;

        while (child->next)
        {
            child = child->next;
        }

        child->next = decl;
        rule->decls_size++;
    }
}


void css_parser_rule_add_rule(css_parser_rule_t* rule, css_parser_rule_t* n_rule)
{
    if (!rule->rules)
    {
        rule->rules = n_rule;
        rule->rules_size++;
    }
    else
    {
        css_parser_rule_t* child = rule->rules;

        while (child->next)
        {
            child = child->next;
        }

        child->next = n_rule;
        rule->rules_size++;
    }
}


void css_parser_comp_val_add_comp_val(css_parser_comp_val_t* c_val, css_parser_comp_val_t* child_val)
{
    if (!c_val->comp_vals)
    {
        c_val->comp_vals = child_val;
        c_val->comp_vals_size++;
    }
    else
    {
        css_parser_comp_val_t* child = c_val->comp_vals;

        while (child->next)
        {
            child = child->next;
        }

        child->next = child_val;
        c_val->comp_vals_size++;
    }
}


void css_parser_decl_add_comp_val(css_parser_decl_t* decl, css_parser_comp_val_t* c_val)
{
    if (!decl->comp_vals)
    {
        decl->comp_vals = c_val;
        decl->comp_vals_size++;
    }
    else
    {
        css_parser_comp_val_t* child = decl->comp_vals;

        while (child->next)
        {
            child = child->next;
        }

        child->next = c_val;
        decl->comp_vals_size++;
    }
}


void css_parser_types_reset()
{
    comp_val_block          = 0;
    comp_val_block_idx      = 0;

    decl_block              = 0;
    decl_block_idx          = 0;

    rule_block              = 0;
    rule_block_idx          = 0;

    stylesheet_block        = 0;
    stylesheet_block_idx    = 0;
}


void css_parser_types_free()
{
    for (uint32_t i = 0; i < BLOCKS_SIZE; i++)
    {
        free(comp_val_blocks[i]);
        free(decl_blocks[i]);
        free(rule_blocks[i]);
        free(stylesheet_blocks[i]);
    }
}