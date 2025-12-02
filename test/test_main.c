#include "test_utils.h"

#include "util/test_utf8.h"
#include "dom/test_hash_str.h"
#include "dom/test_node.h"

#include "html/parser/runner.h"
#include "html/tokenizer/runner.h"
#include "css/tokenizer/runner.h"

#include "dom/hash_str.h"
#include "global_modules.h"

int32_t main()
{
    TESTS_INIT();

    global_modules_init();

    TEST_GROUP(test_utf8);
    TEST_GROUP(test_dom_hash_string);
    TEST_GROUP(test_html_node);

    // external tests
    TEST_GROUP(html_tokenizer_test);
    TEST_GROUP(html_parser_test);
    TEST_GROUP(css_tokenizer_test);
    // before css - 8056

    TESTS_SUMMARY();

    int32_t exit_code = TESTS_FAIL_COUNT() > 0 ? 1 : 0;

    global_modules_free();

    return exit_code;
}