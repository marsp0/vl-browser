#include "test_length.h"

#include "test_utils.h"
#include "css/grammar/utils.h"

#include "css/parser/types.h"
#include "css/parser/parser.h"
#include "css/grammar/length.h"
#include "css/types/value.h"


static void test_length_when_input_valid()
{
    unsigned char data[] = "p { padding-left: 5px; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_length(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_LENGTH, CSS_VALUE_TYPE_UNIT_PX);
    expected->real          = 5.f;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_length_when_input_valid_but_not_px()
{
    unsigned char data[] = "p { padding-left: 5em; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_length(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_LENGTH, CSS_VALUE_TYPE_UNIT_PX);
    expected->real          = 5.f;

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_length_when_input_invalid()
{
    unsigned char data[] = "p { padding-left: random; }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);
    ASSERT_POINTER_EXISTS(sheet->rules);
    ASSERT_POINTER_EXISTS(sheet->rules->decls);

    css_parser_node_t* decl = sheet->rules->decls;
    css_value_t* actual     = css_value_parse_length(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_INITIAL, CSS_VALUE_TYPE_UNIT_NONE);

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_length_when_input_initial()
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
    css_value_t* actual     = css_value_parse_length(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_INITIAL, CSS_VALUE_TYPE_UNIT_NONE);

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_length_when_input_inherit()
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
    css_value_t* actual     = css_value_parse_length(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_INHERIT, CSS_VALUE_TYPE_UNIT_NONE);

    ASSERT_CSS_VALUE(actual, expected);
}


static void test_length_when_input_unset()
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
    css_value_t* actual     = css_value_parse_length(decl->comp_vals);

    css_value_t* expected   = css_value_new(CSS_VALUE_TYPE_UNSET, CSS_VALUE_TYPE_UNIT_NONE);

    ASSERT_CSS_VALUE(actual, expected);
}


void test_css_value_length()
{
    TEST_CASE(test_length_when_input_valid);
    TEST_CASE(test_length_when_input_valid_but_not_px);
    TEST_CASE(test_length_when_input_invalid);
    TEST_CASE(test_length_when_input_initial);
    TEST_CASE(test_length_when_input_inherit);
    TEST_CASE(test_length_when_input_unset);
}