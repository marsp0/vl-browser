#include "test_parser_utils.h"

void print_document_tree(dom_node_t* node, uint32_t level)
{
    for (uint32_t i = 0; i < level; i++)
    {
        printf("  ");
    }

    dom_node_type_e type = node->type;
    if (type == DOM_NODE_ELEMENT)
    {
        dom_element_t* element = dom_element_from_node(node);
        const unsigned char* name = hash_str_get(element->local_name);
        const uint32_t name_size = hash_str_get_size(element->local_name);
        printf("%.*s\n", name_size, name);
    }
    else if (type == DOM_NODE_DOCUMENT)
    {
        printf("#document\n");
    }
    else if (type == DOM_NODE_TEXT)
    {
        dom_text_t* text = dom_text_from_node(node);
        printf("#text - %s\n", text->data);
    }
    else if (type == DOM_NODE_COMMENT)
    {
        dom_comment_t* comment = dom_comment_from_node(node);
        printf("<!-- %s -->\n", comment->data);
    }

    dom_node_t* child = node->first_child;
    while (child)
    {
        print_document_tree(child, level + 1);
        child = child->next_sibling;
    }
}