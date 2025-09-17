#pragma once

#include <stdint.h>

#include "test_utils.h"
#include "dom/test_node_utils.h"

#include "html/parser.h"


void print_document_tree(dom_node_t* node, uint32_t level);


#define RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected, print)                           \
do                                                                                      \
{                                                                                       \
    html_parser_init();                                                                 \
    dom_node_t* actual = html_parser_run(buffer, sizeof(buffer) - 1);                   \
    ASSERT_NODE(actual, expected);                                                      \
    if (print)                                                                          \
    {                                                                                   \
        print_document_tree(actual, 0);                                                 \
        print_document_tree(expected, 0);                                               \
    }                                                                                   \
    dom_node_free(expected);                                                            \
    dom_node_free(actual);                                                              \
    html_parser_free();                                                                 \
} while (0);
