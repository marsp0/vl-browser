#include "test_parser_utils.h"

void print_document_tree(dom_node_t* node, uint32_t level)
{
    for (uint32_t i = 0; i < level; i++)
    {
        printf("  ");
    }

    if (dom_node_is_element(node))
    {
        dom_element_t* element = dom_element_from_node(node);
        const unsigned char* name = hash_str_get(element->local_name);
        const uint32_t name_size = hash_str_get_size(element->local_name);
        printf("%.*s\n", name_size, name);

        dom_attr_t* attr = element->attr;

        for (uint32_t i = 0; i < element->attr_size; i++)
        {
            for (uint32_t j = 0; j < level; j++)
            {
                printf("  ");
            }
            printf("  ");

            const unsigned char* attr_name = hash_str_get(attr->name);
            const uint32_t attr_name_size = hash_str_get_size(attr->name);
            printf("#attr - %.*s\n", attr_name_size, attr_name);
            attr = attr->next;
        }
    }
    else if (dom_node_is_document(node))
    {
        printf("#document\n");
    }
    else if (dom_node_is_text(node))
    {
        dom_text_t* text = dom_text_from_node(node);
        printf("#text - \"%s\"\n", text->data);
    }
    else if (dom_node_is_comment(node))
    {
        dom_comment_t* comment = dom_comment_from_node(node);
        printf("<!-- %s -->\n", comment->data);
    }

    dom_node_t* child = node->first;
    while (child)
    {
        print_document_tree(child, level + 1);
        child = child->next;
    }
}