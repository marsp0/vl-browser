#include "test_parser_utils.h"

void print_document_tree(html_node_t* node, uint32_t level)
{
    for (uint32_t i = 0; i < level; i++)
    {
        printf("\t");
    }
    printf("%s\n", node->name);

    html_node_t* child = node->first_child;
    while (child)
    {
        print_document_tree(child, level + 1);
        child = child->next_sibling;
    }
}