#include "heading.h"

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

dom_node_t* html_heading_new(dom_node_t* document, hash_str_t name, hash_str_t namespace)
{
    html_heading_t* heading = malloc(sizeof(html_heading_t));

    html_element_init((html_element_t*)heading, document, name, namespace);

    heading->align = 0;

    return html_node_from_heading(heading);
}


bool html_node_is_heading(dom_node_t* node)
{
    return node->type & HTML_NODE_HEADING;
}


html_heading_t* html_heading_from_node(dom_node_t* node)
{
    assert(html_node_is_heading(node));

    return (html_heading_t*)node;
}


dom_node_t* html_node_from_heading(html_heading_t* heading)
{
    return (dom_node_t*)heading;
}


void html_heading_free(dom_node_t* node)
{
    html_heading_t* heading = html_heading_from_node(node);
    free(heading);
}

