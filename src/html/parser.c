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

#define HTML                "html"
#define HTML_SIZE           4
#define HEAD                "head"
#define HEAD_SIZE           4
#define BODY                "body"
#define BODY_SIZE           4
#define BR                  "br"
#define BR_SIZE             2
#define BASE                "base"
#define BASE_SIZE           4
#define BASEFONT            "basefont"
#define BASEFONT_SIZE       8
#define BGSOUND             "bgsound"
#define BGSOUND_SIZE        7
#define LINK                "link"
#define LINK_SIZE           4
#define META                "meta"
#define META_SIZE           4
#define TITLE               "title"
#define TITLE_SIZE          5
#define SCRIPT              "script"
#define SCRIPT_SIZE         6
#define NOSCRIPT            "noscript"
#define NOSCRIPT_SIZE       8
#define NOFRAMES            "noframes"
#define NOFRAMES_SIZE       8
#define STYLE               "style"
#define STYLE_SIZE          5
#define TEMPLATE            "template"
#define TEMPLATE_SIZE       8
#define FRAMESET            "frameset"
#define FRAMESET_SIZE       8
#define DD                  "dd"
#define DD_SIZE             2
#define DT                  "dt"
#define DT_SIZE             2
#define LI                  "li"
#define LI_SIZE             2
#define OPTGROUP            "optgroup"
#define OPTGROUP_SIZE       8
#define OPTION              "option"
#define OPTION_SIZE         6
#define P                   "p"
#define P_SIZE              1
#define RB                  "rb"
#define RB_SIZE             2
#define RP                  "rp"
#define RP_SIZE             2
#define RT                  "rt"
#define RT_SIZE             2
#define RTC                 "rtc"
#define RTC_SIZE            3
#define TBODY               "tbody"
#define TBODY_SIZE          5
#define TD                  "td"
#define TD_SIZE             2
#define TFOOT               "tfoot"
#define TFOOT_SIZE          5
#define TH                  "th"
#define TH_SIZE             2
#define THEAD               "thead"
#define THEAD_SIZE          5
#define TR                  "tr"
#define TR_SIZE             2
#define ADDRESS             "address"
#define ADDRESS_SIZE        7
#define ARTICLE             "article"
#define ARTICLE_SIZE        7
#define ASIDE               "aside"
#define ASIDE_SIZE          5
#define BLOCKQUOTE          "blockquote"
#define BLOCKQUOTE_SIZE     10
#define CENTER              "center"
#define CENTER_SIZE         6
#define DETAILS             "details"
#define DETAILS_SIZE        7
#define DIALOG              "dialog"
#define DIALOG_SIZE         6
#define DIR                 "dir"
#define DIR_SIZE            3
#define DIV                 "div"
#define DIV_SIZE            3
#define DL                  "dl"
#define DL_SIZE             2
#define FIELDSET            "fieldset"
#define FIELDSET_SIZE       8
#define FIGCAPTION          "figcaption"
#define FIGCAPTION_SIZE     10
#define FIGURE              "figure"
#define FIGURE_SIZE         6
#define FOOTER              "footer"
#define FOOTER_SIZE         6
#define HEADER              "header"
#define HEADER_SIZE         6
#define HGROUP              "hgroup"
#define HGROUP_SIZE         6
#define MAIN                "main"
#define MAIN_SIZE           4
#define MENU                "menu"
#define MENU_SIZE           4
#define NAV                 "nav"
#define NAV_SIZE            3
#define OL                  "ol"
#define OL_SIZE             2
#define SEARCH              "search"
#define SEARCH_SIZE         6
#define SECTION             "section"
#define SECTION_SIZE        7
#define SUMMARY             "summary"
#define SUMMARY_SIZE        7
#define UL                  "ul"
#define UL_SIZE             2
#define H1                  "h1"
#define H1_SIZE             2
#define H2                  "h2"
#define H2_SIZE             2
#define H3                  "h3"
#define H3_SIZE             2
#define H4                  "h4"
#define H4_SIZE             2
#define H5                  "h5"
#define H5_SIZE             2
#define H6                  "h6"
#define H6_SIZE             2
#define PRE                 "pre"
#define PRE_SIZE            3
#define LISTING             "listing"
#define LISTING_SIZE        7
#define FORM                "form"
#define FORM_SIZE           4
#define BUTTON              "button"
#define BUTTON_SIZE         6
#define PLAINTEXT           "plaintext"
#define PLAINTEXT_SIZE      9
#define A                   "a"
#define A_SIZE              1
#define B                   "b"
#define B_SIZE              1
#define BIG                 "big"
#define BIG_SIZE            3
#define CODE                "code"
#define CODE_SIZE           4
#define EM                  "em"
#define EM_SIZE             2
#define I                   "i"
#define I_SIZE              1
#define S                   "s"
#define S_SIZE              1
#define SMALL               "small"
#define SMALL_SIZE          5
#define STRIKE              "strike"
#define STRIKE_SIZE         6
#define STRONG              "strong"
#define STRONG_SIZE         6
#define TT                  "tt"
#define TT_SIZE             2
#define U                   "u"
#define U_SIZE              1
#define NOBR                "nobr"
#define NOBR_SIZE           4
#define APPLET              "applet"
#define APPLET_SIZE         6
#define MARQUEE             "marquee"
#define MARQUEE_SIZE        7
#define OBJECT              "object"
#define OBJECT_SIZE         6
#define AREA                "area"
#define AREA_SIZE           4
#define EMBED               "embed"
#define EMBED_SIZE          5
#define IMG                 "img"
#define IMG_SIZE            3
#define KEYGEN              "keygen"
#define KEYGEN_SIZE         6
#define WBR                 "wbr"
#define WBR_SIZE            3
#define INPUT               "input"
#define INPUT_SIZE          5
#define PARAM               "param"
#define PARAM_SIZE          5
#define SOURCE              "source"
#define SOURCE_SIZE         6
#define TRACK               "track"
#define TRACK_SIZE          5
#define HR                  "hr"
#define HR_SIZE             2
#define IMAGE               "image"
#define IMAGE_SIZE          5
#define TEXTAREA            "textarea"
#define TEXTAREA_SIZE       8
#define XMP                 "xmp"
#define XMP_SIZE            3
#define IFRAME              "iframe"
#define IFRAME_SIZE         6
#define NOEMBED             "noembed"
#define NOEMBED_SIZE        7
#define SELECT              "select"
#define SELECT_SIZE         6
#define COL                 "col"
#define COL_SIZE            3
#define COLGROUP            "colgroup"
#define COLGROUP_SIZE       8

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
static string_new html_namespace                    = strin_new("http://www.w3.org/1999/xhtml", 28);

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


