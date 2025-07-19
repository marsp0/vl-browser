#pragma once

#include "test_utils.h"
#include "dom/test_node_utils.h"

#include "html/parser.h"


#define RUN_TEST_AND_ASSERT_DOCUMENT(buffer, expected)                                  \
do                                                                                      \
{                                                                                       \
    html_parser_initialize();                                                           \
    html_node_t* actual = html_parser_run(buffer, sizeof(buffer) - 1);                  \
    ASSERT_NODE(actual, expected);                                                      \
    html_parser_free();                                                                 \
} while (0);
