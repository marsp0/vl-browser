#pragma once

#include "test_utils.h"

#include "dom/document.h"
#include "dom/doctype.h"
#include "dom/text.h"
#include "dom/element.h"
#include "dom/comment.h"

void               ASSERT_NODE(dom_node_t* a, dom_node_t* e);


#define APPEND_TO_TREE(root, node)                                                      \
do                                                                                      \
{                                                                                       \
    dom_node_t* last_child = root->last_child;                                          \
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
    ASSERT_HASH_STRING(a->name, e->name);                                               \
    ASSERT_HASH_STRING(a->public_id, e->public_id);                                     \
    ASSERT_HASH_STRING(a->system_id, e->system_id);                                     \
} while (0);


#define ASSERT_NODE_ELEMENT(a, e)                                                       \
do                                                                                      \
{                                                                                       \
    ASSERT_HASH_STRING(a->namespace, e->namespace);                                     \
    ASSERT_HASH_STRING(a->prefix, e->prefix);                                           \
    ASSERT_HASH_STRING(a->local_name, e->local_name);                                   \
    ASSERT_HASH_STRING(a->tag_name, e->tag_name);                                       \
    ASSERT_HASH_STRING(a->id, e->id);                                                   \
    ASSERT_HASH_STRING(a->class_name, e->class_name);                                   \
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
    ASSERT_HASH_STRING(a->url, e->url);                                                 \
    ASSERT_HASH_STRING(a->uri, e->uri);                                                 \
    ASSERT_HASH_STRING(a->compat_mode, e->compat_mode);                                 \
    ASSERT_HASH_STRING(a->character_set, e->character_set);                             \
    ASSERT_HASH_STRING(a->content_type, e->content_type);                               \
} while (0);


#define ASSERT_NODE_COMMENT(a, e)                                                       \
do                                                                                      \
{                                                                                       \
    ASSERT_EQUAL(a->data_size, e->data_size);                                           \
    ASSERT_STRING((char)a->data, (char)e->data, a->data_size);                          \
} while (0);
