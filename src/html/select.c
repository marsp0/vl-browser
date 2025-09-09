/*
 * Notes
 * 
 */

#include "html/select.h"

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

static bool is_select(dom_node_t* node)
{
    return node->type | HTML_NODE_SELECT;
}

/********************/
/* public functions */
/********************/

dom_node_t* html_select_new(dom_node_t* document)
{
    html_select_t* select = malloc(sizeof(html_select_t));

    html_element_init((html_element_t*)select, document, html_tag_select());

    return html_node_from_select(select);
}


html_select_t* html_select_from_node(dom_node_t* node)
{
    assert(is_select(node));

    return (html_select_t*)node;
}


dom_node_t* html_node_from_select(html_select_t* select)
{
    return (dom_node_t*)select;
}


void html_select_free(dom_node_t* node)
{
    html_select_t* select = html_select_from_node(node);
    free(select);
}
