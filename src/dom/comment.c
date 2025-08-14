#include "dom/comment.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

html_node_t* html_comment_new(html_node_t* document, unsigned char* buffer, uint32_t size)
{
    html_node_t* node               = html_node_new(HTML_NODE_COMMENT, document);
    html_comment_t* comment         = malloc(sizeof(html_comment_t));

    memset(comment->data, 0, MAX_HTML_NAME_LEN);

    if (size > 0)
    {
        comment->data_size = size;
        memcpy(comment->data, buffer, size);
    }
    else
    {
        comment->data_size = 0;
    }

    node->data                      = (void*)comment;

    memcpy(node->name, "#comment", sizeof("#comment") - 1);

    return node;
}


void html_comment_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_COMMENT);

    free(node->data);
    html_node_free(node);
}