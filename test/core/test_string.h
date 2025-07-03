#pragma once

#include "test_utils.h"

#define ASSERT_STRING_TYPE(a, b)    do                                                                  \
                                    {                                                                   \
                                        ASSERT_EQUAL(a.size, b.size);                                   \
                                        ASSERT_EQUAL(a.len, b.len);                                     \
                                        ASSERT_STRING((char)a.data, (char)b.data, a.size);              \
                                    } while (0);                                                        \

void test_string();