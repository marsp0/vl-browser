#include "dom/document.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

html_node_t* html_document_new()
{
    html_node_t* node                   = html_node_new(HTML_NODE_DOCUMENT, NULL);
    node->document                      = node;

    html_document_t* document           = malloc(sizeof(html_document_t));
    document->url_size                  = 0;
    document->uri_size                  = 0;
    document->compat_mode_size          = 0;
    document->character_set_size        = 0;
    document->content_type_size         = 0;
    document->parser_cannot_change_mode = false;

    node->data                          = (void*)document;
    memcpy(node->name, "#document", sizeof("#document") - 1);

    return node;
}


void html_document_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_DOCUMENT);

    free(node->data);
    html_node_free(node);
}