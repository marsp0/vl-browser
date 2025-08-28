#include "node.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <stdio.h>

/*
 * Notes
 * 
 */

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/


/********************/
/* static functions */
/********************/



// https://dom.spec.whatwg.org/#concept-node-ensure-pre-insertion-validity
static dom_exception_e dom_check_pre_insert_validity(dom_node_t* parent, dom_node_t* node, dom_node_t* child)
{
    assert(parent || !parent);
    assert(node || !node);
    assert(child || !child);

    return HTML_DOM_ERR_OK;
}


static dom_node_t* dom_insert_node(dom_node_t* parent, dom_node_t* node, dom_node_t* child, bool suppress_observers)
{
    // dom_node_t* nodes = node;
    // todo: step 1
    // todo: step 2
    // todo: step 3
    // todo: step 4
    // todo: step 5

    // dom_node_t* prev = parent->last_child;
    // if (child) { prev = child->prev_sibling; }
    

    if (!child)
    {
        dom_node_t* prev = parent->last_child;
        if (prev)
        {
            prev->next_sibling = node;
            node->prev_sibling = prev;
            parent->last_child = node;
        }
        else
        {
            parent->first_child = node;
            parent->last_child = node;
        }
    }
    else
    {
        dom_node_t* prev = child->prev_sibling;
        if (prev)
        {
            prev->next_sibling = node;
            node->prev_sibling = prev;
            child->prev_sibling = node;
            node->next_sibling = child;
        }
        else
        {
            node->next_sibling = child;
            child->prev_sibling = node;
            parent->first_child = node;
        }
    }

    node->parent = parent;
    
    // todo: step 7.1
    // todo: step 7.4
    // todo: step 7.5
    // todo: step 7.6
    // todo: step 7.7
    // todo: step 8
    // todo: step 9
    // todo: step 10
    // todo: step 11
    // todo: step 12

    assert(suppress_observers || !suppress_observers);

    return node;
}


static dom_node_t* dom_pre_insert_node(dom_node_t* parent, dom_node_t* node, dom_node_t* child)
{
    if (dom_check_pre_insert_validity(parent, node, child) != HTML_DOM_ERR_OK)
    {
        return NULL;
    }

    dom_node_t* ref_child = child;
    if (ref_child == node) { ref_child = node->next_sibling; }

    return dom_insert_node(parent, node, ref_child, false);
}

void dom_element_free(dom_node_t* node);
void dom_document_free(dom_node_t* node);
void dom_doctype_free(dom_node_t* node);
void dom_comment_free(dom_node_t* node);
void dom_text_free(dom_node_t* node);

/********************/
/* public functions */
/********************/


void dom_node_initialize(dom_node_t* node, dom_node_type_e type, dom_node_t* document)
{
    node->type          = type;
    node->document      = document;
    node->is_connected  = false;
    node->first_child   = NULL;
    node->last_child    = NULL;
    node->next_sibling  = NULL;
    node->prev_sibling  = NULL;

    assert(node->type != DOM_NODE_INVALID);
}


dom_node_t* dom_node_insert_before(dom_node_t* node, dom_node_t* new_node, dom_node_t* child)
{
    return dom_pre_insert_node(node, new_node, child);
}


dom_node_t* dom_node_append(dom_node_t* node, dom_node_t* new_node)
{
    return dom_pre_insert_node(node, new_node, NULL);
}


dom_node_t* dom_node_remove(dom_node_t* node, dom_node_t* child)
{
    // todo: this should be returning an error status
    if (child->parent != node) { return NULL; }

    dom_node_t* parent = child->parent;
    assert(parent);

    // todo: live range logic

    dom_node_t* prev_sibling = child->prev_sibling;
    dom_node_t* next_sibling = child->next_sibling;

    if (prev_sibling)
    {
        prev_sibling->next_sibling = next_sibling;
    }

    if (next_sibling)
    {
        next_sibling->prev_sibling = prev_sibling;
    }

    if (node->first_child == child)
    {
        node->first_child = next_sibling;
    }

    if (node->last_child == child)
    {
        node->last_child = prev_sibling;
    }

    child->parent = NULL;

    return child;
}


void dom_node_free(dom_node_t* node)
{
    dom_node_t* child = node->last_child;
    while (child)
    {
        dom_node_t* prev = child->prev_sibling;

        // BIG TODO: how do we dispose of all of this
        if (child->type == DOM_NODE_DOCUMENT)  { dom_document_free(child); }
        if (child->type == DOM_NODE_DOCTYPE)   { dom_doctype_free(child); }
        if (child->type == DOM_NODE_ELEMENT)   { dom_element_free(child); }
        if (child->type == DOM_NODE_COMMENT)   { dom_comment_free(child); }
        if (child->type == DOM_NODE_TEXT)      { dom_text_free(child); }

        child = prev;
    }
}
