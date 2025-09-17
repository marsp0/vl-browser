#include "test_parser.h"

#include "test_parser_utils.h"

#include "html/tag_constants.h"

static void test_parser_37()
{
    // #data
    // <b>A<cite>B<div>C
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,17): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <b>
    // |       "A"
    // |       <cite>
    // |         "B"
    // |         <div>
    // |           "C"

    unsigned char buffer[]  = "<b>A<cite>B<div>C";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* cite        = dom_element_new(expected, html_tag_cite());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* t1          = dom_text_new(expected, "A", 1);
    dom_node_t* t2          = dom_text_new(expected, "B", 1);
    dom_node_t* t3          = dom_text_new(expected, "C", 1);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, b1);
    APPEND_TO_TREE(b1, t1);
    APPEND_TO_TREE(b1, cite);
    APPEND_TO_TREE(cite, t2);
    APPEND_TO_TREE(cite, div);
    APPEND_TO_TREE(div, t3);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_38()
{
    // #data
    // <b>A<cite>B<div>C</cite>D
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,24): unexpected-end-tag
    // (1,25): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <b>
    // |       "A"
    // |       <cite>
    // |         "B"
    // |         <div>
    // |           "CD"

    unsigned char buffer[]  = "<b>A<cite>B<div>C</cite>D";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* cite        = dom_element_new(expected, html_tag_cite());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* t1          = dom_text_new(expected, "A", 1);
    dom_node_t* t2          = dom_text_new(expected, "B", 1);
    dom_node_t* t3          = dom_text_new(expected, "CD", 2);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, b1);
    APPEND_TO_TREE(b1, t1);
    APPEND_TO_TREE(b1, cite);
    APPEND_TO_TREE(cite, t2);
    APPEND_TO_TREE(cite, div);
    APPEND_TO_TREE(div, t3);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_39()
{
    // #data
    // <b>A<cite>B<div>C</b>D
    // #errors
    // (1,3): expected-doctype-but-got-start-tag
    // (1,21): adoption-agency-1.3
    // (1,22): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <b>
    // |       "A"
    // |       <cite>
    // |         "B"
    // |     <div>
    // |       <b>
    // |         "C"
    // |       "D"

    unsigned char buffer[]  = "<b>A<cite>B<div>C</b>D";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* cite        = dom_element_new(expected, html_tag_cite());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* b2          = dom_element_new(expected, html_tag_b());
    dom_node_t* t1          = dom_text_new(expected, "A", 1);
    dom_node_t* t2          = dom_text_new(expected, "B", 1);
    dom_node_t* t3          = dom_text_new(expected, "C", 1);
    dom_node_t* t4          = dom_text_new(expected, "D", 1);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, b1);
    APPEND_TO_TREE(b1, t1);
    APPEND_TO_TREE(b1, cite);
    APPEND_TO_TREE(cite, t2);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, b2);
    APPEND_TO_TREE(b2, t3);
    APPEND_TO_TREE(div, t4);


    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_40()
{
    // #data
    //
    // #errors
    // (1,0): expected-doctype-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>

    unsigned char buffer[]  = "";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_41()
{
    // #data
    // <DIV>
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,5): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <div>

    unsigned char buffer[]  = "<DIV>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_42()
{
    // #data
    // <DIV> abc
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,9): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <div>
    // |       " abc"

    unsigned char buffer[]  = "<DIV> abc";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* t1          = dom_text_new(expected, " abc", 4);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, t1);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_43()
{
    // #data
    // <DIV> abc <B>
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,13): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <div>
    // |       " abc "
    // |       <b>

    unsigned char buffer[]  = "<DIV> abc <B>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* t1          = dom_text_new(expected, " abc ", 5);
    dom_node_t* b           = dom_element_new(expected, html_tag_b());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, t1);
    APPEND_TO_TREE(div, b);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_44()
{
    // #data
    // <DIV> abc <B> def
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,17): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <div>
    // |       " abc "
    // |       <b>
    // |         " def"

    unsigned char buffer[]  = "<DIV> abc <B> def";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* t1          = dom_text_new(expected, " abc ", 5);
    dom_node_t* b           = dom_element_new(expected, html_tag_b());
    dom_node_t* t2          = dom_text_new(expected, " def", 4);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, t1);
    APPEND_TO_TREE(div, b);
    APPEND_TO_TREE(b, t2);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_45()
{
    // #data
    // <DIV> abc <B> def <I>
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,21): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <div>
    // |       " abc "
    // |       <b>
    // |         " def "
    // |         <i>

    unsigned char buffer[]  = "<DIV> abc <B> def <I>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* t1          = dom_text_new(expected, " abc ", 5);
    dom_node_t* b           = dom_element_new(expected, html_tag_b());
    dom_node_t* t2          = dom_text_new(expected, " def ", 5);
    dom_node_t* i           = dom_element_new(expected, html_tag_i());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, t1);
    APPEND_TO_TREE(div, b);
    APPEND_TO_TREE(b, t2);
    APPEND_TO_TREE(b, i);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_46()
{
    // #data
    // <DIV> abc <B> def <I> ghi
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,25): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <div>
    // |       " abc "
    // |       <b>
    // |         " def "
    // |         <i>
    // |           " ghi"

    unsigned char buffer[]  = "<DIV> abc <B> def <I> ghi";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* t1          = dom_text_new(expected, " abc ", 5);
    dom_node_t* b           = dom_element_new(expected, html_tag_b());
    dom_node_t* t2          = dom_text_new(expected, " def ", 5);
    dom_node_t* i           = dom_element_new(expected, html_tag_i());
    dom_node_t* t3          = dom_text_new(expected, " ghi", 4);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, t1);
    APPEND_TO_TREE(div, b);
    APPEND_TO_TREE(b, t2);
    APPEND_TO_TREE(b, i);
    APPEND_TO_TREE(i, t3);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_47()
{
    // #data
    // <DIV> abc <B> def <I> ghi <P>
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,29): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <div>
    // |       " abc "
    // |       <b>
    // |         " def "
    // |         <i>
    // |           " ghi "
    // |           <p>

    unsigned char buffer[]  = "<DIV> abc <B> def <I> ghi <P>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* t1          = dom_text_new(expected, " abc ", 5);
    dom_node_t* b           = dom_element_new(expected, html_tag_b());
    dom_node_t* t2          = dom_text_new(expected, " def ", 5);
    dom_node_t* i           = dom_element_new(expected, html_tag_i());
    dom_node_t* t3          = dom_text_new(expected, " ghi ", 5);
    dom_node_t* p           = dom_element_new(expected, html_tag_p());

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, t1);
    APPEND_TO_TREE(div, b);
    APPEND_TO_TREE(b, t2);
    APPEND_TO_TREE(b, i);
    APPEND_TO_TREE(i, t3);
    APPEND_TO_TREE(i, p);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_48()
{
    // #data
    // <DIV> abc <B> def <I> ghi <P> jkl
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,33): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <div>
    // |       " abc "
    // |       <b>
    // |         " def "
    // |         <i>
    // |           " ghi "
    // |           <p>
    // |             " jkl"

    unsigned char buffer[]  = "<DIV> abc <B> def <I> ghi <P> jkl";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* t1          = dom_text_new(expected, " abc ", 5);
    dom_node_t* b           = dom_element_new(expected, html_tag_b());
    dom_node_t* t2          = dom_text_new(expected, " def ", 5);
    dom_node_t* i           = dom_element_new(expected, html_tag_i());
    dom_node_t* t3          = dom_text_new(expected, " ghi ", 5);
    dom_node_t* p           = dom_element_new(expected, html_tag_p());
    dom_node_t* t4          = dom_text_new(expected, " jkl", 4);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, t1);
    APPEND_TO_TREE(div, b);
    APPEND_TO_TREE(b, t2);
    APPEND_TO_TREE(b, i);
    APPEND_TO_TREE(i, t3);
    APPEND_TO_TREE(i, p);
    APPEND_TO_TREE(p, t4);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_49()
{
    // #data
    // <DIV> abc <B> def <I> ghi <P> jkl </B>
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,38): adoption-agency-1.3
    // (1,38): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <div>
    // |       " abc "
    // |       <b>
    // |         " def "
    // |         <i>
    // |           " ghi "
    // |       <i>
    // |         <p>
    // |           <b>
    // |             " jkl "

    unsigned char buffer[]  = "<DIV> abc <B> def <I> ghi <P> jkl </B>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* t1          = dom_text_new(expected, " abc ", 5);
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* b2          = dom_element_new(expected, html_tag_b());
    dom_node_t* t2          = dom_text_new(expected, " def ", 5);
    dom_node_t* i1          = dom_element_new(expected, html_tag_i());
    dom_node_t* i2          = dom_element_new(expected, html_tag_i());
    dom_node_t* t3          = dom_text_new(expected, " ghi ", 5);
    dom_node_t* p1          = dom_element_new(expected, html_tag_p());
    dom_node_t* t4          = dom_text_new(expected, " jkl ", 5);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, t1);
    APPEND_TO_TREE(div, b1);
    APPEND_TO_TREE(div, i2);
    APPEND_TO_TREE(b1, t2);
    APPEND_TO_TREE(b1, i1);
    APPEND_TO_TREE(i1, t3);
    APPEND_TO_TREE(i2, p1);
    APPEND_TO_TREE(p1, b2);
    APPEND_TO_TREE(b2, t4);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_50()
{
    // #data
    // <DIV> abc <B> def <I> ghi <P> jkl </B> mno
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,38): adoption-agency-1.3
    // (1,42): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <div>
    // |       " abc "
    // |       <b>
    // |         " def "
    // |         <i>
    // |           " ghi "
    // |       <i>
    // |         <p>
    // |           <b>
    // |             " jkl "
    // |           " mno"

    unsigned char buffer[]  = "<DIV> abc <B> def <I> ghi <P> jkl </B> mno";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* t1          = dom_text_new(expected, " abc ", 5);
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* b2          = dom_element_new(expected, html_tag_b());
    dom_node_t* t2          = dom_text_new(expected, " def ", 5);
    dom_node_t* i1          = dom_element_new(expected, html_tag_i());
    dom_node_t* i2          = dom_element_new(expected, html_tag_i());
    dom_node_t* t3          = dom_text_new(expected, " ghi ", 5);
    dom_node_t* p1          = dom_element_new(expected, html_tag_p());
    dom_node_t* t4          = dom_text_new(expected, " jkl ", 5);
    dom_node_t* t5          = dom_text_new(expected, " mno", 4);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, t1);
    APPEND_TO_TREE(div, b1);
    APPEND_TO_TREE(div, i2);
    APPEND_TO_TREE(b1, t2);
    APPEND_TO_TREE(b1, i1);
    APPEND_TO_TREE(i1, t3);
    APPEND_TO_TREE(i2, p1);
    APPEND_TO_TREE(p1, b2);
    APPEND_TO_TREE(b2, t4);
    APPEND_TO_TREE(p1, t5);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_51()
{
    // #data
    // <DIV> abc <B> def <I> ghi <P> jkl </B> mno </I>
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,38): adoption-agency-1.3
    // (1,47): adoption-agency-1.3
    // (1,47): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <div>
    // |       " abc "
    // |       <b>
    // |         " def "
    // |         <i>
    // |           " ghi "
    // |       <i>
    // |       <p>
    // |         <i>
    // |           <b>
    // |             " jkl "
    // |           " mno "

    unsigned char buffer[]  = "<DIV> abc <B> def <I> ghi <P> jkl </B> mno </I>";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* t1          = dom_text_new(expected, " abc ", 5);
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* b2          = dom_element_new(expected, html_tag_b());
    dom_node_t* t2          = dom_text_new(expected, " def ", 5);
    dom_node_t* i1          = dom_element_new(expected, html_tag_i());
    dom_node_t* i2          = dom_element_new(expected, html_tag_i());
    dom_node_t* i3          = dom_element_new(expected, html_tag_i());
    dom_node_t* t3          = dom_text_new(expected, " ghi ", 5);
    dom_node_t* p1          = dom_element_new(expected, html_tag_p());
    dom_node_t* t4          = dom_text_new(expected, " jkl ", 5);
    dom_node_t* t5          = dom_text_new(expected, " mno ", 5);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, t1);
    APPEND_TO_TREE(div, b1);
    APPEND_TO_TREE(b1, t2);
    APPEND_TO_TREE(b1, i1);
    APPEND_TO_TREE(i1, t3);
    APPEND_TO_TREE(div, i2);
    APPEND_TO_TREE(div, p1);
    APPEND_TO_TREE(p1, i3);
    APPEND_TO_TREE(i3, b2);
    APPEND_TO_TREE(b2, t4);
    APPEND_TO_TREE(i3, t5);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}


