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
            if (attr->value == 0)
            {
                printf("#attr - %.*s=""\n", attr_name_size, attr_name);
            }
            else
            {
                const unsigned char* val_name = hash_str_get(attr->value);
                const uint32_t val_name_size = hash_str_get_size(attr->value);
                printf("#attr - %.*s=%.*s\n", attr_name_size, attr_name, val_name_size, val_name);
            }
            attr = attr->next;
        }
    }
    else if (dom_node_is_document(node))
    {
        printf("#document\n");
        dom_document_t* doc = dom_document_from_node(node);
        if (doc->doctype)
        {
            for (uint32_t i = 0; i < level + 1; i++)
            {
                printf("  ");
            }

            printf("DOCTYPE %s\n", hash_str_get(doc->doctype->name));
        }
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