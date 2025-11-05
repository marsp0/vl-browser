#pragma once

#include "dom/hash_str.h"

hash_str_t mathml_tag_mi();
hash_str_t mathml_tag_mo();
hash_str_t mathml_tag_mn();
hash_str_t mathml_tag_ms();
hash_str_t mathml_tag_mtext();
hash_str_t mathml_tag_annotation_xml();
hash_str_t mathml_tag_mglyph();
hash_str_t mathml_tag_malignmark();

void mathml_populate_tags();