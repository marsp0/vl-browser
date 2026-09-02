#pragma once

typedef struct css_style_sheet_t css_style_sheet_t;
typedef struct css_rule_t css_rule_t;
typedef struct css_decl_t css_decl_t;
typedef struct css_value_t css_value_t;

void ASSERT_CSS_SHEET(css_style_sheet_t* a, css_style_sheet_t* e);
void ASSERT_CSS_RULE(css_rule_t* a, css_rule_t* e);
void ASSERT_CSS_DECL(css_decl_t* a, css_decl_t* e);

void test_css_conversions();