#pragma once

#include "dom/node.h"

// https://dom.spec.whatwg.org/#interface-comment
typedef struct
{
    unsigned char   data[MAX_HTML_NAME_LEN];
    uint32_t        data_size;
} html_comment_t;


html_node_t*                html_comment_new(html_node_t* document, unsigned char* buffer, uint32_t size);
void                        html_comment_free(html_node_t* node);