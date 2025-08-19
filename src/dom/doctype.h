#pragma once

#include "dom/node.h"

// https://dom.spec.whatwg.org/#documenttype
typedef struct
{
    html_node_t     node;

    unsigned char   name[MAX_HTML_NAME_LEN];
    uint32_t        name_size;

    unsigned char   public_id[MAX_HTML_NAME_LEN];
    uint32_t        public_id_size;

    unsigned char   system_id[MAX_HTML_NAME_LEN];
    uint32_t        system_id_size;
} html_doctype_t;


html_node_t*    html_doctype_new(html_node_t* document,
                                 unsigned char* name, uint32_t name_size, 
                                 unsigned char* public_id, uint32_t public_id_size, 
                                 unsigned char* system_id, uint32_t system_id_size);
html_node_t*    html_node_from_doctype(html_doctype_t* doctype);
html_doctype_t* html_doctype_from_node(html_node_t* node);
void            html_doctype_free(html_node_t* node);