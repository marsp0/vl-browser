#include "test_node_utils.h"

#include <stdlib.h>


void ASSERT_NODE(html_node_t* a, html_node_t* e)
{
    if (!a || !e)
    {
        if (!a && !e) { return; }

        ASSERT_POINTER(e, a);
        return;
    }

    ASSERT_EQUAL(a->type, e->type);
    ASSERT_EQUAL(a->name_size, e->name_size);
    ASSERT_STRING((char)a->name, (char)e->name, e->name_size);
    printf("%s\n", a->name);

    if (a->type == e->type && a->type == HTML_NODE_DOCUMENT)
    {
        html_document_t* a_data = (html_document_t*)a->data;
        html_document_t* e_data = (html_document_t*)e->data;
        ASSERT_NODE_DOCUMENT(a_data, e_data);
    }
    if (a->type == e->type && a->type == HTML_NODE_ELEMENT)
    {
        html_element_t* a_data = (html_element_t*)a->data;
        html_element_t* e_data = (html_element_t*)e->data;
        ASSERT_NODE_ELEMENT(a_data, e_data);
    }
    if (a->type == e->type && a->type == HTML_NODE_TEXT)
    {
        html_text_t* a_data = (html_text_t*)a->data;
        html_text_t* e_data = (html_text_t*)e->data;
        ASSERT_NODE_TEXT(a_data, e_data);
    }

    html_node_t* a_child = a->first_child;
    html_node_t* e_child = e->first_child;

    while (a_child || e_child)
    {
        ASSERT_NODE(a_child, e_child);

        a_child = a_child->next_sibling;
        e_child = e_child->next_sibling;
    }
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
