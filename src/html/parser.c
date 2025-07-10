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
static uint32_t stack_size                          = 0;
static html_node_t* document                        = NULL;

/********************/
/* static functions */
/********************/

static uint32_t get_tokens_size()
{
    uint32_t result = 0;
    for (uint32_t i = 0; i < MAX_TOKENS; i++) { if (tokens[i].is_valid) { result += 1; } }

    return result;
}


static void stack_push(html_node_t* node)
{
    if (stack_idx > 0) { stack_idx++; }
    stack[stack_idx] = node;

    stack_size++;
}


static void stack_pop()
{
    stack[stack_idx] = NULL;
    stack_idx = stack_idx > 0 ? stack_idx - 1 : 0
    stack_size--;
}


static bool stack_contains_element(string_t name)
{
    for (uint32_t i = 0; i < stack_size; i++)
    {
        if (!stack[i])                              { return false; }
        if (stack[i]->type != HTML_NODE_ELEMENT)    { continue; }
        if (string_compare(stack[i]->name, name))   { return true; }
    }

    return false;
}


static html_node_t* get_appropriate_insertion_location(html_node_t* override)
{
    html_node_t* location   = NULL
    html_node_t* target     = stack[stack_idx];

    if (override) { target = override; }

    if (false) // foster parenting
    {
    
    }
    else
    {
        location = target;
    }

    if (false) // inside template element
    {
    
    }

    return location;
}


static void insert_comment(html_token_t* token, html_node_t* position)
{
    html_node_t* location = get_appropriate_insertion_location(position);
    html_node_t* comment = html_comment_new(token.data, token.data_size, document);
    html_node_append(location, comment);
}

/********************/
/* public functions */
/********************/

