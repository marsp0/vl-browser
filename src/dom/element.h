#pragma once

#include <stdint.h>

#include "dom/node.h"
#include "html/constants.h"

// https://dom.spec.whatwg.org/#element
typedef struct
{
    dom_node_t     node;

    hash_str_t      namespace;
    hash_str_t      prefix;
    hash_str_t      local_name;
    hash_str_t      tag_name;
    hash_str_t      id;
    hash_str_t      class_name;

} dom_element_t;


void            dom_element_initialize(dom_element_t* element, dom_node_t* document, unsigned char* local_name, uint32_t local_name_size);
dom_node_t*     dom_element_new(dom_node_t* document, unsigned char* local_name, uint32_t local_name_size);
dom_element_t*  dom_element_from_node(dom_node_t* node);
dom_node_t*     dom_node_from_element(dom_element_t* element);
void            dom_element_free(dom_node_t* node);