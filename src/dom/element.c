#include "dom/element.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "html/constants.h"

dom_element_t* dom_element_from_node(dom_node_t* node);
dom_node_t*    dom_node_from_element(dom_element_t* element);


void dom_element_initialize(dom_element_t* element, dom_node_t* document, hash_str_t name)
{
    dom_node_t* node = dom_node_from_element(element);

    dom_node_initialize(node, DOM_NODE_ELEMENT, document);

    // todo: step 2
    // todo: step 3
    // todo: step 4
    // todo: step 5

    element->namespace = hash_str_new(HTML_NAMESPACE, HTML_NAMESPACE_SIZE);
    // element->local_name = hash_str_new(local_name, local_name_size);

    unsigned char temp[MAX_HTML_NAME_LEN]   = { 0 };
    const unsigned char* name_str           = hash_str_get(name);
    const uint32_t name_str_size            = hash_str_get_size(name);

    assert(name_str_size <= MAX_HTML_NAME_LEN);

    element->local_name = name;
    node->name = name;

    // set tag name
    for (uint32_t i = 0; i < name_str_size; i++)
    {
        temp[i] = name_str[i];

        if (temp[i] >= 'a' && temp[i] <= 'z')   { temp[i] -= 0x20; }
    }

    element->tag_name = hash_str_new(temp, name_str_size);

    element->prefix = 0;
    element->id = 0;
    element->class_name = 0;

    // todo: step 6.3 - finish
}


dom_node_t* dom_element_new(dom_node_t* document, hash_str_t name)
{
    dom_element_t* element = malloc(sizeof(dom_element_t));
    dom_element_initialize(element, document, name);

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
    assert(node->type == DOM_NODE_ELEMENT);

    dom_element_t* element = dom_element_from_node(node);
    free(element);
}