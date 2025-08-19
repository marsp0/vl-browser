#pragma once

#include <stdint.h>

#include "dom/node.h"
#include "html/constants.h"

// https://dom.spec.whatwg.org/#element
typedef struct
{
    html_node_t     node;

    unsigned char   namespace[MAX_HTML_NAME_LEN];
    uint32_t        namespace_size;

    unsigned char   prefix[MAX_HTML_NAME_LEN];
    uint32_t        prefix_size;

    unsigned char   local_name[MAX_HTML_NAME_LEN];
    uint32_t        local_name_size;

    unsigned char   tag_name[MAX_HTML_NAME_LEN];
    uint32_t        tag_name_size;

    unsigned char   id[MAX_HTML_NAME_LEN];
    uint32_t        id_size;

    unsigned char   class_name[MAX_HTML_NAME_LEN];
    uint32_t        class_name_size;
} html_element_t;


html_node_t*    html_element_new(html_node_t* document, unsigned char* local_name, uint32_t local_name_size);
html_element_t* html_element_from_node(html_node_t* node);
html_node_t*    html_node_from_element(html_element_t* element);
void            html_element_free(html_node_t* node);