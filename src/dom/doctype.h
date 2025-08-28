#pragma once

#include "dom/node.h"

// https://dom.spec.whatwg.org/#documenttype
typedef struct
{
    dom_node_t      node;
    hash_str_t      name;
    hash_str_t      public_id;
    hash_str_t      system_id;

} dom_doctype_t;


dom_node_t*     dom_doctype_new(dom_node_t* document,
                                 unsigned char* name, uint32_t name_size, 
                                 unsigned char* public_id, uint32_t public_id_size, 
                                 unsigned char* system_id, uint32_t system_id_size);
dom_node_t*     dom_node_from_doctype(dom_doctype_t* doctype);
dom_doctype_t*  dom_doctype_from_node(dom_node_t* node);
void            dom_doctype_free(dom_node_t* node);