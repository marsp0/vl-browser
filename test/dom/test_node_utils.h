#pragma once

#include "test_utils.h"

#include "dom/document.h"
#include "dom/doctype.h"
#include "dom/text.h"
#include "dom/element.h"

void               ASSERT_NODE(html_node_t* a, html_node_t* e);

html_node_t        create_base_node(html_node_type_e type);
html_document_t    create_document_node(bool parser_cannot_change_mode);
html_element_t     create_element_node(unsigned char* name, uint32_t name_size);
html_doctype_t     create_doctype_node(unsigned char* name, uint32_t name_size, 
                                       unsigned char* public_id, uint32_t public_id_size, 
                                       unsigned char* system_id, uint32_t system_id_size);


#define APPEND_TO_TREE(root, node)                                                      \
do                                                                                      \
{                                                                                       \
    html_node_t* last_child = root->last_child;                                         \
    if (last_child)                                                                     \
    {                                                                                   \
        last_child->next_sibling = node;                                                \
        node->prev_sibling = last_child;                                                \
        root->last_child = node;                                                        \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        root->first_child = node;                                                       \
        root->last_child = node;                                                        \
    }                                                                                   \
} while(0);


#define ASSERT_NODE_DOCTYPE(a, e)                                                       \
do                                                                                      \
{                                                                                       \
    ASSERT_EQUAL(a->name_size, e->name_size);                                           \
    ASSERT_STRING((char)a->name, (char)e->name, a->name_size);                          \
                                                                                        \
    ASSERT_EQUAL(a->public_id_size, e->public_id_size);                                 \
    ASSERT_STRING((char)a->public_id, (char)e->public_id, a->public_id_size);           \
                                                                                        \
    ASSERT_EQUAL(a->system_id_size, e->system_id_size);                                 \
    ASSERT_STRING((char)a->system_id, (char)e->system_id, a->system_id_size);           \
}while (0);
