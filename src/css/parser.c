#include "parser.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "css/tokenizer.h"
#include "css/parser_types.h"

/*
 * Notes
 * 
 */

/********************/
/*      defines     */
/********************/

static css_parser_comp_val_t*  consume_comp_val();
static css_parser_rule_t*      consume_at_rule(bool nested);

// static bool marks[20]       = { false };
// static uint32_t marks_idx   = 0;

static css_token_t tokens[100] = { 0 };
static uint32_t tokens_idx = 0;
static uint32_t tokens_size = 0;

/********************/
/* static variables */
/********************/


/********************/
/* static functions */
/********************/

static css_token_t* tokens_next()
{
    return &tokens[tokens_idx];
}


static void tokens_consume()
{
    tokens_idx++;
    assert(tokens_idx < 100);
}


static void tokens_discard()
{
    tokens_idx++;
    assert(tokens_idx < 100);
}


static css_parser_comp_val_t* consume_function()
{
    tokens_consume();

    css_parser_comp_val_t* func = css_parser_comp_val_new();

    bool run = true;
    while (run)
    {
        css_token_t* t = tokens_next();

        switch(t->type)
        {
            case CSS_TOKEN_EOF:
            case CSS_TOKEN_CLOSED_PARENTHESIS:
                tokens_discard();
                run = false;
                break;

            default:
                run = false;
                css_parser_comp_val_t* c_val = consume_comp_val();
                if (c_val) { css_parser_comp_val_add_comp_val(func, c_val); }
        }
    }

    return func;
}


static css_parser_comp_val_t* consume_simple_block()
{
    css_parser_comp_val_t* comp_val = css_parser_comp_val_new();

    css_token_t* t = tokens_next();

    css_token_type_e mirror = CSS_TOKEN_EOF;
    if (t->type == CSS_TOKEN_OPEN_BRACE)        { mirror = CSS_TOKEN_CLOSED_BRACE; }
    if (t->type == CSS_TOKEN_OPEN_BRACKET)      { mirror = CSS_TOKEN_CLOSED_BRACKET; }
    if (t->type == CSS_TOKEN_OPEN_PARENTHESIS)  { mirror = CSS_TOKEN_CLOSED_PARENTHESIS; }

    tokens_consume();

    t = tokens_next();

    if (t->type == CSS_TOKEN_EOF || t->type == mirror)
    {
        tokens_discard();
    }
    else
    {
        css_parser_comp_val_t* c_val = consume_comp_val();
        if (c_val) { css_parser_comp_val_add_comp_val(comp_val, c_val); }
    }

    return comp_val;
}


static css_parser_comp_val_t* consume_comp_val()
{
    bool run = true;
    while (run)
    {
        css_token_t* t = tokens_next();

        switch(t->type)
        {
            case CSS_TOKEN_OPEN_BRACE:
            case CSS_TOKEN_OPEN_BRACKET:
            case CSS_TOKEN_OPEN_PARENTHESIS:
                return consume_simple_block();

            case CSS_TOKEN_FUNCTION:
                return consume_function();

            default:
                tokens_consume();
                run = false;
        }
    }

    return NULL;
}


static void consume_comp_val_list(css_parser_decl_t* decl, css_token_type_e stop, bool nested)
{
    bool run = true;

    while (run)
    {
        css_token_t* t = tokens_next();

        if (t->type == CSS_TOKEN_EOF || t->type == stop)
        {
            run = false;
        }
        else if (t->type == CSS_TOKEN_CLOSED_BRACE)
        {
            run = false;
            if (!nested) { tokens_consume(); }
        }
        else
        {
            css_parser_comp_val_t* n_val = consume_comp_val();
            if (n_val) { css_parser_decl_add_comp_val(decl, n_val); }
        }
    }
}


static void consume_bad_decl(bool nested)
{
    bool run = true;
    while (run)
    {
        css_token_t* t = tokens_next();

        switch(t->type)
        {
            case CSS_TOKEN_EOF:
            case CSS_TOKEN_SEMICOLON:
                tokens_discard();
                run = false;
                break;

            case CSS_TOKEN_CLOSED_BRACE:
                if (nested)
                {
                    run = false;
                }
                else
                {
                    tokens_discard();
                }
                break;

            default:
                consume_comp_val();
        }
    }
}


