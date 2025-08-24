#include "dom/element.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "html/constants.h"

html_element_t* html_element_from_node(html_node_t* node);
html_node_t*    html_node_from_element(html_element_t* element);


void html_element_initialize(html_element_t* element, html_node_t* document, unsigned char* local_name, uint32_t local_name_size)
{
    html_node_t* node = html_node_from_element(element);

    html_node_initialize(node, HTML_NODE_ELEMENT, document);

    // todo: step 2
    // todo: step 3
    // todo: step 4
    // todo: step 5

    element->local_name_size    = local_name_size;
    element->tag_name_size      = local_name_size;
    unsigned char* element_name = element->local_name;
    unsigned char* tag_name     = element->tag_name;

    for (uint32_t i = 0; i < local_name_size; i++)
    {
        element_name[i]         = local_name[i];
        tag_name[i]             = local_name[i];

        if (local_name[i] < 'a' && local_name[i] >= 'A')    { element_name[i] += 0x20; }
        if (local_name[i] >= 'a' && local_name[i] <= 'z')   { tag_name[i] -= 0x20; }
    }

    element->namespace_size     = HTML_NAMESPACE_SIZE;
    memcpy(element->namespace, HTML_NAMESPACE, HTML_NAMESPACE_SIZE);
    memcpy(node->name, element->tag_name, element->tag_name_size);

    element->prefix_size = 0;
    memset(element->prefix, 0, MAX_HTML_NAME_LEN);

    element->id_size = 0;
    memset(element->id, 0, MAX_HTML_NAME_LEN);

    element->class_name_size = 0;
    memset(element->class_name, 0, MAX_HTML_NAME_LEN);

    element->type = HTML_ELEMENT_GENERIC;

    // todo: step 6.3 - finish
}


html_node_t* html_element_new(html_node_t* document, unsigned char* local_name, uint32_t local_name_size)
{
    html_element_t* element = malloc(sizeof(html_element_t));
    html_element_initialize(element, document, local_name, local_name_size);

    return html_node_from_element(element);
}


html_element_t* html_element_from_node(html_node_t* node)
{
    assert(node->type == HTML_NODE_ELEMENT);

    return (html_element_t*)node;
}


html_node_t* html_node_from_element(html_element_t* element)
{
    return (html_node_t*)element;
}


void html_element_free(html_node_t* node)
{
    html_node_free(node);
    assert(node->type == HTML_NODE_ELEMENT);

    html_element_t* element = html_element_from_node(node);
    free(element);
}