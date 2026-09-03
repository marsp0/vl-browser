/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "parser_utils.h"

#include <assert.h>
#include <stddef.h>

#include "css/tokenizer_types.h"
#include "css/style_sheet.h"
#include "css/rule.h"
#include "css/decl.h"
#include "css/parser_types.h"
#include "css/prop_constants.h"
#include "css/color_name_map.h"
#include "util/not_implemented.h"

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/


/********************/
/* static functions */
/********************/

static uint32_t char_to_decimal(unsigned char c)
{
    if (c < 'A') { return c - '0'; }
    if (c < 'a') { return 10 + (c - 'A'); }

    return 10 + (c - 'a');
}


static uint32_t color_value_convert_ascii(unsigned char c1, unsigned char c2)
{
    uint32_t d1 = char_to_decimal(c1);
    uint32_t d2 = char_to_decimal(c2);

    return (d1 << 4) + d2;
}


static uint32_t color_value_set_red(uint32_t color, uint32_t red)
{
    return color + ((uint32_t)red << 24);
}


static uint32_t color_value_set_green(uint32_t color, uint32_t green)
{
    return color + ((uint32_t)green << 16);
}


static uint32_t color_value_set_blue(uint32_t color, uint32_t blue)
{
    return color + ((uint32_t)blue << 8);
}


static uint32_t color_value_set_alpha(uint32_t color, uint32_t alpha)
{
    return color + alpha;
}


static bool is_color_valid(css_parser_node_t* node)
{
    css_token_t* t = node->token;

    if (node->type != CSS_PARSER_NODE_TYPE_TOKEN)                   { return false; }
    if (t->type != CSS_TOKEN_IDENT && t->type != CSS_TOKEN_HASH)    { return false; }

    // if (t->

    return true;    
}


static uint32_t parse_color_value(css_parser_node_t* comp_vals)
{
    css_parser_node_t* child    = comp_vals;
    css_token_t* t              = child->token;

    if (child->type == CSS_PARSER_NODE_TYPE_TOKEN && t->type == CSS_TOKEN_IDENT)
    {
        return css_color_name_map_get(child->name);
    }
    else if (child->type == CSS_PARSER_NODE_TYPE_TOKEN && t->type == CSS_TOKEN_HASH)
    {
        uint32_t color = 0;
        if (t->data_size == 8)
        {
            unsigned char* data = t->data;
            color = color_value_set_alpha(color, color_value_convert_ascii(data[6], data[7]));
            color = color_value_set_blue(color, color_value_convert_ascii(data[4], data[5]));
            color = color_value_set_green(color, color_value_convert_ascii(data[2], data[3]));
            color = color_value_set_red(color, color_value_convert_ascii(data[0], data[1]));
        }
        else if (t->data_size == 6)
        {
            unsigned char* data = t->data;
            color = 0xff;
            color = color_value_set_blue(color, color_value_convert_ascii(data[4], data[5]));
            color = color_value_set_green(color, color_value_convert_ascii(data[2], data[3]));
            color = color_value_set_red(color, color_value_convert_ascii(data[0], data[1]));
        }
        return color;
    }
    else if (child->type == CSS_PARSER_NODE_TYPE_FUNCTION)
    {
        NOT_IMPLEMENTED
    }

    return 0;
}


/********************/
/* public functions */
/********************/


css_decl_t* css_parser_decl_to_om_decl(css_parser_node_t* node)
{
    assert(node->type == CSS_PARSER_NODE_TYPE_DECL);

    hash_str_t name = node->name;
    if (name == css_prop_color())
    {
        css_decl_t* decl = css_decl_new();
        decl->prop = CSS_PROP_COLOR;

        if (is_color_valid(node->comp_vals))
        {
            css_value_t* val    = css_value_new(CSS_VALUE_TYPE_COLOR, CSS_VALUE_TYPE_UNIT_NONE);
            val->color          = parse_color_value(node->comp_vals);
            decl->value         = val;
        }
        else
        {
            css_value_t* val    = css_value_new(CSS_VALUE_TYPE_INHERIT, CSS_VALUE_TYPE_UNIT_NONE);
            decl->value         = val;
        }

        return decl;
    }
    else
    {
        NOT_IMPLEMENTED
    }

    return NULL;
}


css_rule_t* css_parser_rule_to_om_rule(css_parser_node_t* node)
{
    assert(node->type == CSS_PARSER_NODE_TYPE_Q_RULE || node->type == CSS_PARSER_NODE_TYPE_AT_RULE);
    css_rule_t* rule = css_rule_new(CSS_RULE_TYPE_STYLE);

    // convert selectors list

    // convert declarations
    css_parser_node_t* child = node->decls;
    while (child)
    {
        css_decl_t* decl = css_parser_decl_to_om_decl(child);
        css_rule_add_decl(rule, decl);
        child = child->next;
    }

    // convert child rules

    return rule;
}


css_style_sheet_t* css_parser_sheet_to_om_sheet(css_parser_node_t* node)
{
    assert(node->type == CSS_PARSER_NODE_TYPE_STYLESHEET);

    css_style_sheet_t* sheet = css_style_sheet_new();

    css_parser_node_t* child = node->rules;
    while (child)
    {
        css_rule_t* rule = css_parser_rule_to_om_rule(child);
        css_style_sheet_add_rule(sheet, rule);
        child = child->next;
    }

    return sheet;
}