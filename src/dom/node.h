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


// https://dom.spec.whatwg.org/#interface-comment
typedef struct
{
    unsigned char   data[MAX_HTML_NAME_LEN];
    uint32_t        data_size;
} html_node_comment_t;


// https://dom.spec.whatwg.org/#documenttype
typedef struct
{
    unsigned char   name[MAX_HTML_NAME_LEN];
    uint32_t        name_size;
    unsigned char   public_id[MAX_HTML_NAME_LEN];
    uint32_t        public_id_size;
    unsigned char   system_id[MAX_HTML_NAME_LEN];
    uint32_t        system_id_size;
} html_node_doctype_t;


struct html_node_t;
// https://dom.spec.whatwg.org/#interface-document
typedef struct
{
    unsigned char url[MAX_HTML_NAME_LEN];
    uint32_t        url_size;
    unsigned char uri[MAX_HTML_NAME_LEN];
    uint32_t        uri_size;
    unsigned char compat_mode[MAX_HTML_NAME_LEN];
    uint32_t        compat_mode_size;
    unsigned char character_set[MAX_HTML_NAME_LEN];
    uint32_t        character_set_size;
    unsigned char content_type[MAX_HTML_NAME_LEN];
    uint32_t        content_type_size;

    struct html_node_t* doctype;

    bool parser_cannot_change_mode;
} html_node_document_t;

// https://dom.spec.whatwg.org/#element
typedef struct
{
    unsigned char namespace[MAX_HTML_NAME_LEN];
    uint32_t        namespace_size;
    unsigned char prefix[MAX_HTML_NAME_LEN];
    uint32_t        prefix_size;
    unsigned char local_name[MAX_HTML_NAME_LEN];
    uint32_t        local_name_size;
    unsigned char tag_name[MAX_HTML_NAME_LEN];
    uint32_t        tag_name_size;

    unsigned char id[MAX_HTML_NAME_LEN];
    uint32_t        id_size;
    unsigned char class_name[MAX_HTML_NAME_LEN];
    uint32_t        class_name_size;
} html_node_element_t;


// https://dom.spec.whatwg.org/#interface-text
typedef struct
{
    unsigned char data[MAX_HTML_NAME_LEN];
    uint32_t data_size;
} html_node_text_t;


// https://dom.spec.whatwg.org/#interface-node
typedef struct html_node_t
{
    html_node_type_e    type;
    unsigned char            name[MAX_HTML_NAME_LEN];
    uint32_t            name_size;

    unsigned char       base_uri[MAX_HTML_NAME_LEN];
    uint32_t            base_uri_size;

    bool                is_connected;
    struct html_node_t* document;
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
        html_node_document_t*                   document_data;
        html_node_doctype_t*                    doctype_data;
        // html_node_doc_frag_t*                   document_fragment;
        // html_node_shadow_root_t*                shadow_root;
        html_node_element_t*                    element_data;
        // html_node_character_data_t*             character_data;
        html_node_text_t*                       text_data;
        // html_node_cdata_section_t*              cdata_section;
        // html_node_processing_instruction_t*     processing_instruction;
        html_node_comment_t*                    comment_data;
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
html_node_t*                html_element_new(html_node_t* document, unsigned char* local_name, uint32_t local_name_size);
void                        html_element_free(html_node_t* node);

// text
html_node_t*                html_text_new(html_node_t* document, unsigned char* data, uint32_t data_size);
void                        html_text_free(html_node_t* node);