#pragma once

#include "dom/element.h"

typedef struct
{
    dom_element_t   element;

    hash_str_t      lang;
    hash_str_t      title;
    hash_str_t      dir;
    bool            translate;
    bool            hidden;
    bool            inert;
    hash_str_t      key;

} html_element_t;

void            html_element_init(html_element_t* element, dom_node_t* document, hash_str_t name);
dom_node_t*     html_element_new(dom_node_t* document, hash_str_t name);
html_element_t* html_element_from_node(dom_node_t* node);
dom_node_t*     html_node_from_element(html_element_t* element);
void            html_element_free(dom_node_t* node);