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

// static uint32_t html_count_children_of_type(html_node_t* node, html_node_type_e type)
// {
//     uint32_t result = 0;

//     html_node_t* child = node->first_child;
//     while (child)
//     {
//         if (child->type == type) { result++; }
//         child = child->next_sibling;
//     }
//     return result;
// }

// static uint32_t html_count_following_children_of_type(html_node_t* node, html_node_type_e type)
// {
//     uint32_t result = 0;

//     if (!node) { return result; }

//     html_node_t* sibling = node->next_sibling;
//     while (sibling)
//     {
//         if (sibling->type == type) { result++; }
//         sibling = sibling->next_sibling;
//     }
//     return result;
// }

// static uint32_t html_count_preceding_children_of_type(html_node_t* node, html_node_type_e type)
// {
//     uint32_t result = 0;

//     if (!node) { return result; }

//     html_node_t* sibling = node->prev_sibling;
//     while (sibling)
//     {
//         if (sibling->type == type) { result++; }
//         sibling = sibling->prev_sibling;
//     }
//     return result;
// }

// static html_node_t* html_get_host(html_node_t* node)
// {
//     if (node->type == HTML_NODE_DOCUMENT_FRAGMENT) { return NULL; }
//     return NULL;
// }

// static html_node_t* html_get_root(html_node_t* node)
// {
//     html_node_t* result = node->parent;
//     while (result && result->parent) { result = result->parent; }

//     return result;
// }

// // https://dom.spec.whatwg.org/#concept-tree-descendant
// static bool html_is_descendant(html_node_t* a, html_node_t* b)
// {
//     html_node_t* temp = a->parent;
//     while (temp)
//     {
//         if (temp == b) { return true; }
//         temp = temp->parent;
//     }

//     return false;
// }

// // https://dom.spec.whatwg.org/#concept-tree-ancestor
// static bool html_is_ancestor(html_node_t* a, html_node_t* b)
// {
//     return html_is_descendant(b, a);
// }

// // https://dom.spec.whatwg.org/#concept-tree-inclusive-ancestor
// static bool html_is_inclusive_ancestor(html_node_t* a, html_node_t* b)
// {
//     return (a == b) || html_is_ancestor(a, b);
// }

// // https://dom.spec.whatwg.org/#concept-tree-host-including-inclusive-ancestor
// static bool html_is_host_including_inclusive_ancestor(html_node_t* a, html_node_t* b)
// {
//     // 1. if A is an inclusive ancestor of B
//     if (html_is_inclusive_ancestor(a, b)) { return true; }
//     // 2. if B’s root has a non-null host and A is a host-including inclusive ancestor of B’s root’s host
//     html_node_t* b_root         = html_get_root(b);
//     html_node_t* b_root_host    = html_get_host(b_root);
//     if (!b_root_host) { return false; }
//     return html_is_host_including_inclusive_ancestor(a, b_root_host);
// }

// https://dom.spec.whatwg.org/#concept-node-ensure-pre-insertion-validity
static html_dom_exception_e html_check_pre_insert_validity(html_node_t* parent, html_node_t* node, html_node_t* child)
{
    // html_node_type_e p_type = parent->type;
    // html_node_type_e n_type = node->type;

    // if (p_type != HTML_NODE_DOCUMENT && p_type != HTML_NODE_DOCUMENT_FRAGMENT && p_type != HTML_NODE_ELEMENT)
    // {
    //     return HTML_DOM_ERR_HIERARCHY_REQUEST;
    // }
    // if (html_is_host_including_inclusive_ancestor(node, parent))
    // {
    //     return HTML_DOM_ERR_HIERARCHY_REQUEST;
    // }
    // if (child && child->parent != parent)
    // {
    //     return HTML_DOM_ERR_NOT_FOUND;
    // }

    // if (n_type != HTML_NODE_DOCUMENT_FRAGMENT && 
    //     n_type != HTML_NODE_DOCUMENT_TYPE && 
    //     n_type != HTML_NODE_ELEMENT && 
    //     n_type != HTML_NODE_TEXT &&
    //     n_type != HTML_NODE_PROCESSING_INSTRUCTION &&
    //     n_type != HTML_NODE_COMMENT)
    // {
    //     return HTML_DOM_ERR_HIERARCHY_REQUEST;
    // }

    // if ((n_type == HTML_NODE_TEXT && p_type == HTML_NODE_DOCUMENT) ||
    //     (n_type == HTML_NODE_DOCUMENT_TYPE && p_type != HTML_NODE_DOCUMENT))
    // {
    //     return HTML_DOM_ERR_HIERARCHY_REQUEST;
    // }

    // if (p_type == HTML_NODE_DOCUMENT)
    // {
    //     bool raise = false;

    //     if (n_type == HTML_NODE_DOCUMENT_FRAGMENT)
    //     {
    //         uint32_t n_element_nodes        = html_count_children_of_type(node, HTML_NODE_ELEMENT);
    //         uint32_t n_text_nodes           = html_count_children_of_type(node, HTML_NODE_TEXT);
    //         uint32_t p_element_nodes        = html_count_children_of_type(parent, HTML_NODE_ELEMENT);
    //         bool child_is_doctype           = child && child->type == HTML_NODE_DOCUMENT_TYPE;
    //         bool following_child_is_doctype = html_count_following_children_of_type(child, HTML_NODE_DOCUMENT_TYPE) > 0;

    //         if (n_element_nodes > 1 || n_text_nodes > 0)
    //         {
    //             raise = true;
    //         }
    //         else if (n_element_nodes > 0 &&  (p_element_nodes > 0 || child_is_doctype || following_child_is_doctype))
    //         {
    //             raise = true;
    //         }
    //     }
    //     else if (n_type == HTML_NODE_ELEMENT)
    //     {
    //         uint32_t p_element_nodes        = html_count_children_of_type(parent, HTML_NODE_ELEMENT);
    //         bool child_is_doctype           = child && child->type == HTML_NODE_DOCUMENT_TYPE;
    //         bool following_child_is_doctype = html_count_following_children_of_type(child, HTML_NODE_DOCUMENT_TYPE) > 0;

    //         if (p_element_nodes > 0 || child_is_doctype || following_child_is_doctype)
    //         {
    //             raise = true;
    //         }
    //     }
    //     else if (n_type == HTML_NODE_DOCUMENT_TYPE)
    //     {
    //         uint32_t p_doctype_nodes        = html_count_children_of_type(parent, HTML_NODE_DOCUMENT_TYPE);
    //         uint32_t p_element_nodes        = html_count_children_of_type(parent, HTML_NODE_ELEMENT);
    //         uint32_t c_element_nodes        = html_count_preceding_children_of_type(child, HTML_NODE_ELEMENT);

    //         if (p_doctype_nodes > 0 || c_element_nodes > 0 || (!child && p_element_nodes > 0))
    //         {
    //             raise = true;
    //         }
    //     }

    //     if (raise) { return HTML_DOM_ERR_HIERARCHY_REQUEST; }
    // }
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

/********************/
/* public functions */
/********************/


html_node_t* html_node_new(html_node_type_e type, html_node_t* document)
{
    html_node_t* node = malloc(sizeof(html_node_t));

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

    return node;
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
        html_node_free(child);
        child = prev;
    }
    free(node);
}
