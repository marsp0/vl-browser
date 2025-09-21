#include "test_parser.h"


#include "test_parser_utils.h"

#include "html/tag_constants.h"

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
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());
    dom_node_t* text       = dom_text_new(document, "Test", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, text);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
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
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());
    dom_node_t* p_one      = dom_element_new(document, html_tag_p());
    dom_node_t* p_two      = dom_element_new(document, html_tag_p());
    dom_node_t* one        = dom_text_new(document, "One", 3);
    dom_node_t* two        = dom_text_new(document, "Two", 3);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, p_one);
    APPEND_TO_TREE(body, p_two);
    APPEND_TO_TREE(p_one, one);
    APPEND_TO_TREE(p_two, two);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
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
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());
    dom_node_t* one        = dom_text_new(document, "Line1", 5);
    dom_node_t* one_br     = dom_element_new(document, html_tag_br());
    dom_node_t* two        = dom_text_new(document, "Line2", 5);
    dom_node_t* two_br     = dom_element_new(document, html_tag_br());
    dom_node_t* three      = dom_text_new(document, "Line3", 5);
    dom_node_t* three_br   = dom_element_new(document, html_tag_br());
    dom_node_t* four       = dom_text_new(document, "Line4", 5);

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

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
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
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
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
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
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
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
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
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
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
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
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
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
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
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
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
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_when_html_tag_is_closed_and_body_end_tag_is_provided_then_close_head_and_add_body_start_tag_automatically()
{
    // #data
    // <html><head></body></html>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "<html><head></body></html>";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_when_only_html_tag_is_closed_then_we_automatically_close_head_and_body_tags()
{
    // #data
    // <html><head><body></html>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "<html><head><body></html>";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_when_head_tag_is_missing_and_body_is_not_closed_then_we_add_head_tag_and_close_body()
{
    // #data
    // <html><body></html>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "<html><body></html>";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_when_html_open_tag_and_body_tags_are_missing_then_they_get_created()
{
    // #data
    // <head></html>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "<head></html>";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_when_only_closed_head_tag_is_provided_then_we_add_missing_html_and_body_tags()
{
    // #data
    // </head>
    // #errors
    // (1,7): expected-doctype-but-got-end-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "</head>";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_when_only_closed_body_tag_is_provided_then_we_add_missing_html_and_head_tags()
{
    // #data
    // </body>
    // #errors
    // (1,7): expected-doctype-but-got-end-tag element.
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "</body>";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_when_only_closed_html_tag_is_provided_then_we_add_missing_body_and_head_tags()
{
    // #data
    // </html>
    // #errors
    // (1,7): expected-doctype-but-got-end-tag element.
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[] = "</html>";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}

static void missing_tags_1()
{
    // #data
    // <b><table><td><i></table>
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,14): unexpected-cell-in-table-body
    // (1,25): unexpected-cell-end-tag
    // (1,25): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <b>
    // |       <table>
    // |         <tbody>
    // |           <tr>
    // |             <td>
    // |               <i>

    unsigned char buffer[] = "<b><table><td><i></table>";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());
    dom_node_t* b          = dom_element_new(document, html_tag_b());
    dom_node_t* table      = dom_element_new(document, html_tag_table());
    dom_node_t* tbody      = dom_element_new(document, html_tag_tbody());
    dom_node_t* tr         = dom_element_new(document, html_tag_tr());
    dom_node_t* td         = dom_element_new(document, html_tag_td());
    dom_node_t* i          = dom_element_new(document, html_tag_i());
    

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, b);
    APPEND_TO_TREE(b, table);
    APPEND_TO_TREE(table, tbody);
    APPEND_TO_TREE(tbody, tr);
    APPEND_TO_TREE(tr, td);
    APPEND_TO_TREE(td, i);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


// static void missing_html_tag_2()
// {
//     // #data
//     // <b><table><td></b><i></table>X
//     // #errors
//     // (1,3): expected-doctype-but-got-start-tag
//     // (1,14): unexpected-cell-in-table-body
//     // (1,18): unexpected-end-tag
//     // (1,29): unexpected-cell-end-tag
//     // (1,30): expected-closing-tag-but-got-eof
//     // #document
//     // | <html>
//     // |   <head>
//     // |   <body>
//     // |     <b>
//     // |       <table>
//     // |         <tbody>
//     // |           <tr>
//     // |             <td>
//     // |               <i>
//     // |       "X"

//     unsigned char buffer[] = "<b><table><td></b><i></table>X";
//     dom_node_t* document   = dom_document_new();
//     dom_node_t* html       = dom_element_new(document, html_tag_html);
//     dom_node_t* head       = dom_element_new(document, html_tag_head);
//     dom_node_t* body       = dom_element_new(document, html_tag_body);
//     dom_node_t* b          = dom_element_new(document, html_tag_b);
//     dom_node_t* table      = dom_element_new(document, html_tag_table);
//     dom_node_t* tbody      = dom_element_new(document, html_tag_tbody);
//     dom_node_t* tr         = dom_element_new(document, html_tag_tr);
//     dom_node_t* td         = dom_element_new(document, html_tag_td);
//     dom_node_t* i          = dom_element_new(document, html_tag_i);
//     dom_node_t* text       = dom_text_new(document, "X", 1);
    

//     APPEND_TO_TREE(document, html);
//     APPEND_TO_TREE(html, head);
//     APPEND_TO_TREE(html, body);
//     APPEND_TO_TREE(body, b);
//     APPEND_TO_TREE(b, table);
//     APPEND_TO_TREE(table, tbody);
//     APPEND_TO_TREE(tbody, tr);
//     APPEND_TO_TREE(tr, td);
//     APPEND_TO_TREE(td, i);
//     APPEND_TO_TREE(b, text);

//     RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
// }


static void test_parser_1()
{
    // #data
    // <h1>Hello<h2>World
    // #errors
    // (1,4): expected-doctype-but-got-start-tag
    // (1,13): unexpected-start-tag
    // (1,18): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <h1>
    // |       "Hello"
    // |     <h2>
    // |       "World"

    unsigned char buffer[] = "<h1>Hello<h2>World";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());
    dom_node_t* h1         = dom_element_new(document, html_tag_h1());
    dom_node_t* h2         = dom_element_new(document, html_tag_h2());
    dom_node_t* t1         = dom_text_new(document, "Hello", 5);
    dom_node_t* t2         = dom_text_new(document, "World", 5);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, h1);
    APPEND_TO_TREE(body, h2);
    APPEND_TO_TREE(h1, t1);
    APPEND_TO_TREE(h2, t2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


// static void test_parser_2()
// {
//     // #data
//     // <a><p>X<a>Y</a>Z</p></a>
//     // #errors
//     // (1,3): expected-doctype-but-got-start-tag
//     // (1,10): unexpected-start-tag-implies-end-tag
//     // (1,10): adoption-agency-1.3
//     // (1,24): unexpected-end-tag
//     // #document
//     // | <html>
//     // |   <head>
//     // |   <body>
//     // |     <a>
//     // |     <p>
//     // |       <a>
//     // |         "X"
//     // |       <a>
//     // |         "Y"
//     // |       "Z"
// }


static void test_parser_3()
{
    // #data
    // <!DOCTYPE html><span><button>foo</span>bar
    // #errors
    // (1,39): unexpected-end-tag
    // (1,42): expected-closing-tag-but-got-eof
    // #document
    // | <!DOCTYPE html>
    // | <html>
    // |   <head>
    // |   <body>
    // |     <span>
    // |       <button>
    // |         "foobar"

    unsigned char buffer[] = "<!DOCTYPE html><span><button>foo</span>bar";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());
    dom_node_t* span       = dom_element_new(document, html_tag_span());
    dom_node_t* button     = dom_element_new(document, html_tag_button());
    dom_node_t* t1         = dom_text_new(document, "foobar", 6);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, span);
    APPEND_TO_TREE(span, button);
    APPEND_TO_TREE(button, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_parser_4()
{
    // #data
    // <p><b><div><marquee></p></b></div>X
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,11): unexpected-end-tag
    // (1,24): unexpected-end-tag
    // (1,28): unexpected-end-tag
    // (1,34): end-tag-too-early
    // (1,35): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <p>
    // |       <b>
    // |     <div>
    // |       <b>
    // |         <marquee>
    // |           <p>
    // |           "X"

    unsigned char buffer[] = "<p><b><div><marquee></p></b></div>X";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());
    dom_node_t* p1         = dom_element_new(document, html_tag_p());
    dom_node_t* b1         = dom_element_new(document, html_tag_b());
    dom_node_t* div        = dom_element_new(document, html_tag_div());
    dom_node_t* b2         = dom_element_new(document, html_tag_b());
    dom_node_t* marquee    = dom_element_new(document, html_tag_marquee());
    dom_node_t* p2         = dom_element_new(document, html_tag_p());
    dom_node_t* t          = dom_text_new(document, "X", 1);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(p1, b1);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, b2);
    APPEND_TO_TREE(b2, marquee);
    APPEND_TO_TREE(marquee, p2);
    APPEND_TO_TREE(marquee, t);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_parser_5()
{
    // #data
    // <script><div></script></div><title><p></title><p><p>
    // #errors
    // (1,8): expected-doctype-but-got-start-tag
    // (1,28): unexpected-end-tag
    // #document
    // | <html>
    // |   <head>
    // |     <script>
    // |       "<div>"
    // |     <title>
    // |       "<p>"
    // |   <body>
    // |     <p>
    // |     <p>

    unsigned char buffer[] = "<script><div></script></div><title><p></title><p><p>";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());
    dom_node_t* script     = dom_element_new(document, html_tag_script());
    dom_node_t* t1         = dom_text_new(document, "<div>", 5);
    dom_node_t* title      = dom_element_new(document, html_tag_title());
    dom_node_t* t2         = dom_text_new(document, "<p>", 3);
    dom_node_t* p1         = dom_element_new(document, html_tag_p());
    dom_node_t* p2         = dom_element_new(document, html_tag_p());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(head, script);
    APPEND_TO_TREE(script, t1);
    APPEND_TO_TREE(head, title);
    APPEND_TO_TREE(title, t2);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(body, p2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_parser_6()
{
    // #data
    // <!--><div>--<!-->
    // #errors
    // (1,5): incorrect-comment
    // (1,10): expected-doctype-but-got-start-tag
    // (1,17): incorrect-comment
    // (1,17): expected-closing-tag-but-got-eof
    // #new-errors
    // (1:5) abrupt-closing-of-empty-comment
    // (1:17) abrupt-closing-of-empty-comment
    // #document
    // | <!--  -->
    // | <html>
    // |   <head>
    // |   <body>
    // |     <div>
    // |       "--"
    // |       <!--  -->

    unsigned char buffer[] = "<!--><div>--<!-->";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* c1         = dom_comment_new(document, "", 0);
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());
    dom_node_t* div        = dom_element_new(document, html_tag_div());
    dom_node_t* t          = dom_text_new(document, "--", 2);
    dom_node_t* c2         = dom_comment_new(document, "", 0);


    APPEND_TO_TREE(document, c1);
    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, t);
    APPEND_TO_TREE(div, c2);
    

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_parser_7()
{
    // #data
    // <p><hr></p>
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,11): unexpected-end-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <p>
    // |     <hr>
    // |     <p>

    unsigned char buffer[] = "<p><hr></p>";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());
    dom_node_t* p1         = dom_element_new(document, html_tag_p());
    dom_node_t* hr         = dom_element_new(document, html_tag_hr());
    dom_node_t* p2         = dom_element_new(document, html_tag_p());

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(body, hr);
    APPEND_TO_TREE(body, p2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_parser_8()
{
    // #data
    // <select><b><option><select><option></b></select>X
    // #errors
    // 1:1: ERROR: Expected a doctype token
    // 1:20: ERROR: Start tag 'select' isn't allowed here. Currently open html_tag_ html, body, select, b, option.
    // 1:36: ERROR: End tag 'b' isn't allowed here. Currently open html_tag_ html, body, b, select, option.
    // 1:50: ERROR: Premature end of file. Currently open html_tag_ html, body, b.
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <select>
    // |       <b>
    // |         <option>
    // |     <b>
    // |       <option>
    // |     "X"

    unsigned char buffer[] = "<select><b><option><select><option></b></select>X";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());
    dom_node_t* select     = dom_element_new(document, html_tag_select());
    dom_node_t* o1         = dom_element_new(document, html_tag_option());
    dom_node_t* o2         = dom_element_new(document, html_tag_option());
    dom_node_t* b1         = dom_element_new(document, html_tag_b());
    dom_node_t* b2         = dom_element_new(document, html_tag_b());
    dom_node_t* t          = dom_text_new(document, "X", 1);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, select);
    APPEND_TO_TREE(body, b2);
    APPEND_TO_TREE(body, t);
    APPEND_TO_TREE(select, b1);
    APPEND_TO_TREE(b1, o1);
    APPEND_TO_TREE(b2, o2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_parser_9()
{
    unsigned char buffer[] = "<p>1<b>2<i>3</b>4</i>5</p>";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());
    dom_node_t* p          = dom_element_new(document, html_tag_p());
    dom_node_t* b          = dom_element_new(document, html_tag_b());
    dom_node_t* i1         = dom_element_new(document, html_tag_i());
    dom_node_t* i2         = dom_element_new(document, html_tag_i());
    dom_node_t* t1         = dom_text_new(document, "1", 1);
    dom_node_t* t2         = dom_text_new(document, "2", 1);
    dom_node_t* t3         = dom_text_new(document, "3", 1);
    dom_node_t* t4         = dom_text_new(document, "4", 1);
    dom_node_t* t5         = dom_text_new(document, "5", 1);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, p);
    APPEND_TO_TREE(p, t1);
    APPEND_TO_TREE(p, b);
    APPEND_TO_TREE(b, t2);
    APPEND_TO_TREE(b, i1);
    APPEND_TO_TREE(i1, t3);
    APPEND_TO_TREE(p, i2);
    APPEND_TO_TREE(i2, t4);
    APPEND_TO_TREE(p, t5);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_parser_10()
{
    unsigned char buffer[] = "<b>1<p>2</b>3</p>";
    dom_node_t* document   = dom_document_new();
    dom_node_t* html       = dom_element_new(document, html_tag_html());
    dom_node_t* head       = dom_element_new(document, html_tag_head());
    dom_node_t* body       = dom_element_new(document, html_tag_body());
    dom_node_t* b1         = dom_element_new(document, html_tag_b());
    dom_node_t* b2         = dom_element_new(document, html_tag_b());
    dom_node_t* p1         = dom_element_new(document, html_tag_p());
    dom_node_t* t1         = dom_text_new(document, "1", 1);
    dom_node_t* t2         = dom_text_new(document, "2", 1);
    dom_node_t* t3         = dom_text_new(document, "3", 1);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, b1);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(b1, t1);
    APPEND_TO_TREE(p1, b2);
    APPEND_TO_TREE(b2, t2);
    APPEND_TO_TREE(p1, t3);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}

