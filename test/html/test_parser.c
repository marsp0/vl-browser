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

static void test_when_input_is_mix_between_text_and_elements_then_add_missing_nodes()
{
    // #data
    // <p>One<p>Two
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <p>
    // |       "One"
    // |     <p>
    // |       "Two"

    unsigned char buffer[] = "<p>One<p>Two";
    html_node_t* document = html_document_new();
    html_node_t* html = html_element_new(document, "html", 4);
    html_node_t* head = html_element_new(document, "head", 4);
    html_node_t* body = html_element_new(document, "body", 4);
    html_node_t* p_one = html_element_new(document, "p", 1);
    html_node_t* p_two = html_element_new(document, "p", 1);
    html_node_t* one = html_text_new(document, "One", 3);
    html_node_t* two = html_text_new(document, "Two", 3);
    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, p_one);
    APPEND_TO_TREE(body, p_two);
    APPEND_TO_TREE(p_one, one);
    APPEND_TO_TREE(p_two, two);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}

void test_html_parser_test1()
{
    TEST_CASE(test_when_input_is_pure_text_then_add_missing_nodes);
    TEST_CASE(test_when_input_is_mix_between_text_and_elements_then_add_missing_nodes);
}