#include "test_hash_str.h"

#include "test_utils.h"

#include "dom/hash_str.h"

static void test_when_adding_same_string_we_get_same_instance()
{
    hash_str_t a = hash_str_new("test", 4);
    hash_str_t b = hash_str_new("test", 4);

    const unsigned char* a_data = hash_str_get(a);
    const uint32_t a_size = hash_str_get_size(a);

    const unsigned char* b_data = hash_str_get(b);
    const uint32_t b_size = hash_str_get_size(b);

    ASSERT_EQUAL(a_size, b_size);
    ASSERT_STRING((char)a_data, (char)b_data, a_size);
    ASSERT_POINTER((void*)a_data, (void*)b_data);
}


static void test_when_adding_diff_strings_we_get_diff_instances()
{
    hash_str_t a = hash_str_new("test", 4);
    hash_str_t b = hash_str_new("div", 3);

    const unsigned char* a_data = hash_str_get(a);
    const uint32_t a_size = hash_str_get_size(a);

    const unsigned char* b_data = hash_str_get(b);
    const uint32_t b_size = hash_str_get_size(b);

    ASSERT_EQUAL(a_size, 4);
    ASSERT_STRING((char)a_data, "test", 4);

    ASSERT_EQUAL(b_size, 3);
    ASSERT_STRING((char)b_data, "div", 3);
}


void test_dom_hash_string()
{
    TEST_CASE(test_when_adding_same_string_we_get_same_instance);
    TEST_CASE(test_when_adding_diff_strings_we_get_diff_instances);
}