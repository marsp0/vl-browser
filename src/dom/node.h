#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "html/constants.h"
#include "dom/exception.h"
#include "dom/hash_str.h"

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
typedef enum
{
    DOM_NODE_INVALID,                  // DO NOT USE
    DOM_NODE_ELEMENT,
    DOM_NODE_ATTRIBUTE,
    DOM_NODE_TEXT,
    DOM_NODE_CDATA_SECTION,
    DOM_NODE_ENTITY_REFERENCE,         // legacy
    DOM_NODE_ENTITY,                   // legacy
    DOM_NODE_PROCESSING_INSTRUCTION,
    DOM_NODE_COMMENT,
    DOM_NODE_DOCTYPE,
    DOM_NODE_DOCUMENT,
    DOM_NODE_DOCUMENT_TYPE,
    DOM_NODE_DOCUMENT_FRAGMENT,
    DOM_NODE_NOTATION                  // legacy
} dom_node_type_e;


// https://dom.spec.whatwg.org/#interface-node
typedef struct dom_node_t
{
    dom_node_type_e         type;
    hash_str_t              name;
    hash_str_t              base_uri;
    bool                    is_connected;

    struct dom_node_t*      document;
    struct dom_node_t*      parent;                         // also used for parent_element

    struct dom_node_t*      first_child;
    struct dom_node_t*      last_child;
    struct dom_node_t*      prev_sibling;
    struct dom_node_t*      next_sibling;

} dom_node_t;


void            dom_node_initialize(dom_node_t* node, dom_node_type_e type, dom_node_t* document);
dom_node_t*     dom_node_insert_before(dom_node_t* node, dom_node_t* new_node, dom_node_t* child);
dom_node_t*     dom_node_append(dom_node_t* node, dom_node_t* new_node);
bool            dom_node_replace(dom_node_t* node, dom_node_t* new_node, dom_node_t* child);
dom_node_t*     dom_node_remove(dom_node_t* node, dom_node_t* child);
void            dom_node_free(dom_node_t* node);
