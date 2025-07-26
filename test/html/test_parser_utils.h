#pragma once

#include <stdint.h>

#include "test_utils.h"
#include "dom/test_node_utils.h"

#include "html/parser.h"


void print_document_tree(html_node_t* node, uint32_t level);


#define RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected)                                  \
do                                                                                      \
{                                                                                       \
    html_parser_init();                                                                 \
    html_node_t* actual = html_parser_run(buffer, sizeof(buffer) - 1);                  \
    ASSERT_NODE(actual, expected);                                                      \
    html_node_free(expected);                                                           \
    html_node_free(actual);                                                             \
    html_parser_free();                                                                 \
} while (0);
