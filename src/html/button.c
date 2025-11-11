#include "button.h"

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

dom_node_t* html_button_new(dom_node_t* document, hash_str_t namespace)
{
    html_button_t* button = malloc(sizeof(html_button_t));

    html_element_init((html_element_t*)button, document, html_tag_button(), namespace);

    button->cmd = 0;
    button->cmd_for_element = NULL;
    button->disabled = false;
    button->form = NULL;
    button->f_action = 0;
    button->f_enc_type = 0;
    button->f_method = 0;
    button->f_validate = false;
    button->f_target = 0;
    button->name = 0;
    button->type = 0;
    button->value = 0;
    button->will_validate = false;
    button->validity = 0;
    button->validation_msg = 0;
    button->labels = 0;

    return html_node_from_button(button);
}


bool html_node_is_button(dom_node_t* node)
{
    return node->type & HTML_NODE_BUTTON;
}


html_button_t* html_button_from_node(dom_node_t* node)
{
    assert(html_node_is_button(node));

    return (html_button_t*)node;
}


dom_node_t* html_node_from_button(html_button_t* button)
{
    return (dom_node_t*)button;
}


void html_button_free(dom_node_t* node)
{
    html_button_t* button = html_button_from_node(node);
    free(button);
}

