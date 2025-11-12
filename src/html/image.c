#include "image.h"

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

dom_node_t* html_img_new(dom_node_t* document, hash_str_t namespace)
{
    html_img_t* img = malloc(sizeof(html_img_t));

    html_element_init((html_element_t*)img, document, html_tag_img(), namespace);

    img->name = 0;
    img->low_src = 0;
    img->align = 0;
    img->hspace = 0;
    img->vspace = 0;
    img->desc = 0;
    img->h_margin = 0;
    img->w_margin = 0;

    return html_node_from_img(img);
}


bool html_node_is_img(dom_node_t* node)
{
    return node->type & HTML_NODE_IMAGE;
}


html_img_t* html_img_from_node(dom_node_t* node)
{
    assert(html_node_is_img(node));

    return (html_img_t*)node;
}


dom_node_t* html_node_from_img(html_img_t* img)
{
    return (dom_node_t*)img;
}


void html_img_free(dom_node_t* node)
{
    html_img_t* img = html_img_from_node(node);
    free(img);
}

