#include "input.h"

#include <stdlib.h>
#include <assert.h>

#include "html/tag_constants.h"

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/


/********************/
/* static functions */
/********************/


/********************/
/* public functions */
/********************/

dom_node_t* html_input_new(dom_node_t* document, hash_str_t namespace)
{
    html_input_t* input = malloc(sizeof(html_input_t));

    html_element_init((html_element_t*)input, document, html_tag_input(), namespace);

    input->align = 0;
    input->use_map = 0;

    return html_node_from_input(input);
}


bool html_node_is_input(dom_node_t* node)
{
    return node->type & HTML_NODE_INPUT;
}


html_input_t* html_input_from_node(dom_node_t* node)
{
    assert(html_node_is_input(node));

    return (html_input_t*)node;
}


dom_node_t* html_node_from_input(html_input_t* input)
{
    return (dom_node_t*)input;
}


void html_input_free(dom_node_t* node)
{
    html_input_t* input = html_input_from_node(node);
    free(input);
}

