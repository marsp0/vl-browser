#include "style_sheet.h"

#include <stdlib.h>
#include <string.h>

#include "css/types/rule.h"

css_style_sheet_t* css_style_sheet_new()
{
    css_style_sheet_t* sheet = malloc(sizeof(css_style_sheet_t));
    memset(sheet, 0, sizeof(css_style_sheet_t));

    return sheet;
}


void css_style_sheet_add_rule(css_style_sheet_t* sheet, css_rule_t* rule)
{
    if (!rule) { return; }

    if (!sheet->rules)
    {
        sheet->rules = rule;
        rule->sheet = sheet;
    }
    else
    {
        css_rule_t* child = sheet->rules;
        while (child->next)
        {
            child = child->next;
        }

        child->next = rule;
        rule->prev = child;
        rule->sheet = sheet;
    }
}


void css_style_sheet_free(css_style_sheet_t* s)
{
    free(s);
}