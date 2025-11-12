#include "label.h"

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

dom_node_t* html_label_new(dom_node_t* document, hash_str_t namespace)
{
    html_label_t* label = malloc(sizeof(html_label_t));

    html_element_init((html_element_t*)label, document, html_tag_label(), namespace);

    label->form = NULL;
    label->html_for = 0;
    label->control = NULL;

    return html_node_from_label(label);
}


bool html_node_is_label(dom_node_t* node)
{
    return node->type & HTML_NODE_LABEL;
}


html_label_t* html_label_from_node(dom_node_t* node)
{
    assert(html_node_is_label(node));

    return (html_label_t*)node;
}


dom_node_t* html_node_from_label(html_label_t* label)
{
    return (dom_node_t*)label;
}


void html_label_free(dom_node_t* node)
{
    html_label_t* label = html_label_from_node(node);
    free(label);
}

