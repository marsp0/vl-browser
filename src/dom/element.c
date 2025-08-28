#include "dom/element.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "html/constants.h"

dom_element_t* dom_element_from_node(dom_node_t* node);
dom_node_t*    dom_node_from_element(dom_element_t* element);


void dom_element_initialize(dom_element_t* element, dom_node_t* document, unsigned char* local_name, uint32_t local_name_size)
{
    dom_node_t* node = dom_node_from_element(element);

    dom_node_initialize(node, DOM_NODE_ELEMENT, document);

    // todo: step 2
    // todo: step 3
    // todo: step 4
    // todo: step 5

    element->namespace = hash_str_new(HTML_NAMESPACE, HTML_NAMESPACE_SIZE);
    // element->local_name = hash_str_new(local_name, local_name_size);

    unsigned char temp[MAX_HTML_NAME_LEN];

    // set local name
    memset(temp, 0, MAX_HTML_NAME_LEN);
    assert(local_name_size <= MAX_HTML_NAME_LEN);

    for (uint32_t i = 0; i < local_name_size; i++)
    {
        temp[i] = local_name[i];

        if (local_name[i] < 'a' && local_name[i] >= 'A')    { temp[i] += 0x20; }
    }

    element->local_name = hash_str_new(temp, local_name_size);

    // set tag name
    for (uint32_t i = 0; i < local_name_size; i++)
    {
        temp[i] = local_name[i];

        if (local_name[i] >= 'a' && local_name[i] <= 'z')   { temp[i] -= 0x20; }
    }

    element->tag_name = hash_str_new(temp, local_name_size);

    // todo: step 6.3 - finish
}


dom_node_t* dom_element_new(dom_node_t* document, unsigned char* local_name, uint32_t local_name_size)
{
    dom_element_t* element = malloc(sizeof(dom_element_t));
    dom_element_initialize(element, document, local_name, local_name_size);

    return dom_node_from_element(element);
}


dom_element_t* dom_element_from_node(dom_node_t* node)
{
    assert(node->type == DOM_NODE_ELEMENT);

    return (dom_element_t*)node;
}


dom_node_t* dom_node_from_element(dom_element_t* element)
{
    return (dom_node_t*)element;
}


void dom_element_free(dom_node_t* node)
{
    dom_node_free(node);
    assert(node->type == DOM_NODE_ELEMENT);

    dom_element_t* element = dom_element_from_node(node);
    free(element);
}