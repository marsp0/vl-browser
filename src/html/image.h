#pragma once

#include "html/element.h"

typedef struct
{
    html_element_t  element;

    hash_str_t name;
    hash_str_t low_src;
    hash_str_t align;
    uint32_t hspace;
    uint32_t vspace;
    hash_str_t desc;
    hash_str_t h_margin;
    hash_str_t w_margin;

} html_img_t;

dom_node_t*     html_img_new(dom_node_t* document, hash_str_t namespace);
bool            html_node_is_img(dom_node_t* node);
html_img_t*     html_img_from_node(dom_node_t* node);
dom_node_t*     html_node_from_img(html_img_t* img);
void            html_button_free(dom_node_t* node);