#include "dom/comment.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

dom_node_t* dom_comment_new(dom_node_t* document, unsigned char* buffer, uint32_t size)
{
    dom_comment_t* comment         = malloc(sizeof(dom_comment_t));
    dom_node_t* node               = dom_node_from_comment(comment);

    dom_node_initialize(node, DOM_NODE_COMMENT, document);

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

    node->name = hash_str_new("#comment", 8);

    return node;
}


dom_comment_t* dom_comment_from_node(dom_node_t* node)
{
    assert(node->type == DOM_NODE_COMMENT);

    return (dom_comment_t*)node;
}


dom_node_t* dom_node_from_comment(dom_comment_t* comment)
{
    return (dom_node_t*)comment;
}


void dom_comment_free(dom_node_t* node)
{
    dom_node_free(node);
    assert(node->type == DOM_NODE_COMMENT);

    dom_comment_t* comment = dom_comment_from_node(node);
    free(comment);
}