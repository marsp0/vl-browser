#include "dom/document.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>


dom_node_t* dom_document_new()
{
    dom_document_t* document    = malloc(sizeof(dom_document_t));
    dom_node_t* node            = dom_node_from_document(document);

    dom_node_initialize(node, DOM_NODE_DOCUMENT, NULL);
    node->document              = node;
    node->name                  = hash_str_new("#document", 9);

    document->url               = 0;
    document->uri               = 0;
    document->compat_mode       = 0;
    document->character_set     = 0;
    document->content_set       = 0;
    document->content_type      = 0;
    document->doctype           = NULL;

    document->mode = hash_str_new("no-quirks", 9);

    document->parser_cannot_change_mode = false;

    return node;
}


bool dom_node_is_document(dom_node_t* node)
{
    return node->type & DOM_NODE_DOCUMENT;
}


void dom_document_set_doctype(dom_document_t* doc, dom_doctype_t* doctype)
{
    doc->doctype = doctype;
}


dom_node_t* dom_node_from_document(dom_document_t* document)
{
    return (dom_node_t*)document;
}


dom_document_t* dom_document_from_node(dom_node_t* node)
{
    assert(dom_node_is_document(node));

    return (dom_document_t*)node;
}


void dom_document_free(dom_node_t* node)
{
    assert(dom_node_is_document(node));

    dom_document_t* document = dom_document_from_node(node);
    free(document);
}