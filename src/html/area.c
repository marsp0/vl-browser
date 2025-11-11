#include "area.h"

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

dom_node_t* html_area_new(dom_node_t* document, hash_str_t namespace)
{
    html_area_t* area = malloc(sizeof(html_area_t));

    html_element_init((html_element_t*)area, document, html_tag_a(), namespace);

    area->alt               = 0;
    area->coords            = 0;
    area->shape             = 0;
    area->target            = 0;
    area->download          = 0;
    area->ping              = 0;
    area->rel               = 0;
    area->referrer_policy   = 0;

    return html_node_from_area(area);
}


bool html_node_is_area(dom_node_t* node)
{
    return node->type & HTML_NODE_AREA;
}


html_area_t* html_area_from_node(dom_node_t* node)
{
    assert(html_node_is_area(node));

    return (html_area_t*)node;
}


dom_node_t* html_node_from_area(html_area_t* area)
{
    return (dom_node_t*)area;
}


void html_area_free(dom_node_t* node)
{
    html_area_t* area = html_area_from_node(node);
    free(area);
}

