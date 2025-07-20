#include "test_parser.h"


#include "test_parser_utils.h"

static void test_when_input_is_pure_text_then_add_missing_nodes()
{
    // #data
    // Test
    // #errors
    // (1,0): expected-doctype-but-got-chars
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     "Test"
    
    unsigned char buffer[] = "Test";
    html_node_t* document = html_document_new();
    html_node_t* html = html_element_new(document, "html", 4);
    html_node_t* head = html_element_new(document, "head", 4);
    html_node_t* body = html_element_new(document, "body", 4);
    html_node_t* text = html_text_new(document, "Test", 4);
    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, text);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}

void test_html_parser_test1()
{
    TEST_CASE(test_when_input_is_pure_text_then_add_missing_nodes);
}