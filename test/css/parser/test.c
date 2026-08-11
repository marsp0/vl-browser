#include "test.h"

#include "test_utils.h"

#include "css/parser.h"

static void test_1()
{
    const unsigned char* buf = "@foo";

    // create expectations
    css_parser_init(buf, sizeof(buf));
}

void test_css_parser()
{
    TEST_CASE(test_1);
}