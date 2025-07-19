#pragma once

#include "dom/node.h"

// https://dom.spec.whatwg.org/#interface-text
typedef struct
{
    unsigned char data[MAX_HTML_NAME_LEN];
    uint32_t data_size;
} html_text_t;


html_node_t*                html_text_new(html_node_t* document, unsigned char* data, uint32_t data_size);
void                        html_text_append_data(html_node_t* node, unsigned char* data, uint32_t data_size);
void                        html_text_free(html_node_t* node);