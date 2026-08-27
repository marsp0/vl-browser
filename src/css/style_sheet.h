#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MAX_CSS_STYLESHEET_FIELD_SIZE 64

struct dom_element_t;

typedef struct css_style_sheet_t
{
    unsigned char               type[MAX_CSS_STYLESHEET_FIELD_SIZE];
    unsigned char               href[MAX_CSS_STYLESHEET_FIELD_SIZE];
    unsigned char               title[MAX_CSS_STYLESHEET_FIELD_SIZE];
    struct dom_element_t*       owner;
    struct css_style_sheet_t*   parent;
    uint32_t                    media;
    bool                        disabled;
    uint32_t                    owner_rule;

} css_style_sheet_t;

css_style_sheet_t*  css_style_sheet_new();
void                css_style_sheet_free(css_style_sheet_t* s);
