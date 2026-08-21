#include "test_parser_utils.h"

#include <assert.h>

#include "test_utils.h"
#include "css/tokenizer/test_tokenizer_utils.h"

#include "css/parser_types.h"


static void print_token(css_token_t* t)
{
    if (t->data_size > 0)
    {
        printf("'%.*s' [token] ", t->data_size, t->data);
    }
    else if (t->type == CSS_TOKEN_NUMBER)
    {
        printf("'%d' (%f) [token] ", t->integer, t->real);
    }
    else
    {
        printf("token ");
    }

    switch(t->type)
    {
        case CSS_TOKEN_EOF:
            printf("[CSS_TOKEN_EOF]");
            break;
        case CSS_TOKEN_DELIM:
            printf("[CSS_TOKEN_DELIM]");
            break;
        case CSS_TOKEN_IDENT:
            printf("[CSS_TOKEN_IDENT]");
            break;
        case CSS_TOKEN_FUNCTION:
            printf("[CSS_TOKEN_FUNCTION]");
            break;
        case CSS_TOKEN_AT_KEYWORD:
            printf("[CSS_TOKEN_AT_KEYWORD]");
            break;
        case CSS_TOKEN_HASH:
            printf("[CSS_TOKEN_HASH]");
            break;
        case CSS_TOKEN_STRING:
            printf("[CSS_TOKEN_STRING]");
            break;
        case CSS_TOKEN_BAD_STRING:
            printf("[CSS_TOKEN_BAD_STRING]");
            break;
        case CSS_TOKEN_PERCENTAGE:
            printf("[CSS_TOKEN_PERCENTAGE]");
            break;
        case CSS_TOKEN_DIMENSION:
            printf("[CSS_TOKEN_DIMENSION]");
            break;
        case CSS_TOKEN_NUMBER:
            printf("[CSS_TOKEN_NUMBER]");
            break;
        case CSS_TOKEN_WHITESPACE:
            printf("[CSS_TOKEN_WHITESPACE]");
            break;
        case CSS_TOKEN_CDO:
            printf("[CSS_TOKEN_CDO]");
            break;
        case CSS_TOKEN_CDC:
            printf("[CSS_TOKEN_CDC]");
            break;
        case CSS_TOKEN_COLON:
            printf("[CSS_TOKEN_COLON]");
            break;
        case CSS_TOKEN_SEMICOLON:
            printf("[CSS_TOKEN_SEMICOLON]");
            break;
        case CSS_TOKEN_COMMA:
            printf("[CSS_TOKEN_COMMA]");
            break;
        case CSS_TOKEN_OPEN_BRACKET:
            printf("[CSS_TOKEN_OPEN_BRACKET]");
            break;
        case CSS_TOKEN_CLOSED_BRACKET:
            printf("[CSS_TOKEN_CLOSED_BRACKET]");
            break;
        case CSS_TOKEN_OPEN_PARENTHESIS:
            printf("[CSS_TOKEN_OPEN_PARENTHESIS]");
            break;
        case CSS_TOKEN_CLOSED_PARENTHESIS:
            printf("[CSS_TOKEN_CLOSED_PARENTHESIS]");
            break;
        case CSS_TOKEN_OPEN_BRACE:
            printf("[CSS_TOKEN_OPEN_BRACE]");
            break;
        case CSS_TOKEN_CLOSED_BRACE:
            printf("[CSS_TOKEN_CLOSED_BRACE]");
            break;
        case CSS_TOKEN_BAD_URL:
            printf("[CSS_TOKEN_BAD_URL]");
            break;
        case CSS_TOKEN_URL:
            printf("[CSS_TOKEN_URL]");
            break;
        case CSS_TOKEN_COMMENT:
            printf("[CSS_TOKEN_COMMENT]");
            break;
        default:
            assert(false);
    }
}


