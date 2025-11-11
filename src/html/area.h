#pragma once

#include "html/element.h"

typedef struct
{
    html_element_t  element;

    hash_str_t alt;
    hash_str_t coords;
    hash_str_t shape;
    hash_str_t target;
    hash_str_t download;
    hash_str_t ping;
    hash_str_t rel;
    hash_str_t referrer_policy;

} html_area_t;

dom_node_t*     html_area_new(dom_node_t* document, hash_str_t namespace);
bool            html_node_is_area(dom_node_t* node);
html_area_t*    html_area_from_node(dom_node_t* node);
dom_node_t*     html_node_from_area(html_area_t* area);
void            html_area_free(dom_node_t* node);