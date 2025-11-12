#pragma once

#include "html/element.h"

typedef struct
{
    html_element_t  element;

    hash_str_t align;

} html_div_t;

dom_node_t* html_div_new(dom_node_t* document, hash_str_t namespace);
bool        html_node_is_div(dom_node_t* node);
html_div_t* html_div_from_node(dom_node_t* node);
dom_node_t* html_node_from_div(html_div_t* div);
void        html_button_free(dom_node_t* node);