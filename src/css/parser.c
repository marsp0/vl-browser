#include "parser.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "util/not_implemented.h"
#include "css/tokenizer.h"
#include "css/parser_types.h"

/*
 * Notes
 * 
 */

/********************/
/*      defines     */
/********************/

#define MAX_TOKENS 1000
#define MAX_MARKS 50

static css_parser_node_t* css_parser_consume_token();
static css_parser_node_t* css_parser_consume_comp_val();
static css_parser_node_t* css_parser_consume_at_rule(bool nested);
static css_parser_node_t* css_parser_consume_q_rule(bool nested, css_token_type_e stop);

/********************/
/* static variables */
/********************/

static css_token_t* buf[MAX_TOKENS] = { 0 };
static uint32_t buf_cur = 0;
static uint32_t buf_size = 0;
static css_token_t eof_token = { 0 };

static uint32_t marks[MAX_MARKS];
static uint32_t mark_idx = 0;

/********************/
/* static functions */
/********************/

static void insert_mark()
{
    mark_idx++;
    marks[mark_idx] = buf_cur;
}


static void discard_mark()
{
    marks[mark_idx] = 0;
    mark_idx--;
}


static void restore_mark()
{
    buf_cur = marks[mark_idx];
    marks[mark_idx] = 0;
    mark_idx--;
}


static css_token_t* next_token()
{
    if (buf_cur >= buf_size) { return &eof_token; }
    return buf[buf_cur];
}


static css_token_t* consume_token()
{
    if (buf_cur >= buf_size) { return &eof_token; }

    css_token_t* t = buf[buf_cur];
    buf_cur++;
    return t;
}


static void discard_token()
{
    buf_cur++;
}


static void discard_whitespace()
{
    css_token_t* t = next_token();
    while (t->type == CSS_TOKEN_WHITESPACE)
    {
        consume_token();
        t = next_token();
    }
}


static bool is_custom_property()
{
    return false;
}


static bool is_valid(css_parser_node_t* node)
{
    // copied from https://github.com/tabatkins/parse-css

    if (node->type == CSS_PARSER_NODE_TYPE_AT_RULE) { return true; }

    // Exclude qualified rules that ended up with a semicolon
    // in their prelude.
    // (Can only happen at the top level of a stylesheet.)
    if (node->type == CSS_PARSER_NODE_TYPE_Q_RULE)
    {
        css_parser_node_t* prelude = node->comp_vals;
        while (prelude)
        {
            if (prelude->type == CSS_PARSER_NODE_TYPE_TOKEN && prelude->token->type == CSS_TOKEN_SEMICOLON) { return false; }
            prelude = prelude->next;
        }

        return true;
    }

    // Exclude properties that ended up with a {}-block
    // in their value, unless they're custom.

    if (node->type == CSS_PARSER_NODE_TYPE_DECL)
    {
        css_parser_node_t* comp_val = node->comp_vals;
        while (comp_val)
        {
            if (comp_val->type == CSS_PARSER_NODE_TYPE_BLOCK) { return false; }
            comp_val = comp_val->next;
        }
        return true;
    }

    return false;
}


static css_parser_node_t* filter_valid(css_parser_node_t* node)
{
    if (!node) { return node; }

    if (is_valid(node)) { return node; }

    return NULL;
}


static void css_parser_tokenize()
{
    // TEMP (mspasov): consume all tokens
    while (true)
    {
        buf[buf_size]     = css_token_new();
        css_token_t new_token   = css_tokenizer_next();
        memcpy(buf[buf_size], &new_token, sizeof(css_token_t));
        buf_size++;

        if (new_token.type == CSS_TOKEN_EOF) { break; }

        assert(buf_size < MAX_TOKENS);
    }
}


static css_parser_node_t* css_parser_consume_function()
{
    css_token_t* t = consume_token();
    css_token_type_e type = t->type;
    assert(type == CSS_TOKEN_FUNCTION);

    hash_str_t f_name = hash_str_new(t->data, t->data_size);
    css_parser_node_t* func = css_parser_node_new(f_name, CSS_PARSER_NODE_TYPE_FUNCTION);

    bool run = true;
    while (run)
    {
        t = next_token();
        type = t->type;

        switch(type)
        {
            case CSS_TOKEN_EOF:
            case CSS_TOKEN_CLOSED_PARENTHESIS:
                discard_token();
                run = false;
                break;

            default:
                ;
                css_parser_node_t* c_val = css_parser_consume_comp_val();
                css_parser_node_add_comp_val(func, c_val);
                break;
        }
    }

    return func;
}


static css_parser_node_t* css_parser_consume_simple_block(css_token_type_e open)
{
    css_token_type_e stop = CSS_TOKEN_CLOSED_PARENTHESIS;
    if (open == CSS_TOKEN_OPEN_BRACE)           { stop = CSS_TOKEN_CLOSED_BRACE; }
    else if (open == CSS_TOKEN_OPEN_BRACKET)    { stop = CSS_TOKEN_CLOSED_BRACKET; }

    css_parser_node_t* block = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_BLOCK);
    css_token_t* t = consume_token();
    css_token_type_e type = t->type;

    bool run = true;
    while (run)
    {
        t = next_token();
        type = t->type;

        if (type == CSS_TOKEN_EOF || type == stop)
        {
            discard_token();
            run = false;
        }
        else
        {
            css_parser_node_t* val = css_parser_consume_comp_val();
            css_parser_node_add_comp_val(block, val);
        }
    }

    return block;
}


