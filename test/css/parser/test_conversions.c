#include "test_conversions.h"

#include <stdint.h>
#include <assert.h>

#include "test_utils.h"

#include "css/style_sheet.h"
#include "css/rule.h"
#include "css/decl.h"
#include "css/value.h"
#include "css/parser.h"
#include "css/parser_types.h"
#include "css/parser_utils.h"


void ASSERT_CSS_SHEET(css_style_sheet_t* a, css_style_sheet_t* e)
{
    if (!a || !e)
    {
        if (!a && !e) { return; }

        ASSERT_POINTER(a, e);
        return;
    }

    css_rule_t* a_rule = a->rules;
    css_rule_t* e_rule = e->rules;

    while (a_rule || e_rule)
    {
        ASSERT_CSS_RULE(a_rule, e_rule);

        if (!a_rule || !e_rule) { break; }

        a_rule = a_rule->next;
        e_rule = e_rule->next;
    }
}


void ASSERT_CSS_RULE(css_rule_t* a, css_rule_t* e)
{
    if (!a || !e)
    {
        if (!a && !e) { return; }

        ASSERT_POINTER(a, e);
        return;
    }

    ASSERT_EQUAL(a->type, e->type);

    css_decl_t* a_decl = a->decls;
    css_decl_t* e_decl = e->decls;

    while (a_decl || e_decl)
    {
        ASSERT_CSS_DECL(a_decl, e_decl);

        if (!a_decl || !e_decl) { break; }

        a_decl = a_decl->next;
        e_decl = e_decl->next;
    }
}


void ASSERT_CSS_DECL(css_decl_t* a, css_decl_t* e)
{
    if (!a || !e)
    {
        if (!a && !e) { return; }

        ASSERT_POINTER(a, e);
        return;
    }

    ASSERT_EQUAL(a->prop, e->prop);
    ASSERT_MEMORY(a->value, e->value, sizeof(css_value_t));
}


static void test_css_parser_sheet_to_om_sheet_1()
{
    unsigned char data[] = "p { color: red; color: #FF0000 }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);

    css_style_sheet_t* actual   = css_parser_sheet_to_om_sheet(sheet);
    css_style_sheet_t* expected = css_style_sheet_new();
    css_rule_t* rule            = css_rule_new(CSS_RULE_TYPE_STYLE);

    css_decl_t* decl1           = css_decl_new();
    css_value_t* value          = css_value_new(CSS_VALUE_TYPE_COLOR, CSS_VALUE_TYPE_UNIT_NONE);
    value->color                = 0xFF0000FF;
    decl1->prop                 = CSS_PROP_COLOR;
    decl1->value                = value;

    css_decl_t* decl2           = css_decl_new();
    css_value_t* value2         = css_value_new(CSS_VALUE_TYPE_COLOR, CSS_VALUE_TYPE_UNIT_NONE);
    value2->color               = 0xFF0000FF;
    decl2->prop                 = CSS_PROP_COLOR;
    decl2->value                = value2;

    css_style_sheet_add_rule(expected, rule);
    css_rule_add_decl(rule, decl1);
    css_rule_add_decl(rule, decl2);

    ASSERT_CSS_SHEET(actual, expected);
}

void test_css_conversions()
{
    TEST_CASE(test_css_parser_sheet_to_om_sheet_1);
}