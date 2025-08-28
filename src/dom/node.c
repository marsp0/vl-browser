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
static dom_exception_e html_check_pre_insert_validity(html_node_t* parent, html_node_t* node, html_node_t* child)
{
    assert(parent || !parent);
    assert(node || !node);
    assert(child || !child);

    return HTML_DOM_ERR_OK;
}


static html_node_t* html_insert_node(html_node_t* parent, html_node_t* node, html_node_t* child, bool suppress_observers)
{
    // html_node_t* nodes = node;
    // todo: step 1
    // todo: step 2
    // todo: step 3
    // todo: step 4
    // todo: step 5

    // html_node_t* prev = parent->last_child;
    // if (child) { prev = child->prev_sibling; }
    

    if (!child)
    {
        html_node_t* prev = parent->last_child;
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
        html_node_t* prev = child->prev_sibling;
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


static html_node_t* html_pre_insert_node(html_node_t* parent, html_node_t* node, html_node_t* child)
{
    if (html_check_pre_insert_validity(parent, node, child) != HTML_DOM_ERR_OK)
    {
        return NULL;
    }

    html_node_t* ref_child = child;
    if (ref_child == node) { ref_child = node->next_sibling; }

    return html_insert_node(parent, node, ref_child, false);
}

void html_element_free(html_node_t* node);
void html_document_free(html_node_t* node);
void html_doctype_free(html_node_t* node);
void html_comment_free(html_node_t* node);
void html_text_free(html_node_t* node);

/********************/
/* public functions */
/********************/


void html_node_initialize(html_node_t* node, html_node_type_e type, html_node_t* document)
{
    memset(node->name, 0, MAX_HTML_NAME_LEN);
    memset(node->base_uri, 0, MAX_HTML_NAME_LEN);

    node->type          = type;
    node->document      = document;
    node->name_size     = 0;
    node->base_uri_size = 0;
    node->is_connected  = false;
    node->first_child   = NULL;
    node->last_child    = NULL;
    node->next_sibling  = NULL;
    node->prev_sibling  = NULL;

    assert(node->type != HTML_NODE_INVALID);
}


html_node_t* html_node_insert_before(html_node_t* node, html_node_t* new_node, html_node_t* child)
{
    return html_pre_insert_node(node, new_node, child);
}


html_node_t* html_node_append(html_node_t* node, html_node_t* new_node)
{
    return html_pre_insert_node(node, new_node, NULL);
}


html_node_t* html_node_remove(html_node_t* node, html_node_t* child)
{
    // todo: this should be returning an error status
    if (child->parent != node) { return NULL; }

    html_node_t* parent = child->parent;
    assert(parent);

    // todo: live range logic

    html_node_t* prev_sibling = child->prev_sibling;
    html_node_t* next_sibling = child->next_sibling;

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


void html_node_free(html_node_t* node)
{
    html_node_t* child = node->last_child;
    while (child)
    {
        html_node_t* prev = child->prev_sibling;

        if (child->type == HTML_NODE_DOCUMENT)  { html_document_free(child); }
        if (child->type == HTML_NODE_DOCTYPE)   { html_doctype_free(child); }
        if (child->type == HTML_NODE_ELEMENT)   { html_element_free(child); }
        if (child->type == HTML_NODE_COMMENT)   { html_comment_free(child); }
        if (child->type == HTML_NODE_TEXT)      { html_text_free(child); }

        child = prev;
    }
}
