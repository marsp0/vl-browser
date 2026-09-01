#include "test_conversions.h"

#include <stdint.h>
#include <assert.h>

#include "test_utils.h"

#include "css/parser.h"
#include "css/parser_types.h"
#include "css/parser_utils.h"


static void test_css_parser_rule_to_om_rule_1()
{
    unsigned char data[] = "p { color: red; }";
    uint32_t data_size = sizeof(data) - 1;

    // 1. parse
    css_parser_init(data, data_size);
    css_parser_node_t* sheet = css_parser_parse_stylesheet();
    css_parser_free();

    ASSERT_EQUAL(sheet->type, CSS_PARSER_NODE_TYPE_STYLESHEET);

    // css_parser_node_t* node = sheet->rules;
    // css_rule_t* first       = css_parser_rule_to_om_rule(node);
    // node = node->next;
    // while (node)
    // {
    //     css_rule_t* rule = css_parser_rule_to_om_rule(node);
    //     css_rule_add_sibling(first, rule);
    //     node = node->next;
    // }

    // create expected tree
    // assert
}

void test_css_conversions()
{
    TEST_CASE(test_css_parser_rule_to_om_rule_1);
}