#pragma once

#include <stdint.h>

typedef struct dom_node_t dom_node_t;

void print_document_tree(dom_node_t* node, uint32_t level);

