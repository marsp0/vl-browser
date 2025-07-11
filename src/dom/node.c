#include "node.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

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

static uint32_t html_count_children_of_type(html_node_t* node, html_node_type_e type)
{
    uint32_t result = 0;

    html_node_t* child = node->children;
    while (child)
    {
        if (child->type == type) { result++; }
        child = child->next_sibling;
    }
    return result;
}

static uint32_t html_count_following_children_of_type(html_node_t* node, html_node_type_e type)
{
    uint32_t result = 0;

    if (!node) { return result; }

    html_node_t* sibling = node->next_sibling;
    while (sibling)
    {
        if (sibling->type == type) { result++; }
        sibling = sibling->next_sibling;
    }
    return result;
}

static uint32_t html_count_preceding_children_of_type(html_node_t* node, html_node_type_e type)
{
    uint32_t result = 0;

    if (!node) { return result; }

    html_node_t* sibling = node->prev_sibling;
    while (sibling)
    {
        if (sibling->type == type) { result++; }
        sibling = sibling->prev_sibling;
    }
    return result;
}

static html_node_t* html_get_host(html_node_t* node)
{
    if (node->type == HTML_NODE_DOCUMENT_FRAGMENT) { return NULL; }
    return NULL;
}

static html_node_t* html_get_root(html_node_t* node)
{
    html_node_t* result = node->parent;
    while (result) { result = result->parent; }

    return result;
}

// https://dom.spec.whatwg.org/#concept-tree-descendant
static bool html_is_descendant(html_node_t* a, html_node_t* b)
{
    html_node_t* temp = a->parent;
    while (temp)
    {
        if (temp == b) { return true; }
        temp = temp->parent;
    }

    return false;
}

// https://dom.spec.whatwg.org/#concept-tree-ancestor
static bool html_is_ancestor(html_node_t* a, html_node_t* b)
{
    return html_is_descendant(b, a);
}

// https://dom.spec.whatwg.org/#concept-tree-inclusive-ancestor
static bool html_is_inclusive_ancestor(html_node_t* a, html_node_t* b)
{
    return (a == b) || html_is_ancestor(a, b);
}

// https://dom.spec.whatwg.org/#concept-tree-host-including-inclusive-ancestor
static bool html_is_host_including_inclusive_ancestor(html_node_t* a, html_node_t* b)
{
    // 1. if A is an inclusive ancestor of B
    if (html_is_inclusive_ancestor(a, b)) { return true; }

    // 2. if B’s root has a non-null host and A is a host-including inclusive ancestor of B’s root’s host
    html_node_t* b_root         = html_get_root(b);
    html_node_t* b_root_host    = html_get_host(b_root);
    if (!b_root_host) { return false; }

    return html_is_host_including_inclusive_ancestor(a, b_root_host);
}

// https://dom.spec.whatwg.org/#concept-node-ensure-pre-insertion-validity
static html_dom_exception_e html_check_pre_insert_validity(html_node_t* parent, html_node_t* node, html_node_t* child)
{
    html_node_type_e p_type = parent->type;
    html_node_type_e n_type = node->type;

    if (p_type != HTML_NODE_DOCUMENT && p_type != HTML_NODE_DOCUMENT_FRAGMENT && p_type != HTML_NODE_ELEMENT)
    {
        return HTML_DOM_ERR_HIERARCHY_REQUEST;
    }

    if (html_is_host_including_inclusive_ancestor(node, parent))
    {
        return HTML_DOM_ERR_HIERARCHY_REQUEST;
    }

    if (child && child->parent != parent)
    {
        return HTML_DOM_ERR_NOT_FOUND;
    }

    if (n_type != HTML_NODE_DOCUMENT_FRAGMENT && 
        n_type != HTML_NODE_DOCUMENT_TYPE && 
        n_type != HTML_NODE_ELEMENT && 
        n_type != HTML_NODE_TEXT &&
        n_type != HTML_NODE_PROCESSING_INSTRUCTION &&
        n_type != HTML_NODE_COMMENT)
    {
        return HTML_DOM_ERR_HIERARCHY_REQUEST;
    }

    if ((n_type == HTML_NODE_TEXT && p_type == HTML_NODE_DOCUMENT) ||
        (n_type == HTML_NODE_DOCUMENT_TYPE && p_type != HTML_NODE_DOCUMENT))
    {
        return HTML_DOM_ERR_HIERARCHY_REQUEST;
    }

    if (p_type == HTML_NODE_DOCUMENT)
    {
        bool raise = false;

        if (n_type == HTML_NODE_DOCUMENT_FRAGMENT)
        {
            uint32_t n_element_nodes        = html_count_children_of_type(node, HTML_NODE_ELEMENT);
            uint32_t n_text_nodes           = html_count_children_of_type(node, HTML_NODE_TEXT);
            uint32_t p_element_nodes        = html_count_children_of_type(parent, HTML_NODE_ELEMENT);
            bool child_is_doctype           = child && child->type == HTML_NODE_DOCUMENT_TYPE;
            bool following_child_is_doctype = html_count_following_children_of_type(child, HTML_NODE_DOCUMENT_TYPE) > 0;

            if (n_element_nodes > 1 || n_text_nodes > 0)
            {
                raise = true;
            }
            else if (n_element_nodes > 0 &&  (p_element_nodes > 0 || child_is_doctype || following_child_is_doctype))
            {
                raise = true;
            }
        }
        else if (n_type == HTML_NODE_ELEMENT)
        {
            uint32_t p_element_nodes        = html_count_children_of_type(parent, HTML_NODE_ELEMENT);
            bool child_is_doctype           = child && child->type == HTML_NODE_DOCUMENT_TYPE;
            bool following_child_is_doctype = html_count_following_children_of_type(child, HTML_NODE_DOCUMENT_TYPE) > 0;

            if (p_element_nodes > 0 || child_is_doctype || following_child_is_doctype)
            {
                raise = true;
            }
        }
        else if (n_type == HTML_NODE_DOCUMENT_TYPE)
        {
            uint32_t p_doctype_nodes        = html_count_children_of_type(parent, HTML_NODE_DOCUMENT_TYPE);
            uint32_t p_element_nodes        = html_count_children_of_type(parent, HTML_NODE_ELEMENT);
            uint32_t c_element_nodes        = html_count_preceding_children_of_type(child, HTML_NODE_ELEMENT);

            if (p_doctype_nodes > 0 || c_element_nodes > 0 || (!child && p_element_nodes > 0))
            {
                raise = true;
            }
        }

        if (raise) { return HTML_DOM_ERR_HIERARCHY_REQUEST; }
    }

    return HTML_DOM_ERR_OK;
}


