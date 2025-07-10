#include "test_string.h"

#include "core/string.h"

static void test_create_string()
{
    unsigned char data[] = "test string";
    string_t actual = string_new(data, sizeof(data) - 1);

    string_t expected;
    expected.data = "test string";
    expected.size = sizeof("test string") - 1;
    expected.len = 11;

    ASSERT_STRING_TYPE(actual, expected);
}

static void test_string_compare()
{
    unsigned char data[] = "test string";
    string_t actual = string_new(data, sizeof(data) - 1);

    ASSERT_TRUE(string_compare(actual, actual));

    string_t other;
    other.data = "test";
    other.size = sizeof("test string") - 1;
    other.len = 4;

    ASSERT_FALSE(string_compare(actual, other));

    string_t another;
    another.data = "tesg";
    another.size = sizeof("test string") - 1;
    another.len = 4;

    ASSERT_FALSE(string_compare(actual, another));
}

void test_string()
{
    TEST_CASE(test_create_string);
    TEST_CASE(test_string_compare);
}