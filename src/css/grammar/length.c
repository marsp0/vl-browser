/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "length.h"

#include <stdbool.h>

#include "css/values.h"
#include "css/types/value.h"
#include "css/parser/types.h"
#include "css/tokenizer/types.h"

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/


/********************/
/* static functions */
/********************/

static bool is_valid(css_parser_node_t* node)
{
    css_parser_node_type_e n_type = node->type;

    if (n_type != CSS_PARSER_NODE_TYPE_TOKEN)
    {
        return false;
    }

    css_token_t* t          = node->token;
    hash_str_t n_name       = node->name;
    css_token_type_e t_type = t->type;
    bool is_initial         = n_name == css_value_initial();
    bool is_inherit         = n_name == css_value_inherit();
    bool is_unset           = n_name == css_value_unset();

    if (t_type == CSS_TOKEN_IDENT && (is_initial || is_inherit || is_unset))
    {
        return true;
    }

    if (t_type != CSS_TOKEN_DIMENSION)
    {
        return false;
    }

    return true;
}


/********************/
/* public functions */
/********************/

css_value_t* css_value_parse_length(css_parser_node_t* node)
{
    css_value_t* value = css_value_new(CSS_VALUE_TYPE_INITIAL, CSS_VALUE_TYPE_UNIT_NONE);

    if (!is_valid(node)) { return value; }

    hash_str_t n_name       = node->name;
    css_token_t* t          = node->token;
    css_token_type_e t_type = t->type;

    if (t_type == CSS_TOKEN_IDENT)
    {
        if (n_name == css_value_initial())          { value->type = CSS_VALUE_TYPE_INITIAL; }
        else if (n_name == css_value_inherit())     { value->type = CSS_VALUE_TYPE_INHERIT; }
        else if (n_name == css_value_unset())       { value->type = CSS_VALUE_TYPE_UNSET; }
    }

    if (t_type == CSS_TOKEN_DIMENSION)
    {
        // treat everything as px
        value->type = CSS_VALUE_TYPE_LENGTH;
        value->unit = CSS_VALUE_TYPE_UNIT_PX;
        value->real = t->real;
    }

    return value;
}