#pragma once

// #include <stdint.h>

// #include "dom/element.h"
// #include "html/constants.h"

// // https://dom.spec.whatwg.org/#element
// typedef struct
// {
//     html_element_t  element;

//     bool            disabled;
//     html_node_t*    form;

//     unsigned char   label[MAX_HTML_NAME_LEN];
//     uint32_t        label_size;

//     bool            defaultSelected;
//     bool            selected;

//     unsigned char   value[MAX_HTML_NAME_LEN];
//     uint32_t        value_size;

//     unsigned char   text[MAX_HTML_NAME_LEN];
//     uint32_t        text_size;

//     uint32_t        index;
// } html_option_t;


// html_node_t*    html_option_new(html_node_t* document);
// html_option_t*  html_option_from_node(html_node_t* node);
// html_node_t*    html_node_from_option(html_option_t* option);
// html_element_t* html_element_from_option(html_option_t* option);
// html_option_t*  html_option_from_element(html_element_t* element);
// void            html_element_free(html_node_t* node);