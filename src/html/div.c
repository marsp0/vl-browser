#include "div.h"

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

dom_node_t* html_div_new(dom_node_t* document, hash_str_t namespace)
{
    html_div_t* div = malloc(sizeof(html_div_t));

    html_element_init((html_element_t*)div, document, html_tag_div(), namespace);

    div->align = 0;

    return html_node_from_div(div);
}


bool html_node_is_div(dom_node_t* node)
{
    return node->type & HTML_NODE_DIV;
}


html_div_t* html_div_from_node(dom_node_t* node)
{
    assert(html_node_is_div(node));

    return (html_div_t*)node;
}


dom_node_t* html_node_from_div(html_div_t* div)
{
    return (dom_node_t*)div;
}


void html_div_free(dom_node_t* node)
{
    html_div_t* div = html_div_from_node(node);
    free(div);
}

