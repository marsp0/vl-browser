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
#include "css/style_sheet.h"
#include "css/parser_types.h"
#include "css/grammar/color.h"
#include "css/prop_constants.h"
#include "css/tokenizer_types.h"
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


/********************/
/* public functions */
/********************/


css_decl_t* css_parser_decl_to_om_decl(css_parser_node_t* node)
{
    assert(node->type == CSS_PARSER_NODE_TYPE_DECL);

    hash_str_t name = node->name;
    if (name == css_prop_color())
    {
        css_decl_t* decl    = css_decl_new();
        decl->prop          = CSS_PROP_COLOR;
        decl->value         = css_value_parse_color(node->comp_vals);
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