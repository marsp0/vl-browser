#pragma once

#include "html/element.h"

typedef struct
{
    html_element_t  element;

    hash_str_t align;

} html_heading_t;

dom_node_t*     html_heading_new(dom_node_t* document, hash_str_t name, hash_str_t namespace);
bool            html_node_is_heading(dom_node_t* node);
html_heading_t* html_heading_from_node(dom_node_t* node);
dom_node_t*     html_node_from_heading(html_heading_t* heading);
void            html_button_free(dom_node_t* node);