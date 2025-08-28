// /*
//  * Notes
//  * 
//  */

// #include "option.h"

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

// html_option_t*  html_option_from_node(html_node_t* node);
// html_node_t*    html_node_from_option(html_option_t* option);
// html_element_t* html_element_from_option(html_option_t* option);
// html_option_t*  html_option_from_element(html_element_t* element);


// html_node_t* html_option_new(html_node_t* document)
// {
//     html_option_t* option = malloc(sizeof(html_option_t));
//     html_element_t* element = html_element_from_option(option);
//     html_element_initialize(element, document, "option", 6);

//     element->type = HTML_ELEMENT_OPTION;

//     return html_node_from_option(option);
// }


// html_element_t* html_element_from_option(html_option_t* option)
// {
//     return (html_element_t*)option;
// }


// html_option_t* html_option_from_element(html_element_t* element)
// {
//     assert(element->type == HTML_ELEMENT_OPTION);

//     return (html_option_t*)element;
// }


// html_option_t* html_option_from_node(html_node_t* node)
// {
//     return html_option_from_element(html_element_from_node(node));
// }


// html_node_t* html_node_from_option(html_option_t* option)
// {
//     return (html_node_t*)option;
// }


// void html_option_free(html_node_t* node)
// {
//     html_node_free(node);
//     assert(node->type == HTML_NODE_ELEMENT);

//     html_element_t* element = html_element_from_node(node);
//     assert(element->type == HTML_ELEMENT_OPTION);

//     free(html_option_from_element(element));
// }