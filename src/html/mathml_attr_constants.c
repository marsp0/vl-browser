#include "mathml_attr_constants.h"

static hash_str_t definition_url;

hash_str_t mathml_attr_definition_url()
{
    return definition_url;
}


void mathml_attrs_init()
{
    definition_url = hash_str_new("definitionURL", 13);
}