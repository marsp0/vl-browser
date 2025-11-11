#pragma once

#include "html/element.h"

typedef struct
{
    html_element_t  element;

    hash_str_t cmd;
    dom_node_t* cmd_for_element;
    bool disabled;
    dom_node_t* form;
    hash_str_t f_action;
    hash_str_t f_enc_type;
    hash_str_t f_method;
    bool f_validate;
    hash_str_t f_target;
    hash_str_t name;
    hash_str_t type;
    hash_str_t value;
    bool will_validate;
    uint32_t validity;
    hash_str_t validation_msg;
    hash_str_t labels;

} html_button_t;

dom_node_t*     html_button_new(dom_node_t* document, hash_str_t namespace);
bool            html_node_is_button(dom_node_t* node);
html_button_t*  html_button_from_node(dom_node_t* node);
dom_node_t*     html_node_from_button(html_button_t* button);
void            html_button_free(dom_node_t* node);