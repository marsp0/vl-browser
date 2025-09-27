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

    ASSERT_EQUAL(a->type, e->type);

    if (a->type != e->type) { return; }

    ASSERT_HASH_STRING(a->name, e->name);

    if (dom_node_is_document(a))    { ASSERT_NODE_DOCUMENT(dom_document_from_node(a),   dom_document_from_node(e)); }
    if (dom_node_is_element(a))     { ASSERT_NODE_ELEMENT(dom_element_from_node(a),     dom_element_from_node(e)); }
    if (dom_node_is_text(a))        { ASSERT_NODE_TEXT(dom_text_from_node(a),           dom_text_from_node(e)); }
    if (dom_node_is_comment(a))     { ASSERT_NODE_COMMENT(dom_comment_from_node(a),     dom_comment_from_node(e)); }

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
