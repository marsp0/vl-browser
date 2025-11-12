#pragma once

#include "html/element.h"

typedef struct
{
    html_element_t  element;

    hash_str_t align;
    hash_str_t use_map;

} html_input_t;

dom_node_t*     html_input_new(dom_node_t* document, hash_str_t namespace);
bool            html_node_is_input(dom_node_t* node);
html_input_t*   html_input_from_node(dom_node_t* node);
dom_node_t*     html_node_from_input(html_input_t* input);
void            html_button_free(dom_node_t* node);