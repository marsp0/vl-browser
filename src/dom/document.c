#include "dom/document.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

html_node_t* html_document_new()
{
    html_document_t* document           = malloc(sizeof(html_document_t));
    html_node_t* node                   = html_node_from_document(document);

    html_node_initialize(node, HTML_NODE_DOCUMENT, NULL);
    node->document                      = node;

    document->url_size                  = 0;
    document->uri_size                  = 0;
    document->compat_mode_size          = 0;
    document->character_set_size        = 0;
    document->content_type_size         = 0;
    document->parser_cannot_change_mode = false;

    memcpy(node->name, "#document", sizeof("#document") - 1);

    return node;
}


html_node_t* html_node_from_document(html_document_t* document)
{
    return (html_node_t*)document;
}


html_document_t* html_document_from_node(html_node_t* node)
{
    assert(node->type == HTML_NODE_DOCUMENT);

    return (html_document_t*)node;
}


void html_document_free(html_node_t* node)
{
    html_node_free(node);
    assert(node->type == HTML_NODE_DOCUMENT);

    html_document_t* document = html_document_from_node(node);
    free(document);
}