static void test_parser_11()
{
    // #data
    // <a><table><td><a><table></table><a></tr><a></table><b>X</b>C<a>Y
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,14): unexpected-cell-in-table-body
    // (1,35): unexpected-start-tag-implies-end-tag
    // (1,40): unexpected-cell-end-tag
    // (1,43): unexpected-start-tag-implies-table-voodoo
    // (1,43): unexpected-start-tag-implies-end-tag
    // (1,43): unexpected-end-tag
    // (1,63): unexpected-start-tag-implies-end-tag
    // (1,64): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <a>
    // |       <a>
    // |       <table>
    // |         <tbody>
    // |           <tr>
    // |             <td>
    // |               <a>
    // |                 <table>
    // |               <a>
    // |     <a>
    // |       <b>
    // |         "X"
    // |       "C"
    // |     <a>
    // |       "Y"

    unsigned char buffer[]  = "<a><table><td><a><table></table><a></tr><a></table><b>X</b>C<a>Y";
    dom_node_t* document    = dom_document_new();
    dom_node_t* html        = dom_element_new(document, html_tag_html());
    dom_node_t* head        = dom_element_new(document, html_tag_head());
    dom_node_t* body        = dom_element_new(document, html_tag_body());
    dom_node_t* a1          = dom_element_new(document, html_tag_a());
    dom_node_t* a2          = dom_element_new(document, html_tag_a());
    dom_node_t* a3          = dom_element_new(document, html_tag_a());
    dom_node_t* a4          = dom_element_new(document, html_tag_a());
    dom_node_t* a5          = dom_element_new(document, html_tag_a());
    dom_node_t* a6          = dom_element_new(document, html_tag_a());
    dom_node_t* table       = dom_element_new(document, html_tag_table());
    dom_node_t* tbody       = dom_element_new(document, html_tag_tbody());
    dom_node_t* tr          = dom_element_new(document, html_tag_tr());
    dom_node_t* td          = dom_element_new(document, html_tag_td());
    dom_node_t* table2      = dom_element_new(document, html_tag_table());
    dom_node_t* b1          = dom_element_new(document, html_tag_b());
    dom_node_t* x           = dom_text_new(document, "X", 1);
    dom_node_t* c           = dom_text_new(document, "C", 1);
    dom_node_t* y           = dom_text_new(document, "Y", 1);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, a1);
    APPEND_TO_TREE(body, a2);
    APPEND_TO_TREE(body, a3);
    APPEND_TO_TREE(a1, a4);
    APPEND_TO_TREE(a1, table);
    APPEND_TO_TREE(table, tbody);
    APPEND_TO_TREE(tbody, tr);
    APPEND_TO_TREE(tr, td);
    APPEND_TO_TREE(td, a5);
    APPEND_TO_TREE(td, a6);
    APPEND_TO_TREE(a5, table2);
    APPEND_TO_TREE(a2, b1);
    APPEND_TO_TREE(b1, x);
    APPEND_TO_TREE(a2, c);
    APPEND_TO_TREE(a3, y);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_parser_12()
{
    // https://html.spec.whatwg.org/multipage/parsing.html#unexpected-markup-in-tables

    unsigned char buffer[]  = "<table><b><tr><td>aaa</td></tr>bbb</table>ccc";
    dom_node_t* document    = dom_document_new();
    dom_node_t* html        = dom_element_new(document, html_tag_html());
    dom_node_t* head        = dom_element_new(document, html_tag_head());
    dom_node_t* body        = dom_element_new(document, html_tag_body());
    dom_node_t* b1          = dom_element_new(document, html_tag_b());
    dom_node_t* b2          = dom_element_new(document, html_tag_b());
    dom_node_t* b3          = dom_element_new(document, html_tag_b());
    dom_node_t* t           = dom_element_new(document, html_tag_table());
    dom_node_t* tb          = dom_element_new(document, html_tag_tbody());
    dom_node_t* tr          = dom_element_new(document, html_tag_tr());
    dom_node_t* td          = dom_element_new(document, html_tag_td());
    dom_node_t* t1          = dom_text_new(document, "bbb", 3);
    dom_node_t* t2          = dom_text_new(document, "aaa", 3);
    dom_node_t* t3          = dom_text_new(document, "ccc", 3);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, b1);
    APPEND_TO_TREE(body, b2);
    APPEND_TO_TREE(b2, t1);
    APPEND_TO_TREE(body, t);
    APPEND_TO_TREE(t, tb);
    APPEND_TO_TREE(tb, tr);
    APPEND_TO_TREE(tr, td);
    APPEND_TO_TREE(td, t2);
    APPEND_TO_TREE(body, b3);
    APPEND_TO_TREE(b3, t3);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document, false);
}


