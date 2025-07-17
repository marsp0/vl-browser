#pragma once

#include "test_utils.h"

#include "html/parser.h"


void assert_node(html_node_t* a, html_node_t* e);


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


#define RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected)                                  \
do                                                                                      \
{                                                                                       \
    html_parser_initialize();                                                           \
    html_node_t* actual = html_parser_run(buffer, sizeof(buffer) - 1);                  \
    assert_node(actual, expected);                                                      \
    html_parser_free();                                                                 \
} while (0);
