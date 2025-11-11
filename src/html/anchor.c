#include "anchor.h"

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

dom_node_t* html_anchor_new(dom_node_t* document, hash_str_t namespace)
{
    html_anchor_t* anchor = malloc(sizeof(html_anchor_t));

    html_element_init((html_element_t*)anchor, document, html_tag_a(), namespace);

    anchor->target          = 0;
    anchor->download        = 0;
    anchor->ping            = 0;
    anchor->rel             = 0;
    anchor->hreflang        = 0;
    anchor->type            = 0;
    anchor->text            = 0;
    anchor->referrer_policy = 0;

    return html_node_from_anchor(anchor);
}


bool html_node_is_anchor(dom_node_t* node)
{
    return node->type & HTML_NODE_ANCHOR;
}


html_anchor_t* html_anchor_from_node(dom_node_t* node)
{
    assert(html_node_is_anchor(node));

    return (html_anchor_t*)node;
}


dom_node_t* html_node_from_anchor(html_anchor_t* anchor)
{
    return (dom_node_t*)anchor;
}


void html_anchor_free(dom_node_t* node)
{
    html_anchor_t* anchor = html_anchor_from_node(node);
    free(anchor);
}

