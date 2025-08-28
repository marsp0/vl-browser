#pragma once

// #include <stdint.h>

// #include "dom/element.h"
// #include "html/constants.h"


// typedef struct
// {
//     html_element_t  element;

//     bool            disabled;

//     unsigned char   autocomplete[MAX_HTML_NAME_LEN];
//     uint32_t        autocomplete_size;

//     html_node_t*    form;

//     bool            multiple;

//     unsigned char   name[MAX_HTML_NAME_LEN];
//     uint32_t        name_size;

//     bool            required;

//     uint32_t        size;
// } html_select_t;


// html_node_t*    html_select_new(html_node_t* document);
// html_select_t*  html_select_from_node(html_node_t* node);
// html_node_t*    html_node_from_select(html_select_t* select);
// html_element_t* html_element_from_select(html_select_t* select);
// html_select_t*  html_select_from_element(html_element_t* element);
// void            html_element_free(html_node_t* node);