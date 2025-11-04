#include "html/attr_constants.h"

static hash_str_t html;
static hash_str_t svg;
static hash_str_t mathml;

void html_populate_namespaces()
{
    html    = hash_str_new("http://www.w3.org/1999/xhtml", 28);
    svg     = hash_str_new("http://www.w3.org/2000/svg", 26);
    mathml  = hash_str_new("http://www.w3.org/1998/Math/MathML", 34);
}

hash_str_t html_ns_html()
{
    return html;
}


hash_str_t html_ns_svg()
{
    return svg;
}


hash_str_t html_ns_mathml()
{
    return mathml;
}
