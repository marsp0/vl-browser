#include "global_modules.h"

#include "dom/hash_str.h"
#include "html/tag_constants.h"
#include "html/attr_constants.h"
#include "html/named_char_refs.h"

void global_modules_init()
{
    hash_str_pool_new();
    html_named_char_ref_map_init();

    // constants
    html_populate_tags();
    html_populate_attrs();
}


void global_modules_free()
{
    hash_str_pool_free();
    html_named_char_ref_map_free();
}