#include "html/attr_constants.h"

static hash_str_t href;

void html_attrs_init()
{
    href = hash_str_new("href", 4);
}

hash_str_t html_attr_href()
{
    return href;
}