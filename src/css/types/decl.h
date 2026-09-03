#pragma once

#include <stdbool.h>

#include "dom/hash_str.h"
#include "css/types/value.h"

typedef enum
{
    CSS_PROP_INVALID,
    CSS_PROP_COLOR
} css_prop_e;

typedef struct css_decl_t
{
    // css_prop_t  prop;
    css_prop_e          prop;
    css_value_t*        value;

    bool                important;
    bool                case_sens;

    struct css_decl_t*  next;
    struct css_decl_t*  prev;

} css_decl_t;


css_decl_t* css_decl_new();
void        css_decl_free(css_decl_t* decl);