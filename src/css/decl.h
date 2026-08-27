#pragma once

typedef struct css_decl_t
{
    hash_str_t name;
    hash_str_t value;

    bool important;
    bool case_sens;

} css_decl_t;


css_decl_t* css_decl_new(hash_str_t name, hash_str_t val);
void        css_decl_free(css_decl_t* decl);