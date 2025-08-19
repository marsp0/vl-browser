#pragma once

#include "dom/node.h"

// https://dom.spec.whatwg.org/#interface-document
typedef struct
{
    html_node_t     node;

    unsigned char   url[MAX_HTML_NAME_LEN];
    uint32_t        url_size;

    unsigned char   uri[MAX_HTML_NAME_LEN];
    uint32_t        uri_size;

    unsigned char   compat_mode[MAX_HTML_NAME_LEN];
    uint32_t        compat_mode_size;

    unsigned char   character_set[MAX_HTML_NAME_LEN];
    uint32_t        character_set_size;

    unsigned char   content_type[MAX_HTML_NAME_LEN];
    uint32_t        content_type_size;

    struct html_doctype_t* doctype;

    bool parser_cannot_change_mode;
} html_document_t;


html_node_t*                html_document_new();
html_node_t*                html_node_from_document(html_document_t* document);
html_document_t*            html_document_from_node(html_node_t* node);
void                        html_document_free(html_node_t* node);