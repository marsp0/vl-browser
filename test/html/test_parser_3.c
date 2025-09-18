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
}