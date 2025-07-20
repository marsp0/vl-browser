#include "dom/element.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "html/constants.h"

html_node_t* html_element_new(html_node_t* document, unsigned char* local_name, uint32_t local_name_size)
{
    html_node_t* node           = html_node_new(HTML_NODE_ELEMENT, document);
    html_element_t* element     = NULL;

    // todo: step 2
    // todo: step 3
    // todo: step 4
    // todo: step 5

    element                     = malloc(sizeof(html_element_t));

    element->local_name_size    = local_name_size;
    element->tag_name_size      = local_name_size;
    unsigned char* element_name = element->local_name;
    unsigned char* tag_name     = element->tag_name;
    for (uint32_t i = 0; i < local_name_size; i++)
    {
        element_name[i]         = local_name[i];
        tag_name[i]             = local_name[i];

        if (local_name[i] < 'a')    { element_name[i] += 0x20; }
        if (local_name[i] >= 'a')   { tag_name[i] -= 0x20; }
    }

    element->namespace_size     = HTML_NAMESPACE_SIZE;
    memcpy(element->namespace, HTML_NAMESPACE, HTML_NAMESPACE_SIZE);

    node->data                  = element;
    memcpy(node->name, element->tag_name, element->tag_name_size);

    // todo: step 6.3 - finish

    return node;
}


void html_element_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_ELEMENT);

    free(node->data);
    html_node_free(node);
}