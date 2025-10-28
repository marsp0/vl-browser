/*
 * Notes
 * 
 */

#include "dom/attribute.h"

#include <stdlib.h>
#include <assert.h>

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/


/********************/
/* static functions */
/********************/


/********************/
/* public functions */
/********************/

dom_node_t* dom_attr_new(hash_str_t name, hash_str_t value, dom_node_t* owner)
{
    dom_attr_t* attr = malloc(sizeof(dom_attr_t));

    attr->node.type = DOM_NODE_ATTRIBUTE;

    attr->name = name;
    attr->value = value;
    attr->owner = owner;
    attr->prev = NULL;
    attr->next = NULL;

    return (dom_node_t*)attr;
}


bool dom_node_is_attr(dom_node_t* node)
{
    return node->type & DOM_NODE_ATTRIBUTE;
}


dom_node_t* dom_node_from_attr(dom_attr_t* attr)
{
    return (dom_node_t*)attr;
}


dom_attr_t* dom_attr_from_node(dom_node_t* node)
{
    assert(dom_node_is_attr(node));

    return (dom_attr_t*)node;
}


void dom_attr_free(dom_node_t* node)
{
    dom_attr_t* attr = dom_attr_from_node(node);

    free(attr);
}

