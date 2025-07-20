#include "dom/text.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

html_node_t* html_text_new(html_node_t* document, unsigned char* data, uint32_t data_size)
{
    html_node_t* node       = html_node_new(HTML_NODE_TEXT, document);

    html_text_t* text       = malloc(sizeof(html_text_t));
    text->data_size         = data_size;
    memcpy(text->data, data, data_size);

    node->data              = (void*)text;
    memcpy(node->name, "#text", sizeof("#text") - 1);

    return node;
}


void html_text_append_data(html_node_t* node, unsigned char* data, uint32_t data_size)
{
    if (node->type != HTML_NODE_TEXT) { return; }
    
    html_text_t* text = node->data;

    for (uint32_t i = 0; i < data_size; i++)
    {
        if (text->data_size >= MAX_HTML_NAME_LEN) { break; }

        text->data[text->data_size] = data[i];
        text->data_size++;

    }
}


void html_text_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_TEXT);

    free(node->data);
    html_node_free(node);
}