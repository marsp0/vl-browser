#include "dom/doctype.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

html_node_t* html_doctype_new(html_node_t* document,
                              unsigned char* name, uint32_t name_size, 
                              unsigned char* public_id, uint32_t public_id_size, 
                              unsigned char* system_id, uint32_t system_id_size)
{
    html_doctype_t* doctype = malloc(sizeof(html_doctype_t));
    html_node_t* node = html_node_from_doctype(doctype);

    html_node_initialize(node, HTML_NODE_DOCTYPE, document);

    doctype->name_size = name_size;
    memcpy(doctype->name, name, name_size);

    doctype->public_id_size = public_id_size;
    memcpy(doctype->public_id, public_id, public_id_size);

    doctype->system_id_size = system_id_size;
    memcpy(doctype->system_id, system_id, system_id_size);

    return node;
}


html_node_t* html_node_from_doctype(html_doctype_t* doctype)
{
    return (html_node_t*)doctype;
}


html_doctype_t* html_doctype_from_node(html_node_t* node)
{
    assert(node->type == HTML_NODE_DOCTYPE);

    return (html_doctype_t*)node;
}


void html_doctype_free(html_node_t* node)
{
    html_node_free(node);
    assert(node->type == HTML_NODE_DOCTYPE);

    html_doctype_t* doctype = html_doctype_from_node(node);
    free(doctype);
}