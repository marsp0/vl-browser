#include "test_node_utils.h"

#include <stdlib.h>


void ASSERT_NODE(html_node_t* a, html_node_t* e)
{
    if (!a || !e)
    {
        if (!a && !e) { return; }

        ASSERT_POINTER(a, e);
        return;
    }

    html_node_type_e e_type         = e->type;
    html_node_type_e a_type         = a->type;
    const unsigned char* e_name     = e->name;
    const unsigned char* a_name     = a->name;
    const uint32_t e_name_size      = e->name_size;
    const uint32_t a_name_size      = a->name_size;

    ASSERT_EQUAL(a_type, e_type);
    ASSERT_EQUAL(a_name_size, e_name_size);
    ASSERT_STRING((char)a_name, (char)e_name, e_name_size);

    if (a_type == e_type && a_type == HTML_NODE_DOCUMENT)
    {
        html_document_t* a_doc = html_document_from_node(a);
        html_document_t* e_doc = html_document_from_node(e);
        ASSERT_NODE_DOCUMENT(a_doc, e_doc);
    }
    if (a_type == e_type && a_type == HTML_NODE_ELEMENT)
    {
        html_element_t* a_data = html_element_from_node(a);
        html_element_t* e_data = html_element_from_node(e);
        ASSERT_NODE_ELEMENT(a_data, e_data);
    }
    if (a_type == e_type && a_type == HTML_NODE_TEXT)
    {
        html_text_t* a_data = html_text_from_node(a);
        html_text_t* e_data = html_text_from_node(e);
        ASSERT_NODE_TEXT(a_data, e_data);
    }
    if (a_type == e_type && a_type == HTML_NODE_COMMENT)
    {
        html_comment_t* a_data = html_comment_from_node(a);
        html_comment_t* e_data = html_comment_from_node(e);
        ASSERT_NODE_COMMENT(a_data, e_data);
    }

    html_node_t* a_child = a->first_child;
    html_node_t* e_child = e->first_child;

    while (a_child || e_child)
    {
        ASSERT_NODE(a_child, e_child);

        if (!a_child || !e_child) { break; }

        a_child = a_child->next_sibling;
        e_child = e_child->next_sibling;
    }
}

