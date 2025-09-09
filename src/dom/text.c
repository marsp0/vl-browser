#include "dom/text.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

dom_node_t* dom_text_new(dom_node_t* document, unsigned char* data, uint32_t data_size)
{
    dom_text_t* text       = malloc(sizeof(dom_text_t));
    dom_node_t* node       = dom_node_from_text(text);

    dom_node_initialize(node, DOM_NODE_TEXT, document);

    text->data_size         = data_size;
    memset(text->data, 0, MAX_HTML_NAME_LEN);
    memcpy(text->data, data, data_size);
    node->name              = hash_str_new("#text", 5);

    return node;
}


void dom_text_append_data(dom_node_t* node, unsigned char* data, uint32_t data_size)
{
    if (node->type != DOM_NODE_TEXT) { return; }
    
    dom_text_t* text = dom_text_from_node(node);

    for (uint32_t i = 0; i < data_size; i++)
    {
        if (text->data_size >= MAX_HTML_NAME_LEN) { break; }

        text->data[text->data_size] = data[i];
        text->data_size++;

    }
}


dom_text_t* dom_text_from_node(dom_node_t* node)
{
    assert(node->type == DOM_NODE_TEXT);

    return (dom_text_t*)node;
}


dom_node_t* dom_node_from_text(dom_text_t* text)
{
    return (dom_node_t*)text;
}


void dom_text_free(dom_node_t* node)
{
    assert(node->type == DOM_NODE_TEXT);

    dom_text_t* text = dom_text_from_node(node);
    free(text);
}