/*
 * Notes
 * 
 */

#include "html/element.h"

#include <stdlib.h>
#include <assert.h>

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

void html_element_init(html_element_t* element, dom_node_t* document, hash_str_t name, hash_str_t namespace)
{
    dom_element_initialize((dom_element_t*)element, document, name, namespace);
}


dom_node_t* html_element_new(dom_node_t* document, hash_str_t name, hash_str_t namespace)
{
    html_element_t* element = malloc(sizeof(html_element_t));

    html_element_init(element, document, name, namespace);

    return html_node_from_element(element);
}


bool html_node_is_element(dom_node_t* node)
{
    return node->type & HTML_NODE_ELEMENT;
}


html_element_t* html_element_from_node(dom_node_t* node)
{
    assert(html_node_is_element(node));

    return (html_element_t*)node;
}


dom_node_t* html_node_from_element(html_element_t* element)
{
    return (dom_node_t*)element;
}


void html_element_free(dom_node_t* node)
{
    html_element_t* element = html_element_from_node(node);
    free(element);
}
