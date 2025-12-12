#include "style_sheet.h"

#include <stdlib.h>

css_style_sheet_t* css_style_sheet_new()
{
    css_style_sheet_t* sheet = malloc(sizeof(css_style_sheet_t));

    return sheet;
}


void css_style_sheet_free(css_style_sheet_t* s)
{
    free(s);
}