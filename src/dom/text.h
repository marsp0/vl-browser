#pragma once

#include "dom/node.h"

// https://dom.spec.whatwg.org/#interface-text
typedef struct
{
    dom_node_t      node;

    unsigned char   data[MAX_HTML_NAME_LEN];
    uint32_t        data_size;
} dom_text_t;


dom_node_t*     dom_text_new(dom_node_t* document, unsigned char* data, uint32_t data_size);
void            dom_text_append_data(dom_node_t* node, unsigned char* data, uint32_t data_size);
dom_text_t*     dom_text_from_node(dom_node_t* node);
dom_node_t*     dom_node_from_text(dom_text_t* text);
void            dom_text_free(dom_node_t* node);