static void test_parser_52()
{
    // #data
    // <DIV> abc <B> def <I> ghi <P> jkl </B> mno </I> pqr
    // #errors
    // (1,5): expected-doctype-but-got-start-tag
    // (1,38): adoption-agency-1.3
    // (1,47): adoption-agency-1.3
    // (1,51): expected-closing-tag-but-got-eof
    // #document
    // | <html>
    // |   <head>
    // |   <body>
    // |     <div>
    // |       " abc "
    // |       <b>
    // |         " def "
    // |         <i>
    // |           " ghi "
    // |       <i>
    // |       <p>
    // |         <i>
    // |           <b>
    // |             " jkl "
    // |           " mno "
    // |         " pqr"

    unsigned char buffer[]  = "<DIV> abc <B> def <I> ghi <P> jkl </B> mno </I> pqr";
    dom_node_t* expected    = dom_document_new();
    dom_node_t* html        = dom_element_new(expected, html_tag_html());
    dom_node_t* head        = dom_element_new(expected, html_tag_head());
    dom_node_t* body        = dom_element_new(expected, html_tag_body());
    dom_node_t* div         = dom_element_new(expected, html_tag_div());
    dom_node_t* t1          = dom_text_new(expected, " abc ", 5);
    dom_node_t* b1          = dom_element_new(expected, html_tag_b());
    dom_node_t* b2          = dom_element_new(expected, html_tag_b());
    dom_node_t* t2          = dom_text_new(expected, " def ", 5);
    dom_node_t* i1          = dom_element_new(expected, html_tag_i());
    dom_node_t* i2          = dom_element_new(expected, html_tag_i());
    dom_node_t* i3          = dom_element_new(expected, html_tag_i());
    dom_node_t* t3          = dom_text_new(expected, " ghi ", 5);
    dom_node_t* p1          = dom_element_new(expected, html_tag_p());
    dom_node_t* t4          = dom_text_new(expected, " jkl ", 5);
    dom_node_t* t5          = dom_text_new(expected, " mno ", 5);
    dom_node_t* t6          = dom_text_new(expected, " pqr", 4);

    APPEND_TO_TREE(expected, html);
    APPEND_TO_TREE(html, head);
    APPEND_TO_TREE(html, body);
    APPEND_TO_TREE(body, div);
    APPEND_TO_TREE(div, t1);
    APPEND_TO_TREE(div, b1);
    APPEND_TO_TREE(b1, t2);
    APPEND_TO_TREE(b1, i1);
    APPEND_TO_TREE(i1, t3);
    APPEND_TO_TREE(div, i2);
    APPEND_TO_TREE(div, p1);
    APPEND_TO_TREE(p1, i3);
    APPEND_TO_TREE(i3, b2);
    APPEND_TO_TREE(b2, t4);
    APPEND_TO_TREE(i3, t5);
    APPEND_TO_TREE(p1, t6);

    RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, false);
}

void test_html_parser_test2()
{
    TEST_CASE(test_parser_37);
    TEST_CASE(test_parser_38);
    TEST_CASE(test_parser_39);
    TEST_CASE(test_parser_40);
    TEST_CASE(test_parser_41);
    TEST_CASE(test_parser_42);
    TEST_CASE(test_parser_43);
    TEST_CASE(test_parser_44);
    TEST_CASE(test_parser_45);
    TEST_CASE(test_parser_46);
    TEST_CASE(test_parser_47);
    TEST_CASE(test_parser_48);
    TEST_CASE(test_parser_49);
    TEST_CASE(test_parser_50);
    TEST_CASE(test_parser_51);
    TEST_CASE(test_parser_52);
}