void print_css_parse_tree(css_parser_node_t* node, uint32_t level)
{
    for (uint32_t i = 0; i < level; i++) { printf("  "); }

    if (node->name != 0 && node->type != CSS_PARSER_NODE_TYPE_TOKEN)
    {
        const unsigned char* data = hash_str_get(node->name);
        const uint32_t data_size = hash_str_get_size(node->name);
        printf("%.*s", data_size, data);
    }
    else if (node->type != CSS_PARSER_NODE_TYPE_TOKEN)
    {
        printf("node");
    }

    if (node->type == CSS_PARSER_NODE_TYPE_STYLESHEET)      { printf(" [stylesheet]"); }
    else if (node->type == CSS_PARSER_NODE_TYPE_AT_RULE)    { printf(" [at-rule]"); }
    else if (node->type == CSS_PARSER_NODE_TYPE_Q_RULE)     { printf(" [qualified-rule]"); }
    else if (node->type == CSS_PARSER_NODE_TYPE_BLOCK)      { printf(" [block]"); }
    else if (node->type == CSS_PARSER_NODE_TYPE_DECL)       { printf(" [declaration]"); }
    else if (node->type == CSS_PARSER_NODE_TYPE_TOKEN)      { print_token(node->token); }
    else if (node->type == CSS_PARSER_NODE_TYPE_FUNCTION)   { printf(" [function]"); }
    else                                                    { assert(false); }

    printf("\n");

    css_parser_node_t* comp_vals = node->comp_vals;
    for (uint32_t i = 0; i < level + 1; i++) { printf("  "); }
    printf("[comp_vals]\n");
    if (comp_vals) { print_css_parse_tree(comp_vals, level + 2); }

    css_parser_node_t* decls = node->decls;
    for (uint32_t i = 0; i < level + 1; i++) { printf("  "); }
    printf("[decls]\n");
    if (decls) { print_css_parse_tree(decls, level + 2); }

    css_parser_node_t* rules = node->rules;
    for (uint32_t i = 0; i < level + 1; i++) { printf("  "); }
    printf("[rules]\n");
    if (rules) { print_css_parse_tree(rules, level + 2); }

    css_parser_node_t* next = node->next;
    if (next) { print_css_parse_tree(next, level); }
}


void ASSERT_CSS_PARSER_NODE(css_parser_node_t* a, css_parser_node_t* e)
{
    if (!a || !e)
    {
        if (!a && !e) { return; }

        ASSERT_POINTER(a, e);
        return;
    }

    ASSERT_HASH_STRING(a->name, e->name);
    ASSERT_EQUAL(a->type, e->type);
    ASSERT_CSS_TOKEN(a->token, e->token);

    css_parser_node_t* a_sibling = a->next;
    css_parser_node_t* e_sibling = e->next;

    while (a_sibling || e_sibling)
    {
        ASSERT_CSS_PARSER_NODE(a_sibling, e_sibling);

        if (!a_sibling || !e_sibling) { break; }

        a_sibling = a_sibling->next;
        e_sibling = e_sibling->next;
    }

    css_parser_node_t* a_comp_vals = a->comp_vals;
    css_parser_node_t* e_comp_vals = e->comp_vals;

    while (a_comp_vals || e_comp_vals)
    {
        ASSERT_CSS_PARSER_NODE(a_comp_vals, e_comp_vals);

        if (!a_comp_vals || !e_comp_vals) { break; }

        a_comp_vals = a_comp_vals->comp_vals;
        e_comp_vals = e_comp_vals->comp_vals;
    }

    css_parser_node_t* a_decls = a->decls;
    css_parser_node_t* e_decls = e->decls;

    while (a_decls || e_decls)
    {
        ASSERT_CSS_PARSER_NODE(a_decls, e_decls);

        if (!a_decls || !e_decls) { break; }

        a_decls = a_decls->decls;
        e_decls = e_decls->decls;
    }

    css_parser_node_t* a_rules = a->rules;
    css_parser_node_t* e_rules = e->rules;

    while (a_rules || e_rules)
    {
        ASSERT_CSS_PARSER_NODE(a_rules, e_rules);

        if (!a_rules || !e_rules) { break; }

        a_rules = a_rules->rules;
        e_rules = e_rules->rules;
    }
}