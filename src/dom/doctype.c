#include "dom/doctype.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

html_doctype_t* html_doctype_new(unsigned char* name, uint32_t name_size, 
                                 unsigned char* public_id, uint32_t public_id_size, 
                                 unsigned char* system_id, uint32_t system_id_size)
{
    html_doctype_t* doctype = malloc(sizeof(html_doctype_t));

    doctype->name_size = name_size;
    memcpy(doctype->name, name, name_size);

    doctype->public_id_size = public_id_size;
    memcpy(doctype->public_id, public_id, public_id_size);

    doctype->system_id_size = system_id_size;
    memcpy(doctype->system_id, system_id, system_id_size);

    return doctype;
}


void html_doctype_free(html_doctype_t* node)
{
    free(node);
}