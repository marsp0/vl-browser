/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "color.h"

#include <stdbool.h>

#include "css/value.h"
#include "css/parser_types.h"
#include "css/grammar/color_name_map.h"
#include "css/tokenizer_types.h"

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/


/********************/
/* static functions */
/********************/

static bool is_color_valid(css_parser_node_t* node)
{
    css_parser_node_type_e n_type   = node->type;

    if (n_type != CSS_PARSER_NODE_TYPE_TOKEN)                   { return false; }

    css_token_t* t                  = node->token;
    css_token_type_e t_type         = t->type;

    if (t_type != CSS_TOKEN_IDENT && t_type != CSS_TOKEN_HASH)  { return false; }

    return true;
}


static uint32_t char_to_dec(unsigned char c)
{
    if (c < 'A') { return c - '0'; }
    if (c < 'a') { return 10 + (c - 'A'); }

    return 10 + (c - 'a');
}


static uint32_t color_value_convert_ascii(unsigned char c1, unsigned char c2)
{
    uint32_t d1 = char_to_dec(c1);
    uint32_t d2 = char_to_dec(c2);

    return (d1 << 4) + d2;
}


static uint32_t color_value_add_comp(uint32_t color, uint32_t comp, uint32_t bits)
{
    return color + (comp << bits);
}

/********************/
/* public functions */
/********************/

css_value_t* css_value_parse_color(css_parser_node_t* node)
{
    css_value_t* value = css_value_new(CSS_VALUE_TYPE_INHERIT, CSS_VALUE_TYPE_UNIT_NONE);

    if (!is_color_valid(node)) { return value; }

    css_token_t* t                  = node->token;
    css_token_type_e t_type         = t->type;
    css_parser_node_type_e n_type   = node->type;
    uint32_t color                  = 0;

    if (n_type == CSS_PARSER_NODE_TYPE_TOKEN && t_type == CSS_TOKEN_IDENT)
    {
        color = css_color_name_map_get(node->name);
    }
    else if (n_type == CSS_PARSER_NODE_TYPE_TOKEN && t_type == CSS_TOKEN_HASH)
    {
        if (t->data_size == 8)
        {
            unsigned char* data = t->data;
            color = color_value_add_comp(color, color_value_convert_ascii(data[6], data[7]), 0);
            color = color_value_add_comp(color, color_value_convert_ascii(data[4], data[5]), 8);
            color = color_value_add_comp(color, color_value_convert_ascii(data[2], data[3]), 16);
            color = color_value_add_comp(color, color_value_convert_ascii(data[0], data[1]), 24);
        }
        else if (t->data_size == 6)
        {
            unsigned char* data = t->data;
            color = 0xff;
            color = color_value_add_comp(color, color_value_convert_ascii(data[4], data[5]), 8);
            color = color_value_add_comp(color, color_value_convert_ascii(data[2], data[3]), 16);
            color = color_value_add_comp(color, color_value_convert_ascii(data[0], data[1]), 24);
        }
    }

    value->type = CSS_VALUE_TYPE_COLOR;
    value->color = color;
    return value;
}