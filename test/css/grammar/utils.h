#pragma once

#define ASSERT_CSS_VALUE(a, e)                                                              \
do                                                                                          \
{                                                                                           \
    ASSERT_EQUAL(a->type, e->type);                                                         \
    ASSERT_EQUAL(a->unit, e->unit);                                                         \
    ASSERT_MEMORY(a, e, sizeof(css_value_t));                                               \
} while(0);
