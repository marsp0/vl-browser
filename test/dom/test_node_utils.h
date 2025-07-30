#pragma once

#include "test_utils.h"

#include "dom/document.h"
#include "dom/doctype.h"
#include "dom/text.h"
#include "dom/element.h"
#include "dom/comment.h"

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
} while (0);


#define ASSERT_NODE_ELEMENT(a, e)                                                       \
do                                                                                      \
{                                                                                       \
    ASSERT_EQUAL(a->namespace_size, e->namespace_size);                                 \
    ASSERT_STRING((char)a->namespace, (char)e->namespace, a->namespace_size);           \
                                                                                        \
    ASSERT_EQUAL(a->prefix_size, e->prefix_size);                                       \
    ASSERT_STRING((char)a->prefix, (char)e->prefix, a->prefix_size);                    \
                                                                                        \
    ASSERT_EQUAL(a->local_name_size, e->local_name_size);                               \
    ASSERT_STRING((char)a->local_name, (char)e->local_name, a->local_name_size);        \
                                                                                        \
    ASSERT_EQUAL(a->tag_name_size, e->tag_name_size);                                   \
    ASSERT_STRING((char)a->tag_name, (char)e->tag_name, a->tag_name_size);              \
                                                                                        \
    ASSERT_EQUAL(a->id_size, e->id_size);                                               \
    ASSERT_STRING((char)a->id, (char)e->id, a->id_size);                                \
                                                                                        \
    ASSERT_EQUAL(a->class_name_size, e->class_name_size);                               \
    ASSERT_STRING((char)a->class_name, (char)e->class_name, a->class_name_size);        \
} while(0);


#define ASSERT_NODE_TEXT(a, e)                                                          \
do                                                                                      \
{                                                                                       \
    ASSERT_EQUAL(a->data_size, e->data_size);                                           \
    ASSERT_STRING((char)a->data, (char)e->data, a->data_size);                          \
} while (0);


#define ASSERT_NODE_DOCUMENT(a, e)                                                      \
do                                                                                      \
{                                                                                       \
    ASSERT_EQUAL(a->parser_cannot_change_mode, e->parser_cannot_change_mode);           \
                                                                                        \
    ASSERT_EQUAL(a->url_size, e->url_size);                                             \
    ASSERT_STRING((char)a->url, (char)e->url, a->url_size);                             \
                                                                                        \
    ASSERT_EQUAL(a->uri_size, e->uri_size);                                             \
    ASSERT_STRING((char)a->uri, (char)e->uri, a->uri_size);                             \
                                                                                        \
    ASSERT_EQUAL(a->compat_mode_size, e->compat_mode_size);                             \
    ASSERT_STRING((char)a->compat_mode, (char)e->compat_mode, a->compat_mode_size);     \
                                                                                        \
    ASSERT_EQUAL(a->character_set_size, e->character_set_size);                         \
    ASSERT_STRING((char)a->character_set, (char)e->character_set, a->character_set_size);\
                                                                                        \
    ASSERT_EQUAL(a->content_type_size, e->content_type_size);                           \
    ASSERT_STRING((char)a->content_type, (char)e->content_type, a->content_type_size);  \
} while (0);


#define ASSERT_NODE_COMMENT(a, e)                                                       \
do                                                                                      \
{                                                                                       \
    ASSERT_EQUAL(a->data_size, e->data_size);                                           \
    ASSERT_STRING((char)a->data, (char)e->data, a->data_size);                          \
} while (0);