static void test_parser_13()
{
    // #data
    // <a X>0<b>1<a Y>2
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,15): unexpected-start-tag-implies-end-tag
    // (1,15): adoption-agency-1.3
    // (1,16): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <a>
    // |       x=""
    // |       "0"
    // |       <b>
    // |         "1"
    // |     <b>
    // |       <a>
    // |         y=""
    // |         "2"

    unsigned char buffer[] = "<a X>0<b>1<a Y>2";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* a1          = dom_element_new(expected, html_tag_a());
    dom_node_t* a1_attr     = dom_attr_new(hash_str_new("x", 1), 0, a1);
    dom_node_t* a2          = dom_element_new(expected, html_tag_a());
    dom_node_t* a2_attr     = dom_attr_new(hash_str_new("y", 1), 0, a2);
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* b2          = dom_element_new(expected, html_tag_b());
    dom_node_t* t1          = dom_text_new(expected, "0", 1);
    dom_node_t* t2          = dom_text_new(expected, "1", 1);
    dom_node_t* t3          = dom_text_new(expected, "2", 1);

    dom_element_append_attr(dom_element_from_node(a1), dom_attr_from_node(a1_attr));
    dom_element_append_attr(dom_element_from_node(a2), dom_attr_from_node(a2_attr));

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, a1);
    APPEND_TO_TREE(body, b1);
    APPEND_TO_TREE(a1, t1);
    APPEND_TO_TREE(a1, b2);
    APPEND_TO_TREE(b2, t2);
    APPEND_TO_TREE(b1, a2);
    APPEND_TO_TREE(a2, t3);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_14()
{
    // #data
    // <!-----><font><div>hello<table>excite!<b>me!<th><i>please!</tr><!--X-->
    // #errors
    // (1,14): expected-doctype-but-got-start-tag
    // (1,41): unexpected-start-tag-implies-table-voodoo
    // (1,48): foster-parenting-character-in-table
    // (1,48): foster-parenting-character-in-table
    // (1,48): foster-parenting-character-in-table
    // (1,48): foster-parenting-character-in-table
    // (1,48): foster-parenting-character-in-table
    // (1,48): foster-parenting-character-in-table
    // (1,48): foster-parenting-character-in-table
    // (1,48): foster-parenting-character-in-table
    // (1,48): foster-parenting-character-in-table
    // (1,48): foster-parenting-character-in-table
    // (1,48): unexpected-cell-in-table-body
    // (1,63): unexpected-cell-end-tag
    // (1,71): eof-in-table
    // #document
    // | <!-- - -->
    // | <html>
    // |   <head>
    // |   <body>
    // |     <font>
    // |       <div>
    // |         "helloexcite!"
    // |         <b>
    // |           "me!"
    // |         <table>
    // |           <tbody>
    // |             <tr>
    // |               <th>
    // |                 <i>
    // |                   "please!"
    // |             <!-- X -->

    unsigned char buffer[] = "<!-----><font><div>hello<table>excite!<b>me!<th><i>please!</tr><!--X-->";
    dom_node_t* expected   = dom_document_new();
    dom_node_t* html       = dom_element_new(expected, html_tag_html());
    dom_node_t* head       = dom_element_new(expected, html_tag_head());
    dom_node_t* body       = dom_element_new(expected, html_tag_body());
    dom_node_t* font       = dom_element_new(expected, html_tag_font());
    dom_node_t* div        = dom_element_new(expected, html_tag_div());
    dom_node_t* t1         = dom_text_new(expected, "helloexcite!", 12);
    dom_node_t* t2         = dom_text_new(expected, "me!", 3);
    dom_node_t* t3         = dom_text_new(expected, "please!", 7);
    dom_node_t* c1         = dom_comment_new(expected, "-", 1);
    dom_node_t* c2         = dom_comment_new(expected, "X", 1);
    dom_node_t* table      = dom_element_new(expected, html_tag_table());
    dom_node_t* tbody      = dom_element_new(expected, html_tag_tbody());
    dom_node_t* tr         = dom_element_new(expected, html_tag_tr());
    dom_node_t* th         = dom_element_new(expected, html_tag_th());
    dom_node_t* i          = dom_element_new(expected, html_tag_i());
    dom_node_t* b          = dom_element_new(expected, html_tag_b());

    APPEND_TO_TREE(expected, c1);
    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, font);
    APPEND_TO_TREE(font, div);
    APPEND_TO_TREE(div, t1);
    APPEND_TO_TREE(div, b);
    APPEND_TO_TREE(b, t2);
    APPEND_TO_TREE(div, table);
    APPEND_TO_TREE(table, tbody);
    APPEND_TO_TREE(tbody, tr);
    APPEND_TO_TREE(tr, th);
    APPEND_TO_TREE(th, i);
    APPEND_TO_TREE(i, t3);
    APPEND_TO_TREE(tbody, c2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_15()
{
    // #data
    // <!DOCTYPE html><li>hello<li>world<ul>how<li>do</ul>you</body><!--do-->
    // #errors
    // #document
    // | <!DOCTYPE html>
    // | <html>
    // |   <head>
    // |   <body>
    // |     <li>
    // |       "hello"
    // |     <li>
    // |       "world"
    // |       <ul>
    // |         "how"
    // |         <li>
    // |           "do"
    // |       "you"
    // |   <!-- do -->

    unsigned char buffer[] = "<!DOCTYPE html><li>hello<li>world<ul>how<li>do</ul>you</body><!--do-->";
    dom_node_t* expected   = dom_document_new();
    dom_node_t* html       = dom_element_new(expected, html_tag_html());
    dom_node_t* head       = dom_element_new(expected, html_tag_head());
    dom_node_t* body       = dom_element_new(expected, html_tag_body());
    dom_node_t* li1        = dom_element_new(expected, html_tag_li());
    dom_node_t* li2        = dom_element_new(expected, html_tag_li());
    dom_node_t* li3        = dom_element_new(expected, html_tag_li());
    dom_node_t* ul         = dom_element_new(expected, html_tag_ul());
    dom_node_t* t1         = dom_text_new(expected, "hello", 5);
    dom_node_t* t2         = dom_text_new(expected, "world", 5);
    dom_node_t* t3         = dom_text_new(expected, "how", 3);
    dom_node_t* t4         = dom_text_new(expected, "do", 2);
    dom_node_t* t5         = dom_text_new(expected, "you", 3);
    dom_node_t* c1         = dom_comment_new(expected, "do", 2);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, li1);
    APPEND_TO_TREE(li1, t1);
    APPEND_TO_TREE(body, li2);
    APPEND_TO_TREE(li2, t2);
    APPEND_TO_TREE(li2, ul);
    APPEND_TO_TREE(ul, t3);
    APPEND_TO_TREE(ul, li3);
    APPEND_TO_TREE(li3, t4);
    APPEND_TO_TREE(li2, t5);
    APPEND_TO_TREE(html, c1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_16()
{
    // #data
    // <!DOCTYPE html>A<option>B<optgroup>C<select>D</option>E
    // #errors
    // (1,54): unexpected-end-tag-in-select
    // (1,55): eof-in-select
    // #document
    // | <!DOCTYPE html>
    // | <html>
    // |   <head>
    // |   <body>
    // |     "A"
    // |     <option>
    // |       "B"
    // |     <optgroup>
    // |       "C"
    // |       <select>
    // |         "DE"

    unsigned char buffer[]  = "<!DOCTYPE html>A<option>B<optgroup>C<select>D</option>E";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* t1          = dom_text_new(expected, "A", 1);
    dom_node_t* o1          = dom_element_new(expected, html_tag_option());
    dom_node_t* t2          = dom_text_new(expected, "B", 1);
    dom_node_t* og1         = dom_element_new(expected, html_tag_optgroup());
    dom_node_t* t3          = dom_text_new(expected, "C", 1);
    dom_node_t* s           = dom_element_new(expected, html_tag_select());
    dom_node_t* t4          = dom_text_new(expected, "DE", 2);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, t1);
    APPEND_TO_TREE(body, o1);
    APPEND_TO_TREE(o1, t2);
    APPEND_TO_TREE(body, og1);
    APPEND_TO_TREE(og1, t3);
    APPEND_TO_TREE(og1, s);
    APPEND_TO_TREE(s, t4);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_17()
{
    // #data
    // <
    // #errors
    // (1,1): expected-tag-name
    // (1,1): expected-doctype-but-got-chars
    // #new-errors
    // (1:2) eof-before-tag-name
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     "<"

    unsigned char buffer[]  = "<";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* t1          = dom_text_new(expected, "<", 1);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_18()
{
    // #data
    // <#
    // #errors
    // (1,1): expected-tag-name
    // (1,1): expected-doctype-but-got-chars
    // #new-errors
    // (1:2) invalid-first-character-of-tag-name
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     "<#"

    unsigned char buffer[]  = "<#";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* t1          = dom_text_new(expected, "<#", 2);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_19()
{
    // #data
    // </
    // #errors
    // (1,1): expected-tag-name
    // (1,1): expected-doctype-but-got-chars
    // #new-errors
    // (1:2) invalid-first-character-of-tag-name
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     "</"

    unsigned char buffer[]  = "</";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* t1          = dom_text_new(expected, "</", 2);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_20()
{
    // #data
    // </#
    // #errors
    // (1,2): expected-closing-tag-but-got-char
    // (1,3): expected-doctype-but-got-eof
    // #new-errors
    // (1:3) invalid-first-character-of-tag-name
    // #document
    // | <!-- # -->
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[]  = "</#";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* c           = dom_comment_new(expected, "#", 1);

    APPEND_TO_TREE(expected, c);
    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_21()
{
    // #data
    // <?
    // #errors
    // (1,1): expected-tag-name-but-got-question-mark
    // (1,2): expected-doctype-but-got-eof
    // #new-errors
    // (1:2) unexpected-question-mark-instead-of-tag-name
    // #document
    // | <!-- ? -->
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[]  = "<?";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* c           = dom_comment_new(expected, "?", 1);

    APPEND_TO_TREE(expected, c);
    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_22()
{
    // #data
    // <?#
    // #errors
    // (1,1): expected-tag-name-but-got-question-mark
    // (1,3): expected-doctype-but-got-eof
    // #new-errors
    // (1:2) unexpected-question-mark-instead-of-tag-name
    // #document
    // | <!-- ?# -->
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[]  = "<?#";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* c           = dom_comment_new(expected, "?#", 2);

    APPEND_TO_TREE(expected, c);
    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_23()
{
    // #data
    // <!
    // #errors
    // (1,2): expected-dashes-or-doctype
    // (1,2): expected-doctype-but-got-eof
    // #new-errors
    // (1:3) incorrectly-opened-comment
    // #document
    // | <!--  -->
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[]  = "<!";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* c           = dom_comment_new(expected, "", 0);

    APPEND_TO_TREE(expected, c);
    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_24()
{
    // #data
    // <!#
    // #errors
    // (1,2): expected-dashes-or-doctype
    // (1,3): expected-doctype-but-got-eof
    // #new-errors
    // (1:3) incorrectly-opened-comment
    // #document
    // | <!-- # -->
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[]  = "<!#";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* c           = dom_comment_new(expected, "#", 1);

    APPEND_TO_TREE(expected, c);
    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_25()
{
    // #data
    // <?COMMENT?>
    // #errors
    // (1,1): expected-tag-name-but-got-question-mark
    // (1,11): expected-doctype-but-got-eof
    // #new-errors
    // (1:2) unexpected-question-mark-instead-of-tag-name
    // #document
    // | <!-- ?COMMENT? -->
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[]  = "<?COMMENT?>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* c           = dom_comment_new(expected, "?COMMENT?", 9);

    APPEND_TO_TREE(expected, c);
    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_26()
{
    // #data
    // <!COMMENT>
    // #errors
    // (1,2): expected-dashes-or-doctype
    // (1,10): expected-doctype-but-got-eof
    // #new-errors
    // (1:3) incorrectly-opened-comment
    // #document
    // | <!-- COMMENT -->
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[]  = "<!COMMENT>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* c           = dom_comment_new(expected, "COMMENT", 7);

    APPEND_TO_TREE(expected, c);
    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_27()
{
    // #data
    // </ COMMENT >
    // #errors
    // (1,2): expected-closing-tag-but-got-char
    // (1,12): expected-doctype-but-got-eof
    // #new-errors
    // (1:3) invalid-first-character-of-tag-name
    // #document
    // | <!--  COMMENT  -->
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[]  = "</ COMMENT >";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* c           = dom_comment_new(expected, " COMMENT ", 9);

    APPEND_TO_TREE(expected, c);
    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_28()
{
    // #data
    // <!DOCTYPE html><style> EOF
    // #errors
    // (1,26): expected-named-closing-tag-but-got-eof
    // #document
    // | <!DOCTYPE html>
    // | <html>
    // |   <head>
    // |     <style>
    // |       " EOF"
    // |   <body>

    unsigned char buffer[]  = "<!DOCTYPE html><style> EOF";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* s           = dom_element_new(expected, html_tag_style());
    dom_node_t* t           = dom_text_new(expected, " EOF", 4);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(head, s);
    APPEND_TO_TREE(s, t);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_29()
{
    // #data
    // <!DOCTYPE html><script> <!-- </script> --> </script> EOF
    // #errors
    // (1,52): unexpected-end-tag
    // #document
    // | <!DOCTYPE html>
    // | <html>
    // |   <head>
    // |     <script>
    // |       " <!-- "
    // |     " "
    // |   <body>
    // |     "-->  EOF"

    unsigned char buffer[]  = "<!DOCTYPE html><script> <!-- </script> --> </script> EOF";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* s           = dom_element_new(expected, html_tag_script());
    dom_node_t* t1          = dom_text_new(expected, " <!-- ", 6);
    dom_node_t* t2          = dom_text_new(expected, " ", 1);
    dom_node_t* t3          = dom_text_new(expected, "-->  EOF", 8);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(head, s);
    APPEND_TO_TREE(head, t2);
    APPEND_TO_TREE(s, t1);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, t3);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_30()
{
    // #data
    // <b><p></b>TEST
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,10): adoption-agency-1.3
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <b>
    // |     <p>
    // |       <b>
    // |       "TEST"

    unsigned char buffer[]  = "<b><p></b>TEST";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* b2          = dom_element_new(expected, html_tag_b());
    dom_node_t* p           = dom_element_new(expected, html_tag_p());
    dom_node_t* t1          = dom_text_new(expected, "TEST", 4);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, b1);
    APPEND_TO_TREE(body, p);
    APPEND_TO_TREE(p, b2);
    APPEND_TO_TREE(p, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_31()
{
    // #data
    // <p id=a><b><p id=b></b>TEST
    // #errors
    // (1,8): expected-doctype-but-got-start-tag
    // (1,19): unexpected-end-tag
    // (1,23): adoption-agency-1.2
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <p>
    // |       id="a"
    // |       <b>
    // |     <p>
    // |       id="b"
    // |       "TEST"

    unsigned char buffer[]  = "<p id=a><b><p id=b></b>TEST";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* p1          = dom_element_new(expected, html_tag_p());
    dom_node_t* p1_attr     = dom_attr_new(hash_str_new("id", 2), hash_str_new("a", 1), p1);
    dom_node_t* p2          = dom_element_new(expected, html_tag_p());
    dom_node_t* p2_attr     = dom_attr_new(hash_str_new("id", 2), hash_str_new("b", 1), p2);
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* t1          = dom_text_new(expected, "TEST", 4);

    dom_element_append_attr(dom_element_from_node(p1), dom_attr_from_node(p1_attr));
    dom_element_append_attr(dom_element_from_node(p2), dom_attr_from_node(p2_attr));

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(body, p2);
    APPEND_TO_TREE(p1, b1);
    APPEND_TO_TREE(p2, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_32()
{
    // #data
    // <b id=a><p><b id=b></p></b>TEST
    // #errors
    // (1,8): expected-doctype-but-got-start-tag
    // (1,23): unexpected-end-tag
    // (1,27): adoption-agency-1.2
    // (1,31): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <b>
    // |       id="a"
    // |       <p>
    // |         <b>
    // |           id="b"
    // |       "TEST"

    unsigned char buffer[]  = "<b id=a><p><b id=b></p></b>TEST";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* b1_attr     = dom_attr_new(hash_str_new("id", 2), hash_str_new("a", 1), b1);
    dom_node_t* p1          = dom_element_new(expected, html_tag_p());
    dom_node_t* b2          = dom_element_new(expected, html_tag_b());
    dom_node_t* b2_attr     = dom_attr_new(hash_str_new("id", 2), hash_str_new("b", 1), b2);
    dom_node_t* t1          = dom_text_new(expected, "TEST", 4);
    
    dom_element_append_attr(dom_element_from_node(b1), dom_attr_from_node(b1_attr));
    dom_element_append_attr(dom_element_from_node(b2), dom_attr_from_node(b2_attr));

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, b1);
    APPEND_TO_TREE(b1, p1);
    APPEND_TO_TREE(p1, b2);
    APPEND_TO_TREE(b1, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_33()
{
    // #data
    // <!DOCTYPE html><title>U-test</title><body><div><p>Test<u></p></div></body>
    // #errors
    // (1,61): unexpected-end-tag
    // #document
    // | <!DOCTYPE html>
    // | <html>
    // |   <head>
    // |     <title>
    // |       "U-test"
    // |   <body>
    // |     <div>
    // |       <p>
    // |         "Test"
    // |         <u>

    unsigned char buffer[]  = "<!DOCTYPE html><title>U-test</title><body><div><p>Test<u></p></div></body>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* title       = dom_element_new(expected, html_tag_title());
    dom_node_t* t1          = dom_text_new(expected, "U-test", 6);
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* p           = dom_element_new(expected, html_tag_p());
    dom_node_t* t2          = dom_text_new(expected, "Test", 4);
    dom_node_t* u           = dom_element_new(expected, html_tag_u());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(head, title);
    APPEND_TO_TREE(title, t1);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, p);
    APPEND_TO_TREE(p, t2);
    APPEND_TO_TREE(p, u);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_34()
{
    // #data
    // <!DOCTYPE html><font><table></font></table></font>
    // #errors
    // (1,35): unexpected-end-tag-implies-table-voodoo
    // (1,35): unexpected-end-tag
    // #document
    // | <!DOCTYPE html>
    // | <html>
    // |   <head>
    // |   <body>
    // |     <font>
    // |       <table>

    unsigned char buffer[]  = "<!DOCTYPE html><font><table></font></table></font>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* font        = dom_element_new(expected, html_tag_font());
    dom_node_t* table       = dom_element_new(expected, html_tag_table());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, font);
    APPEND_TO_TREE(font, table);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_35()
{
    // #data
    // <font><p>hello<b>cruel</font>world
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // (1,29): adoption-agency-1.3
    // (1,29): adoption-agency-1.3
    // (1,34): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <font>
    // |     <p>
    // |       <font>
    // |         "hello"
    // |         <b>
    // |           "cruel"
    // |       <b>
    // |         "world"

    unsigned char buffer[]  = "<font><p>hello<b>cruel</font>world";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* font1       = dom_element_new(expected, html_tag_font());
    dom_node_t* p1          = dom_element_new(expected, html_tag_p());
    dom_node_t* font2       = dom_element_new(expected, html_tag_font());
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* b2          = dom_element_new(expected, html_tag_b());
    dom_node_t* t1          = dom_text_new(expected, "hello", 5);
    dom_node_t* t2          = dom_text_new(expected, "cruel", 5);
    dom_node_t* t3          = dom_text_new(expected, "world", 5);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, font1);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(p1, font2);
    APPEND_TO_TREE(font2, t1);
    APPEND_TO_TREE(font2, b1);
    APPEND_TO_TREE(b1, t2);
    APPEND_TO_TREE(p1, b2);
    APPEND_TO_TREE(b2, t3);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_36()
{
    // #data
    // <b>Test</i>Test
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,11): unexpected-end-tag
    // (1,15): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <b>
    // |       "TestTest"

    unsigned char buffer[]  = "<b>Test</i>Test";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* t1          = dom_text_new(expected, "TestTest", 8);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, b1);
    APPEND_TO_TREE(b1, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
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
    TEST_CASE(test_when_html_tag_is_closed_and_body_end_tag_is_provided_then_close_head_and_add_body_start_tag_automatically);
    TEST_CASE(test_when_only_html_tag_is_closed_then_we_automatically_close_head_and_body_tags);
    TEST_CASE(test_when_head_tag_is_missing_and_body_is_not_closed_then_we_add_head_tag_and_close_body);
    TEST_CASE(test_when_html_open_tag_and_body_tags_are_missing_then_they_get_created);
    TEST_CASE(test_when_only_closed_head_tag_is_provided_then_we_add_missing_html_and_body_tags);
    TEST_CASE(test_when_only_closed_body_tag_is_provided_then_we_add_missing_html_and_head_tags);
    TEST_CASE(test_when_only_closed_html_tag_is_provided_then_we_add_missing_body_and_head_tags);
    TEST_CASE(missing_tags_1);
    // TEST_CASE(missing_html_tag_2);
    TEST_CASE(test_parser_1);
    // TEST_CASE(test_parser_2);
    TEST_CASE(test_parser_3);
    TEST_CASE(test_parser_4);
    TEST_CASE(test_parser_5);
    TEST_CASE(test_parser_6);
    TEST_CASE(test_parser_7);
    TEST_CASE(test_parser_8);
    TEST_CASE(test_parser_9);
    TEST_CASE(test_parser_10);
    TEST_CASE(test_parser_11);
    TEST_CASE(test_parser_12);
    TEST_CASE(test_parser_13);
    TEST_CASE(test_parser_14);
    TEST_CASE(test_parser_15);
    TEST_CASE(test_parser_16);
    TEST_CASE(test_parser_17);
    TEST_CASE(test_parser_18);
    TEST_CASE(test_parser_19);
    TEST_CASE(test_parser_20);
    TEST_CASE(test_parser_21);
    TEST_CASE(test_parser_22);
    TEST_CASE(test_parser_23);
    TEST_CASE(test_parser_24);
    TEST_CASE(test_parser_25);
    TEST_CASE(test_parser_26);
    TEST_CASE(test_parser_27);
    TEST_CASE(test_parser_28);
    TEST_CASE(test_parser_29);
    TEST_CASE(test_parser_30);
    TEST_CASE(test_parser_31);
    TEST_CASE(test_parser_32);
    TEST_CASE(test_parser_33);
    TEST_CASE(test_parser_34);
    TEST_CASE(test_parser_35);
    TEST_CASE(test_parser_36);
}