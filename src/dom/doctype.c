#include "dom/doctype.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

dom_node_t* dom_doctype_new(dom_node_t* document,
                            unsigned char* name, uint32_t name_size, 
                            unsigned char* public_id, uint32_t public_id_size, 
                            unsigned char* system_id, uint32_t system_id_size)
{
    dom_doctype_t* doctype = malloc(sizeof(dom_doctype_t));
    dom_node_t* node = dom_node_from_doctype(doctype);

    dom_node_initialize(node, DOM_NODE_DOCTYPE, document);

    doctype->name = hash_str_new(name, name_size);
    doctype->public_id = hash_str_new(public_id, public_id_size);
    doctype->system_id = hash_str_new(system_id, system_id_size);

    return node;
}


bool dom_node_is_doctype(dom_node_t* node)
{
    return node->type & DOM_NODE_DOCTYPE;
}


dom_node_t* dom_node_from_doctype(dom_doctype_t* doctype)
{
    return (dom_node_t*)doctype;
}


dom_doctype_t* dom_doctype_from_node(dom_node_t* node)
{
    assert(dom_node_is_doctype(node));

    return (dom_doctype_t*)node;
}


void dom_doctype_free(dom_node_t* node)
{
    assert(dom_node_is_doctype(node));

    dom_doctype_t* doctype = dom_doctype_from_node(node);
    free(doctype);
}