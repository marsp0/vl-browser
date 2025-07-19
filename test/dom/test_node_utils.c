#include "test_node_utils.h"

#include <stdlib.h>

static void ASSERT_NODE_ELEMENT(html_element_t* a, html_element_t* e)
{
    ASSERT_EQUAL(a->namespace_size, e->namespace_size);
    ASSERT_STRING((char)a->namespace, (char)e->namespace, a->namespace_size);

    ASSERT_EQUAL(a->prefix_size, e->prefix_size);
    ASSERT_STRING((char)a->prefix, (char)e->prefix, a->prefix_size);

    ASSERT_EQUAL(a->local_name_size, e->local_name_size);
    ASSERT_STRING((char)a->local_name, (char)e->local_name, a->local_name_size);

    ASSERT_EQUAL(a->tag_name_size, e->tag_name_size);
    ASSERT_STRING((char)a->tag_name, (char)e->tag_name, a->tag_name_size);

    ASSERT_EQUAL(a->id_size, e->id_size);
    ASSERT_STRING((char)a->id, (char)e->id, a->id_size);

    ASSERT_EQUAL(a->class_name_size, e->class_name_size);
    ASSERT_STRING((char)a->class_name, (char)e->class_name, a->class_name_size);
}


static void ASSERT_NODE_TEXT(html_text_t* a, html_text_t* e)
{
    ASSERT_EQUAL(a->data_size, e->data_size);
    ASSERT_STRING((char)a->data, (char)e->data, a->data_size);
}


static void ASSERT_NODE_DOCUMENT(html_document_t* a, html_document_t* e)
{
    ASSERT_EQUAL(a->parser_cannot_change_mode, e->parser_cannot_change_mode);

    ASSERT_EQUAL(a->url_size, e->url_size);
    ASSERT_STRING((char)a->url, (char)e->url, a->url_size);

    ASSERT_EQUAL(a->uri_size, e->uri_size);
    ASSERT_STRING((char)a->uri, (char)e->uri, a->uri_size);

    ASSERT_EQUAL(a->compat_mode_size, e->compat_mode_size);
    ASSERT_STRING((char)a->compat_mode, (char)e->compat_mode, a->compat_mode_size);

    ASSERT_EQUAL(a->character_set_size, e->character_set_size);
    ASSERT_STRING((char)a->character_set, (char)e->character_set, a->character_set_size);

    ASSERT_EQUAL(a->content_type_size, e->content_type_size);
    ASSERT_STRING((char)a->content_type, (char)e->content_type, a->content_type_size);

    // ASSERT_NODE_DOCTYPE(a->doctype, e->doctype);
}


html_node_t create_base_node(html_node_type_e type)
{
    html_node_t node;
    node.type = type;
    return node;
}


html_doctype_t create_doctype_node(unsigned char* name, uint32_t name_size, 
                                   unsigned char* public_id, uint32_t public_id_size, 
                                   unsigned char* system_id, uint32_t system_id_size)
{
    html_doctype_t node;

    node.name_size = name_size;
    memcpy(node.name, name, name_size);

    node.public_id_size = public_id_size;
    memcpy(node.public_id, public_id, public_id_size);

    node.system_id_size = system_id_size;
    memcpy(node.system_id, system_id, system_id_size);

    return node;
}


html_document_t create_document_node(bool parser_cannot_change_mode)
{
    html_document_t node;

    node.url_size = 0;
    node.uri_size = 0;
    node.compat_mode_size = 0;
    node.character_set_size = 0;
    node.content_type_size = 0;
    node.parser_cannot_change_mode = parser_cannot_change_mode;

    return node;
}


html_element_t create_element_node(unsigned char* name, uint32_t name_size)
{
    html_element_t node;

    node.local_name_size = name_size;
    memcpy(node.local_name, name, name_size);

    return node;
}


void ASSERT_NODE(html_node_t* a, html_node_t* e)
{
    ASSERT_EQUAL(a->type, e->type);
    ASSERT_EQUAL(a->name_size, e->name_size);
    ASSERT_STRING((char)a->name, (char)e->name, e->name_size);

    if (a->type == e->type && a->type == HTML_NODE_DOCUMENT)
    {
        html_document_t* a_doc = (html_document_t*)a->data;
        html_document_t* e_doc = (html_document_t*)e->data;
        ASSERT_NODE_DOCUMENT(a_doc, e_doc);
    }
    if (a->type == e->type && a->type == HTML_NODE_ELEMENT)
    {
        html_element_t* a_element = (html_element_t*)a->data;
        html_element_t* e_element = (html_element_t*)e->data;
        ASSERT_NODE_ELEMENT(a_element, e_element);
    }
    if (a->type == e->type && a->type == HTML_NODE_TEXT)
    {
        html_text_t* a_text = (html_text_t*)a->data;
        html_text_t* e_text = (html_text_t*)e->data;
        ASSERT_NODE_TEXT(a_text, e_text);
    }

    html_node_t* a_child = a->first_child;
    html_node_t* e_child = e->first_child;
    while (a_child || e_child)
    {
        ASSERT_NODE(a_child, e_child);

        if (a_child->type == HTML_NODE_DOCUMENT)
        {
            html_document_t* a_doc = (html_document_t*)a_child->data;
            html_document_t* e_doc = (html_document_t*)e_child->data;
            ASSERT_NODE_DOCUMENT(a_doc, e_doc);
        }
        if (a_child->type == HTML_NODE_ELEMENT)
        {
            html_element_t* a_element = (html_element_t*)a_child->data;
            html_element_t* e_element = (html_element_t*)e_child->data;
            ASSERT_NODE_ELEMENT(a_element, e_element);
        }
        if (a_child->type == HTML_NODE_TEXT)
        {
            html_text_t* a_text = (html_text_t*)a_child->data;
            html_text_t* e_text = (html_text_t*)e_child->data;
            ASSERT_NODE_TEXT(a_text, e_text);
        }

        a_child = a_child->next_sibling;
        e_child = e_child->next_sibling;
    }
}