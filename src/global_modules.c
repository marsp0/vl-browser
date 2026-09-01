#include "global_modules.h"

#include "dom/hash_str.h"

#include "html/tokenizer.h"
#include "html/svg_attr_map.h"
#include "html/ns_constants.h"
#include "html/tag_constants.h"
#include "html/attr_constants.h"
#include "html/named_char_refs.h"
#include "html/svg_tag_constants.h"
#include "html/mathml_tag_constants.h"
#include "html/mathml_attr_constants.h"

#include "css/tokenizer.h"
#include "css/parser_types.h"
#include "css/prop_constants.h"
#include "css/color_name_map.h"

void global_modules_init()
{
    hash_str_pool_new();
    html_named_char_ref_map_init();
    html_tokenizer_global_init();
    css_tokenizer_global_init();

    // constants
    html_tags_init();
    svg_tags_init();
    mathml_tags_init();

    html_attrs_init();
    mathml_attrs_init();
    svg_attr_map_init();

    html_populate_namespaces();
    css_parser_types_init();
    css_prop_names_init();
    css_color_name_map_init();
}


void global_modules_free()
{
    hash_str_pool_free();
    html_named_char_ref_map_free();
    html_tokenizer_global_free();
    css_tokenizer_global_free();
    css_parser_types_free();
    css_tokenizer_types_free();
}