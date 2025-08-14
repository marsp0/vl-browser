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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* b          = html_element_new(document, "b", 1);
    html_node_t* table      = html_element_new(document, "table", 5);
    html_node_t* tbody      = html_element_new(document, "tbody", 5);
    html_node_t* tr         = html_element_new(document, "tr", 2);
    html_node_t* td         = html_element_new(document, "td", 2);
    html_node_t* i          = html_element_new(document, "i", 1);
    

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, b);
    APPEND_TO_TREE(b, table);
    APPEND_TO_TREE(table, tbody);
    APPEND_TO_TREE(tbody, tr);
    APPEND_TO_TREE(tr, td);
    APPEND_TO_TREE(td, i);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


// static void missing_tags_2()
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
//     html_node_t* document   = html_document_new();
//     html_node_t* html       = html_element_new(document, "html", 4);
//     html_node_t* head       = html_element_new(document, "head", 4);
//     html_node_t* body       = html_element_new(document, "body", 4);
//     html_node_t* b          = html_element_new(document, "b", 1);
//     html_node_t* table      = html_element_new(document, "table", 5);
//     html_node_t* tbody      = html_element_new(document, "tbody", 5);
//     html_node_t* tr         = html_element_new(document, "tr", 2);
//     html_node_t* td         = html_element_new(document, "td", 2);
//     html_node_t* i          = html_element_new(document, "i", 1);
//     html_node_t* text       = html_text_new(document, "X", 1);
    

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

