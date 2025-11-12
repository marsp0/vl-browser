#pragma once

#include "html/element.h"

typedef struct
{
    html_element_t  element;

    dom_node_t* form;
    hash_str_t html_for;
    html_element_t* control;

} html_label_t;

dom_node_t*     html_label_new(dom_node_t* document, hash_str_t namespace);
bool            html_node_is_label(dom_node_t* node);
html_label_t*   html_label_from_node(dom_node_t* node);
dom_node_t*     html_node_from_label(html_label_t* label);
void            html_button_free(dom_node_t* node);