static void html_insert_node(html_node_t* parent, html_node_t* node, html_node_t* child, bool suppress_observers)
{
    assert(parent);
    assert(node);
    assert(child);
    assert(suppress_observers);
}


static void html_pre_insert_node(html_node_t* parent, html_node_t* node, html_node_t* child)
{
    if (!html_check_pre_insert_validity(parent, node, child)) { return; }

    html_node_t* ref_child = child;
    if (ref_child == node) { ref_child = node->next_sibling; }

    html_insert_node(parent, node, ref_child, false);
}


static html_node_t* html_node_new_internal()
{
    html_node_t* node = malloc(sizeof(html_node_t));
    return node;
}


static html_node_t* html_node_new(html_node_t* document)
{
    html_node_t* node = html_node_new_internal();
    node->document = document;

    return node;
}

static void html_node_free(html_node_t* node)
{
    free(node);
}

static html_node_comment_t* html_node_comment_new(unsigned char* buffer, uint32_t size)
{
    html_node_comment_t* comment = malloc(sizeof(html_node_comment_t));
    memcpy(comment->data, buffer, size);

    return comment;
}


static html_node_document_t* html_node_document_new()
{
    html_node_document_t* document = malloc(sizeof(html_node_document_t));

    document->parser_cannot_change_mode = false;

    return document;
}


static html_node_doctype_t* html_node_doctype_new(unsigned char* name, uint32_t name_size,
                                                  unsigned char* public_id, uint32_t public_id_size,
                                                  unsigned char* system_id, uint32_t system_id_size)
{
    html_node_doctype_t* doctype = malloc(sizeof(html_node_doctype_t));
    memcpy(doctype->name, name, name_size);
    memcpy(doctype->public_id, public_id, public_id_size);
    memcpy(doctype->system_id, system_id, system_id_size);

    return doctype;
}


static html_node_element_t* html_node_element_new_internal(unsigned char* local_name, uint32_t local_name_size)
{
    html_node_element_t* element = malloc(sizeof(html_node_element_t));
    memcpy(element->local_name, local_name, local_name_size);

    return element;
}


static html_node_element_t* html_node_element_new(unsigned char* local_name, uint32_t local_name_size)
{
    html_node_element_t* result = NULL;

    // todo: step 2
    // todo: step 3
    // todo: step 4
    // todo: step 5

    result = html_node_element_new_internal(local_name, local_name_size);

    // todo: step 6.3 - finish

    return result;
}


static html_node_text_t* html_node_text_new(unsigned char* data, uint32_t data_size)
{
    html_node_text_t* text = malloc(sizeof(html_node_text_t));
    memcpy(text->data, data, data_size);

    return text;
}

/********************/
/* public functions */
/********************/

void html_node_append(html_node_t* node, html_node_t* new_node)
{
    html_pre_insert_node(node, new_node, NULL);
}


html_node_t* html_comment_new(unsigned char* buffer, uint32_t size, html_node_t* document)
{
    html_node_t* node = html_node_new(document);
    node->type = HTML_NODE_COMMENT;
    node->comment_data = html_node_comment_new(buffer, size);

    return node;
}


void html_comment_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_COMMENT);

    free(node->comment_data);
    html_node_free(node);
}


html_node_t* html_document_new()
{
    html_node_t* node = html_node_new_internal();
    node->document = node;

    node->type = HTML_NODE_DOCUMENT;
    node->document_data = html_node_document_new();

    return node;
}


void html_document_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_DOCUMENT);

    free(node->document_data);
    html_node_free(node);
}


html_node_t* html_doctype_new(html_node_t* document,
                              unsigned char* name, uint32_t name_size, 
                              unsigned char* public_id, uint32_t public_id_size, 
                              unsigned char* system_id, uint32_t system_id_size)
{
    html_node_t* node = html_node_new(document);
    node->type = HTML_NODE_DOCUMENT_TYPE;
    node->doctype_data = html_node_doctype_new(name, name_size, public_id, public_id_size, system_id, system_id_size);

    return node;
}


void html_doctype_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_DOCUMENT_TYPE);

    free(node->doctype_data);
    html_node_free(node);
}


html_node_t* html_element_new(html_node_t* document, unsigned char* local_name, uint32_t local_name_size)
{
    html_node_t* node = html_node_new(document);
    node->type = HTML_NODE_ELEMENT;
    node->element_data = html_node_element_new(local_name, local_name_size);

    return node;
}


void html_element_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_ELEMENT);

    free(node->doctype_data);
    html_node_free(node);
}


html_node_t* html_text_new(html_node_t* document, unsigned char* data, uint32_t data_size)
{
    html_node_t* node = html_node_new(document);
    node->type = HTML_NODE_TEXT;
    node->text_data = html_node_text_new(data, data_size);

    return node;
}


void html_text_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_TEXT);

    free(node->text_data);
    html_node_free(node);
}
