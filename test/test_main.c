#include "test_utils.h"

#include "util/test_utf8.h"
#include "html/test_tokenizer.h"
#include "core/test_string.h"

int32_t main()
{
    TESTS_INIT();

    TEST_GROUP(test_utf8);
    TEST_GROUP(test_html_tokenizer_test1);
    TEST_GROUP(test_html_tokenizer_test2);
    TEST_GROUP(test_html_tokenizer_test3);
    TEST_GROUP(test_html_tokenizer_rcdata);
    TEST_GROUP(test_html_tokenizer_rawtext);
    TEST_GROUP(test_html_tokenizer_script_data);
    TEST_GROUP(test_string);

    TESTS_SUMMARY();

    int32_t exit_code = TESTS_FAIL_COUNT() > 0 ? 1 : 0;

    return exit_code;
}