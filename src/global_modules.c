#include "global_modules.h"

#include "html/tokenizer.h"

#include "dom/hash_str.h"
#include "html/tag_constants.h"
#include "html/svg_tag_constants.h"
#include "html/mathml_tag_constants.h"
#include "html/attr_constants.h"
#include "html/svg_attr_map.h"
#include "html/mathml_attr_constants.h"
#include "html/ns_constants.h"
#include "html/named_char_refs.h"

void global_modules_init()
{
    hash_str_pool_new();
    html_named_char_ref_map_init();
    html_tokenizer_global_init();

    // constants
    html_tags_init();
    svg_tags_init();
    mathml_tags_init();

    html_attrs_init();
    mathml_attrs_init();
    svg_attr_map_init();

    html_populate_namespaces();
}


void global_modules_free()
{
    hash_str_pool_free();
    html_named_char_ref_map_free();
    html_tokenizer_global_free();
}