//     RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* h1         = html_element_new(document, "h1", 2);
    html_node_t* h2         = html_element_new(document, "h2", 2);
    html_node_t* t1         = html_text_new(document, "Hello", 5);
    html_node_t* t2         = html_text_new(document, "World", 5);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, h1);
    APPEND_TO_TREE(body, h2);
    APPEND_TO_TREE(h1, t1);
    APPEND_TO_TREE(h2, t2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* span       = html_element_new(document, "span", 4);
    html_node_t* button     = html_element_new(document, "button", 6);
    html_node_t* t1         = html_text_new(document, "foobar", 6);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, span);
    APPEND_TO_TREE(span, button);
    APPEND_TO_TREE(button, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* p1         = html_element_new(document, "p", 1);
    html_node_t* b1         = html_element_new(document, "b", 1);
    html_node_t* div        = html_element_new(document, "div", 3);
    html_node_t* b2         = html_element_new(document, "b", 1);
    html_node_t* marquee    = html_element_new(document, "marquee", 7);
    html_node_t* p2         = html_element_new(document, "p", 1);
    html_node_t* t          = html_text_new(document, "X", 1);

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

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* script     = html_element_new(document, "script", 6);
    html_node_t* t1         = html_text_new(document, "<div>", 5);
    html_node_t* title      = html_element_new(document, "title", 5);
    html_node_t* t2         = html_text_new(document, "<p>", 3);
    html_node_t* p1         = html_element_new(document, "p", 1);
    html_node_t* p2         = html_element_new(document, "p", 1);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(head, script);
    APPEND_TO_TREE(script, t1);
    APPEND_TO_TREE(head, title);
    APPEND_TO_TREE(title, t2);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(body, p2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* c1         = html_comment_new(document, "", 0);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* div        = html_element_new(document, "div", 3);
    html_node_t* t          = html_text_new(document, "--", 2);
    html_node_t* c2         = html_comment_new(document, "", 0);


    APPEND_TO_TREE(document, c1);
    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, t);
    APPEND_TO_TREE(div, c2);
    

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* p1         = html_element_new(document, "p", 1);
    html_node_t* hr         = html_element_new(document, "hr", 2);
    html_node_t* p2         = html_element_new(document, "p", 1);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(body, hr);
    APPEND_TO_TREE(body, p2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


static void test_parser_8()
{
    // #data
    // <select><b><option><select><option></b></select>X
    // #errors
    // 1:1: ERROR: Expected a doctype token
    // 1:20: ERROR: Start tag 'select' isn't allowed here. Currently open tags: html, body, select, b, option.
    // 1:36: ERROR: End tag 'b' isn't allowed here. Currently open tags: html, body, b, select, option.
    // 1:50: ERROR: Premature end of file. Currently open tags: html, body, b.
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
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* select     = html_element_new(document, "select", 6);
    html_node_t* o1         = html_element_new(document, "option", 6);
    html_node_t* o2         = html_element_new(document, "option", 6);
    html_node_t* b1         = html_element_new(document, "b", 1);
    html_node_t* b2         = html_element_new(document, "b", 1);
    html_node_t* t          = html_text_new(document, "X", 1);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, select);
    APPEND_TO_TREE(body, b2);
    APPEND_TO_TREE(body, t);
    APPEND_TO_TREE(select, b1);
    APPEND_TO_TREE(b1, o1);
    APPEND_TO_TREE(b2, o2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


static void test_parser_9()
{
    unsigned char buffer[] = "<p>1<b>2<i>3</b>4</i>5</p>";
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* p          = html_element_new(document, "p", 1);
    html_node_t* b          = html_element_new(document, "b", 1);
    html_node_t* i1         = html_element_new(document, "i", 1);
    html_node_t* i2         = html_element_new(document, "i", 1);
    html_node_t* t1         = html_text_new(document, "1", 1);
    html_node_t* t2         = html_text_new(document, "2", 1);
    html_node_t* t3         = html_text_new(document, "3", 1);
    html_node_t* t4         = html_text_new(document, "4", 1);
    html_node_t* t5         = html_text_new(document, "5", 1);

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

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


static void test_parser_10()
{
    unsigned char buffer[] = "<b>1<p>2</b>3</p>";
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* b1         = html_element_new(document, "b", 1);
    html_node_t* b2         = html_element_new(document, "b", 1);
    html_node_t* p1         = html_element_new(document, "p", 1);
    html_node_t* t1         = html_text_new(document, "1", 1);
    html_node_t* t2         = html_text_new(document, "2", 1);
    html_node_t* t3         = html_text_new(document, "3", 1);

    APPEND_TO_TREE(document, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, b1);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(b1, t1);
    APPEND_TO_TREE(p1, b2);
    APPEND_TO_TREE(b2, t2);
    APPEND_TO_TREE(p1, t3);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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

    unsigned char buffer[] = "<a><table><td><a><table></table><a></tr><a></table><b>X</b>C<a>Y";
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* a1       = html_element_new(document, "a", 1);
    html_node_t* a2       = html_element_new(document, "a", 1);
    html_node_t* a3       = html_element_new(document, "a", 1);
    html_node_t* a4       = html_element_new(document, "a", 1);
    html_node_t* a5       = html_element_new(document, "a", 1);
    html_node_t* a6       = html_element_new(document, "a", 1);
    html_node_t* table       = html_element_new(document, "table", 5);
    html_node_t* tbody       = html_element_new(document, "tbody", 5);
    html_node_t* tr       = html_element_new(document, "tr", 2);
    html_node_t* td       = html_element_new(document, "td", 2);
    html_node_t* table2       = html_element_new(document, "table", 5);
    html_node_t* b1       = html_element_new(document, "b", 1);
    html_node_t* x         = html_text_new(document, "X", 1);
    html_node_t* c         = html_text_new(document, "C", 1);
    html_node_t* y         = html_text_new(document, "Y", 1);

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

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
}


static void test_parser_12()
{
    // https://html.spec.whatwg.org/multipage/parsing.html#unexpected-markup-in-tables

    unsigned char buffer[] = "<table><b><tr><td>aaa</td></tr>bbb</table>ccc";
    html_node_t* document   = html_document_new();
    html_node_t* html       = html_element_new(document, "html", 4);
    html_node_t* head       = html_element_new(document, "head", 4);
    html_node_t* body       = html_element_new(document, "body", 4);
    html_node_t* b1       = html_element_new(document, "b", 1);
    html_node_t* b2       = html_element_new(document, "b", 1);
    html_node_t* b3       = html_element_new(document, "b", 1);
    html_node_t* t       = html_element_new(document, "table", 5);
    html_node_t* tb       = html_element_new(document, "tbody", 5);
    html_node_t* tr       = html_element_new(document, "tr", 2);
    html_node_t* td       = html_element_new(document, "td", 2);
    html_node_t* t1         = html_text_new(document, "bbb", 3);
    html_node_t* t2         = html_text_new(document, "aaa", 3);
    html_node_t* t3         = html_text_new(document, "ccc", 3);

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

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, document);
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
    html_node_t* expected   = html_document_new();
    html_node_t* html       = html_element_new(expected, "html", 4);
    html_node_t* head       = html_element_new(expected, "head", 4);
    html_node_t* body       = html_element_new(expected, "body", 4);
    html_node_t* a1       = html_element_new(expected, "a", 1);
    html_node_t* a2       = html_element_new(expected, "a", 1);
    html_node_t* b1       = html_element_new(expected, "b", 1);
    html_node_t* b2       = html_element_new(expected, "b", 1);
    html_node_t* t1       = html_text_new(expected, "0", 1);
    html_node_t* t2       = html_text_new(expected, "1", 1);
    html_node_t* t3       = html_text_new(expected, "2", 1);

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

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected);
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
    html_node_t* expected   = html_document_new();
    html_node_t* html       = html_element_new(expected, "html", 4);
    html_node_t* head       = html_element_new(expected, "head", 4);
    html_node_t* body       = html_element_new(expected, "body", 4);
    html_node_t* font       = html_element_new(expected, "font", 4);
    html_node_t* div        = html_element_new(expected, "div", 3);
    html_node_t* t1         = html_text_new(expected, "helloexcite!", 12);
    html_node_t* t2         = html_text_new(expected, "me!", 3);
    html_node_t* t3         = html_text_new(expected, "please!", 7);
    html_node_t* c1         = html_comment_new(expected, "-", 1);
    html_node_t* c2         = html_comment_new(expected, "X", 1);
    html_node_t* table      = html_element_new(expected, "table", 5);
    html_node_t* tbody      = html_element_new(expected, "tbody", 5);
    html_node_t* tr         = html_element_new(expected, "tr", 2);
    html_node_t* th         = html_element_new(expected, "th", 2);
    html_node_t* i          = html_element_new(expected, "i", 1);
    html_node_t* b          = html_element_new(expected, "b", 1);

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

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected);
}

// html_parser_init();
// html_node_t* actual = html_parser_run(buffer, sizeof(buffer) - 1);
// ASSERT_NODE(actual, expected);
// print_document_tree(actual, 0);
// print_document_tree(expected, 0);
// html_node_free(expected);
// html_node_free(actual);
// html_parser_free();

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
    // TEST_CASE(missing_tags_2);
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
}