/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "grammar.h"

#include <stdint.h>
#include <stdbool.h>

#include "css/types/value.h"
#include "css/tokenizer/types.h"
#include "css/parser/types.h"
#include "css/values.h"
#include "css/grammar/color_name_map.h"

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/


/********************/
/* static functions */
/********************/


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


static bool is_color_valid(css_parser_node_t* node)
{
    css_parser_node_type_e n_type   = node->type;

    if (n_type != CSS_PARSER_NODE_TYPE_TOKEN)
    {
        return false;
    }

    css_token_t* t                  = node->token;
    css_token_type_e t_type         = t->type;
    hash_str_t name                 = node->name;

    if (t_type != CSS_TOKEN_IDENT && t_type != CSS_TOKEN_HASH)
    {
        return false;
    }

    // unknown name
    if (t_type == CSS_TOKEN_IDENT)
    {
        if (name != css_value_initial() && name != css_value_inherit() && name != css_value_unset() && css_color_name_map_get(name) == 0)
        {
            return false;
        }
    }

    if (t_type == CSS_TOKEN_HASH)
    {
        unsigned char* data = t->data;
        uint32_t data_size = t->data_size;
        for (uint32_t i = 0; i < data_size; i++)
        {
            unsigned char c = data[i];
            if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
            {
                continue;
            }
            return false;
        }
    }

    return true;
}

static bool is_percentage_valid(css_parser_node_t* node)
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

    if (t_type != CSS_TOKEN_PERCENTAGE)
    {
        return false;
    }

    return true;
}

static bool is_length_valid(css_parser_node_t* node)
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

css_value_t* css_value_parse_color(css_parser_node_t* node)
{
    css_value_t* value = css_value_new(CSS_VALUE_TYPE_INITIAL, CSS_VALUE_TYPE_UNIT_NONE);

    if (!is_color_valid(node)) { return value; }

    css_token_t* t                  = node->token;
    css_token_type_e t_type         = t->type;
    hash_str_t name                 = node->name;
    unsigned char* data             = t->data;
    uint32_t data_size              = t->data_size;
    css_parser_node_type_e n_type   = node->type;
    uint32_t color                  = 0;

    if (n_type == CSS_PARSER_NODE_TYPE_TOKEN && t_type == CSS_TOKEN_IDENT)
    {
        if (name == css_value_initial())
        {
            value->type = CSS_VALUE_TYPE_INITIAL;
        }
        else if (name == css_value_inherit())
        {
            value->type = CSS_VALUE_TYPE_INHERIT;
        }
        else if (name == css_value_unset())
        {
            value->type = CSS_VALUE_TYPE_UNSET;
        }
        else
        {
            value->type = CSS_VALUE_TYPE_COLOR;
            value->color = css_color_name_map_get(name);;
        }
    }
    else if (n_type == CSS_PARSER_NODE_TYPE_TOKEN && t_type == CSS_TOKEN_HASH)
    {
        if (data_size == 8)
        {
            color = color_value_add_comp(color, color_value_convert_ascii(data[6], data[7]), 0);
            color = color_value_add_comp(color, color_value_convert_ascii(data[4], data[5]), 8);
            color = color_value_add_comp(color, color_value_convert_ascii(data[2], data[3]), 16);
            color = color_value_add_comp(color, color_value_convert_ascii(data[0], data[1]), 24);
        }
        else if (data_size == 6)
        {
            color = 0xff;
            color = color_value_add_comp(color, color_value_convert_ascii(data[4], data[5]), 8);
            color = color_value_add_comp(color, color_value_convert_ascii(data[2], data[3]), 16);
            color = color_value_add_comp(color, color_value_convert_ascii(data[0], data[1]), 24);
        }
        else if (data_size == 3)
        {
            color = 0xff;
            color = color_value_add_comp(color, color_value_convert_ascii(data[2], data[2]), 8);
            color = color_value_add_comp(color, color_value_convert_ascii(data[1], data[1]), 16);
            color = color_value_add_comp(color, color_value_convert_ascii(data[0], data[0]), 24);
        }
        else if (data_size == 4)
        {
            color = color_value_add_comp(color, color_value_convert_ascii(data[3], data[3]), 0);
            color = color_value_add_comp(color, color_value_convert_ascii(data[2], data[2]), 8);
            color = color_value_add_comp(color, color_value_convert_ascii(data[1], data[1]), 16);
            color = color_value_add_comp(color, color_value_convert_ascii(data[0], data[0]), 24);
        }

        value->type = CSS_VALUE_TYPE_COLOR;
        value->color = color;
    }

    return value;
}


css_value_t* css_value_parse_percentage(css_parser_node_t* node)
{
    css_value_t* value = css_value_new(CSS_VALUE_TYPE_INITIAL, CSS_VALUE_TYPE_UNIT_NONE);

    if (!is_percentage_valid(node)) { return value; }

    hash_str_t n_name       = node->name;
    css_token_t* t          = node->token;
    css_token_type_e t_type = t->type;

    if (t_type == CSS_TOKEN_IDENT)
    {
        if (n_name == css_value_initial())
        {
            value->type = CSS_VALUE_TYPE_INITIAL;
        }
        else if (n_name == css_value_inherit())
        {
            value->type = CSS_VALUE_TYPE_INHERIT;
        }
        else if (n_name == css_value_unset())
        {
            value->type = CSS_VALUE_TYPE_UNSET;
        }
    }
    else if (t_type == CSS_TOKEN_PERCENTAGE)
    {
        value->type = CSS_VALUE_TYPE_PERCENTAGE;
        value->real = t->real;
    }

    return value;
}


css_value_t* css_value_parse_length(css_parser_node_t* node)
{
    css_value_t* value = css_value_new(CSS_VALUE_TYPE_INITIAL, CSS_VALUE_TYPE_UNIT_NONE);

    if (!is_length_valid(node)) { return value; }

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
