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

static void test_when_code_point_is_ascii_then_read_one_byte()
{
    uint32_t result;
    unsigned char buffer[]  = { 0x30, 0x44 };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, 1);
    ASSERT_EQUAL(result, '0');
}

static void test_when_code_point_is_2_bytes_then_read_2_bytes()
{
    uint32_t result;
    unsigned char buffer[]  = { 0xc3, 0x87 };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, 2);
    ASSERT_EQUAL(result, 0x00c7);
}

static void test_when_code_point_is_3_bytes_then_read_3_bytes()
{
    uint32_t result;
    unsigned char buffer[]  = { 0xe0, 0xb3, 0xa9 };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, 3);
    ASSERT_EQUAL(result, 0x0ce9);
}

static void test_when_code_point_is_4_bytes_then_read_4_bytes()
{
    uint32_t result;
    unsigned char buffer[]  = { 0xf0, 0x90, 0x8d, 0x83 };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, 4);
    ASSERT_EQUAL(result, 0x010343);
}

static void test_when_2_byte_code_point_is_not_full_then_error()
{
    uint32_t result = 0;
    unsigned char buffer[]  = { 0xc3 };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, -1);
    ASSERT_EQUAL(result, 0);
}

static void test_when_3_byte_code_point_is_not_full_then_error()
{
    uint32_t result = 0;
    unsigned char buffer[]  = { 0xe0, 0xa9 };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, -1);
    ASSERT_EQUAL(result, 0);
}

static void test_when_4_byte_code_point_is_not_full_then_error()
{
    uint32_t result = 0;
    unsigned char buffer[]  = { 0xf0, 0x90, 0x8d };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, -1);
    ASSERT_EQUAL(result, 0);
}

static void test_when_cursor_higher_than_size_then_error()
{
    uint32_t result = 0;
    unsigned char buffer[]  = { 0xf0, 0x90, 0x8d };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 5, &result);

    ASSERT_EQUAL(bytes_read, -1);
    ASSERT_EQUAL(result, 0);
}

static void test_when_passing_invalid_start_byte_then_raise()
{
    uint32_t result = 0;
    unsigned char buffer[]  = { 0xff, 0x90, 0x8d, 0x83 };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, -1);
    ASSERT_EQUAL(result, 0);
}

static void test_when_2nd_byte_in_2byte_code_point_is_invalid_then_raise()
{
    uint32_t result         = 0;
    unsigned char buffer[]  = { 0xc3, 0x7f };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, -1);
    ASSERT_EQUAL(result, 0);
}

static void test_when_2nd_byte_in_3byte_code_point_is_invalid_then_raise()
{
    uint32_t result         = 0;
    unsigned char buffer[]  = { 0xe0, 0x7f, 0xa9 };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, -1);
    ASSERT_EQUAL(result, 0);
}

static void test_when_3nd_byte_in_3byte_code_point_is_invalid_then_raise()
{
    uint32_t result         = 0;
    unsigned char buffer[]  = { 0xe0, 0xb3, 0x7f };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, -1);
    ASSERT_EQUAL(result, 0);
}

static void test_when_2nd_byte_in_4byte_code_point_is_invalid_then_raise()
{
    uint32_t result         = 0;
    unsigned char buffer[]  = { 0xf0, 0x7f, 0x8d, 0x83 };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, -1);
    ASSERT_EQUAL(result, 0);
}

static void test_when_3nd_byte_in_4byte_code_point_is_invalid_then_raise()
{
    uint32_t result         = 0;
    unsigned char buffer[]  = { 0xf0, 0x90, 0x7f, 0x83 };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, -1);
    ASSERT_EQUAL(result, 0);
}

static void test_when_4th_byte_in_4byte_code_point_is_invalid_then_raise()
{
    uint32_t result         = 0;
    unsigned char buffer[]  = { 0xf0, 0x90, 0x8d, 0x7f };
    int32_t bytes_read      = utf8_decode(buffer, sizeof(buffer), 0, &result);

    ASSERT_EQUAL(bytes_read, -1);
    ASSERT_EQUAL(result, 0);
}

static void test_when_code_point_isalpha_then_return_true()
{
    uint32_t code_point = 'G';
    bool is_alpha       = utf8_is_alpha(code_point);
    ASSERT_TRUE(is_alpha);

    code_point          = 'h';
    is_alpha            = utf8_is_alpha(code_point);
    ASSERT_TRUE(is_alpha);
}

static void test_when_code_point_is_not_alpha_then_return_false()
{
    uint32_t code_point = '/';
    bool is_alpha       = utf8_is_alpha(code_point);
    ASSERT_FALSE(is_alpha);

    code_point          = 3200;
    is_alpha            = utf8_is_alpha(code_point);
    ASSERT_FALSE(is_alpha);
}

static void test_when_code_point_is_digit()
{
    uint32_t code_point = '9';
    bool is_digit       = utf8_is_digit(code_point);
    ASSERT_TRUE(is_digit);

    code_point          = 'a';
    is_digit            = utf8_is_digit(code_point);
    ASSERT_FALSE(is_digit);
}

void test_utf8()
{
    TEST_CASE(test_when_buffer_is_valid_then_return_normally);
    TEST_CASE(test_when_buffer_contains_invalid_chars_then_raise);
    TEST_CASE(test_when_buffer_contains_continuation_byte_at_start_then_raise);
    TEST_CASE(test_when_buffer_contains_non_cont_byte_before_end_then_raise);
    TEST_CASE(test_when_buffer_contains_overlong_encoding_then_raise);
    TEST_CASE(test_when_buffer_contains_value_greater_than_max_allowed_then_raise);
    TEST_CASE(test_when_code_point_is_ascii_then_read_one_byte);
    TEST_CASE(test_when_code_point_is_2_bytes_then_read_2_bytes);
    TEST_CASE(test_when_code_point_is_3_bytes_then_read_3_bytes);
    TEST_CASE(test_when_code_point_is_4_bytes_then_read_4_bytes);
    TEST_CASE(test_when_2_byte_code_point_is_not_full_then_error);
    TEST_CASE(test_when_3_byte_code_point_is_not_full_then_error);
    TEST_CASE(test_when_4_byte_code_point_is_not_full_then_error);
    TEST_CASE(test_when_cursor_higher_than_size_then_error);
    TEST_CASE(test_when_passing_invalid_start_byte_then_raise);
    TEST_CASE(test_when_2nd_byte_in_2byte_code_point_is_invalid_then_raise);
    TEST_CASE(test_when_2nd_byte_in_3byte_code_point_is_invalid_then_raise);
    TEST_CASE(test_when_3nd_byte_in_3byte_code_point_is_invalid_then_raise);
    TEST_CASE(test_when_2nd_byte_in_4byte_code_point_is_invalid_then_raise);
    TEST_CASE(test_when_3nd_byte_in_4byte_code_point_is_invalid_then_raise);
    TEST_CASE(test_when_4th_byte_in_4byte_code_point_is_invalid_then_raise);
    TEST_CASE(test_when_code_point_isalpha_then_return_true);
    TEST_CASE(test_when_code_point_is_not_alpha_then_return_false);
    TEST_CASE(test_when_code_point_is_digit);
}