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

    if (!child)
    {
        dom_node_t* last = parent->last;
        if (last)
        {
            last->next = node;
            node->prev = last;
            parent->last = node;
        }
        else
        {
            parent->first = node;
            parent->last = node;
        }
    }
    else
    {
        dom_node_t* prev = child->prev;
        if (prev)
        {
            prev->next = node;
            node->prev = prev;
            child->prev = node;
            node->next = child;
        }
        else
        {
            node->next = child;
            child->prev = node;
            parent->first = node;
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
    if (ref_child == node) { ref_child = node->next; }

    return dom_insert_node(parent, node, ref_child, false);
}

void dom_element_free(dom_node_t* node);
void dom_document_free(dom_node_t* node);
void dom_doctype_free(dom_node_t* node);
void dom_comment_free(dom_node_t* node);
void dom_text_free(dom_node_t* node);
void html_element_free(dom_node_t* node);
void html_select_free(dom_node_t* node);
void dom_attr_free(dom_node_t* node);
bool html_node_is_select(dom_node_t* node);
bool html_node_is_element(dom_node_t* node);
bool dom_node_is_document(dom_node_t* node);
bool dom_node_is_doctype(dom_node_t* node);
bool dom_node_is_element(dom_node_t* node);
bool dom_node_is_comment(dom_node_t* node);
bool dom_node_is_attr(dom_node_t* node);
bool dom_node_is_text(dom_node_t* node);

/********************/
/* public functions */
/********************/


void dom_node_initialize(dom_node_t* node, uint64_t type, dom_node_t* document)
{
    // note: should name be initialized here?

    node->type          = type;
    node->document      = document;
    node->is_connected  = false;
    node->first   = NULL;
    node->last    = NULL;
    node->next  = NULL;
    node->prev  = NULL;

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

    dom_node_t* prev = child->prev;
    dom_node_t* next = child->next;

    if (prev)
    {
        prev->next = next;
    }

    if (next)
    {
        next->prev = prev;
    }

    if (node->first == child)
    {
        node->first = next;
    }

    if (node->last == child)
    {
        node->last = prev;
    }

    child->parent = NULL;
    child->next = NULL;
    child->prev = NULL;

    return child;
}


void dom_node_free(dom_node_t* node)
{
    dom_node_t* child = node->last;
    while (child)
    {
        dom_node_t* prev = child->prev;
        dom_node_free(child);
        child = prev;
    }

    if      (html_node_is_select(node))         { html_select_free(node);   }
    else if (html_node_is_element(node))        { html_element_free(node);  }
    else if (dom_node_is_document(node))        { dom_document_free(node);  }
    else if (dom_node_is_doctype(node))         { dom_doctype_free(node);   }
    else if (dom_node_is_element(node))         { dom_element_free(node);   }
    else if (dom_node_is_comment(node))         { dom_comment_free(node);   }
    else if (dom_node_is_attr(node))            { dom_attr_free(node);      }
    else if (dom_node_is_text(node))            { dom_text_free(node);      }
}
