#include "test_utf8.h"

#include "test_utils.h"

#include <stdio.h>

#include "util/utf8.h"

static void test_when_buffer_is_valid_then_return_normally()
{
    char buffer[] = {   (char)0x41, (char)0x42,                             // ascii
                        (char)0xd5, (char)0xa0,                             // 2 byte
                        (char)0xe2, (char)0xb7, (char)0xb8,                 // 3 byte
                        (char)0x7e,                                         // ascii
                        (char)0xf4, (char)0x80, (char)0xaf, (char)0xb9 };   // 4 byte

    bool is_valid = utf8_validate(buffer, sizeof(buffer));
    ASSERT_TRUE(is_valid);
}

static void test_when_buffer_contains_invalid_chars_then_raise()
{
    char buffer[] = {   (char)0x41, (char)0x42,                             // ascii
                        (char)0xd5, (char)0xa0,                             // 2 byte
                        (char)0xe2, (char)0xb7, (char)0xb8,                 // 3 byte
                        (char)0x7e,                                         // ascii
                        (char)0xc0,                                         // invalid byte
                        (char)0xf4, (char)0x80, (char)0xaf, (char)0xb9 };   // 4 byte

    bool is_valid = utf8_validate(buffer, sizeof(buffer));
    ASSERT_FALSE(is_valid);
}

static void test_when_buffer_contains_continuation_byte_at_start_then_raise()
{
    char buffer[] = {   (char)0x41, (char)0x42,                             // ascii
                        (char)0xd5, (char)0xa0,                             // 2 byte
                        (char)0x9c,                                         // continuation byte
                        (char)0xe2, (char)0xb7, (char)0xb8,                 // 3 byte
                        (char)0x7e,                                         // ascii
                        (char)0xf4, (char)0x80, (char)0xaf, (char)0xb9 };   // 4 byte

    bool is_valid = utf8_validate(buffer, sizeof(buffer));
    ASSERT_FALSE(is_valid);
}

static void test_when_buffer_contains_non_cont_byte_before_end_then_raise()
{
    char buffer[] = {   (char)0x41, (char)0x42,                             // ascii
                        (char)0xd5, (char)0xa0,                             // 2 byte
                        (char)0xe2, (char)0xb7, (char)0x42,                 // 3 byte - 2 valid + non continuation
                        (char)0x7e,                                         // ascii
                        (char)0xf4, (char)0x80, (char)0xaf, (char)0xb9 };   // 4 byte

    bool is_valid = utf8_validate(buffer, sizeof(buffer));
    ASSERT_FALSE(is_valid);
}

static void test_when_buffer_contains_overlong_encoding_then_raise()
{
    char buffer[] = {   (char)0x41, (char)0x42,                             // ascii
                        (char)0xd5, (char)0xa0,                             // 2 byte
                        (char)0xe0, (char)0x8b, (char)0xb8,                 // overlong encoding
                        (char)0x7e,                                         // ascii
                        (char)0xf4, (char)0x80, (char)0xaf, (char)0xb9 };   // 4 byte

    bool is_valid = utf8_validate(buffer, sizeof(buffer));
    ASSERT_FALSE(is_valid);
}

static void test_when_buffer_contains_value_greater_than_max_allowed_then_raise()
{
    char buffer[] = {   (char)0x41, (char)0x42,                             // ascii
                        (char)0xd5, (char)0xa0,                             // 2 byte
                        (char)0xe2, (char)0xb7, (char)0xb8,                 // 3 byte
                        (char)0x7e,                                         // ascii
                        (char)0xf4, (char)0x90, (char)0xaf, (char)0xb9 };   // 4 byte

    bool is_valid = utf8_validate(buffer, sizeof(buffer));
    ASSERT_FALSE(is_valid);
}

void test_utf8()
{
    TEST_CASE(test_when_buffer_is_valid_then_return_normally);
    TEST_CASE(test_when_buffer_contains_invalid_chars_then_raise);
    TEST_CASE(test_when_buffer_contains_continuation_byte_at_start_then_raise);
    TEST_CASE(test_when_buffer_contains_non_cont_byte_before_end_then_raise);
    TEST_CASE(test_when_buffer_contains_overlong_encoding_then_raise);
    TEST_CASE(test_when_buffer_contains_value_greater_than_max_allowed_then_raise);
}