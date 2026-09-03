#include "test_color.h"

#include "test_utils.h"
#include "css/values/utils.h"

#include "css/value.h"
#include "css/parser.h"
#include "css/parser_types.h"
#include "css/grammar/color.h"

static void test_color_name_valid()
{
    unsigned char data[] = "p { color: red; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_color(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_COLOR, CSS_VALUE_TYPE_UNIT_NONE);
    expected->color         = 0xFF0000FF;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_color_hex8_valid()
{
    unsigned char data[] = "p { color: #FF000000; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_color(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_COLOR, CSS_VALUE_TYPE_UNIT_NONE);
    expected->color         = 0xFF000000;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_color_hex6_valid()
{
    unsigned char data[] = "p { color: #FF0000; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_color(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_COLOR, CSS_VALUE_TYPE_UNIT_NONE);
    expected->color         = 0xFF0000FF;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_color_hex3_valid()
{
    unsigned char data[] = "p { color: #FFF; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_color(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_COLOR, CSS_VALUE_TYPE_UNIT_NONE);
    expected->color         = 0xFFFFFFFF;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_color_hex4_valid()
{
    unsigned char data[] = "p { color: #FABC; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_color(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_COLOR, CSS_VALUE_TYPE_UNIT_NONE);
    expected->color         = 0xFFAABBCC;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_color_name_invalid()
{
    unsigned char data[] = "p { color: dsa; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_color(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_INHERIT, CSS_VALUE_TYPE_UNIT_NONE);
    expected->color         = 0;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_color_hex8_invalid()
{
    unsigned char data[] = "p { color: #MARTINGG; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_color(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_INHERIT, CSS_VALUE_TYPE_UNIT_NONE);
    expected->color         = 0;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_color_hex6_invalid()
{
    unsigned char data[] = "p { color: #MARTIN; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_color(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_INHERIT, CSS_VALUE_TYPE_UNIT_NONE);
    expected->color         = 0;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_color_hex3_invalid()
{
    unsigned char data[] = "p { color: #Mrt; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_color(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_INHERIT, CSS_VALUE_TYPE_UNIT_NONE);
    expected->color         = 0;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_color_inherit()
{
    unsigned char data[] = "p { color: inherit; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_color(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_INHERIT, CSS_VALUE_TYPE_UNIT_NONE);
    expected->color         = 0;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_color_initial()
{
    unsigned char data[] = "p { color: initial; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_color(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_INITIAL, CSS_VALUE_TYPE_UNIT_NONE);
    expected->color         = 0;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_color_unset()
{
    unsigned char data[] = "p { color: unset; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_color(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_UNSET, CSS_VALUE_TYPE_UNIT_NONE);
    expected->color         = 0;

    ASSERT_CSS_VALUE(actual, expected);
}


void test_css_value_color()
{
    TEST_CASE(test_color_name_valid);
    TEST_CASE(test_color_hex8_valid);
    TEST_CASE(test_color_hex6_valid);
    TEST_CASE(test_color_hex3_valid);
    TEST_CASE(test_color_hex4_valid);
    TEST_CASE(test_color_name_invalid);
    TEST_CASE(test_color_hex8_invalid);
    TEST_CASE(test_color_hex6_invalid);
    TEST_CASE(test_color_hex3_invalid);
    TEST_CASE(test_color_inherit);
    TEST_CASE(test_color_initial);
    TEST_CASE(test_color_unset);
}