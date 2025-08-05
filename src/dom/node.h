#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "html/constants.h"
#include "dom/exception.h"

typedef enum
{
  HTML_DOCUMENT_POSITION_DISCONNECTED,
  HTML_DOCUMENT_POSITION_PRECEDING,
  HTML_DOCUMENT_POSITION_FOLLOWING,
  HTML_DOCUMENT_POSITION_CONTAINS,
  HTML_DOCUMENT_POSITION_CONTAINED_BY,
  HTML_DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC
} html_document_position_e;


// https://dom.spec.whatwg.org/#interface-node
typedef enum
{
    HTML_NODE_INVALID,                  // DO NOT USE
    HTML_NODE_ELEMENT,
    HTML_NODE_ATTRIBUTE,
    HTML_NODE_TEXT,
    HTML_NODE_CDATA_SECTION,
    HTML_NODE_ENTITY_REFERENCE,         // legacy
    HTML_NODE_ENTITY,                   // legacy
    HTML_NODE_PROCESSING_INSTRUCTION,
    HTML_NODE_COMMENT,
    HTML_NODE_DOCUMENT,
    HTML_NODE_DOCUMENT_TYPE,
    HTML_NODE_DOCUMENT_FRAGMENT,
    HTML_NODE_NOTATION                  // legacy
} html_node_type_e;


// https://dom.spec.whatwg.org/#interface-node
typedef struct html_node_t
{
    html_node_type_e        type;

    unsigned char           name[MAX_HTML_NAME_LEN];
    uint32_t                name_size;

    unsigned char           base_uri[MAX_HTML_NAME_LEN];
    uint32_t                base_uri_size;

    bool                    is_connected;
    struct html_node_t*     document;
    struct html_node_t*     parent;                         // also used for parent_element

    struct html_node_t*     first_child;
    struct html_node_t*     last_child;
    struct html_node_t*     prev_sibling;
    struct html_node_t*     next_sibling;

    void*                   data;

} html_node_t;


html_node_t*                html_node_new(html_node_type_e type, html_node_t* document);
bool                        html_node_insert_before(html_node_t* node, html_node_t* new_node, html_node_t* child);
html_node_t*                html_node_append(html_node_t* node, html_node_t* new_node);
bool                        html_node_replace(html_node_t* node, html_node_t* new_node, html_node_t* child);
html_node_t*                html_node_remove(html_node_t* node, html_node_t* child);
void                        html_node_free(html_node_t* node);
