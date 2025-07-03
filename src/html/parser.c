#include "parser.h"

#include "html/tokenizer.h"
#include "dom/node.h"

/*
 * Notes
 * 
 */

/********************/
/*      defines     */
/********************/

#define MAX_TOKENS          10
#define OPEN_STACK_MAX_SIZE 500

/********************/
/* static variables */
/********************/

static html_parser_mode_e mode                      = HTML_PARSER_MODE_INITIAL;
static html_parser_mode_e original_mode             = HTML_PARSER_MODE_INITIAL;
static html_token_t tokens[MAX_TOKENS]              = { 0 };
static html_node_t* stack[OPEN_STACK_MAX_SIZE]      = { 0 };
static uint32_t stack_idx                           = 0;

/********************/
/* static functions */
/********************/

static uint32_t get_tokens_size()
{
    uint32_t result = 0;
    for (uint32_t i = 0; i < MAX_TOKENS; i++) { if (tokens[i].is_valid) { result += 1; } }

    return result;
}


static html_node_t* get_appropriate_insert_position(html_node_t* current, html_node_t* override_target)
{
    html_node_t* position = NULL;
    html_node_t* target = current;
    if (override_target) { target = override_target; }

    if (false)                                          // foster parenting
    {
        assert(false)
    }
    else
    {
        position = target;
    }

    if (false)                                          // inside template
    {
        assert(false);
    }

    return position;
}


static html_node_t* create_comment_from_token(html_token_t token, html_node_t* document)
{
    assert(token->type == HTML_COMMENT_TOKEN);

    return html_comment_new(token.data, token.data_size, document);
}


static html_node_t* create_doctype_from_token(html_token_t token, html_node_t* document)
{
    assert(token->type == HTML_DOCTYPE_TOKEN)

    unsigned char* name         = token.name;
    unsigned char* public_id    = token.public_id;
    unsigned char* system_id    = token.system_id;
    uint32_t name_size          = token.name_size;
    uint32_t public_id_size     = token.public_id_size;
    uint32_t system_id_size     = token.system_id_size;

    return html_doctype_new(name, name_size, public_id, public_id_size, system_id, system_id_size);
}


/********************/
/* public functions */
/********************/

void html_parser_run(const unsigned char* buffer, const uint32_t size)
{
    mode = HTML_PARSER_MODE_INITIAL;

    html_tokenizer_init(buffer, size, tokens, MAX_TOKENS);
    html_node_t* document = html_document_new();

    while (true)
    {
        html_tokenizer_error_e err  = html_tokenizer_next();
        uint32_t tokens_size        = get_tokens_size();

        if (err != HTML_TOKENIZER_OK || tokens_size == 0) { break; }

        for (uint32_t i = 0; i < tokens_size; i++)
        {
            html_token_t t = tokens[i];

            switch (mode)
            {

            // https://html.spec.whatwg.org/multipage/parsing.html#the-initial-insertion-mode
            case HTML_PARSER_MODE_INITIAL:
                if (t.type == HTML_CHARACTER_TOKEN && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    // ignore
                }
                else if (t.type == HTML_COMMENT_TOKEN)
                {
                    html_node_t* comment = create_comment_from_token(t, document);
                    html_node_append(document, comment);
                }
                else if (t.type == HTML_DOCTYPE_TOKEN)
                {
                    html_node_t* doctype = create_doctype_from_token(t, document);
                    document->doctype = doctype;

                    bool is_html = string_compare_buffer(doctype->name, token.name, token.name_size);
                    bool public_id_missing = doctype->public_id.size == 0;
                    bool system_id_missing = doctype->system_id.size == 0;
                    unsigned char legacy_compat[] = "about:legacy-compat"
                    bool system_id_legacy_compat = string_compare_buffer(doctype->system_id, legacy_compat, sizeof(legacy_compat) - 1);

                    if (!is_html || !public_id_missing || !system_id_missing || !system_id_legacy_compat)
                    {
                        // parse error
                    }

                    // todo: https://trello.com/c/gArsGkAc/11-add-iframe-related-logic-to-the-parser

                    mode = HTML_PARSER_MODE_BEFORE_HTML;
                }
                else
                {
                    // todo: https://trello.com/c/gArsGkAc/11-add-iframe-related-logic-to-the-parser

                    mode = HTML_PARSER_MODE_BEFORE_HTML;
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#the-before-html-insertion-mode
            case HTML_PARSER_MODE_BEFORE_HTML:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#the-before-head-insertion-mode
            case HTML_PARSER_MODE_BEFORE_HEAD:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inhead
            case HTML_PARSER_MODE_IN_HEAD:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inheadnoscript
            case HTML_PARSER_MODE_IN_HEAD_NOSCRIPT:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#the-after-head-insertion-mode
            case HTML_PARSER_MODE_AFTER_HEAD:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inbody
            case HTML_PARSER_MODE_IN_BODY:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incdata
            case HTML_PARSER_MODE_TEXT:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intable
            case HTML_PARSER_MODE_IN_TABLE:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intabletext
            case HTML_PARSER_MODE_IN_TABLE_TEXT:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incaption
            case HTML_PARSER_MODE_IN_CAPTION:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incolgroup
            case HTML_PARSER_MODE_IN_COLUMN_GROUP:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intbody
            case HTML_PARSER_MODE_IN_TABLE_BODY:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intr
            case HTML_PARSER_MODE_IN_ROW:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intd
            case HTML_PARSER_MODE_IN_CELL:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inselect
            case HTML_PARSER_MODE_IN_SELECT:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inselectintable
            case HTML_PARSER_MODE_IN_SELECT_IN_TABLE:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intemplate
            case HTML_PARSER_MODE_IN_TEMPLATE:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-afterbody
            case HTML_PARSER_MODE_AFTER_BODY:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inframeset
            case HTML_PARSER_MODE_IN_FRAMESET:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-afterframeset
            case HTML_PARSER_MODE_AFTER_FRAMESET:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#the-after-after-body-insertion-mode
            case HTML_PARSER_MODE_AFTER_AFTER_BODY:
                
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#the-after-after-frameset-insertion-mode
            case HTML_PARSER_MODE_AFTER_AFTER_FRAMESET:
                
                break;
            }
        }
    }

    html_tokenizer_free();
}

void html_parser_free()
{
    
}