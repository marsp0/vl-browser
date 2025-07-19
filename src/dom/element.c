#include "dom/element.h"

html_node_t* html_element_new(html_node_t* document, unsigned char* local_name, uint32_t local_name_size)
{
    html_node_t* node           = html_node_new(HTML_NODE_ELEMENT, document);
    html_element_t* result      = NULL;

    // todo: step 2
    // todo: step 3
    // todo: step 4
    // todo: step 5

    result                      = malloc(sizeof(html_element_t));
    result->local_name_size     = local_name_size;
    memcpy(result->local_name, local_name, local_name_size);

    // todo: step 6.3 - finish

    return node;
}


void html_element_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_ELEMENT);

    free(node->element_data);
    html_node_free(node);
}