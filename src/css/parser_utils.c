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


uint32_t css_parser_parse_value(css_parser_node_t* comp_vals)
{
    css_parser_node_t* child = comp_vals;

    if (child->type == CSS_PARSER_NODE_TYPE_TOKEN)
    {
        return css_color_name_map_get(child->name);
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

        css_value_t* val = css_value_new(CSS_VALUE_TYPE_COLOR, CSS_VALUE_TYPE_UNIT_NONE);
        uint32_t c = css_parser_parse_value(node->comp_vals);
        val->color = c;

        decl->value = val;

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
    css_rule_t* rule = css_rule_new();
    rule->type = CSS_RULE_TYPE_STYLE;

    // convert selectors list

    // convert declarations
    css_parser_node_t* parser_decl = node->decls;
    css_decl_t* decl = css_parser_decl_to_om_decl(parser_decl);
    rule->decls = decl;

    // convert child rules

    return rule;
}
