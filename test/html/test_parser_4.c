#include "html/test_parser_4.h"

#include "test_parser_utils.h"

#include "html/tag_constants.h"
#include "html/attr_constants.h"

static void test_parser_81()
{
    // #data
    // <ul><li></li><div><li></div><li><li><div><li><address><li><b><em></b><li></ul>
    // #errors
    // (1,4): expected-doctype-but-got-start-tag
    // (1,45): end-tag-too-early
    // (1,58): end-tag-too-early
    // (1,69): adoption-agency-1.3
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <ul>
    // |       <li>
    // |       <div>
    // |         <li>
    // |       <li>
    // |       <li>
    // |         <div>
    // |       <li>
    // |         <address>
    // |       <li>
    // |         <b>
    // |           <em>
    // |       <li>

    unsigned char buffer[]  = "<ul><li></li><div><li></div><li><li><div><li><address><li><b><em></b><li></ul>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* ul          = dom_element_new(expected, html_tag_ul());
    dom_node_t* li1         = dom_element_new(expected, html_tag_li());
    dom_node_t* li2         = dom_element_new(expected, html_tag_li());
    dom_node_t* li3         = dom_element_new(expected, html_tag_li());
    dom_node_t* li4         = dom_element_new(expected, html_tag_li());
    dom_node_t* li5         = dom_element_new(expected, html_tag_li());
    dom_node_t* li6         = dom_element_new(expected, html_tag_li());
    dom_node_t* li7         = dom_element_new(expected, html_tag_li());
    dom_node_t* div1        = dom_element_new(expected, html_tag_div());
    dom_node_t* div2        = dom_element_new(expected, html_tag_div());
    dom_node_t* address     = dom_element_new(expected, html_tag_address());
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* em1         = dom_element_new(expected, html_tag_em());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, ul);
    APPEND_TO_TREE(ul, li1);
    APPEND_TO_TREE(ul, div1);
    APPEND_TO_TREE(div1, li2);
    APPEND_TO_TREE(ul, li3);
    APPEND_TO_TREE(ul, li4);
    APPEND_TO_TREE(li4, div2);
    APPEND_TO_TREE(ul, li5);
    APPEND_TO_TREE(li5, address);
    APPEND_TO_TREE(ul, li6);
    APPEND_TO_TREE(li6, b1);
    APPEND_TO_TREE(b1, em1);
    APPEND_TO_TREE(ul, li7);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_82()
{
    // #data
    // <frameset><frame><frameset><frame></frameset><noframes></noframes></frameset>
    // #errors
    // (1,10): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <frameset>
    // |     <frame>
    // |     <frameset>
    // |       <frame>
    // |     <noframes>

    unsigned char buffer[]  = "<frameset><frame><frameset><frame></frameset><noframes></noframes></frameset>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* fs1         = dom_element_new(expected, html_tag_frameset());
    dom_node_t* f1          = dom_element_new(expected, html_tag_frame());
    dom_node_t* fs2         = dom_element_new(expected, html_tag_frameset());
    dom_node_t* f2          = dom_element_new(expected, html_tag_frame());
    dom_node_t* nf1         = dom_element_new(expected, html_tag_noframes());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, fs1);
    APPEND_TO_TREE(fs1, f1);
    APPEND_TO_TREE(fs1, fs2);
    APPEND_TO_TREE(fs1, nf1);
    APPEND_TO_TREE(fs2, f2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_83()
{
    // #data
    // <h1><table><td><h3></table><h3></h1>
    // #errors
    // (1,4): expected-doctype-but-got-start-tag
    // (1,15): unexpected-cell-in-table-body
    // (1,27): unexpected-cell-end-tag
    // (1,31): unexpected-start-tag
    // (1,36): end-tag-too-early
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <h1>
    // |       <table>
    // |         <tbody>
    // |           <tr>
    // |             <td>
    // |               <h3>
    // |     <h3>

    unsigned char buffer[]  = "<h1><table><td><h3></table><h3></h1>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* h1        = dom_element_new(expected, html_tag_h1());
    dom_node_t* table        = dom_element_new(expected, html_tag_table());
    dom_node_t* tbody        = dom_element_new(expected, html_tag_tbody());
    dom_node_t* tr        = dom_element_new(expected, html_tag_tr());
    dom_node_t* td        = dom_element_new(expected, html_tag_td());
    dom_node_t* h31        = dom_element_new(expected, html_tag_h3());
    dom_node_t* h32        = dom_element_new(expected, html_tag_h3());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, h1);
    APPEND_TO_TREE(body, h32);
    APPEND_TO_TREE(h1, table);
    APPEND_TO_TREE(table, tbody);
    APPEND_TO_TREE(tbody, tr);
    APPEND_TO_TREE(tr, td);
    APPEND_TO_TREE(td, h31);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


void test_html_parser_test4()
{
    TEST_CASE(test_parser_81);
    TEST_CASE(test_parser_82);
    TEST_CASE(test_parser_83);
}