#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "../math.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void        increment_test_assert_counter();
void        reset_test_assert_counter();
uint32_t    get_test_assert_counter();
void        TESTS_INIT();
void        TESTS_SUMMARY();
uint32_t    TESTS_FAIL_COUNT();


#define GET_COMPARISON(a, b) _Generic(  a, \
                                        float: fabs((float)a - (float)b) > 0.0005f, \
                                        default: a != b)

#define GET_FORMAT(a) _Generic( a, \
                                int:         "%d != %d (%s != %s)\n", \
                                int64_t:    "%ld != %ld (%s != %s)\n", \
                                uint32_t:    "%u != %u (%s != %s)\n", \
                                uint64_t:   "%lu != %lu (%s != %s)\n", \
                                char:        "%c != %c (%s != %s)\n", \
                                float:       "%f != %f (%s != %s)\n", \
                                bool:        "%d != %d (%s != %s)\n")


#define ASSERT_EQUAL(a, b)          do \
                                    { \
                                        bool fail = GET_COMPARISON(a, b); \
                                        if (fail) \
                                        { \
                                            increment_test_assert_counter(); \
                                        } \
                                        if (fail && get_test_assert_counter() <= 10) \
                                        { \
                                            printf("    [%u]", __LINE__); \
                                            printf("[" ANSI_COLOR_RED "FAIL" ANSI_COLOR_RESET "]: "); \
                                            printf(GET_FORMAT(a), a, b, #a, #b); \
                                        } \
                                    } while(0);


#define ASSERT_TRUE(a)              do \
                                    { \
                                        ASSERT_EQUAL((a), true); \
                                    } while(0);


#define ASSERT_FALSE(a)             do \
                                    { \
                                        ASSERT_EQUAL((a), false); \
                                    } while(0);


#define ASSERT_POINTER(a, b)        do \
                                    { \
                                        ASSERT_EQUAL((int64_t)a, (int64_t)b); \
                                    } while(0);


#define ASSERT_STRING(a, b, size)   do \
                                    { \
                                        for (uint32_t tst_str_idx = 0; tst_str_idx < size; tst_str_idx++) \
                                        { \
                                            ASSERT_EQUAL(a[tst_str_idx], b[tst_str_idx]); \
                                        } \
                                    } while(0);


#define TEST_CASE(test)             do \
                                    { \
                                        printf("  %s\n", #test); \
                                        reset_test_assert_counter(); \
                                        test(); \
                                        uint32_t count = get_test_assert_counter(); \
                                        if (count > 10) \
                                        { \
                                            printf("    ... %u more assert fails\n", count - 10); \
                                        } \
                                    } while(0);


#define TEST_GROUP(group)           do \
                                    { \
                                        printf("%s\n", #group); \
                                        group(); \
                                    } while(0);