void html_parser_run(const unsigned char* buffer, const uint32_t size)
{
    html_tokenizer_init(buffer, size, tokens, MAX_TOKENS);

    mode                        = HTML_PARSER_MODE_INITIAL;

    document                    = html_document_new();
    string_t html_string        = string_new("html", 4);
    string_t head_string        = string_new("head", 4);
    string_t body_string        = string_new("body", 4);
    string_t br_string          = string_new("br", 2);
    string_t base_string        = string_new("base", 4);
    string_t basefont_string    = string_new("basefont", 8);
    string_t bgsound_string     = string_new("bgsound", 7);
    string_t link_string        = string_new("link", 4);
    string_t meta_string        = string_new("meta", 4);
    string_t title_string       = string_new("title", 5);
    string_t script_string      = string_new("script", 6);
    string_t noscript_string    = string_new("noscript", 8);
    string_t noframes_string    = string_new("noframes", 8);
    string_t style_string       = string_new("style", 5);
    string_t template_string    = string_new("template", 8);
    string_t frameset_string    = string_new("frameset", 8);
    string_t dd_string          = string_new("dd", 2);
    string_t dt_string          = string_new("dt", 2);
    string_t li_string          = string_new("li", 2);
    string_t optgroup_string    = string_new("optgroup", 8);
    string_t option_string      = string_new("option", 6);
    string_t p_string           = string_new("p", 1);
    string_t rb_string          = string_new("rb", 2);
    string_t rp_string          = string_new("rp", 2);
    string_t rt_string          = string_new("rt", 2);
    string_t rtc_string         = string_new("rtc", 3);
    string_t tbody_string       = string_new("tbody", 5);
    string_t td_string          = string_new("td", 2);
    string_t tfoot_string       = string_new("tfoot", 5);
    string_t th_string          = string_new("th", 2);
    string_t thead_string       = string_new("thead", 5);
    string_t tr_string          = string_new("tr", 2);
    string_t address_string     = string_new("address", 7);
    string_t article_string     = string_new("article", 7);
    string_t aside_string       = string_new("aside", 5);
    string_t blockquote_string  = string_new("blockquote", 10);
    string_t center_string      = string_new("center", 6);
    string_t details_string     = string_new("details", 7);
    string_t dialog_string      = string_new("dialog", 6);
    string_t dir_string         = string_new("dir", 3);
    string_t div_string         = string_new("div", 3);
    string_t dl_string          = string_new("dl", 2);
    string_t fieldset_string    = string_new("fieldset", 8);
    string_t figcaption_string  = string_new("figcaption", 10);
    string_t figure_string      = string_new("figure", 6);
    string_t footer_string      = string_new("footer", 6);
    string_t header_string      = string_new("header", 6);
    string_t hgroup_string      = string_new("hgroup", 6);
    string_t main_string        = string_new("main", 4);
    string_t menu_string        = string_new("menu", 4);
    string_t nav_string         = string_new("nav", 3);
    string_t ol_string          = string_new("ol", 2);
    string_t search_string      = string_new("search", 6);
    string_t section_string     = string_new("section", 7);
    string_t summary_string     = string_new("summary", 7);
    string_t ul_string          = string_new("ul", 2);
    string_t h1_string          = string_new("h1", 2);
    string_t h2_string          = string_new("h2", 2);
    string_t h3_string          = string_new("h3", 2);
    string_t h4_string          = string_new("h4", 2);
    string_t h5_string          = string_new("h5", 2);
    string_t h6_string          = string_new("h6", 2);
    string_t pre_string         = string_new("pre", 3);
    string_t listing_string     = string_new("listing", 7);
    string_t form_string        = string_new("form", 4);
    string_t button_text        = string_new("button", 6);
    string_t plaintext_string   = string_new("plaintext", 9);
    string_t a_string           = string_new("a", 1);
    string_t b_string           = string_new("b", 1);
    string_t big_string         = string_new("big", 3);
    string_t code_string        = string_new("code", 4);
    string_t em_string          = string_new("em", 2);
    string_t i_string           = string_new("i", 1);
    string_t s_string           = string_new("s", 1);
    string_t small_string       = string_new("small", 5);
    string_t strike_string      = string_new("strike", 6);
    string_t strong_string      = string_new("strong", 6);
    string_t tt_string          = string_new("tt", 2);
    string_t u_string           = string_new("u", 1);
    string_t nobr_string        = string_new("nobr", 4);
    string_t applet_string      = string_new("applet", 6);
    string_t marquee_string     = string_new("marquee", 7);
    string_t object_string      = string_new("object", 6);
    string_t area_string        = string_new("area", 5);
    string_t embed_string       = string_new("embed", 5);
    string_t img_string         = string_new("img", 3);
    string_t keygen_string      = string_new("keygen", 6);
    string_t wbr_string         = string_new("wbr", 3);
    string_t input_string       = string_new("input", 5);
    string_t param_string       = string_new("param", 5);
    string_t source_string      = string_new("source", 6);
    string_t track_string       = string_new("track", 5);
    string_t hr_string          = string_new("hr", 2);
    string_t image_string       = string_new("image", 5);
    string_t textarea_string    = string_new("textarea", 8);
    string_t xmp_string         = string_new("xmp", 3);
    string_t iframe_string      = string_new("iframe", 6);
    string_t noembed_string     = string_new("noembed", 6);
    string_t select_string      = string_new("select", 6);
    string_t col_string         = string_new("col", 3);
    string_t colgroup_string    = string_new("colgroup", 8);
    
    bool will_use_rules_for     = false;
    bool use_rules_for          = false;
    html_node_t* head_element   = NULL;
    html_node_t* form_element   = NULL;
    bool scripting_enabled      = false;

    while (true)
    {
        html_tokenizer_error_e err  = html_tokenizer_next();
        uint32_t tokens_size        = get_tokens_size();
        uint32_t i                  = 0;

        if (err != HTML_TOKENIZER_OK || tokens_size == 0) { break; }


        while (i < tokens_size)
        {
            consume = true;
            html_token_t t = tokens[i];

            bool is_doctype     = t.type == HTML_DOCTYPE_TOKEN;
            bool is_start       = t.type == HTML_START_TOKEN;
            bool is_end         = t.type == HTML_END_TOKEN;
            bool is_comment     = t.type == HTML_COMMENT_TOKEN;
            bool is_character   = t.type == HTML_CHARACTER_TOKEN;
            bool is_eof         = t.type == HTML_EOF_TOKEN;

            switch (mode)
            {

            // https://html.spec.whatwg.org/multipage/parsing.html#the-initial-insertion-mode
            case HTML_PARSER_MODE_INITIAL:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    // ignore
                }
                else if (is_comment)
                {
                    insert_comment(&t, document);
                }
                else if (is_doctype)
                {
                    
                }
                else
                {
                    
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#the-before-html-insertion-mode
            case HTML_PARSER_MODE_BEFORE_HTML:
                if (is_doctype)
                {
                    // todo: parse error
                }
                else if (is_comment)
                {
                    insert_comment(&t, document);
                }
                else if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    // ignore
                }
                else if (is_start && name_is(html_string, &t))
                {
                    
                }
                else
                {
                    
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#the-before-head-insertion-mode
            case HTML_PARSER_MODE_BEFORE_HEAD:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    // ignore
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                    // todo: parse error, ignore token
                }
                else if (is_start && name_is(html_string, &t))
                {
                    // consume = false;
                    // mode = HTML_PARSER_MODE_IN_BODY;
                    // original_mode = HTML_PARSER_MODE_BEFORE_HEAD;
                    // will_use_rules_for = true;
                }
                else if (is_start && name_is(head_string, &t))
                {
                    
                }
                else
                {
                    
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inhead
            case HTML_PARSER_MODE_IN_HEAD:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                    // todo: parse error, ignore token
                }
                else if (is_start && name_is(html_string, &t))
                {
                    
                }
                else if (is_start && (name_is(base_string, &t) || name_is(basefont_string, &t) ||
                                      name_is(bgsound_string, &t) || name_is(link_string, &t))
                {
                    
                }
                else if (is_start && name_is(meta_string, &t))
                {
                    
                }
                else if (is_start && name_is(title_string, &t))
                {
                    
                }
                else if ((is_start && name_is(noscript_string, &t) && scripting_enabled) || 
                         (is_start && (name_is(noframes_string, &t) || name_is(style_string, &t)))
                {
                    
                }
                else if (is_start && name_is(noscript_string, &t) && !scripting_enabled)
                {
                    
                }
                else if (is_start && name_is(script_string, &t))
                {
                    assert(false);
                }
                else if (is_end && name_is(head_string, &t))
                {
                    
                }
                else if (is_start && name_is(template_string, &t))
                {
                    assert(false)
                }
                else if (is_end && name_is(template_string, &t))
                {
                    assert(false);
                }
                else
                {
                    
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inheadnoscript
            case HTML_PARSER_MODE_IN_HEAD_NOSCRIPT:
                if (is_doctype)
                {
                    // todo: parse error, ignore token
                }
                else if (is_start && name_is(html_string, &t))
                {
                    
                }
                else if (t.type == HTML_END_TAG && name_is(noscript_string, &t))
                {
                    
                }
                else if ((is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' ') ) ||
                         (is_comment) ||
                         (is_start && (name_is(basefont_string, &t)|| name_is(bgsound_string, &t) ||
                                       name_is(link_string, &t)    || name_is(meta_string, &t) ||
                                       name_is(noframes_string, &t)|| name_is(style_string, &t))
                {
                    
                }
                else if (is_start && (name_is(head_string, &t) || name_is(noscript_string, &t))
                {
                    // todo: parse error
                }
                else
                {
                    // todo: parse error
                    
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#the-after-head-insertion-mode
            case HTML_PARSER_MODE_AFTER_HEAD:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                    // todo: parse error
                }
                else if (is_start && name_is(html_string, &t))
                {
                    
                }
                else if (is_start && name_is(body_string, &t))
                {
                    
                }
                else if (is_start && name_is(frameset_string, &t))
                {
                    
                }
                else if (is_start && (name_is(base_string, &t)     || name_is(basefont_string, &t)|| name_is(bgsound_string, &t) ||
                                      name_is(link_string, &t)     || name_is(meta_string, &t)    || name_is(noframes_string, &t) ||
                                      name_is(script_string, &t)   || name_is(template_string, &t)|| name_is(title_string, &t) ||
                                      name_is(style_string, &t))
                {
                    // todo: parse error
                    
                }
                else if (is_end && name_is(template_string, &t))
                {
                    
                }
                else if ( (is_start && name_is(head_string, &t) ||
                          (is_end && !(name_is(body_string, &t) || name_is(html_string, &t) || name_is(br_string, &t)))
                {
                    // todo: parse error
                }
                else
                {
                    
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inbody
            case HTML_PARSER_MODE_IN_BODY:
                if (is_character && t.data[0] == '\0')
                {
                    // todo: parse error
                }
                else if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    
                }
                else if (is_character)
                {
                    
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                    // todo: parse error
                }
                else if (is_start && name_is(html_string, &t))
                {
                    // todo: parse error
                    
                }
                else if ((is_start && (name_is(base_string, &t)    || name_is(basefont_string, &t)|| name_is(bgsound_string, &t) ||
                                       name_is(link_string, &t)    || name_is(meta_string, &t)    || name_is(noframes_string, &t) ||
                                       name_is(script_string, &t)  || name_is(template_string, &t)|| name_is(title_string, &t) ||
                                       name_is(style_string, &t))  ||
                        (is_end && name_is(template_string, &t))
                {
                    
                }
                else if (is_start && name_is(body_string, &t)
                {
                    // todo: parse error
                }
                else if (is_start && name_is(frameset_string, &t)
                {
                    // todo: parse error
                }
                else if (is_eof)
                {
                    
                }
                else if (is_end && string_compare(body_string, &t))
                {
                    
                }
                else if (is_end && string_compare(html_string, &t))
                {
                    
                }
                else if (is_start && (name_is(address_string, &t)      || name_is(article_string, &t) || name_is(aside_string, &t)       ||
                                      name_is(blockquote_string, &t)   || name_is(center_string, &t)  || name_is(details_string, &t)     ||
                                      name_is(dialog_string, &t)       || name_is(dir_string, &t)     || name_is(div_string, &t)         ||
                                      name_is(dl_string, &t)           || name_is(fieldset_string, &t)|| name_is(figcaption_string, &t)  ||
                                      name_is(figure_string, &t)       || name_is(footer_string, &t)  || name_is(header_string, &t)      ||
                                      name_is(hgroup_string, &t)       || name_is(main_string, &t)    || name_is(menu_string, &t)        ||
                                      name_is(nav_string, &t)          || name_is(ol_string, &t)      || name_is(p_string, &t)           ||
                                      name_is(search_string, &t)       || name_is(section_string, &t) || name_is(summary_string, &t)     ||
                                      name_is(ul_string, &t)))
                {
                    
                }
                else if (is_start && (name_is(h1_string, &t)   || name_is(h2_string, &t)  || name_is(h3_string, &t)  ||
                                      name_is(h4_string, &t)   || name_is(h5_string, &t)  || name_is(h6_string, &t)  || ))
                {
                    
                }
                else if (is_start && (name_is(pre_string, &t) || name_is(listing_string, &t)))
                {
                    
                }
                else if (is_start && name_is(form_string, &t)
                {
                    
                }
                else if (is_start && name_is(li_string, &t))
                {
                
                }
                else if (is_start && (name_is(dt_string, &t) || name_is(dd_string, &t)))
                {
                    // todo: implement
                }
                else if (is_start && name_is(plaintext_string, &t))
                {
                    // todo: handle scope logic - If the stack of open elements has a p element in button scope, then close a p element.
                    
                }
                else if (is_start && name_is(button_text, &t))
                {
                    
                }
                else if (is_end && (name_is(address_string, &t)    || name_is(article_string, &t) || name_is(aside_string, &t)   ||
                                    name_is(blockquote_string, &t) || name_is(button_string, &t)  || name_is(center_string, &t)  ||
                                    name_is(details_string, &t)    || name_is(dialog_string, &t)  || name_is(dir_string, &t)     ||
                                    name_is(div_string, &t)        || name_is(dl_string, &t)      || name_is(fieldset_string, &t)||
                                    name_is(figcaption_string, &t) || name_is(figure_string, &t)  || name_is(footer_string, &t)  ||
                                    name_is(header_string, &t)     || name_is(hgroup_string, &t)  || name_is(listing_string, &t) ||
                                    name_is(main_string, &t)       || name_is(menu_string, &t)    || name_is(nav_string, &t)     ||
                                    name_is(ol_string, &t)         || name_is(pre_string, &t)     || name_is(search_string, &t)  ||
                                    name_is(section_string, &t)    || name_is(summary_string, &t) || name_is(ul_string, &t) ))
                {
                    
                }
                else if (is_end && name_is(form_string, &t))
                {
                    
                }
                else if (is_end && name_is(p_string, &t))
                {
                    
                }
                else if (is_end && name_is(li_string, &t))
                {
                    
                }
                else if (is_end && (name_is(dd_string, &t) || name_is(dt_string, &t) ))
                {
                
                }
                else if (is_end && (name_is(h1_string, &t) || name_is(h2_string, &t) || name_is(h3_string, &t) ||
                                    name_is(h4_string, &t) || name_is(h5_string, &t) || name_is(h6_string, &t)))
                {
                
                }
                else if (is_start && name_is(a_string, &t))
                {
                
                }
                else if (is_start && (name_is(b_string, &t) || name_is(big_string, &t) || name_is(code_string, &t) ||
                                      name_is(em_string, &t) || name_is(font_string, &t) || name_is(i_string, &t) ||
                                      name_is(s_string, &t) || name_is(small_string, &t) || name_is(strike_string, &t) ||
                                      name_is(strong_string, &t) || name_is(tt_string, &t) || name_is(u_string, &t)) )
                {
                
                }
                else if (is_start && name_is(nobr_string, &t))
                {
                
                }
                else if (is_end && (name_is(a_string, &t) || name_is(b_string, &t) || name_is(big_string, &t) || name_is(code_string, &t) ||
                                    name_is(em_string, &t) || name_is(font_string, &t) || name_is(i_string, &t) ||
                                    name_is(nobr_string, &t) || name_is(s_string, &t) || name_is(small_string, &t) ||
                                    name_is(strike_string, &t) || name_is(strong_string, &t) || name_is(tt_string, &t) || name_is(u_string, &t)))
                {
                    
                }
                else if (is_start && (name_is(applet_string, &t) || name_is(marquee_string, &t) || name_is(object_string, &t)) )
                {
                
                }
                else if (is_end && (name_is(applet_string, &t) || name_is(marquee_string, &t) || name_is(object_string, &t)) )
                {
                
                }
                else if (is_start && name_is(table_string, &t)
                {
                
                }
                else if (is_end && name_is(br_string, &t))
                {
                
                }
                else if (is_start && (name_is(area_string, &t) || name_is(br_string, &t) || name_is(embed_string, &t) ||
                                      name_is(img_string, &t) || name_is(keygen_string, &t) || name_is(wbr_string, &t) ))
                {
                    
                }
                else if (is_start && name_is(input_string, &t))
                {
                
                }
                else if (is_start && (name_is(param_string, &t) || name_is(source_string, &t) || name_is(track_string, &t) ))
                {
                
                }
                else if (is_start && name_is(hr_string, &t))
                {
                
                }
                else if (is_start && name_is(image_string, &t))
                {
                
                }
                else if (is_start && name_is(textarea_string, &t))
                {
                
                }
                else if (is_start && name_is(xmp_string, &t))
                {
                
                }
                else if (is_start && name_is(iframe_string, &t) )
                {
                
                }
                else if ( (is_start && name_is(noembed_string, &t) ) ||
                          (is_start && name_is(noscript_string) && scripting_enabled) )
                {
                
                }
                else if (is_start && name_is(select_string, &t) )
                {
                
                }
                else if (is_start && (name_is(optgroup_string, &t) || name_is(option_string, &t)))
                {
                
                }
                else if (is_start && (name_is(rb_string, &t) || name_is(rtc_string, &t)))
                {
                
                }
                else if (is_start && (name_is(rp_string, &t) || name_is(rt_string, &t)))
                {
                
                }
                else if (is_start && name_is(math_string, &t))
                {
                
                }
                else if (is_start && name_is(svg_string, &t))
                {
                
                }
                else if (is_start && (name_is(caption_string, &t) || name_is(col_string, &t) || name_is(colgroup_string, &t) || 
                                      name_is(frame_string, &t) || name_is(head_string, &t) || name_is(tbody_string, &t) ||
                                      name_is(td_string, &t) || name_is(tfoot_string, &t) || name_is(th_string, &t) ||
                                      name_is(thead_string, &t) || name_is(tr_string, &t) ))
                {
                    
                }
                else if (is_start)
                {
                
                }
                else if (is_end)
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incdata
            case HTML_PARSER_MODE_TEXT:
                if (is_character)
                {
                
                }
                else if (is_eof)
                {
                
                }
                else if (is_end && name_is(script_string, &t))
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intable
            case HTML_PARSER_MODE_IN_TABLE:
                string_t current = stack[stack_idx]->name;
                if (is_character && (string_compare(current, table_string) || string_compare(current, tbody_string) ||
                                     string_compare(current, template_string) || string_compare(current, tfoot_string) ||
                                     string_compare(current, thead_string) || string_compare(current, tr_string)))
                {
                
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                
                }
                else if (is_start && name_is(caption_string, &t))
                {
                
                }
                else if (is_start && name_is(colgroup_string, &t))
                {
                
                }
                else if (is_start && name_is(col_string, &t))
                {
                
                }
                else if (is_start && (name_is(tbody_string, &t) || name_is(tfoot_string, &t) || name_is(thead_string, &t)))
                {
                
                }
                else if (is_start && (name_is(td_string, &t) || name_is(th_string, &t) || name_is(tr_string)))
                {
                
                }
                else if (is_start && name_is(table_string, &t))
                {
                    
                }
                else if (is_end && name_is(table_string, &t))
                {
                
                }
                else if (is_end && (name_is(body_string, &t) || name_is(caption_string, &t) ||
                                    name_is(col_string, &t) || name_is(colgroup_string, &t) ||
                                    name_is(html_string, &t) || name_is(tbody_string, &t) ||
                                    name_is(td_string, &t) || name_is(tfoot_string, &t) ||
                                    name_is(th_string, &t) || name_is(thead_string, &t) ||
                                    name_is(tr_string, &t)))
                {
                
                }
                else if ((is_start && (name_is(style_string, &t) || name_is(script_string, &t) || name_is(template_string))) ||
                         (is_end && name_is(template_string)))
                {
                
                }
                else if (is_start && name_is(input_string, &t))
                {
                
                }
                else if (is_start && name_is(form_string, &t))
                {
                
                }
                else if (is_eof)
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intabletext
            case HTML_PARSER_MODE_IN_TABLE_TEXT:
                if (is_character && t.data_size == 1 && t.data[0] == '\0')
                {
                
                }
                else if (is_character)
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incaption
            case HTML_PARSER_MODE_IN_CAPTION:
                if (is_end && name_is(caption_string, &t))
                {
                
                }
                else if ((is_start && (name_is(caption_string, &t)  || name_is(col_string, &t)  ||
                                       name_is(colgroup_string, &t) || name_is(tbody_string, &t)||
                                       name_is(td_string, &t)       || name_is(tfoot_string, &t)||
                                       name_is(th_string, &t)       || name_is(thead_string, &t)||
                                       name_is(tr_string, &t))) ||
                         (is_end && name_is(table_string, &t)))
                {
                
                }
                else if (is_end && (name_is(body_string, &t) || name_is(col_string, &t) ||
                                    name_is(colgroup_string, &t) || name_is(html_string, &t) ||
                                    name_is(tbody_string, &t) || name_is(td_string, &t) ||
                                    name_is(tfoot_string, &t) || name_is(th_string, &t) ||
                                    name_is(thead_string, &t) || name_is(tr_string, &t)))
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incolgroup
            case HTML_PARSER_MODE_IN_COLUMN_GROUP:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                
                }
                else if (is_start && name_is(html_string, &t))
                {
                
                }
                else if (is_start && name_is(col_string, &t))
                {
                
                }
                else if (is_end && name_is(colgroup_string, &t))
                {
                
                }
                else if (is_end && name_is(col_string, &t))
                {
                
                }
                else if ((is_start || is_end) && (name_is(template_string, &t)))
                {
                
                }
                else if (is_eof)
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intbody
            case HTML_PARSER_MODE_IN_TABLE_BODY:
                if (is_start && name_is(tr_string))
                {
                
                }
                else if (is_start && (name_is(th_string, &t) || name_is(td_string, &t)))
                {
                
                }
                else if (is_end && ( name_is(tbody_string, &t) || name_is(tfoot_string, &t) || name_is(thead_string, &t) ))
                {
                
                }
                else if ((is_start && (name_is(caption_string, &t) || name_is(col_string, &t) ||
                                       name_is(colgroup_string, &t) || name_is(tbody_string, &t) ||
                                       name_is(tfoot_string, &t) || name_is(thead_string, &t))) ||
                         (is_end && name_is(table_string, &t)))
                {
                
                }
                else if (is_end && && (name_is(body_string, &t) || name_is(caption_string, &t) ||
                                       name_is(col_string, &t) || name_is(colgroup_string, &t) ||
                                       name_is(html_string, &t) || name_is(td_string, &t) ||
                                       name_is(th_string, &t) || name_is(tr_string, &t)))
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intr
            case HTML_PARSER_MODE_IN_ROW:
                if (is_start && (name_is(th_string, &t) || name_is(td_string, &t)))
                {
                
                }
                else if (is_end && name_is(tr_string, &t))
                {
                
                }
                else if ((is_start && (name_is(caption_string, &t) || name_is(col_string, &t) ||
                                       name_is(colgroup_string, &t) || name_is(tbody_string, &t) ||
                                       name_is(tfoot_string, &t) || name_is(thead_string, &t)) ||
                                       name_is(tr_string, &t))) ||
                         (is_end && name_is(table_string, &t)))
                {
                
                }
                else if (is_end && (name_is(tbody_string, &t) || name_is(thead_string, &t) || name_is(tfoot_string, &t)))
                {
                
                }
                else if (is_end && (name_is(body_string, &t) || name_is(caption_string, &t) ||
                                    name_is(col_string, &t) || name_is(colgroup_string, &t) ||
                                    name_is(html_string, &t) || name_is(td_string, &t) ||
                                    name_is(th_string, &t)))
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intd
            case HTML_PARSER_MODE_IN_CELL:
                if (is_end && (name_is(td_string, &t) || name_is(th_string, &t)))
                {
                
                }
                else if (is_start && (name_is(caption_string, &t) || name_is(col_string, &t) ||
                                      name_is(colgroup_string, &t) || name_is(tbody_string, &t) ||
                                      name_is(tfoot_string, &t) || name_is(thead_string, &t)) ||
                                      name_is(tr_string, &t) || name_is(th_string, &t) ||
                                      name_is(td_string, &t)))
                {
                
                }
                else if (is_end && (name_is(body_string, &t) || name_is(caption_string, &t) ||
                                    name_is(col_string, &t) || name_is(colgroup_string, &t) ||
                                    name_is(html_string, &t)))
                {
                
                }
                else if (is_end && (name_is(table_string, &t) || name_is(tfoot_string, &t) ||
                                    name_is(thead_string, &t) || name_is(tbody_string, &t) ||
                                    name_is(tr_string, &t)))
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inselect
            case HTML_PARSER_MODE_IN_SELECT:
                if (is_character && t.data_size == 1 && t.data[0] == '\0')
                {
                
                }
                else if (is_character)
                {
                
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                
                }
                else if (is_start && name_is(html_string, &t))
                {
                
                }
                else if (is_start && name_is(option_string, &t))
                {
                
                }
                else if (is_start && name_is(optgroup_string, &t))
                {
                
                }
                else if (is_start && name_is(hr_string, &t))
                {
                
                }
                else if (is_end && name_is(optgroup_string, &t))
                {
                
                }
                else if (is_end && name_is(option_string, &t))
                {
                
                }
                else if (is_end && name_is(select_string, &t))
                {
                
                }
                else if (is_start && name_is(select_string, &t))
                {
                
                }
                else if (is_start && (name_is(input_string, &t) || name_is(keygen_string, &t) || name_is(textarea_string, &t)))
                {
                
                }
                else if ((is_start && (name_is(script_string, &t) || name_is(template_string, &t)) ||
                         (is_end && name_is(template_string, &t)))
                {
                
                }
                else if (is_eof)
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inselectintable
            case HTML_PARSER_MODE_IN_SELECT_IN_TABLE:
                if (is_start && (name_is(caption_string, &t) || name_is(table_string, &t) ||
                                 name_is(tbody_string, &t) || name_is(tfoot_string, &t) ||
                                 name_is(thead_string, &t) || name_is(tr_string, &t) ||
                                 name_is(td_string, &t) || name_is(th_string, &t)))
                {
                
                }
                else if (is_end && (name_is(caption_string, &t) || name_is(table_string, &t) ||
                                    name_is(tbody_string, &t) || name_is(tfoot_string, &t) ||
                                    name_is(thead_string, &t) || name_is(tr_string, &t) ||
                                    name_is(td_string, &t) || name_is(th_string, &t)))
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intemplate
            case HTML_PARSER_MODE_IN_TEMPLATE:
                if (is_character || is_comment || is_doctype)
                {
                
                }
                else if ((is_start && (name_is(base_string, &t) || name_is(basefont_string, &t) ||
                                      name_is(bgsound_string, &t) || name_is(link_string, &t) ||
                                      name_is(meta_string, &t) || name_is(noframes_string, &t) ||
                                      name_is(script_string, &t) || name_is(style_string, &t) ||
                                      name_is(template_string, &t) || name_is(title_string, &t))) ||
                         (is_end && name_is(template_string, &t)))
                {
                
                }
                else if (is_start && (name_is(caption_string, &t) || name_is(colgroup_string, &t) ||
                                      name_is(tbody_string, &t) || name_is(tfoot_string, &t) ||
                                      name_is(thead_string, &t)))
                {
                
                }
                else if (is_start && name_is(col_string, &t))
                {
                
                }
                else if (is_start && name_is(tr_string, &t))
                {
                
                }
                else if (is_start && (name_is(td_string, &t) || name_is(th_string, &t)))
                {
                
                }
                else if (is_start)
                {
                
                }
                else if (is_end)
                {
                
                }
                else if (is_eof)
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-afterbody
            case HTML_PARSER_MODE_AFTER_BODY:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                
                }
                else if (is_comment)
                {
                    insert_comment(&t, stack[0]);
                }
                else if (is_doctype)
                {
                
                }
                else if (is_start && name_is(html_string, &t))
                {
                
                }
                else if (is_end && name_is(html_string, &t))
                {
                
                }
                else if (is_eof)
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inframeset
            case HTML_PARSER_MODE_IN_FRAMESET:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                
                }
                else if (is_start && name_is(html_string, &t))
                {
                
                }
                else if (is_start && name_is(frameset_string, &t))
                {
                
                }
                else if (is_end && name_is(frameset_string, &t))
                {
                
                }
                else if (is_start && name_is(frame_string, &t))
                {
                
                }
                else if (is_start && name_is(noframes_string, &t))
                {
                
                }
                else if (is_eof)
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-afterframeset
            case HTML_PARSER_MODE_AFTER_FRAMESET:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                
                }
                else if (is_start && name_is(html_string, &t))
                {
                
                }
                else if (is_end && name_is(html_string, &t))
                {
                
                }
                else if (is_start && name_is(noframes_string, &t))
                {
                
                }
                else if (is_eof)
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#the-after-after-body-insertion-mode
            case HTML_PARSER_MODE_AFTER_AFTER_BODY:
                if (is_comment)
                {
                    insert_comment(&t, document);
                }
                else if ((is_doctype) ||
                         (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' ')) ||
                         (is_start && name_is(html_string, &t)))
                {
                
                }
                else if (is_eof)
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#the-after-after-frameset-insertion-mode
            case HTML_PARSER_MODE_AFTER_AFTER_FRAMESET:
                if (is_comment)
                {
                    insert_comment(&t, document);
                }
                else if ((is_doctype) ||
                         (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' ')) ||
                         (is_start && name_is(html_string, &t)))
                {
                
                }
                else if (is_eof)
                {
                
                }
                else if (is_start && name_is(noframes_string, &t))
                {
                
                }
                else
                {
                
                }
                break;
            }
        }

        if (use_rules_for)      { use_rules_for = false; mode = original_mode; }
        if (will_use_rules_for) { will_use_rules_for = false; use_rules_for = true; }
        if (consume)            { i++; }
    }

    html_tokenizer_free();
}

void html_parser_free()
{
    
}