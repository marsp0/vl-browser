#include "test_utils.h"

#include "util/test_utf8.h"
#include "dom/test_hash_str.h"
#include "dom/test_node.h"

#include "html/parser/runner.h"
#include "html/tokenizer/runner.h"

#include "css/tokenizer/runner.h"
#include "css/parser/test_parser.h"
#include "css/parser/test_conversions.h"
#include "css/grammar/test_color.h"
#include "css/grammar/test_percentage.h"
#include "css/grammar/test_length.h"

#include "dom/hash_str.h"
#include "global_modules.h"

int32_t main()
{
    TESTS_INIT();

    global_modules_init();

    TEST_GROUP(test_utf8);
    TEST_GROUP(test_dom_hash_string);

    // HTML
    TEST_GROUP(test_html_node);
    TEST_GROUP(test_html_tokenizer);
    TEST_GROUP(test_html_parser);

    // CSS
    TEST_GROUP(test_css_tokenizer);
    TEST_GROUP(test_css_parser);
    TEST_GROUP(test_css_conversions);
    TEST_GROUP(test_css_value_color);
    TEST_GROUP(test_css_value_percentage);
    TEST_GROUP(test_css_value_length);

    TESTS_SUMMARY();

    int32_t exit_code = TESTS_FAIL_COUNT() > 0 ? 1 : 0;

    global_modules_free();

    return exit_code;
}