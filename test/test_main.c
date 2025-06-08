#include "test_utils.h"

#include "util/test_utf8.h"
#include "html/test_tokenizer.h"

int32_t main()
{
    TESTS_INIT();

    TEST_GROUP(test_utf8);
    TEST_GROUP(test_html_tokenizer);

    TESTS_SUMMARY();

    int32_t exit_code = TESTS_FAIL_COUNT() > 0 ? 1 : 0;

    return exit_code;
}