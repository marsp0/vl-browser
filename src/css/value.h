#pragma once

typedef enum css_value_type_e
{
    CSS_VALUE_TYPE_INVALID,
    CSS_VALUE_TYPE_NUMBER
} css_value_type_e;

typedef struct css_value_t
{
    css_value_type_e type;
    
} css_value_t;

css_value_t css_value_new();