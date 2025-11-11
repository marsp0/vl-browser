#pragma once

#include "html/element.h"

typedef struct
{
    html_element_t  element;

    hash_str_t charset;
    hash_str_t action;
    hash_str_t auto_complete;
    hash_str_t enc_type;
    hash_str_t encoding;
    hash_str_t method;
    hash_str_t name;
    hash_str_t validate;
    hash_str_t target;
    hash_str_t rel;

} html_form_t;

dom_node_t*     html_form_new(dom_node_t* document, hash_str_t namespace);
bool            html_node_is_form(dom_node_t* node);
html_form_t*    html_form_from_node(dom_node_t* node);
dom_node_t*     html_node_from_form(html_form_t* form);
void            html_button_free(dom_node_t* node);