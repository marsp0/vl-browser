#pragma once

#include "html/element.h"

typedef struct
{
    html_element_t  element;

    hash_str_t      autocomplete;
    bool            disabled;
    bool            multiple;

} html_select_t;

dom_node_t*     html_select_new(dom_node_t* document, hash_str_t namespace);
bool            html_node_is_select(dom_node_t* node);
html_select_t*  html_select_from_node(dom_node_t* node);
dom_node_t*     html_node_from_select(html_select_t* select);
void            html_select_free(dom_node_t* node);