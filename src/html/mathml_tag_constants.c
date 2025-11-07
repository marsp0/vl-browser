#include "mathml_tag_constants.h"

static hash_str_t mi;
static hash_str_t mo;
static hash_str_t mn;
static hash_str_t ms;
static hash_str_t mtext;
static hash_str_t annotation_xml;
static hash_str_t mglyph;
static hash_str_t malignmark;

hash_str_t mathml_tag_mi()
{
    return mi;
}


hash_str_t mathml_tag_mo()
{
    return mo;
}


hash_str_t mathml_tag_mn()
{
    return mn;
}


hash_str_t mathml_tag_ms()
{
    return ms;
}


hash_str_t mathml_tag_mtext()
{
    return mtext;
}


hash_str_t mathml_tag_annotation_xml()
{
    return annotation_xml;
}


hash_str_t mathml_tag_mglyph()
{
    return mglyph;
}


hash_str_t mathml_tag_malignmark()
{
    return malignmark;
}


void mathml_tags_init()
{
    mi              = hash_str_new("mi", 2);
    mo              = hash_str_new("mo", 2);
    mn              = hash_str_new("mn", 2);
    ms              = hash_str_new("ms", 2);
    mtext           = hash_str_new("mtext", 5);
    annotation_xml  = hash_str_new("annotation-xml", 14);
    mglyph          = hash_str_new("mglyph", 6);
    malignmark      = hash_str_new("malignmark", 10);
}