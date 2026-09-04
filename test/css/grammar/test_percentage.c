#include "css/grammar/test_percentage.h"

#include "test_utils.h"

#include "css/grammar/utils.h"

#include "css/types/value.h"
#include "css/parser/types.h"
#include "css/parser/parser.h"
#include "css/grammar/percentage.h"


static void test_percentage_valid_1()
{
    unsigned char data[] = "p { padding-left: 5%; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_percentage(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_PERCENTAGE, CSS_VALUE_TYPE_UNIT_NONE);
    expected->real          = 5.f;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_percentage_valid_2()
{
    unsigned char data[] = "p { padding-left: 100%; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_percentage(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_PERCENTAGE, CSS_VALUE_TYPE_UNIT_NONE);
    expected->real          = 100.f;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_percentage_initial()
{
    unsigned char data[] = "p { padding-left: initial; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_percentage(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_INITIAL, CSS_VALUE_TYPE_UNIT_NONE);

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_percentage_inherit()
{
    unsigned char data[] = "p { padding-left: inherit; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_percentage(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_INHERIT, CSS_VALUE_TYPE_UNIT_NONE);

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_percentage_unset()
{
    unsigned char data[] = "p { padding-left: unset; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_percentage(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_UNSET, CSS_VALUE_TYPE_UNIT_NONE);

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_percentage_invalid_1()
{
    unsigned char data[] = "p { padding-left: dsadsaE; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_percentage(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_INITIAL, CSS_VALUE_TYPE_UNIT_NONE);

    ASSERT_CSS_VALUE(actual, expected);
}


void test_css_value_percentage()
{
    TEST_CASE(test_percentage_valid_1);
    TEST_CASE(test_percentage_valid_2);
    TEST_CASE(test_percentage_initial);
    TEST_CASE(test_percentage_inherit);
    TEST_CASE(test_percentage_unset);
    TEST_CASE(test_percentage_invalid_1);
}