static bool name_is(unsigned char* name, uint32_t name_size, html_token_t* token)
{
    return strncmp(name, token->name, name_size) == 0;
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


static html_node_t* create_element(string_t name, string_t namespace, html_node_t* parent)
{
    html_node_t* doc = parent->owner;
    html_node_t* element = html_element_new(doc, name, namespace);
}


static html_node_t* create_element_from_buffer(string_t name, string_t namespace, html_node_t* parent)
{
    
}


static html_node_t* create_element_from_token(html_token_t* token, string_t namespace, html_node_t* parent)
{
    // todo: step 1
    // todo: step 2

    html_node_t* doc = parent->owner;
    string_t local_name = string_new(token->name, token->name_size);

    // todo: step 5
    // todo: step 6
    // todo: step 7
    // todo: step 8
    // todo: step 9

    html_node_t* element = create_element(local_name, namespace, parent);

    // todo: step 11
    // todo: step 12
    // todo: step 13
    // todo: step 14
    // todo: step 15

    return element;
}

/********************/
/* public functions */
/********************/

void html_parser_run(const unsigned char* buffer, const uint32_t size)
{
    html_tokenizer_init(buffer, size, tokens, MAX_TOKENS);

    mode                        = HTML_PARSER_MODE_INITIAL;

    document                    = html_document_new();
    
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
                    string_t name           = string_new(t.name, t.name_size);
                    string_t public_id      = string_new(t.public_id, t.public_id_size);
                    string_t system_id      = string_new(t.system_id, t.system_id_size);
                    string_t legacy_compat  = string_new("about:legacy-compat", 19);
                    bool name_is_html       = string_compare_buffer(name, "html", 4);
                    bool is_legacy_compat   = string_compare(system_id, legacy_compat);
                    bool public_id_missing  = public_id.size == 0;
                    bool system_id_missing  = system_id.size == 0;

                    if (!name_is_html || !public_id_missing || !system_id_missing || !is_legacy_compat)
                    {
                        // todo: parse error
                    }
                    else
                    {
                        // todo: implement
                    }

                    string_free(name);
                    string_free(public_id);
                    string_free(system_id);
                    string_free(legacy_compat);

                    mode = HTML_PARSER_MODE_BEFORE_HTML;
                }
                else
                {
                    // todo: If the document is not an iframe srcdoc document, then this is a parse error; 

                    html_node_document_t* document_data = document->document;

                    if (!document_data->parser_cannot_change_mode)
                    {
                        document_data->compat_mode = string_new("quirks", 6);
                    }

                    mode = HTML_PARSER_MODE_BEFORE_HTML;
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
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                    html_node_t* element = create_element_from_token(t, html_namespace, document);
                    html_node_append(document, element);
                    stack_push(element);

                    mode = HTML_PARSER_MODE_BEFORE_HEAD;
                }
                else
                {
                    if (is_end && !(name_is(HTML, HTML_SIZE, &t) ||
                                    name_is(HEAD, HEAD_SIZE, &t) ||
                                    name_is(BODY, BODY_SIZE, &t) ||
                                    name_is(BR, BR_SIZE, &t)))
                    {
                        // todo: parse error
                    }
                    else
                    {
                        html_node_t* element = create_element_from_buffer(string_new("html", 4), html_namespace, document);
                        html_node_append(document, element);
                        stack_push(element);
    
                        mode = HTML_PARSER_MODE_BEFORE_HEAD;
                        consume = false;
                    }
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
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                    consume             = false;
                    mode                = HTML_PARSER_MODE_IN_BODY;
                    original_mode       = HTML_PARSER_MODE_BEFORE_HEAD;
                    will_use_rules_for  = true;
                }
                else if (is_start && name_is(HEAD, HEAD_SIZE, &t))
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
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                    
                }
                else if (is_start && (name_is(BASE, BASE_SIZE, &t) ||
                                      name_is(BASEFONT, BASEFONT_SIZE, &t) ||
                                      name_is(BGSOUND, BGSOUND_SIZE, &t) ||
                                      name_is(LINK, LINK_SIZE, &t))
                {
                    
                }
                else if (is_start && name_is(META, META_SIZE, &t))
                {
                    
                }
                else if (is_start && name_is(TITLE, TITLE_SIZE, &t))
                {
                    
                }
                else if ((is_start && name_is(NOSCRIPT, NOSCRIPT_SIZE, &t) && scripting_enabled) || 
                         (is_start && (name_is(NOFRAMES, NOFRAMES_SIZE, &t) || name_is(STYLE, STYLE_SIZE, &t)))
                {
                    
                }
                else if (is_start && name_is(NOSCRIPT, NOSCRIPT_SIZE, &t) && !scripting_enabled)
                {
                    
                }
                else if (is_start && name_is(SCRIPT, SCRIPT_SIZE, &t))
                {
                    assert(false);
                }
                else if (is_end && name_is(HEAD, HEAD_SIZE, &t))
                {
                    
                }
                else if (is_start && name_is(TEMPLATE, TEMPLATE_SIZE, &t))
                {
                    assert(false)
                }
                else if (is_end && name_is(TEMPLATE, TEMPLATE_SIZE, &t))
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
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                    
                }
                else if (t.type == HTML_END_TAG && name_is(NOSCRIPT, NOSCRIPT_SIZE, &t))
                {
                    
                }
                else if ((is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' ') ) ||
                         (is_comment) ||
                         (is_start && (name_is(BASEFONT, BASEFONT_SIZE, &t) ||
                                       name_is(BGSOUND, BGSOUND_SIZE, &t)   ||
                                       name_is(LINK, LINK_SIZE, &t)         ||
                                       name_is(META, META_SIZE, &t)         ||
                                       name_is(NOFRAMES, NOFRAMES_SIZE, &t) ||
                                       name_is(STYLE, STYLE_SIZE, &t))
                {
                    
                }
                else if (is_start && (name_is(HEAD, HEAD_SIZE, &t) || name_is(NOSCRIPT, NOSCRIPT_SIZE, &t))
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
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                    
                }
                else if (is_start && name_is(BODY, BODY_SIZE, &t))
                {
                    
                }
                else if (is_start && name_is(FRAMESET, FRAMESET_SIZE, &t))
                {
                    
                }
                else if (is_start && (name_is(BASE, BASE_SIZE, &t)          ||
                                      name_is(BASEFONT, BASEFONT_SIZE, &t)  ||
                                      name_is(BGSOUND, BGSOUND_SIZE, &t)    ||
                                      name_is(LINK, LINK_SIZE, &t)          ||
                                      name_is(META, META_SIZE, &t)          ||
                                      name_is(NOFRAMES, NOFRAMES_SIZE, &t)  ||
                                      name_is(SCRIPT, SCRIPT_SIZE, &t)      || 
                                      name_is(TEMPLATE, TEMPLATE_SIZE, &t)  ||
                                      name_is(TITLE, TITLE_SIZE, &t)        ||
                                      name_is(STYLE, STYLE_SIZE, &t))
                {
                    // todo: parse error
                    
                }
                else if (is_end && name_is(TEMPLATE, TEMPLATE_SIZE, &t))
                {
                    
                }
                else if ( (is_start && name_is(HEAD, HEAD_SIZE, &t) ||
                          (is_end && !(name_is(BODY, BODY_SIZE, &t) || name_is(HTML, HTML_SIZE, &t) || name_is(BR, BR_SIZE, &t)))
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
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                    // todo: parse error
                    
                }
                else if ((is_start && (name_is(BASE, BASE_SIZE, &t)         ||
                                       name_is(BASEFONT, BASEFONT_SIZE, &t) ||
                                       name_is(BGSOUND, BGSOUND_SIZE, &t)   ||
                                       name_is(LINK, LINK_SIZE, &t)         ||
                                       name_is(META, META_SIZE, &t)         ||
                                       name_is(NOFRAMES, NOFRAMES_SIZE, &t) ||
                                       name_is(SCRIPT, SCRIPT_SIZE, &t)     ||
                                       name_is(TEMPLATE, TEMPLATE_SIZE, &t) ||
                                       name_is(TITLE, TITLE_SIZE, &t)       ||
                                       name_is(STYLE, STYLE_SIZE, &t))      ||
                        (is_end && name_is(TEMPLATE, TEMPLATE_SIZE, &t))
                {
                    
                }
                else if (is_start && name_is(BODY, BODY_SIZE, &t))
                {
                    // todo: parse error
                }
                else if (is_start && name_is(FRAMESET, FRAMESET_SIZE, &t))
                {
                    // todo: parse error
                }
                else if (is_eof)
                {
                    
                }
                else if (is_end && name_is(BODY, BODY_SIZE, &t))
                {
                    
                }
                else if (is_end && name_is(BODY, BODY_SIZE, &t))
                {
                    
                }
                else if (is_start && (name_is(ADDRESS, ADDRESS_SIZE, &t)        ||
                                      name_is(ARTICLE, ARTICLE_SIZE, &t)        ||
                                      name_is(ASIDE, ASIDE_SIZE, &t)            ||
                                      name_is(BLOCKQUOTE, BLOCKQUOTE_SIZE, &t)  ||
                                      name_is(CENTER, CENTER_SIZE, &t)          ||
                                      name_is(DETAILS, DETAILS_SIZE, &t)        ||
                                      name_is(DIALOG, DIALOG_SIZE, &t)          ||
                                      name_is(DIR, DIR_SIZE, &t)                ||
                                      name_is(DIV, DIV_SIZE, &t)                ||
                                      name_is(DL, DL_SIZE, &t)                  ||
                                      name_is(FIELDSET, FIELDSET_SIZE, &t)      ||
                                      name_is(FIGCAPTION, FIGCAPTION_SIZE, &t)  ||
                                      name_is(FIGURE, FIGURE_SIZE, &t)          ||
                                      name_is(FOOTER, FOOTER_SIZE, &t)          ||
                                      name_is(HEADER, HEADER_SIZE, &t)          ||
                                      name_is(HGROUP, HGROUP_SIZE, &t)          ||
                                      name_is(MAIN, MAIN_SIZE, &t)              ||
                                      name_is(MENU, MENU_SIZE, &t)              ||
                                      name_is(NAV, NAV_SIZE, &t)                ||
                                      name_is(OL, OL_SIZE, &t)                  ||
                                      name_is(P, P_SIZE, &t)                    ||
                                      name_is(SEARCH, SEARCH_SIZE, &t)          ||
                                      name_is(SECTION, SECTION_SIZE, &t)        ||
                                      name_is(SUMMARY, SUMMARY_SIZE, &t)        ||
                                      name_is(UL, UL_SIZE, &t)))
                {
                    
                }
                else if (is_start && (name_is(H1, H1_SIZE, &t)  ||
                                      name_is(H2, H2_SIZE, &t)  ||
                                      name_is(H3, H3_SIZE, &t)  ||
                                      name_is(H4, H4_SIZE, &t)  ||
                                      name_is(H5, H5_SIZE, &t)  ||
                                      name_is(H6, H6_SIZE, &t)))
                {
                    
                }
                else if (is_start && (name_is(PRE, PRE_SIZE, &t) || name_is(LISTING, LISTING_SIZE, &t)))
                {
                    
                }
                else if (is_start && name_is(FORM, FORM_SIZE, &t)
                {
                    
                }
                else if (is_start && name_is(LI, LI_SIZE, &t))
                {
                
                }
                else if (is_start && (name_is(DT, DT_SIZE, &t) || name_is(DD, DD_SIZE, &t)))
                {
                    // todo: implement
                }
                else if (is_start && name_is(PLAINTEXT, PLAINTEXT_SIZE, &t))
                {
                    // todo: handle scope logic - If the stack of open elements has a p element in button scope, then close a p element.
                    
                }
                else if (is_start && name_is(button_text, &t))
                {
                    
                }
                else if (is_end && (name_is(ADDRESS, ADDRESS_SIZE, &t)      || name_is(ARTICLE, ARTICLE_SIZE, &t)       || 
                                    name_is(ASIDE, ASIDE_SIZE, &t)          || name_is(BLOCKQUOTE, BLOCKQUOTE_SIZE, &t) ||
                                    name_is(BUTTON, BUTTON_SIZE, &t)        || name_is(CENTER, CENTER_SIZE, &t)         ||
                                    name_is(DETAILS, DETAILS_SIZE, &t)      || name_is(DIALOG, DIALOG_SIZE, &t)         ||
                                    name_is(DIR, DIR_SIZE, &t)              || name_is(DIV, DIV_SIZE, &t)               ||
                                    name_is(DL, DL_SIZE, &t)                || name_is(FIELDSET, FIELDSET_SIZE, &t)     ||
                                    name_is(FIGCAPTION, FIGCAPTION_SIZE, &t)|| name_is(FIGURE, FIGURE_SIZE, &t)         ||
                                    name_is(FOOTER, FOOTER_SIZE, &t)        || name_is(HEADER, HEADER_SIZE, &t)         ||
                                    name_is(HGROUP, HGROUP_SIZE, &t)        || name_is(LISTING, LISTING_SIZE, &t)       ||
                                    name_is(MAIN, MAIN_SIZE, &t)            || name_is(MENU, MENU_SIZE, &t)             ||
                                    name_is(NAV, NAV_SIZE, &t)              || name_is(OL, OL_SIZE, &t)                 ||
                                    name_is(PRE, PRE_SIZE, &t)              || name_is(SEARCH, SEARCH_SIZE, &t)         ||
                                    name_is(SECTION, SECTION_SIZE, &t)      || name_is(SUMMARY, SUMMARY_SIZE, &t)       ||
                                    name_is(UL, UL_SIZE, &t) ))
                {
                    
                }
                else if (is_end && name_is(FORM, FORM_SIZE, &t))
                {
                    
                }
                else if (is_end && name_is(P, P_SIZE, &t))
                {
                    
                }
                else if (is_end && name_is(LI, LI_SIZE, &t))
                {
                    
                }
                else if (is_end && (name_is(DD, DD_SIZE, &t) || name_is(DT, DT_SIZE, &t) ))
                {
                
                }
                else if (is_end && (name_is(H1, H1_SIZE, &t) || 
                                    name_is(H2, H2_SIZE, &t) || 
                                    name_is(H3, H3_SIZE, &t) ||
                                    name_is(H4, H4_SIZE, &t) || 
                                    name_is(H5, H5_SIZE, &t) || 
                                    name_is(H6, H6_SIZE, &t)))
                {
                
                }
                else if (is_start && name_is(A, A_SIZE, &t))
                {
                
                }
                else if (is_start && (name_is(B, B_SIZE, &t)            || 
                                      name_is(BIG, BIG_SIZE, &t)        ||
                                      name_is(CODE, CODE_SIZE, &t)      ||
                                      name_is(EM, EM_SIZE, &t)          ||
                                      name_is(FONT, FONT_SIZE, &t)      ||
                                      name_is(I, I_SIZE, &t)            ||
                                      name_is(S, S_SIZE, &t)            ||
                                      name_is(SMALL, SMALL_SIZE, &t)    ||
                                      name_is(STRIKE, STRIKE_SIZE, &t)  ||
                                      name_is(STRONG, STRONG_SIZE, &t)  ||
                                      name_is(TT, TT_SIZE, &t)          ||
                                      name_is(U, U_SIZE, &t)) )
                {
                
                }
                else if (is_start && name_is(NOBR, NOBR_SIZE, &t))
                {
                
                }
                else if (is_end && (name_is(A, A_SIZE, &t)          ||
                                    name_is(B, B_SIZE, &t)          ||
                                    name_is(BIG, BIG_SIZE, &t)      ||
                                    name_is(CODE, CODE_SIZE, &t)    ||
                                    name_is(EM, EM_SIZE, &t)        ||
                                    name_is(FONT, FONT_SIZE, &t)    ||
                                    name_is(I, I_SIZE, &t)          ||
                                    name_is(NOBR, NOBR_SIZE, &t)    ||
                                    name_is(S, S_SIZE, &t)          ||
                                    name_is(SMALL, SMALL_SIZE, &t)  ||
                                    name_is(STRIKE, STRIKE_SIZE, &t)||
                                    name_is(STRONG, STRONG_SIZE, &t)||
                                    name_is(TT, TT_SIZE, &t)        ||
                                    name_is(U, U_SIZE, &t)))
                {
                    
                }
                else if (is_start && (name_is(APPLET, APPLET_SIZE, &t)  ||
                                      name_is(MARQUEE, MARQUEE_SIZE, &t)||
                                      name_is(OBJECT, OBJECT_SIZE, &t)) )
                {
                
                }
                else if (is_end && (name_is(APPLET, APPLET_SIZE, &t)    ||
                                    name_is(MARQUEE, MARQUEE_SIZE, &t)  ||
                                    name_is(OBJECT, OBJECT_SIZE, &t)) )
                {
                
                }
                else if (is_start && name_is(TABLE, TABLE_SIZE, &t)
                {
                
                }
                else if (is_end && name_is(BR, BR_SIZE, &t))
                {
                
                }
                else if (is_start && (name_is(AREA, AREA_SIZE, &t)      ||
                                      name_is(BR, BR_SIZE, &t)          ||
                                      name_is(EMBED, EMBED_SIZE, &t)    ||
                                      name_is(IMG, IMG_SIZE, &t)        ||
                                      name_is(KEYGEN, KEYGEN_SIZE, &t)  ||
                                      name_is(WBR, WBR_SIZE, &t) ))
                {
                    
                }
                else if (is_start && name_is(INPUT, INPUT_SIZE, &t))
                {
                
                }
                else if (is_start && (name_is(PARAM, PARAM_SIZE, &t)    ||
                                      name_is(SOURCE, SOURCE_SIZE, &t)  ||
                                      name_is(TRACK, TRACK_SIZE, &t) ))
                {
                
                }
                else if (is_start && name_is(HR, HR_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(IMAGE, IMAGE_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(TEXTAREA, TEXTAREA_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(XMP, XMP_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(IFRAME, IFRAME_SIZE, &t) )
                {
                
                }
                else if ( (is_start && name_is(NOEMBED, NOEMBED_SIZE, &t) ) ||
                          (is_start && name_is(NOSCRIPT, NOSCRIPT_SIZE) && scripting_enabled) )
                {
                
                }
                else if (is_start && name_is(SELECT, SELECT_SIZE, &t) )
                {
                
                }
                else if (is_start && (name_is(OPTGROUP, OPTGROUP_SIZE, &t) ||
                                      name_is(OPTION, OPTION_SIZE, &t)))
                {
                
                }
                else if (is_start && (name_is(RB, RB_SIZE, &t) || 
                                      name_is(RTC, RTC_SIZE, &t)))
                {
                
                }
                else if (is_start && (name_is(RP, RP_SIZE, &t) ||
                                      name_is(RT, RT_SIZE, &t)))
                {
                
                }
                else if (is_start && name_is(MATH, MATH_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(SVG, SVG_SIZE, &t))
                {
                
                }
                else if (is_start && (name_is(CAPTION, CAPTION_SIZE, &t)    ||
                                      name_is(COL, COL_SIZE, &t)            ||
                                      name_is(COLGROUP, COLGROUP_SIZE, &t)  || 
                                      name_is(FRAME, FRAME_SIZE, &t)        ||
                                      name_is(HEAD, HEAD_SIZE, &t)          ||
                                      name_is(TBODY, TBODY_SIZE, &t)        ||
                                      name_is(TD, TD_SIZE, &t)              ||
                                      name_is(TFOOT, TFOOT_SIZE, &t)        ||
                                      name_is(TH, TH_SIZE, &t)              ||
                                      name_is(THEAD, THEAD_SIZE, &t)        ||
                                      name_is(TR, TR_SIZE, &t) ))
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
                else if (is_end && name_is(SCRIPT, SCRIPT_SIZE, &t))
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intable
            case HTML_PARSER_MODE_IN_TABLE:
                string_t current = stack[stack_idx]->name;
                if (is_character && (string_compare_buffer(current, TABLE, TABLE_SIZE)      ||
                                     string_compare_buffer(current, TBODY, TBODY_SIZE)      ||
                                     string_compare_buffer(current, TEMPLATE, TEMPLATE_SIZE)||
                                     string_compare_buffer(current, TFOOT, TFOOT_SIZE)      ||
                                     string_compare_buffer(current, THEAD, THEAD_SIZE)      ||
                                     string_compare_buffer(current, TR, TR_SIZE)))
                {
                
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                
                }
                else if (is_start && name_is(CAPTION, CAPTION_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(COLGROUP, COLGROUP_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(COL, COL_SIZE, &t))
                {
                
                }
                else if (is_start && (name_is(TBODY, TBODY_SIZE, &t) ||
                                      name_is(TFOOT, TFOOT_SIZE, &t) ||
                                      name_is(THEAD, THEAD_SIZE, &t)))
                {
                
                }
                else if (is_start && (name_is(TD, TD_SIZE, &t) ||
                                      name_is(TH, TH_SIZE, &t) ||
                                      name_is(TR, TR_SIZE)))
                {
                
                }
                else if (is_start && name_is(TABLE, TABLE_SIZE, &t))
                {
                    
                }
                else if (is_end && name_is(TABLE, TABLE_SIZE, &t))
                {
                
                }
                else if (is_end && (name_is(BODY, BODY_SIZE, &t)        ||
                                    name_is(CAPTION, CAPTION_SIZE, &t)  ||
                                    name_is(COL, COL_SIZE, &t)          ||
                                    name_is(COLGROUP, COLGROUP_SIZE, &t)||
                                    name_is(HTML, HTML_SIZE, &t)        ||
                                    name_is(TBODY, TBODY_SIZE, &t)      ||
                                    name_is(TD, TD_SIZE, &t)            ||
                                    name_is(TFOOT, TFOOT_SIZE, &t)      ||
                                    name_is(TH, TH_SIZE, &t)            ||
                                    name_is(THEAD, THEAD_SIZE, &t)      ||
                                    name_is(TR, TR_SIZE, &t)))
                {
                
                }
                else if ((is_start && (name_is(STYLE, STYLE_SIZE, &t)       ||
                                       name_is(SCRIPT, SCRIPT_SIZE, &t)     ||
                                       name_is(TEMPLATE, TEMPLATE_SIZE)))   ||
                         (is_end && name_is(TEMPLATE, TEMPLATE_SIZE)))
                {
                
                }
                else if (is_start && name_is(INPUT, INPUT_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(FORM, FORM_SIZE, &t))
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
                if (is_end && name_is(CAPTION, CAPTION_SIZE, &t))
                {
                
                }
                else if ((is_start && (name_is(CAPTION, CAPTION_SIZE, &t)   ||
                                       name_is(COL, COL_SIZE, &t)           ||
                                       name_is(COLGROUP, COLGROUP_SIZE, &t) || 
                                       name_is(TBODY, TBODY_SIZE, &t)       ||
                                       name_is(TD, TD_SIZE, &t)             ||
                                       name_is(TFOOT, TFOOT_SIZE, &t)       ||
                                       name_is(TH, TH_SIZE, &t)             ||
                                       name_is(THEAD, THEAD_SIZE, &t)       ||
                                       name_is(TR, TR_SIZE, &t)))           ||
                         (is_end && name_is(TABLE, TABLE_SIZE, &t)))
                {
                
                }
                else if (is_end && (name_is(BODY, BODY_SIZE, &t)        ||
                                    name_is(COL, COL_SIZE, &t)          ||
                                    name_is(COLGROUP, COLGROUP_SIZE, &t)||
                                    name_is(HTML, HTML_SIZE, &t)        ||
                                    name_is(TBODY, TBODY_SIZE, &t)      ||
                                    name_is(TD, TD_SIZE, &t)            ||
                                    name_is(TFOOT, TFOOT_SIZE, &t)      ||
                                    name_is(TH, TH_SIZE, &t)            ||
                                    name_is(THEAD, THEAD_SIZE, &t)      ||
                                    name_is(TR, TR_SIZE, &t)))
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
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(COL, COL_SIZE, &t))
                {
                
                }
                else if (is_end && name_is(COLGROUP, COLGROUP_SIZE, &t))
                {
                
                }
                else if (is_end && name_is(COL, COL_SIZE, &t))
                {
                
                }
                else if ((is_start || is_end) && (name_is(TEMPLATE, TEMPLATE_SIZE, &t)))
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
                if (is_start && name_is(TR, TR_SIZE))
                {
                
                }
                else if (is_start && (name_is(TH, TH_SIZE, &t) ||
                                      name_is(TD, TD_SIZE, &t)))
                {
                
                }
                else if (is_end && (name_is(TBODY, TBODY_SIZE, &t) ||
                                    name_is(TFOOT, TFOOT_SIZE, &t) ||
                                    name_is(THEAD, THEAD_SIZE, &t) ))
                {
                
                }
                else if ((is_start && (name_is(CAPTION, CAPTION_SIZE, &t)   ||
                                       name_is(COL, COL_SIZE, &t)           ||
                                       name_is(COLGROUP, COLGROUP_SIZE, &t) ||
                                       name_is(TBODY, TBODY_SIZE, &t)       ||
                                       name_is(TFOOT, TFOOT_SIZE, &t)       ||
                                       name_is(THEAD, THEAD_SIZE, &t)))     ||
                         (is_end && name_is(TABLE, TABLE_SIZE, &t)))
                {
                
                }
                else if (is_end && && (name_is(BODY, BODY_SIZE, &t)         ||
                                       name_is(CAPTION, CAPTION_SIZE, &t)   ||
                                       name_is(COL, COL_SIZE, &t)           ||
                                       name_is(COLGROUP, COLGROUP_SIZE, &t) ||
                                       name_is(HTML, HTML_SIZE, &t)         ||
                                       name_is(TD, TD_SIZE, &t)             ||
                                       name_is(TH, TH_SIZE, &t)             ||
                                       name_is(TR, TR_SIZE, &t)))
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intr
            case HTML_PARSER_MODE_IN_ROW:
                if (is_start && (name_is(TH, TH_SIZE, &t) ||
                                 name_is(TD, TD_SIZE, &t)))
                {
                
                }
                else if (is_end && name_is(TR, TR_SIZE, &t))
                {
                
                }
                else if ((is_start && (name_is(CAPTION, CAPTION_SIZE, &t)   ||
                                       name_is(COL, COL_SIZE, &t)           ||
                                       name_is(COLGROUP, COLGROUP_SIZE, &t) ||
                                       name_is(TBODY, TBODY_SIZE, &t)       ||
                                       name_is(TFOOT, TFOOT_SIZE, &t)       ||
                                       name_is(THEAD, THEAD_SIZE, &t))      ||
                                       name_is(TR, TR_SIZE, &t)))           ||
                         (is_end && name_is(TABLE, TABLE_SIZE, &t)))
                {
                
                }
                else if (is_end && (name_is(TBODY, TBODY_SIZE, &t) ||
                                    name_is(THEAD, THEAD_SIZE, &t) ||
                                    name_is(TFOOT, TFOOT_SIZE, &t)))
                {
                
                }
                else if (is_end && (name_is(BODY, BODY_SIZE, &t)        ||
                                    name_is(CAPTION, CAPTION_SIZE, &t)  ||
                                    name_is(COL, COL_SIZE, &t)          ||
                                    name_is(COLGROUP, COLGROUP_SIZE, &t)||
                                    name_is(HTML, HTML_SIZE, &t)        ||
                                    name_is(TD, TD_SIZE, &t)            ||
                                    name_is(TH, TH_SIZE, &t)))
                {
                
                }
                else
                {
                
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intd
            case HTML_PARSER_MODE_IN_CELL:
                if (is_end && (name_is(TD, TD_SIZE, &t) ||
                               name_is(TH, TH_SIZE, &t)))
                {
                
                }
                else if (is_start && (name_is(CAPTION, CAPTION_SIZE, &t)    ||
                                      name_is(COL, COL_SIZE, &t)            ||
                                      name_is(COLGROUP, COLGROUP_SIZE, &t)  ||
                                      name_is(TBODY, TBODY_SIZE, &t)        ||
                                      name_is(TFOOT, TFOOT_SIZE, &t)        ||
                                      name_is(THEAD, THEAD_SIZE, &t))       ||
                                      name_is(TR, TR_SIZE, &t)              ||
                                      name_is(TH, TH_SIZE, &t)              ||
                                      name_is(TD, TD_SIZE, &t)))
                {
                
                }
                else if (is_end && (name_is(BODY, BODY_SIZE, &t)        ||
                                    name_is(CAPTION, CAPTION_SIZE, &t)  ||
                                    name_is(COL, COL_SIZE, &t)          ||
                                    name_is(COLGROUP, COLGROUP_SIZE, &t)||
                                    name_is(HTML, HTML_SIZE, &t)))
                {
                
                }
                else if (is_end && (name_is(TABLE, TABLE_SIZE, &t) ||
                                    name_is(TFOOT, TFOOT_SIZE, &t) ||
                                    name_is(THEAD, THEAD_SIZE, &t) ||
                                    name_is(TBODY, TBODY_SIZE, &t) ||
                                    name_is(TR, TR_SIZE, &t)))
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
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(OPTION, OPTION_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(OPTGROUP, OPTGROUP_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(HR, HR_SIZE, &t))
                {
                
                }
                else if (is_end && name_is(OPTGROUP, OPTGROUP_SIZE, &t))
                {
                
                }
                else if (is_end && name_is(OPTION, OPTION_SIZE, &t))
                {
                
                }
                else if (is_end && name_is(SELECT, SELECT_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(SELECT, SELECT_SIZE, &t))
                {
                
                }
                else if (is_start && (name_is(INPUT, INPUT_SIZE, &t)    ||
                                      name_is(KEYGEN, KEYGEN_SIZE, &t)  ||
                                      name_is(TEXTAREA, TEXTAREA_SIZE, &t)))
                {
                
                }
                else if ((is_start && (name_is(SCRIPT, SCRIPT_SIZE, &t)     ||
                                       name_is(TEMPLATE, TEMPLATE_SIZE, &t))||
                         (is_end && name_is(TEMPLATE, TEMPLATE_SIZE, &t)))
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
                if (is_start && (name_is(CAPTION, CAPTION_SIZE, &t) ||
                                 name_is(TABLE, TABLE_SIZE, &t)     ||
                                 name_is(TBODY, TBODY_SIZE, &t)     ||
                                 name_is(TFOOT, TFOOT_SIZE, &t)     ||
                                 name_is(THEAD, THEAD_SIZE, &t)     ||
                                 name_is(TR, TR_SIZE, &t)           ||
                                 name_is(TD, TD_SIZE, &t)           ||
                                 name_is(TH, TH_SIZE, &t)))
                {
                
                }
                else if (is_end && (name_is(CAPTION, CAPTION_SIZE, &t)  ||
                                    name_is(TABLE, TABLE_SIZE, &t)      ||
                                    name_is(TBODY, TBODY_SIZE, &t)      ||
                                    name_is(TFOOT, TFOOT_SIZE, &t)      ||
                                    name_is(THEAD, THEAD_SIZE, &t)      ||
                                    name_is(TR, TR_SIZE, &t)            ||
                                    name_is(TD, TD_SIZE, &t)            ||
                                    name_is(TH, TH_SIZE, &t)))
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
                else if ((is_start && (name_is(BASE, BASE_SIZE, &t)         ||
                                       name_is(BASEFONT, BASEFONT_SIZE, &t) ||
                                       name_is(BGSOUND, BGSOUND_SIZE, &t)   ||
                                       name_is(LINK, LINK_SIZE, &t)         ||
                                       name_is(META, META_SIZE, &t)         ||
                                       name_is(NOFRAMES, NOFRAMES_SIZE, &t) ||
                                       name_is(SCRIPT, SCRIPT_SIZE, &t)     ||
                                       name_is(STYLE, STYLE_SIZE, &t)       ||
                                       name_is(TEMPLATE, TEMPLATE_SIZE, &t) ||
                                       name_is(TITLE, TITLE_SIZE, &t)))     ||
                         (is_end && name_is(TEMPLATE, TEMPLATE_SIZE, &t)))
                {
                
                }
                else if (is_start && (name_is(CAPTION, CAPTION_SIZE, &t)    ||
                                      name_is(COLGROUP, COLGROUP_SIZE, &t)  ||
                                      name_is(TBODY, TBODY_SIZE, &t)        ||
                                      name_is(TFOOT, TFOOT_SIZE, &t)        ||
                                      name_is(THEAD, THEAD_SIZE, &t)))
                {
                
                }
                else if (is_start && name_is(COL, COL_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(TR, TR_SIZE, &t))
                {
                
                }
                else if (is_start && (name_is(TD, TD_SIZE, &t) ||
                                      name_is(TH, TH_SIZE, &t)))
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
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                
                }
                else if (is_end && name_is(HTML, HTML_SIZE, &t))
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
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(FRAMESET, FRAMESET_SIZE, &t))
                {
                
                }
                else if (is_end && name_is(FRAMESET, FRAMESET_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(FRAME, FRAME_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(NOFRAMES, NOFRAMES_SIZE, &t))
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
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                
                }
                else if (is_end && name_is(HTML, HTML_SIZE, &t))
                {
                
                }
                else if (is_start && name_is(NOFRAMES, NOFRAMES_SIZE, &t))
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
                         (is_start && name_is(HTML, HTML_SIZE, &t)))
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
                         (is_start && name_is(HTML, HTML_SIZE, &t)))
                {
                
                }
                else if (is_eof)
                {
                
                }
                else if (is_start && name_is(NOFRAMES, NOFRAMES_SIZE, &t))
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