static css_parser_node_t* css_parser_consume_comp_val()
{
    css_token_t* t = next_token();
    css_token_type_e type = t->type;

    if (type == CSS_TOKEN_OPEN_BRACE || type == CSS_TOKEN_OPEN_BRACKET || type == CSS_TOKEN_OPEN_PARENTHESIS)
    {
        return css_parser_consume_simple_block(type);
    }
    else if (type == CSS_TOKEN_FUNCTION)
    {
        return css_parser_consume_function();
    }

    return css_parser_consume_token();
}


static css_parser_node_t* css_parser_consume_comp_vals(bool nested, css_token_type_e stop)
{
    css_parser_node_t* first = NULL;

    bool run = true;
    while (run)
    {
        css_token_t* t = next_token();
        css_token_type_e type = t->type;

        if (type == CSS_TOKEN_EOF || type == stop)
        {
            run = false;
        }
        else if (type == CSS_TOKEN_CLOSED_BRACE)
        {
            if (nested)
            {
                run = false;
            }
            else
            {
                css_parser_node_t* p_token = css_parser_consume_token();
                if (first)
                {
                    css_parser_node_add_sibling(first, p_token);
                }
                else
                {
                    first = p_token;
                }
            }
        }
        else
        {
            css_parser_node_t* comp_val = css_parser_consume_comp_val();
            if (first)
            {
                css_parser_node_add_sibling(first, comp_val);
            }
            else
            {
                first = comp_val;
            }
        }
    }

    return first;
}


static void css_parser_consume_bad_decl(bool nested)
{
    bool run = true;
    while (run)
    {
        css_token_t* t = next_token();
        css_token_type_e type = t->type;

        if (type == CSS_TOKEN_EOF || type == CSS_TOKEN_SEMICOLON)
        {
            discard_token();
            return;
        }
        else if (type == CSS_TOKEN_CLOSED_BRACE)
        {
            if (nested)
            {
                return;
            }
            else
            {
                discard_token();
            }
        }
        else
        {
            css_parser_consume_comp_val();
        }
    }
}


static css_parser_node_t* css_parser_consume_token()
{
    css_token_t* t = consume_token();
    css_parser_node_t* node = css_parser_node_new(hash_str_new(t->data, t->data_size), CSS_PARSER_NODE_TYPE_TOKEN);
    node->token = t;

    return node;
}


static css_parser_node_t* css_parser_consume_decl(bool nested)
{
    css_parser_node_t* decl = NULL;
    css_token_t* t = next_token();
    css_token_type_e type = t->type;

    if (type == CSS_TOKEN_IDENT)
    {
        hash_str_t name = hash_str_new(t->data, t->data_size);
        decl = css_parser_node_new(name, CSS_PARSER_NODE_TYPE_DECL);
        consume_token();
    }
    else
    {
        css_parser_consume_bad_decl(nested);
        return NULL;
    }

    discard_whitespace();

    t = next_token();
    type = t->type;

    if (type == CSS_TOKEN_COLON)
    {
        discard_token();
    }
    else
    {
        css_parser_consume_bad_decl(nested);
        return NULL;
    }

    discard_whitespace();
    decl->comp_vals = css_parser_consume_comp_vals(nested, CSS_TOKEN_SEMICOLON);

    // handle !important flag

    // remove trailing whitespace tokens
    css_parser_node_t* tmp = decl->comp_vals;
    while (tmp->next) { tmp = tmp->next; }

    if (tmp->type == CSS_PARSER_NODE_TYPE_TOKEN && tmp->token->type == CSS_TOKEN_WHITESPACE)
    {
        tmp = tmp->prev;
        tmp->next = NULL;
    }
    return filter_valid(decl);
}


static void css_parser_consume_block_contents(css_parser_node_t* parent)
{
    css_parser_node_t* rules = NULL;
    css_parser_node_t* decls = NULL;

    bool run = true;
    while (run)
    {
        css_token_t* t = next_token();
        css_token_type_e type = t->type;

        switch(type)
        {
            case CSS_TOKEN_WHITESPACE:
            case CSS_TOKEN_SEMICOLON:
                discard_token();
                break;

            case CSS_TOKEN_CLOSED_BRACE:
            case CSS_TOKEN_EOF:
                run = false;
                break;

            case CSS_TOKEN_AT_KEYWORD:
                ;
                css_parser_node_t* at_rule = css_parser_consume_at_rule(true);
                if (rules)
                {
                    css_parser_node_add_sibling(rules, at_rule);
                }
                else
                {
                    rules = at_rule;
                }
                break;

            default:
                insert_mark();
                css_parser_node_t* decl = css_parser_consume_decl(true);
                if (decl)
                {
                    if (decls)
                    {
                        css_parser_node_add_sibling(decls, decl);
                    }
                    else
                    {
                        decls = decl;
                    }
                    discard_mark();
                }
                else
                {
                    restore_mark();
                    css_parser_node_t* rule = css_parser_consume_q_rule(true, CSS_TOKEN_SEMICOLON);
                    if (rules)
                    {
                        css_parser_node_add_sibling(rules, rule);
                    }
                    else
                    {
                        rules = rule;
                    }
                }
                break;
        }
    }

    css_parser_node_add_decl(parent, decls);
    css_parser_node_add_rule(parent, rules);
}


