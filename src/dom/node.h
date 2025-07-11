#pragma once

#include <stdbool.h>

#include "html/constants.h"
#include "core/string.h"
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


// https://dom.spec.whatwg.org/#interface-comment
typedef struct
{
    string_t data;
} html_node_comment_t;


// https://dom.spec.whatwg.org/#documenttype
typedef struct
{
    string_t name;
    string_t public_id;
    string_t system_id;
} html_node_doctype_t;


// https://dom.spec.whatwg.org/#interface-document
typedef struct
{
    string_t url;
    string_t uri;
    string_t compat_mode;
    string_t character_set;
    string_t content_type;

    html_node_t* doctype;

    bool parser_cannot_change_mode;
} html_node_document_t;

// https://dom.spec.whatwg.org/#element
typedef struct
{
    string_t namespace;
    string_t prefix;
    string_t local_name;
    string_t tag_name;

    string_t id;
    string_t class_name;
} html_node_element_t;


// https://dom.spec.whatwg.org/#interface-text
typedef struct
{
    string_t data;
} html_node_text_t*;


// https://dom.spec.whatwg.org/#interface-node
typedef struct html_node_t
{
    html_node_type_e    type;
    string_t            name;

    char                base_uri[MAX_HTML_NAME_LEN];

    bool                is_connected;
    struct html_node_t* owner;
    struct html_node_t* parent; // also used for parent_element

    struct html_node_t* children;
    uint32_t            children_size;
    struct html_node_t* first_child;
    struct html_node_t* last_child;
    struct html_node_t* prev_sibling;
    struct html_node_t* next_sibling;

    char                value[MAX_HTML_NAME_LEN];
    char                text[MAX_HTML_NAME_LEN];

    union
    {
        html_node_document_t*                   document;
        html_node_doctype_t*                    doctype;
        // html_node_doc_frag_t*                   document_fragment;
        // html_node_shadow_root_t*                shadow_root;
        html_node_element_t*                    element;
        // html_node_character_data_t*             character_data;
        html_node_text_t*                       text;
        // html_node_cdata_section_t*              cdata_section;
        // html_node_processing_instruction_t*     processing_instruction;
        html_node_comment_t*                    comment;
    };

} html_node_t;

bool                        html_node_has_children(html_node_t* node);
void                        html_node_normalize(html_node_t* node);
html_node_t*                html_node_clone(html_node_t* node, bool subtree);
bool                        html_node_is_equal(html_node_t* node, html_node_t* other);
bool                        html_node_is_same(html_node_t* node, html_node_t* other);
bool                        html_node_contains(html_node_t* node, html_node_t* other);
html_document_position_e    html_node_compare_document_position(html_node_t* node, html_node_t* other);
bool                        html_node_insert_before(html_node_t* node, html_node_t* new_node, html_node_t* child);
void                        html_node_append(html_node_t* node, html_node_t* new_node);
bool                        html_node_replace(html_node_t* node, html_node_t* new_node, html_node_t* child);
bool                        html_node_remove(html_node_t* node, html_node_t* child);

// comment
html_node_t*                html_comment_new(unsigned char* buffer, uint32_t size, html_node_t* document);
void                        html_comment_free(html_node_t* node);

// document
html_node_t*                html_document_new();
void                        html_document_free(html_node_t* node);

// doctype
html_node_t*                html_doctype_new();
void                        html_doctype_free(html_node_t* node);

// element
html_node_t*                html_element_new(html_node_t* document, string_t local_name, string_t namespace);
void                        html_element_free(html_node_t* node);

// text
html_node_t*                html_text_new(html_node_t* document, string_t data);
void                        html_text_free(html_node_t* node);