#include "form.h"

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

dom_node_t* html_form_new(dom_node_t* document, hash_str_t namespace)
{
    html_form_t* form = malloc(sizeof(html_form_t));

    html_element_init((html_element_t*)form, document, html_tag_form(), namespace);

    form->charset = 0;
    form->action = 0;
    form->auto_complete = 0;
    form->enc_type = 0;
    form->encoding = 0;
    form->method = 0;
    form->name = 0;
    form->validate = 0;
    form->target = 0;
    form->rel = 0;

    return html_node_from_form(form);
}


bool html_node_is_form(dom_node_t* node)
{
    return node->type & HTML_NODE_FORM;
}


html_form_t* html_form_from_node(dom_node_t* node)
{
    assert(html_node_is_form(node));

    return (html_form_t*)node;
}


dom_node_t* html_node_from_form(html_form_t* form)
{
    return (dom_node_t*)form;
}


void html_form_free(dom_node_t* node)
{
    html_form_t* form = html_form_from_node(node);
    free(form);
}

