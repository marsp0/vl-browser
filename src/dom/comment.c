#include "dom/comment.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

html_node_t* html_comment_new(unsigned char* buffer, uint32_t size, html_node_t* document)
{
    html_node_t* node               = html_node_new(HTML_NODE_COMMENT, document);
    html_comment_t* comment         = malloc(sizeof(html_comment_t));
    memcpy(comment->data, buffer, size);
    node->data                      = (void*)comment;

    return node;
}


void html_comment_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_COMMENT);

    free(node->data);
    html_node_free(node);
}