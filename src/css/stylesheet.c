#include "stylesheet.h"

#include <stdlib.h>

css_stylesheet_t* css_style_sheet_new()
{
    css_stylesheet_t* sheet = malloc(sizeof(css_stylesheet_t));

    return sheet;
}


void css_style_sheet_free(css_stylesheet_t* s)
{
    free(s);
}