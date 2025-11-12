#pragma once

#include "html/element.h"

typedef struct
{
    html_element_t  element;

    hash_str_t      target;
    hash_str_t      download;
    hash_str_t      ping;
    hash_str_t      rel;
    hash_str_t      hreflang;
    hash_str_t      type;

    hash_str_t      text;
    hash_str_t      referrer_policy;

} html_anchor_t;

dom_node_t*     html_anchor_new(dom_node_t* document, hash_str_t namespace);
bool            html_node_is_anchor(dom_node_t* node);
html_anchor_t*  html_anchor_from_node(dom_node_t* node);
dom_node_t*     html_node_from_anchor(html_anchor_t* anchor);
void            html_anchor_free(dom_node_t* node);