#include "dom/comment.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

html_node_t* html_comment_new(html_node_t* document, unsigned char* buffer, uint32_t size)
{
    html_comment_t* comment         = malloc(sizeof(html_comment_t));
    html_node_t* node               = html_node_from_comment(comment);

    html_node_initialize(node, HTML_NODE_COMMENT, document);

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

    memcpy(node->name, "#comment", sizeof("#comment") - 1);

    return node;
}


html_comment_t* html_comment_from_node(html_node_t* node)
{
    assert(node->type == HTML_NODE_COMMENT);

    return (html_comment_t*)node;
}


html_node_t* html_node_from_comment(html_comment_t* comment)
{
    return (html_node_t*)comment;
}


void html_comment_free(html_node_t* node)
{
    html_node_free(node);
    assert(node->type == HTML_NODE_COMMENT);

    html_comment_t* comment = html_comment_from_node(node);
    free(comment);
}