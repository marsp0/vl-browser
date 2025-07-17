#include "test_parser_utils.h"

void assert_node(html_node_t* a, html_node_t* e)
{
    ASSERT_EQUAL(a->type, e->type);
    ASSERT_EQUAL(a->name_size, e->name_size);
    ASSERT_STRING((char)a->name, (char)e->name, e->name_size);

    html_node_t* a_child = a->first_child;
    html_node_t* e_child = e->first_child;
    while (a_child || e_child)
    {
        assert_node(a_child, e_child);
        a_child = a_child->next_sibling;
        e_child = e_child->next_sibling;
    }
}