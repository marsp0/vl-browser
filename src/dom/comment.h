#pragma once

#include "dom/node.h"

// https://dom.spec.whatwg.org/#interface-comment
typedef struct
{
    dom_node_t      node;

    unsigned char   data[MAX_HTML_NAME_LEN];
    uint32_t        data_size;
} dom_comment_t;


dom_node_t*     dom_comment_new(dom_node_t* document, unsigned char* buffer, uint32_t size);
bool            dom_node_is_comment(dom_node_t* node);
dom_comment_t*  dom_comment_from_node(dom_node_t* node);
dom_node_t*     dom_node_from_comment(dom_comment_t* comment);
void            dom_comment_free(dom_node_t* node);