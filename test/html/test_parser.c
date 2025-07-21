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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* text       = html_text_new(document, "Test", 4);

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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* p_one      = html_element_new(document, "p", 1);
    html_node_t* p_two      = html_element_new(document, "p", 1);
    html_node_t* one        = html_text_new(document, "One", 3);
    html_node_t* two        = html_text_new(document, "Two", 3);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, p_one);
    APPEND_TO_TREE(body, p_two);
    APPEND_TO_TREE(p_one, one);
    APPEND_TO_TREE(p_two, two);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


static void test_when_input_is_mix_between_text_and_elements_then_add_missing_nodes_2()
{
    // #data
    // Line1<br>Line2<br>Line3<br>Line4
    // #errors
    // (1,0): expected-doctype-but-got-chars
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     "Line1"
    // |     <br>
    // |     "Line2"
    // |     <br>
    // |     "Line3"
    // |     <br>
    // |     "Line4"

    unsigned char buffer[] = "Line1<br>Line2<br>Line3<br>Line4";
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* one        = html_text_new(document, "Line1", 5);
    html_node_t* one_br     = html_element_new(document, "br", 2);
    html_node_t* two        = html_text_new(document, "Line2", 5);
    html_node_t* two_br     = html_element_new(document, "br", 2);
    html_node_t* three      = html_text_new(document, "Line3", 5);
    html_node_t* three_br   = html_element_new(document, "br", 2);
    html_node_t* four       = html_text_new(document, "Line4", 5);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, one);
    APPEND_TO_TREE(body, one_br);
    APPEND_TO_TREE(body, two);
    APPEND_TO_TREE(body, two_br);
    APPEND_TO_TREE(body, three);
    APPEND_TO_TREE(body, three_br);
    APPEND_TO_TREE(body, four);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


static void test_when_only_html_is_provided_then_add_head_and_body_tags()
{
    // #data
    // <html>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    
    unsigned char buffer[] = "<html>";
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


static void test_when_only_head_tag_is_provided_then_add_html_and_body_tags()
{
    // #data
    // <head>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "<head>";
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


static void test_when_only_body_tag_is_provided_then_add_html_and_head_tags()
{
    // #data
    // <body>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "<body>";
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


static void test_when_only_html_and_head_are_given_add_body()
{
    // #data
    // <html><head>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "<html><head>";
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


static void test_when_html_and_complete_head_are_given_add_body()
{
    // #data
    // <html><head></head>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "<html><head></head>";
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


static void test_when_html_head_and_body_present_but_only_head_is_closed_then_still_create_valid_tree()
{
    // #data
    // <html><head></head><body>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "<html><head></head><body>";
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


static void test_when_html_head_body_provided_but_only_head_and_body_are_closed_we_still_create_valid_tree()
{
    // #data
    // <html><head></head><body></body>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "<html><head></head><body></body>";
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


static void test_when_html_and_body_are_closed_then_close_head_too()
{
    // #data
    // <html><head><body></body></html>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "<html><head><body></body></html>";
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}

void test_html_parser_test1()
{
    TEST_CASE(test_when_input_is_pure_text_then_add_missing_nodes);
    TEST_CASE(test_when_input_is_mix_between_text_and_elements_then_add_missing_nodes);
    TEST_CASE(test_when_input_is_mix_between_text_and_elements_then_add_missing_nodes_2);
    TEST_CASE(test_when_only_html_is_provided_then_add_head_and_body_tags);
    TEST_CASE(test_when_only_head_tag_is_provided_then_add_html_and_body_tags);
    TEST_CASE(test_when_only_body_tag_is_provided_then_add_html_and_head_tags);
    TEST_CASE(test_when_only_html_and_head_are_given_add_body);
    TEST_CASE(test_when_html_and_complete_head_are_given_add_body);
    TEST_CASE(test_when_html_head_and_body_present_but_only_head_is_closed_then_still_create_valid_tree);
    TEST_CASE(test_when_html_head_body_provided_but_only_head_and_body_are_closed_we_still_create_valid_tree);
    TEST_CASE(test_when_html_and_body_are_closed_then_close_head_too);
}