static css_parser_decl_t* consume_decl(bool nested)
{
    css_parser_decl_t* decl = css_parser_decl_new();

    css_token_t* t = tokens_next();

    if (t->type == CSS_TOKEN_IDENT)
    {
        memcpy(decl->name, t->data, t->data_size);
        decl->name_size = t->data_size;
        tokens_consume();
    }
    else
    {
        consume_bad_decl(nested);
        return NULL;
    }

    t = tokens_next();
    while (t->type == CSS_TOKEN_WHITESPACE)
    {
        tokens_discard();
        t = tokens_next();
    }

    if (t->type == CSS_TOKEN_COLON)
    {
        tokens_discard();
    }
    else
    {
        consume_bad_decl(nested);
        return NULL;
    }

    t = tokens_next();
    while (t->type == CSS_TOKEN_WHITESPACE)
    {
        tokens_discard();
        t = tokens_next();
    }

    consume_comp_val_list(decl, CSS_TOKEN_SEMICOLON, nested);

    return decl;
}


static void consume_block(css_parser_rule_t* rule)
{
    css_parser_rule_t* t_rule = css_parser_rule_new();

    tokens_consume();

    bool run = true;
    while (run)
    {
        css_token_t* t = tokens_next();

        switch(t->type)
        {
            case CSS_TOKEN_WHITESPACE:
            case CSS_TOKEN_SEMICOLON:
                tokens_discard();
                break;

            case CSS_TOKEN_EOF:
            case CSS_TOKEN_CLOSED_BRACE:
                run = false;
                break;

            case CSS_TOKEN_AT_KEYWORD:
                if (t_rule->decls_size > 0)
                {
                    css_parser_decl_t* d = t_rule->decls;
                    css_parser_rule_add_decl(rule, d);
                    t_rule->decls = NULL;
                    t_rule->decls_size = 0;
                }

                css_parser_rule_t* n_rule = consume_at_rule(true);
                if (n_rule) { css_parser_rule_add_rule(rule, n_rule); }
                break;

            default:
                ;
                css_parser_decl_t* decl = consume_decl(true);
                if (decl) { css_parser_rule_add_decl(rule, decl); }

                // todo: handle the Otherwise clause
        }
    }

    return;
}


static css_parser_rule_t* consume_at_rule(bool nested)
{
    css_token_t* t = tokens_next();
    css_parser_rule_t* rule = css_parser_rule_new();
    memcpy(rule->name, t->data, t->data_size);
    rule->name_size = t->data_size;

    tokens_consume();

    bool run = true;
    while (run)
    {
        t = tokens_next();

        switch (t->type)
        {
            case CSS_TOKEN_SEMICOLON:
            case CSS_TOKEN_EOF:
                tokens_discard();
                run = false;
                break;

            case CSS_TOKEN_CLOSED_BRACE:
                if (nested)
                {
                    run = false;
                }
                else
                {
                    css_parser_comp_val_t* c_val = css_parser_comp_val_new();
                    c_val->type = CSS_PARSER_COMP_VAL_TOKEN;
                    c_val->token = *t;
                    css_parser_rule_add_comp_val(rule, c_val);
                }
                break;

            case CSS_TOKEN_OPEN_BRACE:
                consume_block(rule);
                break;

            default:
                ;
                css_parser_comp_val_t* c_val = consume_comp_val();
                if (c_val) { css_parser_rule_add_comp_val(rule, c_val); }
        }
    }

    return rule;
}


static css_parser_rule_t* consume_qualified_rule()
{
    return NULL;
}


/********************/
/* public functions */
/********************/


void css_parser_init(const unsigned char* buf, uint32_t buf_size)
{
    tokens_size = 0;
    memset(tokens, 0, sizeof(tokens));
    css_parser_types_reset();
    css_tokenizer_init(buf, buf_size);
}


css_parser_result_t css_parser_run()
{
    css_parser_result_t p_result = { .result = OPERATION_OK };

    // TEMP (mspasov): consume all tokens
    while (true)
    {
        tokens[tokens_size] = css_tokenizer_next();
        tokens_size++;

        if (tokens[tokens_size - 1].type == CSS_TOKEN_EOF)
        {
            break;
        }

        assert(tokens_size < 100);
    }

    bool run = true;

    while (run)
    {
        css_token_t* t = tokens_next();

        switch (t->type)
        {
            case CSS_TOKEN_WHITESPACE:
            case CSS_TOKEN_CDO:
            case CSS_TOKEN_CDC:
                tokens_discard();
                break;
    
            case CSS_TOKEN_EOF:
                run = false;
                break;
    
            case CSS_TOKEN_AT_KEYWORD:
                consume_at_rule(false);
                break;
    
            default:
                consume_qualified_rule();
        }
    }

    return p_result;
}


void css_parser_free()
{
    css_tokenizer_free();
}
