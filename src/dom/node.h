#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "html/constants.h"
#include "dom/exception.h"
#include "dom/hash_str.h"
#include "dom/types.h"

typedef enum
{
    DOM_DOCUMENT_POSITION_DISCONNECTED,
    DOM_DOCUMENT_POSITION_PRECEDING,
    DOM_DOCUMENT_POSITION_FOLLOWING,
    DOM_DOCUMENT_POSITION_CONTAINS,
    DOM_DOCUMENT_POSITION_CONTAINED_BY,
    DOM_DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC
} dom_document_position_e;


// https://dom.spec.whatwg.org/#interface-node
typedef struct dom_node_t
{
    uint64_t                type;
    hash_str_t              name;
    hash_str_t              base_uri;
    bool                    is_connected;

    struct dom_node_t*      document;
    struct dom_node_t*      parent;                         // also used for parent_element

    struct dom_node_t*      first;
    struct dom_node_t*      last;
    struct dom_node_t*      prev;
    struct dom_node_t*      next;

} dom_node_t;


void            dom_node_initialize(dom_node_t* node, uint64_t type, dom_node_t* document);

// add / remove
dom_node_t*     dom_node_insert_before(dom_node_t* node, dom_node_t* new_node, dom_node_t* child);
dom_node_t*     dom_node_append(dom_node_t* node, dom_node_t* new_node);
bool            dom_node_replace(dom_node_t* node, dom_node_t* new_node, dom_node_t* child);
dom_node_t*     dom_node_remove(dom_node_t* node, dom_node_t* child);

// tree utility
dom_node_t*     dom_node_root(dom_node_t* node);
bool            dom_node_is_desc(dom_node_t* b, dom_node_t* a);
bool            dom_node_is_incl_desc(dom_node_t* b, dom_node_t* a);
bool            dom_node_is_ancstr(dom_node_t* b, dom_node_t* a);
bool            dom_node_is_incl_ancstr(dom_node_t* b, dom_node_t* a);
bool            dom_node_is_sibl(dom_node_t* b, dom_node_t* a);

void            dom_node_free(dom_node_t* node);
