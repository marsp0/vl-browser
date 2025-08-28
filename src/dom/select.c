// /*
//  * Notes
//  * 
//  */

// #include "select.h"

// #include <stdlib.h>
// #include <assert.h>

// /********************/
// /*      defines     */
// /********************/


// /********************/
// /* static variables */
// /********************/


// /********************/
// /* static functions */
// /********************/


// /********************/
// /* public functions */
// /********************/

// html_select_t*  html_select_from_node(html_node_t* node);
// html_node_t*    html_node_from_select(html_select_t* select);
// html_element_t* html_element_from_select(html_select_t* select);
// html_select_t*  html_select_from_element(html_element_t* element);


// html_node_t* html_select_new(html_node_t* document)
// {
//     html_select_t* select = malloc(sizeof(html_select_t));
//     html_element_t* element = html_element_from_select(select);
//     html_element_initialize(element, document, "select", 6);

//     element->type = HTML_ELEMENT_SELECT;

//     return html_node_from_select(select);
// }


// html_element_t* html_element_from_select(html_select_t* select)
// {
//     return (html_element_t*)select;
// }


// html_select_t* html_select_from_element(html_element_t* element)
// {
//     assert(element->type == HTML_ELEMENT_SELECT);

//     return (html_select_t*)element;
// }


// html_select_t* html_select_from_node(html_node_t* node)
// {
//     return html_select_from_element(html_element_from_node(node));
// }


// html_node_t* html_node_from_select(html_select_t* select)
// {
//     return (html_node_t*)select;
// }


// void html_select_free(html_node_t* node)
// {
//     html_node_free(node);
//     assert(node->type == HTML_NODE_ELEMENT);

//     html_element_t* element = html_element_from_node(node);
//     assert(element->type == HTML_ELEMENT_SELECT);

//     free(html_select_from_element(element));
// }