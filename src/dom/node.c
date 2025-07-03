#include "node.h"

#include <stdlib.h>
#include <assert.h>

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

    if (!child) { return result; }

    html_node_t* child = node->children;
    while (child)
    {
        if (child->type == type) { result++; }
        child = child->next_sibling;
    }
    return result
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
    if (node == HTML_NODE_DOCUMENT_FRAGMENT) { return node->document_fragment->host; }
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

    return is_host_including_inclusive_ancestor(a, b_root_host);
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

    if (n_type != HTML_NODE_DOCUMENT_FRAGMENT && n_type != HTML_NODE_DOCUMENT_TYPE && n_type != HTML_NODE_ELEMENT && n_type != HTML_NODE_CHARACTER_DATA)
    {
        return HTML_DOM_ERR_HIERARCHY_REQUEST;
    }

    if ((n_type == HTML_NODE_TEXT && p_type == HTML_NODE_DOCUMENT) || (n_type == HTML_NODE_DOCUMENT_TYPE && p_type != HTML_NODE_DOCUMENT)
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
}


static void run_live_range_pre_remove_steps(html_node_t* node)
{
    html_node_t* parent = node->parent;
    assert(parent);

    // uint32_t index = get_node_index(node);

}


static void html_remove_node(html_node_t* node, bool suppress_observers)
{
    html_node_t* parent = node->parent;
    if (!parent) { return; }

    run_live_range_pre_remove_steps(node);
}


static void html_adopt_node(html_node_t* node, html_node_t* document)
{
    assert(document->type == HTML_NODE_DOCUMENT);
}


static void html_insert_node(html_node_t* parent, html_node_t* node, html_node_t* child, bool suppress_observers)
{
    html_node_t* nodes = node;
    uint32_t count = 1;

    if (node->type == HTML_NODE_DOCUMENT_FRAGMENT)
    {
        nodes = node->children;
        count = node->children_size;
    }

    if (count == 0) { return; }

    if (node->type == HTML_NODE_DOCUMENT_FRAGMENT)
    {
        // remove children with supress flag on
        // queue tree mutation record
        assert(false);
    }

    if (child)
    {
        // For each live range whose start node is parent and start offset is greater than child’s index, increase its start offset by count.
        // For each live range whose end node is parent and end offset is greater than child’s index, increase its end offset by count.
    }

    html_node_t* prev_sibling = child ? child->prev_sibling : parent->last_child;

    for (uint32_t i = 0; i < count; i++)
    {
        html_node_t* current = nodes[i];

        html_adopt_node(current, parent->document);
        if (!child)
        {
            bool insert = true;
            html_node_t* temp = parent->first_child;
            while (temp)
            {
                if (temp == current) { insert = false; }
                temp = temp->next_sibling;
            }

            html_node_t* last_child = parent->last_child;
            if (last_child)
            {
                last_child->next_sibling = current;
                node->prev_sibling = last_child;
            }
            else
            {
                parent->first_child = current;
                parent->children = current;
            }
            parent->last_child = current;
            parent->children_size++;
        }
        else
        {
            html_node_t* c_prev_sibling = child->prev_sibling;
            child->prev_sibling = current;
            current->next_sibling = child;
            if (c_prev_sibling)
            {
                c_prev_sibling->next_sibling = current;
                current->prev_sibling = c_prev_sibling;
            }
            else
            {
                parent->children = current;
                parent->first_child = current;
            }
            parent->children_size++;
        }
    }
}


static void html_pre_insert_node(html_node_t* parent, html_node_t* node, html_node_t* child)
{
    if (!html_check_pre_insert_validity(parent, node, child)) { return; }

    html_node_t* ref_child = child;
    if (ref_child == node) { ref_child = node->next_sibling; }

    html_insert_node(parent, node, ref_child, false);
}

static html_node_t* html_node_new(html_node_t* document)
{
    html_node_t* node = malloc(sizeof(html_node_t));

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
    comment->data = string_new(buffer, size);

    return comment;
}


static html_node_document_t* html_node_document_new()
{
    html_node_document_t* document = malloc(sizeof(html_node_document_t));

    return document;
}


static html_node_doctype_t* html_node_doctype_new(unsigned char* name, uint32_t name_size,
                                                  unsigned char* public_id, uint32_t public_id_size,
                                                  unsigned char* system_id, uint32_t system_id_size)
{
    html_node_doctype_t* doctype = malloc(sizeof(html_node_doctype_t));
    doctype->name = string_new(name, name_size);
    doctype->public_id = string_new(public_id, public_id_size);
    doctype->system_id = string_new(system_id, system_id_size);

    return doctype;
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
    node->comment = html_node_comment_new(buffer, size);

    return node;
}


void html_comment_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_COMMENT);

    free(node->comment);
    html_node_free(node);
}


html_node_t* html_document_new()
{
    html_node_t* node = html_node_new();
    node->type = HTML_NODE_DOCUMENT;
    node->document = html_node_document_new();

    return node;
}


void html_document_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_DOCUMENT);

    free(node->document);
    html_node_free(node);
}


html_node_t* html_doctype_new(unsigned char* name, uint32_t name_size, 
                              unsigned char* public_id, uint32_t public_id_size, 
                              unsigned char* system_id, uint32_t system_id_size)
{
    html_node_t* node = html_node_new(document);
    node->type = HTML_NODE_DOCUMENT_TYPE;
    node->doctype = html_node_doctype_new();
}


void html_doctype_free(html_node_t* node)
{
    assert(node->type == HTML_NODE_DOCUMENT_TYPE);

    free(node->doctype);
    html_node_free(node);
}
