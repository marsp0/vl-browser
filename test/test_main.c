#include "test_utils.h"

#include "util/test_utf8.h"
#include "html/test_tokenizer.h"
#include "html/test_parser.h"
#include "dom/test_node.h"

int32_t main()
{
    TESTS_INIT();

    TEST_GROUP(test_utf8);
    // TEST_GROUP(test_html_tokenizer_test1);
    // TEST_GROUP(test_html_tokenizer_test2);
    // TEST_GROUP(test_html_tokenizer_test3);
    // TEST_GROUP(test_html_tokenizer_rcdata);
    // TEST_GROUP(test_html_tokenizer_rawtext);
    // TEST_GROUP(test_html_tokenizer_script_data);
    TEST_GROUP(test_html_node);
    TEST_GROUP(test_html_parser_test1);

    TESTS_SUMMARY();

    int32_t exit_code = TESTS_FAIL_COUNT() > 0 ? 1 : 0;

    return exit_code;
}