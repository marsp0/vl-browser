#include "test_parser.h"

#include "test_parser_utils.h"

#include "html/tag_constants.h"
#include "html/attr_constants.h"

static void test_parser_55()
{
    // #data
    // <test attribute---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
    // #errors
    // (1,1040): expected-doctype-but-got-start-tag
    // (1,1040): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <test>
    // |       attribute----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------=""

    unsigned char buffer[]  = "<test attribute---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* test        = dom_element_new(expected, hash_str_new("test", 4));
    hash_str_t a_name       = hash_str_new("attribute-------------------------------------------------------", 64);
    dom_node_t* attr        = dom_attr_new(a_name, 0, test);

    dom_element_append_attr(dom_element_from_node(test), dom_attr_from_node(attr));

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, test);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_56()
{
    // #data
    // <a href="blah">aba<table><a href="foo">br<tr><td></td></tr>x</table>aoe
    // #errors
    // (1,15): expected-doctype-but-got-start-tag
    // (1,39): unexpected-start-tag-implies-table-voodoo
    // (1,39): unexpected-start-tag-implies-end-tag
    // (1,39): unexpected-end-tag
    // (1,45): foster-parenting-character-in-table
    // (1,45): foster-parenting-character-in-table
    // (1,68): foster-parenting-character-in-table
    // (1,71): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <a>
    // |       href="blah"
    // |       "aba"
    // |       <a>
    // |         href="foo"
    // |         "br"
    // |       <a>
    // |         href="foo"
    // |         "x"
    // |       <table>
    // |         <tbody>
    // |           <tr>
    // |             <td>
    // |     <a>
    // |       href="foo"
    // |       "aoe"

    unsigned char buffer[]  = "<a href=\"blah\">aba<table><a href=\"foo\">br<tr><td></td></tr>x</table>aoe";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* a1          = dom_element_new(expected, html_tag_a());
    dom_node_t* a2          = dom_element_new(expected, html_tag_a());
    dom_node_t* a3          = dom_element_new(expected, html_tag_a());
    dom_node_t* a4          = dom_element_new(expected, html_tag_a());
    dom_node_t* attr1       = dom_attr_new(html_attr_href(), hash_str_new("blah", 4), a1);
    dom_node_t* attr2       = dom_attr_new(html_attr_href(), hash_str_new("foo", 3), a2);
    dom_node_t* attr3       = dom_attr_new(html_attr_href(), hash_str_new("foo", 3), a3);
    dom_node_t* attr4       = dom_attr_new(html_attr_href(), hash_str_new("foo", 3), a4);
    dom_node_t* t1          = dom_text_new(expected, "aba", 3);
    dom_node_t* t2          = dom_text_new(expected, "br", 2);
    dom_node_t* t3          = dom_text_new(expected, "x", 1);
    dom_node_t* t4          = dom_text_new(expected, "aoe", 3);
    dom_node_t* table       = dom_element_new(expected, html_tag_table());
    dom_node_t* tbody       = dom_element_new(expected, html_tag_tbody());
    dom_node_t* tr          = dom_element_new(expected, html_tag_tr());
    dom_node_t* td          = dom_element_new(expected, html_tag_td());

    dom_element_append_attr(dom_element_from_node(a1), dom_attr_from_node(attr1));
    dom_element_append_attr(dom_element_from_node(a2), dom_attr_from_node(attr2));
    dom_element_append_attr(dom_element_from_node(a3), dom_attr_from_node(attr3));
    dom_element_append_attr(dom_element_from_node(a4), dom_attr_from_node(attr4));

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, a1);
    APPEND_TO_TREE(body, a4);
    APPEND_TO_TREE(a1, t1);
    APPEND_TO_TREE(a1, a2);
    APPEND_TO_TREE(a1, a3);
    APPEND_TO_TREE(a1, table);
    APPEND_TO_TREE(a2, t2);
    APPEND_TO_TREE(a3, t3);
    APPEND_TO_TREE(table, tbody);
    APPEND_TO_TREE(tbody, tr);
    APPEND_TO_TREE(tr, td);
    APPEND_TO_TREE(a4, t4);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_57()
{
    // #data
    // <a href="blah">aba<table><tr><td><a href="foo">br</td></tr>x</table>aoe
    // #errors
    // (1,15): expected-doctype-but-got-start-tag
    // (1,54): unexpected-cell-end-tag
    // (1,68): unexpected text in table
    // (1,71): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <a>
    // |       href="blah"
    // |       "abax"
    // |       <table>
    // |         <tbody>
    // |           <tr>
    // |             <td>
    // |               <a>
    // |                 href="foo"
    // |                 "br"
    // |       "aoe"

    unsigned char buffer[]  = "<a href=\"blah\">aba<table><tr><td><a href=\"foo\">br</td></tr>x</table>aoe";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* a1          = dom_element_new(expected, html_tag_a());
    dom_node_t* a2          = dom_element_new(expected, html_tag_a());
    dom_node_t* attr1       = dom_attr_new(html_attr_href(), hash_str_new("blah", 4), a1);
    dom_node_t* attr2       = dom_attr_new(html_attr_href(), hash_str_new("foo", 3), a2);
    dom_node_t* t1          = dom_text_new(expected, "abax", 4);
    dom_node_t* t2          = dom_text_new(expected, "br", 2);
    dom_node_t* t3          = dom_text_new(expected, "aoe", 3);
    dom_node_t* table       = dom_element_new(expected, html_tag_table());
    dom_node_t* tbody       = dom_element_new(expected, html_tag_tbody());
    dom_node_t* tr          = dom_element_new(expected, html_tag_tr());
    dom_node_t* td          = dom_element_new(expected, html_tag_td());

    dom_element_append_attr(dom_element_from_node(a1), dom_attr_from_node(attr1));
    dom_element_append_attr(dom_element_from_node(a2), dom_attr_from_node(attr2));

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, a1);
    APPEND_TO_TREE(a1, t1);
    APPEND_TO_TREE(a1, table);
    APPEND_TO_TREE(a1, t3);
    APPEND_TO_TREE(table, tbody);
    APPEND_TO_TREE(tbody, tr);
    APPEND_TO_TREE(tr, td);
    APPEND_TO_TREE(td, a2);
    APPEND_TO_TREE(a2, t2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_58()
{
    // #data
    // <table><a href="blah">aba<tr><td><a href="foo">br</td></tr>x</table>aoe
    // #errors
    // (1,7): expected-doctype-but-got-start-tag
    // (1,22): unexpected-start-tag-implies-table-voodoo
    // (1,29): foster-parenting-character-in-table
    // (1,29): foster-parenting-character-in-table
    // (1,29): foster-parenting-character-in-table
    // (1,54): unexpected-cell-end-tag
    // (1,68): foster-parenting-character-in-table
    // (1,71): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <a>
    // |       href="blah"
    // |       "aba"
    // |     <a>
    // |       href="blah"
    // |       "x"
    // |     <table>
    // |       <tbody>
    // |         <tr>
    // |           <td>
    // |             <a>
    // |               href="foo"
    // |               "br"
    // |     <a>
    // |       href="blah"
    // |       "aoe"

    unsigned char buffer[]  = "<table><a href=\"blah\">aba<tr><td><a href=\"foo\">br</td></tr>x</table>aoe";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* a1          = dom_element_new(expected, html_tag_a());
    dom_node_t* a2          = dom_element_new(expected, html_tag_a());
    dom_node_t* a3          = dom_element_new(expected, html_tag_a());
    dom_node_t* a4          = dom_element_new(expected, html_tag_a());
    dom_node_t* attr1       = dom_attr_new(html_attr_href(), hash_str_new("blah", 4), a1);
    dom_node_t* attr2       = dom_attr_new(html_attr_href(), hash_str_new("blah", 4), a2);
    dom_node_t* attr3       = dom_attr_new(html_attr_href(), hash_str_new("foo", 3), a3);
    dom_node_t* attr4       = dom_attr_new(html_attr_href(), hash_str_new("blah", 4), a4);
    dom_node_t* t1          = dom_text_new(expected, "aba", 3);
    dom_node_t* t2          = dom_text_new(expected, "x", 1);
    dom_node_t* t3          = dom_text_new(expected, "br", 2);
    dom_node_t* t4          = dom_text_new(expected, "aoe", 3);
    dom_node_t* table       = dom_element_new(expected, html_tag_table());
    dom_node_t* tbody       = dom_element_new(expected, html_tag_tbody());
    dom_node_t* tr          = dom_element_new(expected, html_tag_tr());
    dom_node_t* td          = dom_element_new(expected, html_tag_td());

    dom_element_append_attr(dom_element_from_node(a1), dom_attr_from_node(attr1));
    dom_element_append_attr(dom_element_from_node(a2), dom_attr_from_node(attr2));
    dom_element_append_attr(dom_element_from_node(a3), dom_attr_from_node(attr3));
    dom_element_append_attr(dom_element_from_node(a4), dom_attr_from_node(attr4));

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, a1);
    APPEND_TO_TREE(body, a2);
    APPEND_TO_TREE(body, table);
    APPEND_TO_TREE(body, a4);
    APPEND_TO_TREE(a1, t1);
    APPEND_TO_TREE(a2, t2);
    APPEND_TO_TREE(table, tbody);
    APPEND_TO_TREE(tbody, tr);
    APPEND_TO_TREE(tr, td);
    APPEND_TO_TREE(td, a3);
    APPEND_TO_TREE(a3, t3);
    APPEND_TO_TREE(a4, t4);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_59()
{
    // #data
    // <a href=a>aa<marquee>aa<a href=b>bb</marquee>aa
    // #errors
    // (1,10): expected-doctype-but-got-start-tag
    // (1,45): end-tag-too-early
    // (1,47): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <a>
    // |       href="a"
    // |       "aa"
    // |       <marquee>
    // |         "aa"
    // |         <a>
    // |           href="b"
    // |           "bb"
    // |       "aa"

    unsigned char buffer[]  = "<a href=a>aa<marquee>aa<a href=b>bb</marquee>aa";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* a1          = dom_element_new(expected, html_tag_a());
    dom_node_t* a2          = dom_element_new(expected, html_tag_a());
    dom_node_t* attr1       = dom_attr_new(html_attr_href(), hash_str_new("a", 1), a1);
    dom_node_t* attr2       = dom_attr_new(html_attr_href(), hash_str_new("b", 1), a2);
    dom_node_t* t1          = dom_text_new(expected, "aa", 2);
    dom_node_t* t2          = dom_text_new(expected, "aa", 2);
    dom_node_t* t3          = dom_text_new(expected, "bb", 2);
    dom_node_t* t4          = dom_text_new(expected, "aa", 2);
    dom_node_t* m1          = dom_element_new(expected, html_tag_marquee());

    dom_element_append_attr(dom_element_from_node(a1), dom_attr_from_node(attr1));
    dom_element_append_attr(dom_element_from_node(a2), dom_attr_from_node(attr2));

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, a1);
    APPEND_TO_TREE(a1, t1);
    APPEND_TO_TREE(a1, m1);
    APPEND_TO_TREE(a1, t4);
    APPEND_TO_TREE(m1, t2);
    APPEND_TO_TREE(m1, a2);
    APPEND_TO_TREE(a2, t3);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_60()
{
    // #data
    // <wbr><strike><code></strike><code><strike></code>
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,28): adoption-agency-1.3
    // (1,49): adoption-agency-1.3
    // (1,49): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <wbr>
    // |     <strike>
    // |       <code>
    // |     <code>
    // |       <code>
    // |         <strike>

    unsigned char buffer[]  = "<wbr><strike><code></strike><code><strike></code>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* wbr         = dom_element_new(expected, html_tag_wbr());
    dom_node_t* str1        = dom_element_new(expected, html_tag_strike());
    dom_node_t* code1       = dom_element_new(expected, html_tag_code());
    dom_node_t* code2       = dom_element_new(expected, html_tag_code());
    dom_node_t* code3       = dom_element_new(expected, html_tag_code());
    dom_node_t* str2        = dom_element_new(expected, html_tag_strike());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, wbr);
    APPEND_TO_TREE(body, str1);
    APPEND_TO_TREE(body, code2);
    APPEND_TO_TREE(str1, code1);
    APPEND_TO_TREE(code2, code3);
    APPEND_TO_TREE(code3, str2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_61()
{
    // #data
    // <!DOCTYPE html><spacer>foo
    // #errors
    // (1,26): expected-closing-tag-but-got-eof
    // #document
    // | <!DOCTYPE html>
    // | <html>
    // |   <head>
    // |   <body>
    // |     <spacer>
    // |       "foo"

    unsigned char buffer[]  = "<!DOCTYPE html><spacer>foo";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* spacer      = dom_element_new(expected, html_tag_spacer());
    dom_node_t* t1          = dom_text_new(expected, "foo", 3);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, spacer);
    APPEND_TO_TREE(spacer, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_62()
{
    // #data
    // <title><meta></title><link><title><meta></title>
    // #errors
    // (1,7): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |     <title>
    // |       "<meta>"
    // |     <link>
    // |     <title>
    // |       "<meta>"
    // |   <body>

    unsigned char buffer[]  = "<title><meta></title><link><title><meta></title>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* title1      = dom_element_new(expected, html_tag_title());
    dom_node_t* link        = dom_element_new(expected, html_tag_link());
    dom_node_t* title2      = dom_element_new(expected, html_tag_title());
    dom_node_t* t1          = dom_text_new(expected, "<meta>", 6);
    dom_node_t* t2          = dom_text_new(expected, "<meta>", 6);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(head, title1);
    APPEND_TO_TREE(title1, t1);
    APPEND_TO_TREE(head, link);
    APPEND_TO_TREE(head, title2);
    APPEND_TO_TREE(title2, t2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_63()
{
    // #data
    // <style><!--</style><meta><script>--><link></script>
    // #errors
    // (1,7): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |     <style>
    // |       "<!--"
    // |     <meta>
    // |     <script>
    // |       "--><link>"
    // |   <body>

    unsigned char buffer[]  = "<style><!--</style><meta><script>--><link></script>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* style       = dom_element_new(expected, html_tag_style());
    dom_node_t* meta        = dom_element_new(expected, html_tag_meta());
    dom_node_t* script      = dom_element_new(expected, html_tag_script());
    dom_node_t* t1          = dom_text_new(expected, "<!--", 4);
    dom_node_t* t2          = dom_text_new(expected, "--><link>", 9);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(head, style);
    APPEND_TO_TREE(head, meta);
    APPEND_TO_TREE(head, script);
    APPEND_TO_TREE(style, t1);
    APPEND_TO_TREE(script, t2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_64()
{
    // #data
    // <head><meta></head><link>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // (1,25): unexpected-start-tag-out-of-my-head
    // #document
    // | <html>
    // |   <head>
    // |     <meta>
    // |     <link>
    // |   <body>

    unsigned char buffer[]  = "<head><meta></head><link>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* meta        = dom_element_new(expected, html_tag_meta());
    dom_node_t* link        = dom_element_new(expected, html_tag_link());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(head, meta);
    APPEND_TO_TREE(head, link);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_65()
{
    // #data
    // <table><tr><tr><td><td><span><th><span>X</table>
    // #errors
    // (1,7): expected-doctype-but-got-start-tag
    // (1,33): unexpected-cell-end-tag
    // (1,48): unexpected-cell-end-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <table>
    // |       <tbody>
    // |         <tr>
    // |         <tr>
    // |           <td>
    // |           <td>
    // |             <span>
    // |           <th>
    // |             <span>
    // |               "X"

    unsigned char buffer[]  = "<table><tr><tr><td><td><span><th><span>X</table>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* table       = dom_element_new(expected, html_tag_table());
    dom_node_t* tbody       = dom_element_new(expected, html_tag_tbody());
    dom_node_t* tr1         = dom_element_new(expected, html_tag_tr());
    dom_node_t* tr2         = dom_element_new(expected, html_tag_tr());
    dom_node_t* td1         = dom_element_new(expected, html_tag_td());
    dom_node_t* td2         = dom_element_new(expected, html_tag_td());
    dom_node_t* span1       = dom_element_new(expected, html_tag_span());
    dom_node_t* th1         = dom_element_new(expected, html_tag_th());
    dom_node_t* span2       = dom_element_new(expected, html_tag_span());
    dom_node_t* t1          = dom_text_new(expected, "X", 1);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, table);
    APPEND_TO_TREE(table, tbody);
    APPEND_TO_TREE(tbody, tr1);
    APPEND_TO_TREE(tbody, tr2);
    APPEND_TO_TREE(tr2, td1);
    APPEND_TO_TREE(tr2, td2);
    APPEND_TO_TREE(tr2, th1);
    APPEND_TO_TREE(td2, span1);
    APPEND_TO_TREE(th1, span2);
    APPEND_TO_TREE(span2, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_66()
{
    // #data
    // <body><body><base><link><meta><title><p></title><body><p></body>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // (1,12): unexpected-start-tag
    // (1,54): unexpected-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <base>
    // |     <link>
    // |     <meta>
    // |     <title>
    // |       "<p>"
    // |     <p>

    unsigned char buffer[]  = "<body><body><base><link><meta><title><p></title><body><p></body>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* base        = dom_element_new(expected, html_tag_base());
    dom_node_t* link        = dom_element_new(expected, html_tag_link());
    dom_node_t* meta        = dom_element_new(expected, html_tag_meta());
    dom_node_t* title       = dom_element_new(expected, html_tag_title());
    dom_node_t* p1          = dom_element_new(expected, html_tag_p());
    dom_node_t* t1          = dom_text_new(expected, "<p>", 3);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, base);
    APPEND_TO_TREE(body, link);
    APPEND_TO_TREE(body, meta);
    APPEND_TO_TREE(body, title);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(title, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_67()
{
    // #data
    // <textarea><p></textarea>
    // #errors
    // (1,10): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <textarea>
    // |       "<p>"

    unsigned char buffer[]  = "<textarea><p></textarea>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* textarea    = dom_element_new(expected, html_tag_textarea());
    dom_node_t* t1          = dom_text_new(expected, "<p>", 3);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, textarea);
    APPEND_TO_TREE(textarea, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_68()
{
    // #data
    // <p><image></p>
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,10): unexpected-start-tag-treated-as
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <p>
    // |       <img>

    unsigned char buffer[]  = "<p><image></p>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* p           = dom_element_new(expected, html_tag_p());
    dom_node_t* img         = dom_element_new(expected, html_tag_img());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, p);
    APPEND_TO_TREE(p, img);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_69()
{
    // #data
    // <a><table><a></table><p><a><div><a>
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,13): unexpected-start-tag-implies-table-voodoo
    // (1,13): unexpected-start-tag-implies-end-tag
    // (1,13): adoption-agency-1.3
    // (1,27): unexpected-start-tag-implies-end-tag
    // (1,27): adoption-agency-1.2
    // (1,32): unexpected-end-tag
    // (1,35): unexpected-start-tag-implies-end-tag
    // (1,35): adoption-agency-1.2
    // (1,35): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <a>
    // |       <a>
    // |       <table>
    // |     <p>
    // |       <a>
    // |     <div>
    // |       <a>

    unsigned char buffer[]  = "<a><table><a></table><p><a><div><a>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* a1          = dom_element_new(expected, html_tag_a());
    dom_node_t* a2          = dom_element_new(expected, html_tag_a());
    dom_node_t* a3          = dom_element_new(expected, html_tag_a());
    dom_node_t* a4          = dom_element_new(expected, html_tag_a());
    dom_node_t* table       = dom_element_new(expected, html_tag_table());
    dom_node_t* p1          = dom_element_new(expected, html_tag_p());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, a1);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(a1, a2);
    APPEND_TO_TREE(a1, table);
    APPEND_TO_TREE(p1, a3);
    APPEND_TO_TREE(div, a4);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_70()
{
    // #data
    // <head></p><meta><p>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // (1,10): unexpected-end-tag
    // #document
    // | <html>
    // |   <head>
    // |     <meta>
    // |   <body>
    // |     <p>

    unsigned char buffer[]  = "<head></p><meta><p>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* meta        = dom_element_new(expected, html_tag_meta());
    dom_node_t* p           = dom_element_new(expected, html_tag_p());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(head, meta);
    APPEND_TO_TREE(body, p);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_71()
{
    // #data
    // <head></html><meta><p>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // (1,19): expected-eof-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <meta>
    // |     <p>

    unsigned char buffer[]  = "<head></html><meta><p>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* meta        = dom_element_new(expected, html_tag_meta());
    dom_node_t* p           = dom_element_new(expected, html_tag_p());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, meta);
    APPEND_TO_TREE(body, p);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_72()
{
    // #data
    // <b><table><td></b><i></table>
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,14): unexpected-cell-in-table-body
    // (1,18): unexpected-end-tag
    // (1,29): unexpected-cell-end-tag
    // (1,29): expected-closing-tag-but-got-eof
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

    unsigned char buffer[]  = "<b><table><td></b><i></table>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* b           = dom_element_new(expected, html_tag_b());
    dom_node_t* table       = dom_element_new(expected, html_tag_table());
    dom_node_t* tbody       = dom_element_new(expected, html_tag_tbody());
    dom_node_t* tr          = dom_element_new(expected, html_tag_tr());
    dom_node_t* td          = dom_element_new(expected, html_tag_td());
    dom_node_t* i           = dom_element_new(expected, html_tag_i());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, b);
    APPEND_TO_TREE(b, table);
    APPEND_TO_TREE(table, tbody);
    APPEND_TO_TREE(tbody, tr);
    APPEND_TO_TREE(tr, td);
    APPEND_TO_TREE(td, i);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_73()
{
    // #data
    // <h1><h2>
    // #errors
    // (1,4): expected-doctype-but-got-start-tag
    // (1,8): unexpected-start-tag
    // (1,8): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <h1>
    // |     <h2>

    unsigned char buffer[]  = "<h1><h2>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* h1          = dom_element_new(expected, html_tag_h1());
    dom_node_t* h2          = dom_element_new(expected, html_tag_h2());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, h1);
    APPEND_TO_TREE(body, h2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_74()
{
    // #data
    // <a><p><a></a></p></a>
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,9): unexpected-start-tag-implies-end-tag
    // (1,9): adoption-agency-1.3
    // (1,21): unexpected-end-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <a>
    // |     <p>
    // |       <a>
    // |       <a>

    unsigned char buffer[]  = "<a><p><a></a></p></a>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* a1          = dom_element_new(expected, html_tag_a());
    dom_node_t* p1          = dom_element_new(expected, html_tag_p());
    dom_node_t* a2          = dom_element_new(expected, html_tag_a());
    dom_node_t* a3          = dom_element_new(expected, html_tag_a());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, a1);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(p1, a2);
    APPEND_TO_TREE(p1, a3);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_75()
{
    // #data
    // <b><button></b></button></b>
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,15): adoption-agency-1.3
    // (1,28): unexpected-end-tag
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <b>
    // |     <button>
    // |       <b>

    unsigned char buffer[]  = "<b><button></b></button></b>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* button      = dom_element_new(expected, html_tag_button());
    dom_node_t* b2          = dom_element_new(expected, html_tag_b());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, b1);
    APPEND_TO_TREE(body, button);
    APPEND_TO_TREE(button, b2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_76()
{
    // #data
    // <p><b><div><marquee></p></b></div>
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,11): unexpected-end-tag
    // (1,24): unexpected-end-tag
    // (1,28): unexpected-end-tag
    // (1,34): end-tag-too-early
    // (1,34): expected-closing-tag-but-got-eof
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

    unsigned char buffer[]  = "<p><b><div><marquee></p></b></div>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* p1          = dom_element_new(expected, html_tag_p());
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* b2          = dom_element_new(expected, html_tag_b());
    dom_node_t* marquee     = dom_element_new(expected, html_tag_marquee());
    dom_node_t* p2          = dom_element_new(expected, html_tag_p());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(p1, b1);
    APPEND_TO_TREE(div, b2);
    APPEND_TO_TREE(b2, marquee);
    APPEND_TO_TREE(marquee, p2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_77()
{
    // #data
    // <script></script></div><title></title><p><p>
    // #errors
    // (1,8): expected-doctype-but-got-start-tag
    // (1,23): unexpected-end-tag
    // #document
    // | <html>
    // |   <head>
    // |     <script>
    // |     <title>
    // |   <body>
    // |     <p>
    // |     <p>

    unsigned char buffer[]  = "<script></script></div><title></title><p><p>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* script      = dom_element_new(expected, html_tag_script());
    dom_node_t* title       = dom_element_new(expected, html_tag_title());
    dom_node_t* p1          = dom_element_new(expected, html_tag_p());
    dom_node_t* p2          = dom_element_new(expected, html_tag_p());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(head, script);
    APPEND_TO_TREE(head, title);
    APPEND_TO_TREE(body, p1);
    APPEND_TO_TREE(body, p2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_78()
{
    // #data
    // <select><b><option><select><option></b></select>
    // #errors
    // 1:1: ERROR: Expected a doctype token
    // 1:20: ERROR: Start tag 'select' isn't allowed here. Currently open tags: html, body, select, b, option.
    // 1:36: ERROR: End tag 'b' isn't allowed here. Currently open tags: html, body, b, select, option.
    // 1:49: ERROR: Premature end of file. Currently open tags: html, body, b.
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <select>
    // |       <b>
    // |         <option>
    // |     <b>
    // |       <option>

    unsigned char buffer[]  = "<select><b><option><select><option></b></select>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* select      = dom_element_new(expected, html_tag_select());
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* b2          = dom_element_new(expected, html_tag_b());
    dom_node_t* o1          = dom_element_new(expected, html_tag_option());
    dom_node_t* o2          = dom_element_new(expected, html_tag_option());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, select);
    APPEND_TO_TREE(body, b2);
    APPEND_TO_TREE(select, b1);
    APPEND_TO_TREE(b1, o1);
    APPEND_TO_TREE(b2, o2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_79()
{
    // #data
    // <html><head><title></title><body></body></html>
    // #errors
    // (1,6): expected-doctype-but-got-start-tag
    // #document
    // | <html>
    // |   <head>
    // |     <title>
    // |   <body>

    unsigned char buffer[]  = "<html><head><title></title><body></body></html>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* title       = dom_element_new(expected, html_tag_title());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(head, title);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_80()
{
    // #data
    // <a><table><td><a><table></table><a></tr><a></table><a>
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,14): unexpected-cell-in-table-body
    // (1,35): unexpected-start-tag-implies-end-tag
    // (1,40): unexpected-cell-end-tag
    // (1,43): unexpected-start-tag-implies-table-voodoo
    // (1,43): unexpected-start-tag-implies-end-tag
    // (1,43): unexpected-end-tag
    // (1,54): unexpected-start-tag-implies-end-tag
    // (1,54): adoption-agency-1.2
    // (1,54): expected-closing-tag-but-got-eof
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

    unsigned char buffer[]  = "<a><table><td><a><table></table><a></tr><a></table><a>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* a1          = dom_element_new(expected, html_tag_a());
    dom_node_t* a2          = dom_element_new(expected, html_tag_a());
    dom_node_t* a3          = dom_element_new(expected, html_tag_a());
    dom_node_t* a4          = dom_element_new(expected, html_tag_a());
    dom_node_t* a5          = dom_element_new(expected, html_tag_a());
    dom_node_t* table1      = dom_element_new(expected, html_tag_table());
    dom_node_t* tbody       = dom_element_new(expected, html_tag_tbody());
    dom_node_t* tr          = dom_element_new(expected, html_tag_tr());
    dom_node_t* td          = dom_element_new(expected, html_tag_td());
    dom_node_t* table2      = dom_element_new(expected, html_tag_table());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, a1);
    APPEND_TO_TREE(body, a5);
    APPEND_TO_TREE(a1, a2);
    APPEND_TO_TREE(a1, table1);
    APPEND_TO_TREE(table1, tbody);
    APPEND_TO_TREE(tbody, tr);
    APPEND_TO_TREE(tr, td);
    APPEND_TO_TREE(td, a3);
    APPEND_TO_TREE(a3, table2);
    APPEND_TO_TREE(td, a4);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}

void test_html_parser_test3()
{
    TEST_CASE(test_parser_55);
    TEST_CASE(test_parser_56);
    TEST_CASE(test_parser_57);
    TEST_CASE(test_parser_58);
    TEST_CASE(test_parser_59);
    TEST_CASE(test_parser_60);
    TEST_CASE(test_parser_61);
    TEST_CASE(test_parser_62);
    TEST_CASE(test_parser_63);
    TEST_CASE(test_parser_64);
    TEST_CASE(test_parser_65);
    TEST_CASE(test_parser_66);
    TEST_CASE(test_parser_67);
    TEST_CASE(test_parser_68);
    TEST_CASE(test_parser_69);
    TEST_CASE(test_parser_70);
    TEST_CASE(test_parser_71);
    TEST_CASE(test_parser_72);
    TEST_CASE(test_parser_73);
    TEST_CASE(test_parser_74);
    TEST_CASE(test_parser_75);
    TEST_CASE(test_parser_76);
    TEST_CASE(test_parser_77);
    TEST_CASE(test_parser_78);
    TEST_CASE(test_parser_79);
    TEST_CASE(test_parser_80);
}