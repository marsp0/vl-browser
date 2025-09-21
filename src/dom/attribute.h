#pragma once

#include "dom/node.h"

typedef struct dom_attr_t
{
    dom_node_t      node;

    hash_str_t      name;
    hash_str_t      value;
    dom_node_t*     owner;

    struct dom_attr_t*     next;
    struct dom_attr_t*     prev;
} dom_attr_t;

dom_node_t*     dom_attr_new(hash_str_t name, hash_str_t value, dom_node_t* owner);
bool            dom_node_is_attr(dom_node_t* node);
dom_node_t*     dom_node_from_attr(dom_attr_t* attr);
dom_attr_t*     dom_attr_from_node(dom_node_t* node);
void            dom_attr_free(dom_node_t* node);