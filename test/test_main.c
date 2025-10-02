#include "test_utils.h"

#include "util/test_utf8.h"
#include "html/test_tokenizer.h"
#include "dom/test_hash_str.h"
#include "dom/test_node.h"

#include "html/parser/runner.h"
#include "html/tokenizer/runner.h"

#include "dom/hash_str.h"
#include "util/hash_str_constants.h"

int32_t main()
{
    TESTS_INIT();

    hash_str_pool_new();
    hash_str_constants_initialize();

    TEST_GROUP(test_utf8);
    TEST_GROUP(test_html_tokenizer_test1);
    TEST_GROUP(test_html_tokenizer_test2);
    TEST_GROUP(test_html_tokenizer_test3);
    TEST_GROUP(test_html_tokenizer_rcdata);
    TEST_GROUP(test_html_tokenizer_rawtext);
    TEST_GROUP(test_html_tokenizer_script_data);
    TEST_GROUP(test_dom_hash_string);
    TEST_GROUP(test_html_node);

    // external tests
    TEST_GROUP(html_tokenizer_test);
    TEST_GROUP(html_parser_test);

    TESTS_SUMMARY();

    int32_t exit_code = TESTS_FAIL_COUNT() > 0 ? 1 : 0;

    hash_str_pool_free();

    return exit_code;
}