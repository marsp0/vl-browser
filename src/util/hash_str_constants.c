#include "util/hash_str_constants.h"

#include "html/tag_constants.h"
#include "html/attr_constants.h"

void hash_str_constants_initialize()
{
    html_populate_tags();
    html_populate_attrs();
}