#pragma once

#include <stdint.h>

#include "css/tokenizer.h"

typedef enum
{
    CSS_PARSER_COMP_VAL_TOKEN,
    CSS_PARSER_COMP_VAL_FUNC,
    CSS_PARSER_COMP_VAL_BLOCK

} css_parser_comp_val_type_e;


typedef struct css_parser_comp_val_t
{
    css_parser_comp_val_type_e      type;
    css_token_t                     token;

    unsigned char                   name[CSS_TOKEN_MAX_DATA_SIZE];
    uint32_t                        name_size;

    struct css_parser_comp_val_t*   comp_vals;
    uint32_t                        comp_vals_size;

    struct css_parser_comp_val_t*   next;

} css_parser_comp_val_t;


typedef struct css_parser_decl_t
{
    unsigned char           name[CSS_TOKEN_MAX_DATA_SIZE];
    uint32_t                name_size;

    css_parser_comp_val_t*  comp_vals;
    uint32_t                comp_vals_size;

    unsigned char           original_text[CSS_TOKEN_MAX_DATA_SIZE];
    uint32_t                original_text_size;

    struct css_parser_decl_t* next;
    
} css_parser_decl_t;


typedef struct css_parser_rule_t
{
    unsigned char               name[CSS_TOKEN_MAX_DATA_SIZE];
    uint32_t                    name_size;

    css_parser_decl_t*          decls;
    uint32_t                    decls_size;

    css_parser_comp_val_t*      comp_vals;
    uint32_t                    comp_vals_size;

    struct css_parser_rule_t*   rules;
    uint32_t                    rules_size;

    struct css_parser_rule_t*   next;

} css_parser_rule_t;

typedef struct
{
    css_parser_rule_t*  rules;
    uint32_t            rules_size;

} css_parser_stylesheet_t;

css_parser_comp_val_t*      css_parser_comp_val_new();
void                        css_parser_comp_val_add_comp_val(css_parser_comp_val_t* c_val, css_parser_comp_val_t* child_val);

css_parser_decl_t*          css_parser_decl_new();
void                        css_parser_decl_add_comp_val(css_parser_decl_t* decl, css_parser_comp_val_t* c_val);

css_parser_rule_t*          css_parser_rule_new();
void                        css_parser_rule_add_rule(css_parser_rule_t* rule, css_parser_rule_t* n_rule);
void                        css_parser_rule_add_comp_val(css_parser_rule_t* rule, css_parser_comp_val_t* c_val);
void                        css_parser_rule_add_decl(css_parser_rule_t* rule, css_parser_decl_t* decl);

void                        css_parser_types_init();
void                        css_parser_types_reset();
void                        css_parser_types_free();