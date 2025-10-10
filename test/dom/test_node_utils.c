#include "test_node_utils.h"

#include <stdlib.h>

void ASSERT_NODE_DOCTYPE(dom_doctype_t* a, dom_doctype_t* e)
{
    if (!a || !e)
    {
        ASSERT_POINTER(a, e);
    }
    else
    {
        ASSERT_HASH_STRING(a->name, e->name);
    }
}


void ASSERT_NODE_ELEMENT_ATTRIBUTES(dom_element_t* a, dom_element_t* e)
{
    ASSERT_EQUAL(a->attr_size, e->attr_size);
    if (a->attr_size == e->attr_size)
    {
        dom_attr_t* a_attr = a->attr;
        dom_attr_t* e_attr = e->attr;
        while(a_attr && e_attr)
        {
            ASSERT_HASH_STRING(a_attr->name, e_attr->name);
            ASSERT_HASH_STRING(a_attr->value, e_attr->value);
            a_attr = a_attr->next;
            e_attr = e_attr->next;
        }
    }
}


void ASSERT_NODE_TEXT(dom_text_t* a, dom_text_t* e)
{
    ASSERT_EQUAL(a->data_size, e->data_size);
    ASSERT_STRING((char)a->data, (char)e->data, a->data_size);
}


void ASSERT_NODE_DOCUMENT(dom_document_t* a, dom_document_t* e)
{
    ASSERT_EQUAL(a->parser_cannot_change_mode, e->parser_cannot_change_mode);

    ASSERT_HASH_STRING(a->url, e->url);
    ASSERT_HASH_STRING(a->uri, e->uri);
    ASSERT_HASH_STRING(a->compat_mode, e->compat_mode);
    ASSERT_HASH_STRING(a->character_set, e->character_set);
    ASSERT_HASH_STRING(a->content_type, e->content_type);
    ASSERT_NODE_DOCTYPE(a->doctype, e->doctype);
}


void ASSERT_NODE_COMMENT(dom_comment_t* a, dom_comment_t* e)
{
    ASSERT_EQUAL(a->data_size, e->data_size);
    ASSERT_STRING((char)a->data, (char)e->data, a->data_size);
}


void ASSERT_NODE_ELEMENT(dom_element_t* a, dom_element_t* e)
{
    ASSERT_HASH_STRING(a->namespace, e->namespace);
    ASSERT_HASH_STRING(a->prefix, e->prefix);
    ASSERT_HASH_STRING(a->local_name, e->local_name);
    ASSERT_HASH_STRING(a->tag_name, e->tag_name);
    ASSERT_HASH_STRING(a->id, e->id);
    ASSERT_HASH_STRING(a->class_name, e->class_name);
    ASSERT_NODE_ELEMENT_ATTRIBUTES(a, e);
}


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
