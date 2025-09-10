#include "test_node_utils.h"

#include <stdlib.h>


void ASSERT_NODE(dom_node_t* a, dom_node_t* e)
{
    if (!a || !e)
    {
        if (!a && !e) { return; }

        ASSERT_POINTER(a, e);
        return;
    }

    uint64_t e_type = e->type;
    uint64_t a_type = a->type;

    ASSERT_HASH_STRING(a->name, e->name);

    if (a_type == e_type && dom_node_is_document(a))
    {
        dom_document_t* a_doc = dom_document_from_node(a);
        dom_document_t* e_doc = dom_document_from_node(e);
        ASSERT_NODE_DOCUMENT(a_doc, e_doc);
    }
    if (a_type == e_type && dom_node_is_element(a))
    {
        dom_element_t* a_data = dom_element_from_node(a);
        dom_element_t* e_data = dom_element_from_node(e);
        ASSERT_NODE_ELEMENT(a_data, e_data);
    }
    if (a_type == e_type && dom_node_is_text(a))
    {
        dom_text_t* a_data = dom_text_from_node(a);
        dom_text_t* e_data = dom_text_from_node(e);
        ASSERT_NODE_TEXT(a_data, e_data);
    }
    if (a_type == e_type && dom_node_is_comment(a))
    {
        dom_comment_t* a_data = dom_comment_from_node(a);
        dom_comment_t* e_data = dom_comment_from_node(e);
        ASSERT_NODE_COMMENT(a_data, e_data);
    }

    dom_node_t* a_child = a->first;
    dom_node_t* e_child = e->first;

    while (a_child || e_child)
    {
        ASSERT_NODE(a_child, e_child);

        if (!a_child || !e_child) { break; }

        a_child = a_child->next;
        e_child = e_child->next;
    }
}

