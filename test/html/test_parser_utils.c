#include "test_parser_utils.h"

void print_document_tree(html_node_t* node, uint32_t level)
{
    for (uint32_t i = 0; i < level; i++)
    {
        printf("  ");
    }

    html_node_type_e type = node->type;
    if (type == HTML_NODE_ELEMENT)
    {
        html_element_t* element = html_element_from_node(node);
        printf("%s\n", element->local_name);
    }
    else if (type == HTML_NODE_DOCUMENT)
    {
        printf("#document\n");
    }
    else if (type == HTML_NODE_TEXT)
    {
        html_text_t* element = html_text_from_node(node);
        printf("#text - %s\n", element->data);
    }
    else if (type == HTML_NODE_COMMENT)
    {
        html_comment_t* comment = html_comment_from_node(node);
        printf("<!-- %s -->\n", comment->data);
    }

    html_node_t* child = node->first_child;
    while (child)
    {
        print_document_tree(child, level + 1);
        child = child->next_sibling;
    }
}