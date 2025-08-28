#pragma once

#include "dom/node.h"
#include "dom/doctype.h"

// https://dom.spec.whatwg.org/#interface-document
typedef struct
{
    dom_node_t      node;

    hash_str_t      url;
    hash_str_t      uri;
    hash_str_t      compat_mode;
    hash_str_t      character_set;
    hash_str_t      content_set;
    hash_str_t      content_type;

    dom_doctype_t*  doctype;

    bool parser_cannot_change_mode;
} dom_document_t;


dom_node_t*         dom_document_new();
dom_node_t*         dom_node_from_document(dom_document_t* document);
dom_document_t*     dom_document_from_node(dom_node_t* node);
void                dom_document_free(dom_node_t* node);
