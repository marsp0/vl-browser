#include "svg_tag_constants.h"

static hash_str_t foreign_object;
static hash_str_t desc;
static hash_str_t title;

hash_str_t svg_tag_foreign_object()
{
    return foreign_object;
}


hash_str_t svg_tag_desc()
{
    return desc;
}


hash_str_t svg_tag_title()
{
    return title;
}


void svg_tags_init()
{
    foreign_object  = hash_str_new("foreignobject", 13);
    desc            = hash_str_new("desc", 4);
    title           = hash_str_new("title", 5);
}