static void css_parser_consume_block(css_parser_node_t* parent)
{
    css_token_t* t = consume_token();
    assert(t->type == CSS_TOKEN_OPEN_BRACE);

    css_parser_consume_block_contents(parent);

    discard_token();
    return;
}


static css_parser_node_t* css_parser_consume_at_rule(bool nested)
{
    css_token_t* t = consume_token();
    assert(t->type == CSS_TOKEN_AT_KEYWORD);

    hash_str_t name = hash_str_new(t->data, t->data_size);
    css_parser_node_t* rule = css_parser_node_new(name, CSS_PARSER_NODE_TYPE_AT_RULE);

    bool run = true;
    while (run)
    {
        t = next_token();
        css_token_type_e type = t->type;

        switch (type)
        {
            case CSS_TOKEN_SEMICOLON:
            case CSS_TOKEN_EOF:
                discard_token();
                run = false;
                break;

            case CSS_TOKEN_CLOSED_BRACE:
                if (nested)
                {
                    run = false;
                }
                else
                {
                    // todo: parse error
                    css_parser_node_t* p_token = css_parser_consume_token();
                    css_parser_node_add_comp_val(rule, p_token);
                }
                break;

            case CSS_TOKEN_OPEN_BRACE:
                css_parser_consume_block(rule);
                run = false;
                break;

            default:
                ;
                css_parser_node_t* comp_val = css_parser_consume_comp_val();
                css_parser_node_add_comp_val(rule, comp_val);
                break;
        }
    }

    return filter_valid(rule);
}


static css_parser_node_t* css_parser_consume_q_rule(bool nested, css_token_type_e stop)
{
    css_parser_node_t* rule = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_Q_RULE);

    bool run = true;
    while (run)
    {
        css_token_t* t = next_token();
        css_token_type_e type = t->type;

        if (type == CSS_TOKEN_EOF || type == stop)
        {
            rule = NULL;
            run = false;
        }
        else if (type == CSS_TOKEN_CLOSED_BRACE)
        {
            if (nested)
            {
                rule = NULL;
                run = false;
            }
            else
            {
                css_parser_node_add_comp_val(rule, css_parser_consume_token());
            }
        }
        else if (type == CSS_TOKEN_OPEN_BRACE)
        {
            if (is_custom_property()) // custom property
            {
                NOT_IMPLEMENTED
            }
            else
            {
                css_parser_consume_block(rule);

                run = false;
            }
        }
        else
        {
            css_parser_node_add_comp_val(rule, css_parser_consume_comp_val());
        }
    }

    return filter_valid(rule);
}


static css_parser_node_t* css_parser_consume_stylesheet_contents()
{
    css_parser_node_t* first = NULL;
    bool run = true;

    while (run)
    {
        css_token_t* t = next_token();
        css_token_type_e type = t->type;
        css_parser_node_t* rule = NULL;

        switch(type)
        {
            case CSS_TOKEN_WHITESPACE:
                discard_token();
                break;

            case CSS_TOKEN_EOF:
                run = false;
                break;

            case CSS_TOKEN_CDO:
            case CSS_TOKEN_CDC:
                discard_token();
                break;

            case CSS_TOKEN_AT_KEYWORD:
                rule = css_parser_consume_at_rule(false);

                if (first)  { css_parser_node_add_sibling(first, rule); }
                else        { first = rule; }

                rule = NULL;
                break;

            default:
                rule = css_parser_consume_q_rule(false, CSS_TOKEN_EOF);

                if (first)  { css_parser_node_add_sibling(first, rule); }
                else        { first = rule; }

                rule = NULL;
                break;
        }
    }

    return first;
}


/********************/
/* public functions */
/********************/


void css_parser_init(const unsigned char* raw_buf, uint32_t raw_buf_size)
{
    buf_size = 0;
    buf_cur = 0;
    memset(buf, 0, sizeof(buf));
    css_parser_types_reset();
    css_tokenizer_init(raw_buf, raw_buf_size);
}


css_parser_node_t* css_parser_parse_stylesheet()
{
    css_parser_tokenize();
    css_parser_node_t* stylesheet = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);
    css_parser_node_t* rules = css_parser_consume_stylesheet_contents();
    css_parser_node_add_rule(stylesheet, rules);

    return stylesheet;
}


void css_parser_free()
{
    css_tokenizer_free();
}
