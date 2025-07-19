#pragma once

#include "dom/node.h"

// https://dom.spec.whatwg.org/#documenttype
typedef struct
{
    unsigned char   name[MAX_HTML_NAME_LEN];
    uint32_t        name_size;

    unsigned char   public_id[MAX_HTML_NAME_LEN];
    uint32_t        public_id_size;

    unsigned char   system_id[MAX_HTML_NAME_LEN];
    uint32_t        system_id_size;
} html_doctype_t;


html_doctype_t*     html_doctype_new(unsigned char* name, uint32_t name_size, 
                                     unsigned char* public_id, uint32_t public_id_size, 
                                     unsigned char* system_id, uint32_t system_id_size);
void                html_doctype_free(html_doctype_t* node);