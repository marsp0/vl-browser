#include "parser.h"

#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "html/tokenizer.h"
#include "dom/node.h"
#include "dom/comment.h"
#include "dom/document.h"
#include "dom/doctype.h"
#include "dom/element.h"
#include "dom/text.h"
#include "util/not_implemented.h"

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
#define FRAME               "frame"
#define FRAME_SIZE          5
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
#define SVG                 "svg"
#define SVG_SIZE            3
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
#define TABLE               "table"
#define TABLE_SIZE          5
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
#define FONT                "font"
#define FONT_SIZE           4
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
#define MATH                "math"
#define MATH_SIZE           4
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
#define CAPTION             "caption"
#define CAPTION_SIZE        7


/********************/
/* static variables */
/********************/

typedef enum
{
    GENERIC_SCOPE,
    BUTTON_SCOPE,
    TABLE_SCOPE,
} html_element_scope_e;

typedef enum
{
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
} html_formatting_elements_step_e;

static html_parser_mode_e mode                      = HTML_PARSER_MODE_INITIAL;
static html_parser_mode_e original_mode             = HTML_PARSER_MODE_INITIAL;
static html_token_t tokens[MAX_TOKENS]              = { 0 };
static html_node_t* stack[OPEN_STACK_MAX_SIZE]      = { 0 };
static uint32_t stack_idx                           = 0;
static uint32_t stack_size                          = 0;
static html_node_t* document                        = NULL;
static bool stop                                    = false;

static html_node_t* formatting_elements[10]         = { 0 };
static bool formatting_elements_m[10]               = { 0 };
static html_token_t formatting_elements_t[10]       = { 0 };
static uint32_t formatting_elements_size            = 0;

/********************/
/* static functions */
/********************/

static bool string_compare(const unsigned char* first, const uint32_t first_size, const unsigned char* second, const uint32_t second_size)
{
    if (first_size != second_size) { return false; }

    return strncmp(first, second, first_size) == 0;
}

static uint32_t get_tokens_size()
{
    uint32_t result = 0;
    for (uint32_t i = 0; i < MAX_TOKENS; i++) { if (tokens[i].is_valid) { result += 1; } }

    return result;
}


static void stack_push(html_node_t* node)
{
    if (stack_size > 0) { stack_idx++; }
    stack[stack_idx] = node;

    stack_size++;
}


static void stack_pop()
{
    stack[stack_idx] = NULL;
    stack_idx = stack_idx > 0 ? stack_idx - 1 : 0;
    stack_size--;
}

static void remove_from_stack(html_node_t* node)
{
    for (uint32_t i = 0; i < stack_size; i++)
    {
        if (node != stack[i]) { continue; }

        for (uint32_t j = i; j < stack_size - 1; j++)
        {
            stack[j] = stack[j + 1];
        }

        stack_size--;
        return;
    }
}


static bool name_is(const unsigned char* name, const uint32_t name_size, const html_token_t* token)
{
    if (token->name_size != name_size) { return false; }

    return string_compare(name, name_size, token->name, token->name_size);
}


static bool stack_contains_element(unsigned char* name, uint32_t name_size)
{
    for (uint32_t i = 0; i < stack_size; i++)
    {
        html_node_t* current = stack[i];
        if (!current)                                           { return false; }
        if (current->type != HTML_NODE_ELEMENT)                 { continue; }

        html_element_t* element = (html_element_t*)current->data;
        if (strncmp(element->local_name, name, name_size) == 0) { return true; }
        // todo: figure out which name to use and perform case insensitive comparison
    }

    return false;
}


static bool stack_contains_node(html_node_t* node)
{
    for (uint32_t i = 0; i < stack_size; i++)
    {
        if (stack[i] == node)
        {
            return true;
        }
    }

    return false;
}


static bool in_scope(unsigned char* name, uint32_t name_size, html_element_scope_e scope)
{
    html_node_t* node = stack[stack_idx];
    html_element_t* element = (html_element_t*)node->data;
    int32_t i = (int32_t)stack_idx;

    while (i >= 0)
    {
        const unsigned char* local_name = element->local_name;

        if (strncmp(name, local_name, name_size) == 0) { return true; }

        if (strncmp(HTML, local_name, HTML_SIZE) == 0       ||
            strncmp(TABLE, local_name, TABLE_SIZE) == 0     ||
            strncmp(TEMPLATE, local_name, TEMPLATE_SIZE) == 0)
        {
            return false;
        }

        if ((scope != TABLE_SCOPE) && (strncmp(APPLET, local_name, APPLET_SIZE) == 0   ||
                                       strncmp(CAPTION, local_name, CAPTION_SIZE) == 0 ||
                                       strncmp(TD, local_name, TD_SIZE) == 0           ||
                                       strncmp(TH, local_name, TH_SIZE) == 0           ||
                                       strncmp(MARQUEE, local_name, MARQUEE_SIZE) == 0 ||
                                       strncmp(OBJECT, local_name, OBJECT_SIZE) == 0))
        {
            return false;
        }

        if (scope == BUTTON_SCOPE && strncmp(BUTTON, element->local_name, BUTTON_SIZE) == 0)
        {
            return false;
        }

        i--;
        node = stack[i];
        element = (html_element_t*)node->data;
    }

    return false;
}


static html_node_t* get_appropriate_insertion_location(html_node_t* override)
{
    html_node_t* location   = NULL;
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
    html_node_t* location   = get_appropriate_insertion_location(position);
    html_node_t* comment    = html_comment_new(document, token->data, token->data_size);

    html_node_append(location, comment);
}


static html_node_t* create_element(unsigned char* name, uint32_t name_size, html_node_t* parent)
{
    // todo: step 1
    // todo: step 2

    // html_node_t* doc = parent->document;

    // todo: step 5
    // todo: step 6
    // todo: step 7
    // todo: step 8
    // todo: step 9

    html_node_t* doc        = parent->document;
    html_node_t* element    = html_element_new(doc, name, name_size);

    // todo: step 11
    // todo: step 12
    // todo: step 13
    // todo: step 14
    // todo: step 15

    return element;
}


static html_node_t* insert_foreign_element(unsigned char* name, uint32_t name_size, bool only_add_to_stack)
{
    html_node_t* location = get_appropriate_insertion_location(NULL);
    html_node_t* element = create_element(name, name_size, location);

    if (!only_add_to_stack)
    {
        // todo: step 2
        // todo: step 3
        html_node_append(location, element);
        // todo: step 5
    }

    stack_push(element);

    return element;
}


static html_node_t* insert_html_element(unsigned char* name, uint32_t name_size)
{
    return insert_foreign_element(name, name_size, false);
}


static void insert_character(html_token_t* token)
{
    
    html_node_t* location = get_appropriate_insertion_location(NULL);
    if (location->type == HTML_NODE_DOCUMENT) { return; }

    html_node_t* last_child = location->last_child;
    if (last_child && last_child->type == HTML_NODE_TEXT)
    {
        html_text_append_data(last_child, token->data, token->data_size);
    }
    else
    {
        html_node_t* node = html_text_new(document, token->data, token->data_size);
        html_node_append(location, node);
    }
    
    return;
}


static void stop_parsing()
{
    // todo: step 1
    // todo: step 2
    // todo: step 3

    while (stack_size > 0) { stack_pop(); }
    stop = true;

    // todo: step 5
    // todo: step 6
    // todo: step 7
    // todo: step 8
    // todo: step 9
    // todo: step 10
    // todo: step 11
}


static void generate_implied_end_tags(unsigned char* name, uint32_t name_size)
{
    while (stack_size > 0)
    {
        html_node_t* node = stack[stack_idx];
        html_element_t* element = (html_element_t*)node->data;
        const unsigned char* local_name = element->local_name;

        if (name_size > 0 && strncmp(local_name, name, name_size) == 0)
        {
            return;
        }

        if (strncmp(local_name, DD, DD_SIZE) == 0              ||
            strncmp(local_name, DT, DT_SIZE) == 0              ||
            strncmp(local_name, LI, LI_SIZE) == 0              ||
            strncmp(local_name, OPTGROUP, OPTGROUP_SIZE) == 0  ||
            strncmp(local_name, OPTION, OPTION_SIZE) == 0      ||
            strncmp(local_name, P, P_SIZE) == 0                ||
            strncmp(local_name, RB, RB_SIZE) == 0              ||
            strncmp(local_name, RP, RP_SIZE) == 0              ||
            strncmp(local_name, RT, RT_SIZE) == 0              ||
            strncmp(local_name, RTC, RTC_SIZE) == 0)
        {
            stack_pop();
        }
        else
        {
            return;
        }
    }
}


static void close_p_element()
{
    generate_implied_end_tags(P, P_SIZE);

    html_node_t* node = stack[stack_idx];
    html_element_t* element = (html_element_t*)node->data;
    if (strncmp(element->local_name, P, P_SIZE) != 0)
    {
        // todo: parse error
    }

    bool run = true;
    // todo: when do we stop ? 
    while (run)
    {
        if (strncmp(element->local_name, P, P_SIZE) == 0) { run = false; }
        stack_pop();

        node = stack[stack_idx];
        element = (html_element_t*)node->data;
    }
}


static void close_cell()
{
    generate_implied_end_tags(NULL, 0);

    html_node_t* node = stack[stack_idx];
    html_element_t* element = (html_element_t*)(node->data);
    if (strncmp(element->local_name, TD, TD_SIZE) != 0 && strncmp(element->local_name, TH, TH_SIZE) != 0)
    {
        // todo: parse errors
    }

    while (stack_size > 0 && strncmp(element->local_name, TH, TH_SIZE) != 0 && strncmp(element->local_name, TD, TD_SIZE))
    {
        stack_pop();
        node = stack[stack_idx];
        element = (html_element_t*)node->data;
    }

    // todo: Clear the list of active formatting elements up to the last marker.

    mode = HTML_PARSER_MODE_IN_ROW;
}


static void pop_all_including(html_node_t* node)
{
    html_node_t* current = stack[stack_idx];
    while (current != node)
    {
        stack_pop();
        current = stack[stack_idx];
    }

    stack_pop();
}


static void pop_elements_until_name_included(const unsigned char* name, const uint32_t name_size)
{
    html_node_t* current = stack[stack_idx];
    html_element_t* element = (html_element_t*)current->data;
    while (true)
    {
        if (string_compare(element->local_name, element->local_name_size, name, name_size))
        {
            break;
        }
        stack_pop();
        current = stack[stack_idx];
        element = (html_element_t*)current->data;
    }
    stack_pop();
}


static bool is_special(html_node_t* node)
{
    html_element_t* element = (html_element_t*)node->data;
    const unsigned char* name = element->local_name;
    const uint32_t name_size = element->local_name_size;

    return  string_compare(name, name_size, ADDRESS, ADDRESS_SIZE)      || string_compare(name, name_size, APPLET, APPLET_SIZE) ||
            string_compare(name, name_size, AREA, AREA_SIZE)            || string_compare(name, name_size, ARTICLE, ARTICLE_SIZE) ||
            string_compare(name, name_size, ASIDE, ASIDE_SIZE)          || string_compare(name, name_size, BASE, BASE_SIZE) ||
            string_compare(name, name_size, BASEFONT, BASEFONT_SIZE)    || string_compare(name, name_size, BGSOUND, BGSOUND_SIZE) ||
            string_compare(name, name_size, BLOCKQUOTE, BLOCKQUOTE_SIZE)|| string_compare(name, name_size, BODY, BODY_SIZE) ||
            string_compare(name, name_size, BR, BR_SIZE)                || string_compare(name, name_size, BUTTON, BUTTON_SIZE) ||
            string_compare(name, name_size, CAPTION, CAPTION_SIZE)      || string_compare(name, name_size, CENTER, CENTER_SIZE) ||
            string_compare(name, name_size, COL, COL_SIZE)              || string_compare(name, name_size, COLGROUP, COLGROUP_SIZE) ||
            string_compare(name, name_size, DD, DD_SIZE)                || string_compare(name, name_size, DETAILS, DETAILS_SIZE) ||
            string_compare(name, name_size, DIR, DIR_SIZE)              || string_compare(name, name_size, DIV, DIV_SIZE) ||
            string_compare(name, name_size, DL, DL_SIZE)                || string_compare(name, name_size, DT, DT_SIZE) ||
            string_compare(name, name_size, EMBED, EMBED_SIZE)          || string_compare(name, name_size, FIELDSET, FIELDSET_SIZE) ||
            string_compare(name, name_size, FIGCAPTION, FIGCAPTION_SIZE)|| string_compare(name, name_size, FIGURE, FIGURE_SIZE) ||
            string_compare(name, name_size, FOOTER, FOOTER_SIZE)        || string_compare(name, name_size, FORM, FORM_SIZE) ||
            string_compare(name, name_size, FRAME, FRAME_SIZE)          || string_compare(name, name_size, FRAMESET, FRAMESET_SIZE) ||
            string_compare(name, name_size, H1, H1_SIZE)                || string_compare(name, name_size, H2, H2_SIZE) ||
            string_compare(name, name_size, H3, H3_SIZE)                || string_compare(name, name_size, H4, H4_SIZE) ||
            string_compare(name, name_size, H5, H5_SIZE)                || string_compare(name, name_size, H6, H6_SIZE) ||
            string_compare(name, name_size, HEAD, HEAD_SIZE)            || string_compare(name, name_size, HEADER, HEADER_SIZE) ||
            string_compare(name, name_size, HGROUP, HGROUP_SIZE)        || string_compare(name, name_size, HR, HR_SIZE) ||
            string_compare(name, name_size, HTML, HTML_SIZE)            || string_compare(name, name_size, IFRAME, IFRAME_SIZE) ||
            string_compare(name, name_size, IMG, IMG_SIZE)              || string_compare(name, name_size, INPUT, INPUT_SIZE) ||
            string_compare(name, name_size, KEYGEN, KEYGEN_SIZE)        || string_compare(name, name_size, LI, LI_SIZE) ||
            string_compare(name, name_size, LINK, LINK_SIZE)            || string_compare(name, name_size, LISTING, LISTING_SIZE) ||
            string_compare(name, name_size, MAIN, MAIN_SIZE)            || string_compare(name, name_size, MARQUEE, MARQUEE_SIZE) ||
            string_compare(name, name_size, MENU, MENU_SIZE)            || string_compare(name, name_size, META, META_SIZE) ||
            string_compare(name, name_size, NAV, NAV_SIZE)              || string_compare(name, name_size, NOEMBED, NOEMBED_SIZE) ||
            string_compare(name, name_size, NOFRAMES, NOFRAMES_SIZE)    || string_compare(name, name_size, NOSCRIPT, NOSCRIPT_SIZE) ||
            string_compare(name, name_size, OBJECT, OBJECT_SIZE)        || string_compare(name, name_size, OL, OL_SIZE) ||
            string_compare(name, name_size, P, P_SIZE)                  || string_compare(name, name_size, PARAM, PARAM_SIZE) ||
            string_compare(name, name_size, PLAINTEXT, PLAINTEXT_SIZE)  || string_compare(name, name_size, PRE, PRE_SIZE) ||
            string_compare(name, name_size, SCRIPT, SCRIPT_SIZE)        || string_compare(name, name_size, SEARCH, SEARCH_SIZE) ||
            string_compare(name, name_size, SECTION, SECTION_SIZE)      || string_compare(name, name_size, SELECT, SELECT_SIZE) ||
            string_compare(name, name_size, SOURCE, SOURCE_SIZE)        || string_compare(name, name_size, STYLE, STYLE_SIZE) ||
            string_compare(name, name_size, SUMMARY, SUMMARY_SIZE)      || string_compare(name, name_size, TABLE, TABLE_SIZE) ||
            string_compare(name, name_size, TBODY, TBODY_SIZE)          || string_compare(name, name_size, TD, TD_SIZE) ||
            string_compare(name, name_size, TEMPLATE, TEMPLATE_SIZE)    || string_compare(name, name_size, TEXTAREA, TEXTAREA_SIZE) ||
            string_compare(name, name_size, TFOOT, TFOOT_SIZE)          || string_compare(name, name_size, TH, TH_SIZE) ||
            string_compare(name, name_size, THEAD, THEAD_SIZE)          || string_compare(name, name_size, TITLE, TITLE_SIZE) ||
            string_compare(name, name_size, TR, TR_SIZE)                || string_compare(name, name_size, TRACK, TRACK_SIZE) ||
            string_compare(name, name_size, UL, UL_SIZE)                || string_compare(name, name_size, WBR, WBR_SIZE);
}


static void insert_marker()
{
    assert(formatting_elements_size < 10);

    formatting_elements[formatting_elements_size] = NULL;
    formatting_elements_m[formatting_elements_size] = true;
    // formatting_elements_t[formatting_elements_size] = { 0 };
    memset(&formatting_elements_t[formatting_elements_size], 0, sizeof(html_token_t));

    formatting_elements_size++;
}

static void push_formatting_element(html_node_t* node, html_token_t* token)
{
    // todo: go backwards instead
    uint32_t last_marker = 0;
    for (uint32_t i = 0; i < formatting_elements_size; i++)
    {
        if (formatting_elements_m[i])
        {
            last_marker = i;
        }
    }

    if (formatting_elements_size - last_marker >= 3)
    {
        // todo: compare current element to the ones after the last marker and if match, then remove the oldest entry
    }

    formatting_elements[formatting_elements_size] = node;
    formatting_elements_m[formatting_elements_size] = false;
    memcpy(&formatting_elements_t[formatting_elements_size], token, sizeof(html_token_t));
    formatting_elements_size++;
}


static void reconstruct_formatting_elements()
{
    if (formatting_elements_size == 0)
    {
        return;
    }

    if (formatting_elements_m[formatting_elements_size - 1] || stack_contains_node(formatting_elements[formatting_elements_size - 1]))
    {
        return;
    }

    int32_t idx = (int32_t)formatting_elements_size - 1;
    html_node_t* entry = formatting_elements[idx];

    html_formatting_elements_step_e step    = FOUR;
    html_node_t* new_element                = NULL;
    bool run                                = true;

    while (run)
    {
        switch (step)
        {
        case FOUR:
            if (idx - 1 < 0)
            {
                step = EIGHT;
            }
            else
            {
                step = FIVE;
            }
            break;

        case FIVE:
            idx--;
            entry = formatting_elements[idx];
            step = SIX;
            break;

        case SIX:
            if (formatting_elements_m[idx] || stack_contains_node(entry))
            {
                step = SEVEN;
            }
            else
            {
                step = FOUR;
            }
            break;

        case SEVEN:
            idx++;
            entry = formatting_elements[idx];
            step = EIGHT;
            break;

        case EIGHT:
            ;
            html_token_t* t = &(formatting_elements_t[idx]);
            new_element = insert_html_element(t->name, t->name_size);
            step = NINE;
            break;

        case NINE:
            formatting_elements[idx] = new_element;
            step = TEN;
            break;

        case TEN:
            if (idx + 1 != (int32_t)formatting_elements_size)
            {
                step = SEVEN;
            }
            else
            {
                run = false;
            }
            break;
        }
    }
}

/********************/
/* public functions */
/********************/


void html_parser_init()
{
    mode            = HTML_PARSER_MODE_INITIAL;
    original_mode   = HTML_PARSER_MODE_INITIAL;
    stack_idx       = 0;
    stack_size      = 0;
    document        = NULL;
    stop            = false;

    formatting_elements_size = 0;
}


html_node_t* html_parser_run(const unsigned char* buffer, const uint32_t size)
{
    html_tokenizer_init(buffer, size, tokens, MAX_TOKENS);

    mode                        = HTML_PARSER_MODE_INITIAL;

    document                    = html_document_new();
    
    bool will_use_rules_for     = false;
    bool use_rules_for          = false;
    html_node_t* head_element   = NULL;
    html_node_t* form_element   = NULL;
    bool scripting_enabled      = false;

    while (!stop)
    {
        html_tokenizer_error_e err  = html_tokenizer_next();
        uint32_t tokens_size        = get_tokens_size();
        uint32_t i                  = 0;

        // todo: placeholder until i start using err
        assert((int)err >= 0);

        if (tokens_size == 0)
        {
            break;
        }


        while (i < tokens_size)
        {
            bool consume = true;
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
                    unsigned char compat[]  = "about:legacy-compat";
                    uint32_t compat_size    = sizeof(compat) - 1;
                    bool name_is_html       = t.name_size == 4 && strncmp(t.name, "html", 4) == 0;
                    bool public_id_missing  = t.public_id_size == 0;
                    bool system_id_missing  = t.system_id_size == 0;
                    bool is_legacy_compat   = t.system_id_size == compat_size && strncmp(t.system_id, compat, compat_size);

                    if (!name_is_html || !public_id_missing || !system_id_missing || !is_legacy_compat)
                    {
                        // todo: parse error
                    }
                    else
                    {
                        // todo: implement
                    }

                    mode = HTML_PARSER_MODE_BEFORE_HTML;
                }
                else
                {
                    // todo: If the document is not an iframe srcdoc document, then this is a parse error; 

                    // html_node_document_t* document_data = document->document_data;

                    // if (!document_data->parser_cannot_change_mode)
                    // {
                    //     document_data->compat_mode = string_new("quirks", 6);
                    // }

                    consume                 = false;
                    mode                    = HTML_PARSER_MODE_BEFORE_HTML;
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
                    html_node_t* element = create_element(t.name, t.name_size, document);
                    html_node_append(document, element);
                    stack_push(element);

                    mode = HTML_PARSER_MODE_BEFORE_HEAD;
                }
                else if (is_end && !(name_is(HTML, HTML_SIZE, &t) ||
                                     name_is(HEAD, HEAD_SIZE, &t) ||
                                     name_is(BODY, BODY_SIZE, &t) ||
                                     name_is(BR, BR_SIZE, &t)))
                {
                    // todo: parse error, ignore token
                }
                else
                {
                    html_node_t* element    = create_element(HTML, HTML_SIZE, document);

                    html_node_append(document, element);
                    stack_push(element);

                    mode                    = HTML_PARSER_MODE_BEFORE_HEAD;
                    consume                 = false;
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
                    head_element        = insert_html_element(t.name, t.name_size);
                    mode                = HTML_PARSER_MODE_IN_HEAD;
                }
                else if (is_end && !(name_is(HTML, HTML_SIZE, &t) ||
                                     name_is(HEAD, HEAD_SIZE, &t) ||
                                     name_is(BODY, BODY_SIZE, &t) ||
                                     name_is(BR, BR_SIZE, &t)))
                {
                    // todo: parse error
                }
                else
                {
                    head_element        = insert_html_element(HEAD, HEAD_SIZE);
                    mode                = HTML_PARSER_MODE_IN_HEAD;
                    consume             = false;
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inhead
            case HTML_PARSER_MODE_IN_HEAD:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    insert_character(&t);
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
                    original_mode       = HTML_PARSER_MODE_IN_HEAD;
                    will_use_rules_for  = true;
                }
                else if (is_start && (name_is(BASE, BASE_SIZE, &t) ||
                                      name_is(BASEFONT, BASEFONT_SIZE, &t) ||
                                      name_is(BGSOUND, BGSOUND_SIZE, &t) ||
                                      name_is(LINK, LINK_SIZE, &t)))
                {
                    insert_html_element(t.name, t.name_size);
                    stack_pop();

                    // todo: ack self closing tag
                }
                else if (is_start && name_is(META, META_SIZE, &t))
                {
                    insert_html_element(t.name, t.name_size);
                    stack_pop();

                    // todo: ack self closing tag
                    // todo: speculative parsing logic
                }
                else if (is_start && name_is(TITLE, TITLE_SIZE, &t))
                {
                    insert_html_element(t.name, t.name_size);
                    html_tokenizer_set_state(HTML_TOKENIZER_RCDATA_STATE);

                    original_mode       = mode;
                    mode                = HTML_PARSER_MODE_TEXT;
                }
                else if ((is_start && name_is(NOSCRIPT, NOSCRIPT_SIZE, &t) && scripting_enabled) || 
                         (is_start && (name_is(NOFRAMES, NOFRAMES_SIZE, &t) || name_is(STYLE, STYLE_SIZE, &t))))
                {
                    insert_html_element(t.name, t.name_size);
                    html_tokenizer_set_state(HTML_TOKENIZER_RAWTEXT_STATE);

                    original_mode       = mode;
                    mode                = HTML_PARSER_MODE_TEXT;
                }
                else if (is_start && name_is(NOSCRIPT, NOSCRIPT_SIZE, &t) && !scripting_enabled)
                {
                    insert_html_element(t.name, t.name_size);
                    mode                = HTML_PARSER_MODE_IN_HEAD_NOSCRIPT;
                }
                else if (is_start && name_is(SCRIPT, SCRIPT_SIZE, &t))
                {
                    // breakpoint
                    html_node_t* location   = get_appropriate_insertion_location(NULL);
                    html_node_t* element    = create_element(t.name, t.name_size, document);

                    // todo: step 3
                    // todo: step 4
                    // todo: step 5

                    html_node_append(location, element);
                    stack_push(element);
                    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

                    original_mode           = mode;
                    mode                    = HTML_PARSER_MODE_TEXT;
                }
                else if (is_end && name_is(HEAD, HEAD_SIZE, &t))
                {
                    stack_pop();
                    mode                = HTML_PARSER_MODE_AFTER_HEAD;
                }
                else if (is_start && name_is(TEMPLATE, TEMPLATE_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && name_is(TEMPLATE, TEMPLATE_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if ((is_start && name_is(HEAD, HEAD_SIZE, &t)) ||
                         (is_end && !(name_is(BODY, BODY_SIZE, &t) ||
                                      name_is(HTML, HTML_SIZE, &t) ||
                                      name_is(BR, BR_SIZE, &t))))
                {
                    // todo: parse error
                }
                else
                {
                    stack_pop();
                    mode                = HTML_PARSER_MODE_AFTER_HEAD;
                    consume             = false; 
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
                    consume             = false;
                    mode                = HTML_PARSER_MODE_IN_BODY;
                    original_mode       = HTML_PARSER_MODE_IN_HEAD_NOSCRIPT;
                    will_use_rules_for  = true;
                }
                else if (is_end && name_is(NOSCRIPT, NOSCRIPT_SIZE, &t))
                {
                    stack_pop();
                    mode                = HTML_PARSER_MODE_IN_HEAD;
                }
                else if ((is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' ') ) ||
                         (is_comment) ||
                         (is_start && (name_is(BASEFONT, BASEFONT_SIZE, &t) ||
                                       name_is(BGSOUND, BGSOUND_SIZE, &t)   ||
                                       name_is(LINK, LINK_SIZE, &t)         ||
                                       name_is(META, META_SIZE, &t)         ||
                                       name_is(NOFRAMES, NOFRAMES_SIZE, &t) ||
                                       name_is(STYLE, STYLE_SIZE, &t))))
                {
                    consume             = false;
                    mode                = HTML_PARSER_MODE_IN_HEAD;
                    original_mode       = HTML_PARSER_MODE_IN_HEAD_NOSCRIPT;
                    will_use_rules_for  = true;
                }
                else if ((is_start && (name_is(HEAD, HEAD_SIZE, &t) ||
                                       name_is(NOSCRIPT, NOSCRIPT_SIZE, &t))) ||
                        (is_end && !(name_is(BR, BR_SIZE, &t))))
                {
                    // todo: parse error, ignore token
                }
                else
                {
                    // todo: parse error
                    stack_pop();
                    mode                = HTML_PARSER_MODE_IN_HEAD;
                    consume             = false; 
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#the-after-head-insertion-mode
            case HTML_PARSER_MODE_AFTER_HEAD:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    insert_character(&t);
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
                    original_mode       = HTML_PARSER_MODE_AFTER_HEAD;
                    will_use_rules_for  = true;
                }
                else if (is_start && name_is(BODY, BODY_SIZE, &t))
                {
                    insert_html_element(t.name, t.name_size);
                    mode                = HTML_PARSER_MODE_IN_BODY;
                    // todo: self-closing tag ack
                }
                else if (is_start && name_is(FRAMESET, FRAMESET_SIZE, &t))
                {
                    insert_html_element(t.name, t.name_size);
                    mode                = HTML_PARSER_MODE_IN_FRAMESET;
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
                                      name_is(STYLE, STYLE_SIZE, &t)))
                {
                    // todo: parse error
                    assert(head_element);
                    stack_push(head_element);

                    consume             = false;
                    mode                = HTML_PARSER_MODE_IN_HEAD;
                    original_mode       = HTML_PARSER_MODE_AFTER_HEAD;
                    will_use_rules_for  = true;

                    remove_from_stack(head_element);
                }
                else if (is_end && name_is(TEMPLATE, TEMPLATE_SIZE, &t))
                {
                    consume             = false;
                    mode                = HTML_PARSER_MODE_IN_HEAD;
                    original_mode       = HTML_PARSER_MODE_AFTER_HEAD;
                    will_use_rules_for  = true;
                }
                else if ( (is_start && name_is(HEAD, HEAD_SIZE, &t))||
                          (is_end && !(name_is(BODY, BODY_SIZE, &t) ||
                                       name_is(HTML, HTML_SIZE, &t) ||
                                       name_is(BR, BR_SIZE, &t))))
                {
                    // todo: parse error
                }
                else
                {
                    insert_html_element(BODY, BODY_SIZE);
                    mode                = HTML_PARSER_MODE_IN_BODY;
                    consume             = false;
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inbody
            case HTML_PARSER_MODE_IN_BODY:
                if (is_character && t.data[0] == '\0')
                {
                    // todo: parse error, ignore token
                }
                else if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    // todo: reconstruct active formatting elements
                    insert_character(&t);
                }
                else if (is_character)
                {
                    // todo: reconstruct active formatting elements
                    insert_character(&t);
                    // todo: frameset-ok flag
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
                    if (stack_contains_element(TEMPLATE, TEMPLATE_SIZE)) { break; }
                    // todo: handle attributes logic
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
                                       name_is(STYLE, STYLE_SIZE, &t)))     ||
                        (is_end && name_is(TEMPLATE, TEMPLATE_SIZE, &t)))
                {
                    consume             = false;
                    mode                = HTML_PARSER_MODE_IN_HEAD;
                    original_mode       = HTML_PARSER_MODE_IN_BODY;
                    will_use_rules_for  = true;
                }
                else if (is_start && name_is(BODY, BODY_SIZE, &t))
                {
                    // todo: parse error

                    // If the stack of open elements has only one node on it, 
                    // or if the second element on the stack of open elements is not a body element, 
                    // or if there is a template element on the stack of open elements, 
                    // then ignore the token. (fragment case or there is a template element on the stack)

                    if ((stack_size == 0) ||
                        (stack_size > 1 && !string_compare(stack[1]->name, stack[1]->name_size, BODY, BODY_SIZE)) ||
                        (stack_contains_element(TEMPLATE, TEMPLATE_SIZE)))
                    {
                        // ignore
                    }
                    else
                    {
                        // todo: frameset-ok 
                        // todo: handle attribute logic
                    }
                }
                else if (is_start && name_is(FRAMESET, FRAMESET_SIZE, &t))
                {
                    // todo: parse error

                    if ((stack_size == 1) ||
                        (stack_size > 1 && !string_compare(BODY, BODY_SIZE,  stack[1]->name, stack[1]->name_size)))
                    {
                        // ignore token
                    }

                    // todo: frameset-ok flag logic

                    // todo: step 1 
                    // todo: step 2
                    insert_html_element(t.name, t.name_size);
                    mode            = HTML_PARSER_MODE_IN_FRAMESET;
                }
                else if (is_eof)
                {
                    // todo: handle stack of open template insertion modes
                    if (false)
                    {
                    
                    }
                    else
                    {
                        if (!(stack_contains_element(DD, DD_SIZE)               ||
                              stack_contains_element(DT, DT_SIZE)               ||
                              stack_contains_element(LI, LI_SIZE)               ||
                              stack_contains_element(OPTGROUP, OPTGROUP_SIZE)   ||
                              stack_contains_element(OPTION, OPTION_SIZE)       ||
                              stack_contains_element(P, P_SIZE)                 ||
                              stack_contains_element(RB, RP_SIZE)               ||
                              stack_contains_element(RT, RT_SIZE)               ||
                              stack_contains_element(RTC, RTC_SIZE)             ||
                              stack_contains_element(TBODY, TBODY_SIZE)         ||
                              stack_contains_element(TD, TD_SIZE)               ||
                              stack_contains_element(TFOOT, TFOOT_SIZE)         ||
                              stack_contains_element(TH, TH_SIZE)               ||
                              stack_contains_element(THEAD, THEAD_SIZE)         ||
                              stack_contains_element(TR, TR_SIZE)               ||
                              stack_contains_element(BODY, BODY_SIZE)           ||
                              stack_contains_element(HTML, HTML_SIZE)))
                        {
                            // todo: parse error
                        }
                        stop_parsing();
                    }
                }
                else if (is_end && name_is(BODY, BODY_SIZE, &t))
                {
                    // todo: handle scope logic
                    // If the stack of open elements does not have a body element in scope, this is a parse error; ignore the token.
                    if (!in_scope(BODY, BODY_SIZE, GENERIC_SCOPE))
                    {
                        // todo: parse error
                        NOT_IMPLEMENTED
                    }

                    if (!(stack_contains_element(DD, DD_SIZE)               ||
                          stack_contains_element(DT, DT_SIZE)               ||
                          stack_contains_element(LI, LI_SIZE)               ||
                          stack_contains_element(OPTGROUP, OPTGROUP_SIZE)   ||
                          stack_contains_element(OPTION, OPTION_SIZE)       ||
                          stack_contains_element(P, P_SIZE)                 ||
                          stack_contains_element(RB, RP_SIZE)               ||
                          stack_contains_element(RT, RT_SIZE)               ||
                          stack_contains_element(RTC, RTC_SIZE)             ||
                          stack_contains_element(TBODY, TBODY_SIZE)         ||
                          stack_contains_element(TD, TD_SIZE)               ||
                          stack_contains_element(TFOOT, TFOOT_SIZE)         ||
                          stack_contains_element(TH, TH_SIZE)               ||
                          stack_contains_element(THEAD, THEAD_SIZE)         ||
                          stack_contains_element(TR, TR_SIZE)               ||
                          stack_contains_element(BODY, BODY_SIZE)           ||
                          stack_contains_element(HTML, HTML_SIZE)))
                    {
                        // todo: parse error
                        NOT_IMPLEMENTED
                    }

                    mode                = HTML_PARSER_MODE_AFTER_BODY;
                }
                else if (is_end && name_is(HTML, HTML_SIZE, &t))
                {
                    // todo: handle scope logic
                    if (!(stack_contains_element(DD, DD_SIZE)               ||
                          stack_contains_element(DT, DT_SIZE)               ||
                          stack_contains_element(LI, LI_SIZE)               ||
                          stack_contains_element(OPTGROUP, OPTGROUP_SIZE)   ||
                          stack_contains_element(OPTION, OPTION_SIZE)       ||
                          stack_contains_element(P, P_SIZE)                 ||
                          stack_contains_element(RB, RP_SIZE)               ||
                          stack_contains_element(RT, RT_SIZE)               ||
                          stack_contains_element(RTC, RTC_SIZE)             ||
                          stack_contains_element(TBODY, TBODY_SIZE)         ||
                          stack_contains_element(TD, TD_SIZE)               ||
                          stack_contains_element(TFOOT, TFOOT_SIZE)         ||
                          stack_contains_element(TH, TH_SIZE)               ||
                          stack_contains_element(THEAD, THEAD_SIZE)         ||
                          stack_contains_element(TR, TR_SIZE)               ||
                          stack_contains_element(BODY, BODY_SIZE)           ||
                          stack_contains_element(HTML, HTML_SIZE)))
                    {
                        // todo: parse error
                    }

                    mode                = HTML_PARSER_MODE_AFTER_BODY;
                    consume             = false;
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
                    if (in_scope(P, P_SIZE, BUTTON_SCOPE))
                    {
                        close_p_element();
                    }

                    insert_html_element(t.name, t.name_size);
                }
                else if (is_start && (name_is(H1, H1_SIZE, &t)  ||
                                      name_is(H2, H2_SIZE, &t)  ||
                                      name_is(H3, H3_SIZE, &t)  ||
                                      name_is(H4, H4_SIZE, &t)  ||
                                      name_is(H5, H5_SIZE, &t)  ||
                                      name_is(H6, H6_SIZE, &t)))
                {
                    // todo: scope logic

                    html_node_t* node = stack[stack_idx];
                    html_element_t* element = (html_element_t*)node->data;
                    const unsigned char* local_name = element->local_name;
                    const uint32_t local_name_size = element->local_name_size;

                    if (string_compare(H1, H1_SIZE, local_name, local_name_size) ||
                        string_compare(H2, H2_SIZE, local_name, local_name_size) ||
                        string_compare(H3, H3_SIZE, local_name, local_name_size) ||
                        string_compare(H4, H4_SIZE, local_name, local_name_size) ||
                        string_compare(H5, H5_SIZE, local_name, local_name_size) ||
                        string_compare(H6, H6_SIZE, local_name, local_name_size))
                    {
                        // todo: parse error
                        stack_pop();
                    }
                    insert_html_element(t.name, t.name_size);
                }
                else if (is_start && (name_is(PRE, PRE_SIZE, &t) || 
                                      name_is(LISTING, LISTING_SIZE, &t)))
                {
                    // todo: scope logic
                    insert_html_element(t.name, t.name_size);
                    // todo: check if next token is \n
                    // todo: frameset-ok flag
                }
                else if (is_start && name_is(FORM, FORM_SIZE, &t))
                {
                    if (form_element && !stack_contains_element(TEMPLATE, TEMPLATE_SIZE))
                    {
                        // todo: parse error
                        // ignore token
                    }
                    else
                    {
                        // todo: scope logic
                        html_node_t* element = insert_html_element(t.name, t.name_size);
                        if (!stack_contains_element(TEMPLATE, TEMPLATE_SIZE))
                        {
                            form_element = element;
                        }
                    }
                }
                else if (is_start && name_is(LI, LI_SIZE, &t))
                {
                    // todo: implement
                    NOT_IMPLEMENTED
                }
                else if (is_start && (name_is(DT, DT_SIZE, &t) || name_is(DD, DD_SIZE, &t)))
                {
                    // todo: implement
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(PLAINTEXT, PLAINTEXT_SIZE, &t))
                {
                    // todo: handle scope logic - If the stack of open elements has a p element in button scope, then close a p element.
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(BUTTON, BUTTON_SIZE, &t))
                {
                    if (in_scope(BUTTON, BUTTON_SIZE, BUTTON_SCOPE))
                    {
                        generate_implied_end_tags(NULL, 0);

                        html_node_t* node = stack[stack_idx];
                        html_element_t* element = (html_element_t*)node->data;

                        while (!string_compare(element->local_name, element->local_name_size, BUTTON, BUTTON_SIZE))
                        {
                            stack_pop();
                            node = stack[stack_idx];
                            element = (html_element_t*)node->data;
                        }

                        stack_pop();
                    }

                    // todo: reconstruct the active formatting elements
                    insert_html_element(t.name, t.name_size);
                    // set frameset-ok flag to not ok
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
                    if (!in_scope(t.name, t.name_size, GENERIC_SCOPE))
                    {
                        // todo: parse error
                    }
                    else
                    {
                        generate_implied_end_tags(NULL, 0);

                        html_node_t* node = stack[stack_idx];
                        html_element_t* element = (html_element_t*)node->data;
                        if (!string_compare(element->local_name, element->local_name_size, t.name, t.name_size))
                        {
                            // todo: parse error
                        }

                        pop_elements_until_name_included(t.name, t.name_size);
                    }
                }
                else if (is_end && name_is(FORM, FORM_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && name_is(P, P_SIZE, &t))
                {
                    if (!in_scope(P, P_SIZE, BUTTON_SCOPE))
                    {
                        // todo: parse error
                        insert_html_element(P, P_SIZE);
                    }
                    
                    close_p_element();
                }
                else if (is_end && name_is(LI, LI_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (name_is(DD, DD_SIZE, &t) || name_is(DT, DT_SIZE, &t) ))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (name_is(H1, H1_SIZE, &t) || 
                                    name_is(H2, H2_SIZE, &t) || 
                                    name_is(H3, H3_SIZE, &t) ||
                                    name_is(H4, H4_SIZE, &t) || 
                                    name_is(H5, H5_SIZE, &t) || 
                                    name_is(H6, H6_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(A, A_SIZE, &t))
                {
                    NOT_IMPLEMENTED
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
                    reconstruct_formatting_elements();
                    html_node_t* node = insert_html_element(t.name, t.name_size);
                    push_formatting_element(node, &t);
                }
                else if (is_start && name_is(NOBR, NOBR_SIZE, &t))
                {
                    NOT_IMPLEMENTED
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
                    // todo: adoption agency algorithm
                    // run_agency_adoption();
                }
                else if (is_start && (name_is(APPLET, APPLET_SIZE, &t)  ||
                                      name_is(MARQUEE, MARQUEE_SIZE, &t)||
                                      name_is(OBJECT, OBJECT_SIZE, &t)) )
                {
                    reconstruct_formatting_elements();
                    insert_html_element(t.name, t.name_size);
                    insert_marker();
                    // todo: set frameset-ok flag to not ok
                }
                else if (is_end && (name_is(APPLET, APPLET_SIZE, &t)    ||
                                    name_is(MARQUEE, MARQUEE_SIZE, &t)  ||
                                    name_is(OBJECT, OBJECT_SIZE, &t)) )
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(TABLE, TABLE_SIZE, &t))
                {
                    // todo: If the Document is not set to quirks mode, and the stack of open elements has a p element in button scope, then close a p element.
                    insert_html_element(t.name, t.name_size);
                    // Set the frameset-ok flag to "not ok".

                    mode                = HTML_PARSER_MODE_IN_TABLE;
                }
                else if (is_end && name_is(BR, BR_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (name_is(AREA, AREA_SIZE, &t)      ||
                                      name_is(BR, BR_SIZE, &t)          ||
                                      name_is(EMBED, EMBED_SIZE, &t)    ||
                                      name_is(IMG, IMG_SIZE, &t)        ||
                                      name_is(KEYGEN, KEYGEN_SIZE, &t)  ||
                                      name_is(WBR, WBR_SIZE, &t)))
                {
                    // todo: reconstruct the active formatting elements

                    insert_html_element(t.name, t.name_size);
                    stack_pop();

                    // todo: acknowledge self closing tag
                    // todo: set frameset-ok flag to not ok
                }
                else if (is_start && name_is(INPUT, INPUT_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (name_is(PARAM, PARAM_SIZE, &t)    ||
                                      name_is(SOURCE, SOURCE_SIZE, &t)  ||
                                      name_is(TRACK, TRACK_SIZE, &t) ))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(HR, HR_SIZE, &t))
                {
                    if (in_scope(P, P_SIZE, BUTTON_SCOPE))
                    {
                        close_p_element();
                    }
                    if (in_scope(SELECT, SELECT_SIZE, GENERIC_SCOPE))
                    {
                        generate_implied_end_tags(NULL, 0);
                        if (in_scope(OPTION, OPTION_SIZE, GENERIC_SCOPE) || in_scope(OPTGROUP, OPTGROUP_SIZE, GENERIC_SCOPE))
                        {
                            // todo: parse error
                        }
                    }
                    insert_html_element(t.name, t.name_size);
                    stack_pop();
                    // todo: ack self closing tag if set
                    // todo: set frameset-ok to "not ok"
                }
                else if (is_start && name_is(IMAGE, IMAGE_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(TEXTAREA, TEXTAREA_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(XMP, XMP_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(IFRAME, IFRAME_SIZE, &t) )
                {
                    NOT_IMPLEMENTED
                }
                else if ( (is_start && name_is(NOEMBED, NOEMBED_SIZE, &t) ) ||
                          (is_start && name_is(NOSCRIPT, NOSCRIPT_SIZE, &t) && scripting_enabled) )
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(SELECT, SELECT_SIZE, &t) )
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (name_is(OPTGROUP, OPTGROUP_SIZE, &t) ||
                                      name_is(OPTION, OPTION_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (name_is(RB, RB_SIZE, &t) || 
                                      name_is(RTC, RTC_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (name_is(RP, RP_SIZE, &t) ||
                                      name_is(RT, RT_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(MATH, MATH_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(SVG, SVG_SIZE, &t))
                {
                    NOT_IMPLEMENTED
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
                        NOT_IMPLEMENTED
                }
                else if (is_start)
                {
                    // todo: Reconstruct the active formatting elements, if any.
                    insert_html_element(t.name, t.name_size);
                }
                else if (is_end)
                {
                    uint32_t idx = stack_idx;
                    html_node_t* node = stack[idx];
                    html_element_t* element = (html_element_t*)node->data;

                    while (true)
                    {
                        if (string_compare(element->local_name, element->local_name_size, t.name, t.name_size))
                        {
                            generate_implied_end_tags(t.name, t.name_size);
                            if (node == stack[stack_idx])
                            {
                                // todo: parse error
                            }

                            pop_all_including(node);
                            break;
                        }
                        else if (is_special(node))
                        {
                            // todo: parse error
                            break;
                        }

                        idx--;
                        node = stack[idx];
                        element = (html_element_t*)node->data;
                    }
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incdata
            case HTML_PARSER_MODE_TEXT:
                if (is_character)
                {
                    insert_character(&t);
                }
                else if (is_eof)
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && name_is(SCRIPT, SCRIPT_SIZE, &t))
                {
                    // breakpoint
                    // todo: speculative parsing

                    stack_pop();
                    mode = original_mode;

                    // todo: there is more implementation logic related to speculative parsing
                    // todo: nesting level logic
                }
                else if (is_end)
                {
                    stack_pop();
                    mode = original_mode;
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intable
            case HTML_PARSER_MODE_IN_TABLE:
                ;
                unsigned char* name = stack[stack_idx]->name;
                uint32_t name_size = stack[stack_idx]->name_size;
                if (is_character && ((name_size == TABLE_SIZE && strncmp(name, TABLE, name_size))        ||
                                     (name_size == TBODY_SIZE && strncmp(name, TBODY, name_size))        ||
                                     (name_size == TEMPLATE_SIZE && strncmp(name, TEMPLATE, name_size))  ||
                                     (name_size == TFOOT_SIZE && strncmp(name, TFOOT, name_size))        ||
                                     (name_size == THEAD_SIZE && strncmp(name, THEAD, name_size))        ||
                                     (name_size == TR_SIZE && strncmp(name, TR, name_size))))
                {
                
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(CAPTION, CAPTION_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(COLGROUP, COLGROUP_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(COL, COL_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (name_is(TBODY, TBODY_SIZE, &t) ||
                                      name_is(TFOOT, TFOOT_SIZE, &t) ||
                                      name_is(THEAD, THEAD_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (name_is(TD, TD_SIZE, &t) ||
                                      name_is(TH, TH_SIZE, &t) ||
                                      name_is(TR, TR_SIZE, &t)))
                {
                    // Clear the stack back to a table context. (See below.)
                    insert_html_element(TBODY, TBODY_SIZE);

                    mode                = HTML_PARSER_MODE_IN_TABLE_BODY;
                    consume             = false;
                }
                else if (is_start && name_is(TABLE, TABLE_SIZE, &t))
                {
                        NOT_IMPLEMENTED
                }
                else if (is_end && name_is(TABLE, TABLE_SIZE, &t))
                {
                    NOT_IMPLEMENTED
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
                    NOT_IMPLEMENTED
                }
                else if ((is_start && (name_is(STYLE, STYLE_SIZE, &t)           ||
                                       name_is(SCRIPT, SCRIPT_SIZE, &t)         ||
                                       name_is(TEMPLATE, TEMPLATE_SIZE, &t)))   ||
                         (is_end && name_is(TEMPLATE, TEMPLATE_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(INPUT, INPUT_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(FORM, FORM_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_eof)
                {
                    NOT_IMPLEMENTED
                }
                else
                {
                    NOT_IMPLEMENTED
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intabletext
            case HTML_PARSER_MODE_IN_TABLE_TEXT:
                if (is_character && t.data_size == 1 && t.data[0] == '\0')
                {
                    NOT_IMPLEMENTED
                }
                else if (is_character)
                {
                    NOT_IMPLEMENTED
                }
                else
                {
                    NOT_IMPLEMENTED
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incaption
            case HTML_PARSER_MODE_IN_CAPTION:
                if (is_end && name_is(CAPTION, CAPTION_SIZE, &t))
                {
                    NOT_IMPLEMENTED
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
                    NOT_IMPLEMENTED
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
                    NOT_IMPLEMENTED
                }
                else
                {
                    NOT_IMPLEMENTED
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incolgroup
            case HTML_PARSER_MODE_IN_COLUMN_GROUP:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(COL, COL_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && name_is(COLGROUP, COLGROUP_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && name_is(COL, COL_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if ((is_start || is_end) && (name_is(TEMPLATE, TEMPLATE_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_eof)
                {
                    NOT_IMPLEMENTED
                }
                else
                {
                    NOT_IMPLEMENTED
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intbody
            case HTML_PARSER_MODE_IN_TABLE_BODY:
                if (is_start && name_is(TR, TR_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (name_is(TH, TH_SIZE, &t) ||
                                      name_is(TD, TD_SIZE, &t)))
                {
                    // todo: parse error
                    // todo: Clear the stack back to a table body context. (See below.)

                    insert_html_element(TR, TR_SIZE);

                    mode                = HTML_PARSER_MODE_IN_ROW;
                    consume             = false;
                }
                else if (is_end && (name_is(TBODY, TBODY_SIZE, &t) ||
                                    name_is(TFOOT, TFOOT_SIZE, &t) ||
                                    name_is(THEAD, THEAD_SIZE, &t) ))
                {
                    NOT_IMPLEMENTED
                }
                else if ((is_start && (name_is(CAPTION, CAPTION_SIZE, &t)   ||
                                       name_is(COL, COL_SIZE, &t)           ||
                                       name_is(COLGROUP, COLGROUP_SIZE, &t) ||
                                       name_is(TBODY, TBODY_SIZE, &t)       ||
                                       name_is(TFOOT, TFOOT_SIZE, &t)       ||
                                       name_is(THEAD, THEAD_SIZE, &t)))     ||
                         (is_end && name_is(TABLE, TABLE_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (name_is(BODY, BODY_SIZE, &t)         ||
                                    name_is(CAPTION, CAPTION_SIZE, &t)   ||
                                    name_is(COL, COL_SIZE, &t)           ||
                                    name_is(COLGROUP, COLGROUP_SIZE, &t) ||
                                    name_is(HTML, HTML_SIZE, &t)         ||
                                    name_is(TD, TD_SIZE, &t)             ||
                                    name_is(TH, TH_SIZE, &t)             ||
                                    name_is(TR, TR_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else
                {
                    NOT_IMPLEMENTED
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intr
            case HTML_PARSER_MODE_IN_ROW:
                if (is_start && (name_is(TH, TH_SIZE, &t) ||
                                 name_is(TD, TD_SIZE, &t)))
                {
                    // todo: Clear the stack back to a table row context. (See below.)
                    insert_html_element(t.name, t.name_size);
                    // todo: Insert a marker at the end of the list of active formatting elements.
                    mode                = HTML_PARSER_MODE_IN_CELL;
                }
                else if (is_end && name_is(TR, TR_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if ((is_start && (name_is(CAPTION, CAPTION_SIZE, &t)   ||
                                       name_is(COL, COL_SIZE, &t)           ||
                                       name_is(COLGROUP, COLGROUP_SIZE, &t) ||
                                       name_is(TBODY, TBODY_SIZE, &t)       ||
                                       name_is(TFOOT, TFOOT_SIZE, &t)       ||
                                       name_is(THEAD, THEAD_SIZE, &t)       ||
                                       name_is(TR, TR_SIZE, &t)))           ||
                         (is_end && name_is(TABLE, TABLE_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (name_is(TBODY, TBODY_SIZE, &t) ||
                                    name_is(THEAD, THEAD_SIZE, &t) ||
                                    name_is(TFOOT, TFOOT_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (name_is(BODY, BODY_SIZE, &t)        ||
                                    name_is(CAPTION, CAPTION_SIZE, &t)  ||
                                    name_is(COL, COL_SIZE, &t)          ||
                                    name_is(COLGROUP, COLGROUP_SIZE, &t)||
                                    name_is(HTML, HTML_SIZE, &t)        ||
                                    name_is(TD, TD_SIZE, &t)            ||
                                    name_is(TH, TH_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else
                {
                    NOT_IMPLEMENTED
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intd
            case HTML_PARSER_MODE_IN_CELL:
                if (is_end && (name_is(TD, TD_SIZE, &t) ||
                               name_is(TH, TH_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (name_is(CAPTION, CAPTION_SIZE, &t)    ||
                                      name_is(COL, COL_SIZE, &t)            ||
                                      name_is(COLGROUP, COLGROUP_SIZE, &t)  ||
                                      name_is(TBODY, TBODY_SIZE, &t)        ||
                                      name_is(TFOOT, TFOOT_SIZE, &t)        ||
                                      name_is(THEAD, THEAD_SIZE, &t)        ||
                                      name_is(TR, TR_SIZE, &t)              ||
                                      name_is(TH, TH_SIZE, &t)              ||
                                      name_is(TD, TD_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (name_is(BODY, BODY_SIZE, &t)        ||
                                    name_is(CAPTION, CAPTION_SIZE, &t)  ||
                                    name_is(COL, COL_SIZE, &t)          ||
                                    name_is(COLGROUP, COLGROUP_SIZE, &t)||
                                    name_is(HTML, HTML_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (name_is(TABLE, TABLE_SIZE, &t) ||
                                    name_is(TFOOT, TFOOT_SIZE, &t) ||
                                    name_is(THEAD, THEAD_SIZE, &t) ||
                                    name_is(TBODY, TBODY_SIZE, &t) ||
                                    name_is(TR, TR_SIZE, &t)))
                {
                    if (in_scope(t.name, t.name_size, TABLE_SCOPE))
                    {
                        // todo: parse error
                    }
                    else
                    {
                        close_cell();
                        consume                 = false;
                    }
                }
                else
                {
                    consume                 = false;
                    mode                    = HTML_PARSER_MODE_IN_BODY;
                    original_mode           = HTML_PARSER_MODE_IN_CELL;
                    will_use_rules_for      = true;
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inselect
            case HTML_PARSER_MODE_IN_SELECT:
                if (is_character && t.data_size == 1 && t.data[0] == '\0')
                {
                    NOT_IMPLEMENTED
                }
                else if (is_character)
                {
                    NOT_IMPLEMENTED
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(OPTION, OPTION_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(OPTGROUP, OPTGROUP_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(HR, HR_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && name_is(OPTGROUP, OPTGROUP_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && name_is(OPTION, OPTION_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && name_is(SELECT, SELECT_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(SELECT, SELECT_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (name_is(INPUT, INPUT_SIZE, &t)    ||
                                      name_is(KEYGEN, KEYGEN_SIZE, &t)  ||
                                      name_is(TEXTAREA, TEXTAREA_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if ((is_start && (name_is(SCRIPT, SCRIPT_SIZE, &t)         ||
                                       name_is(TEMPLATE, TEMPLATE_SIZE, &t)))   ||
                         (is_end && name_is(TEMPLATE, TEMPLATE_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_eof)
                {
                    NOT_IMPLEMENTED
                }
                else
                {
                    NOT_IMPLEMENTED
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
                    NOT_IMPLEMENTED
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
                    NOT_IMPLEMENTED
                }
                else
                {
                    NOT_IMPLEMENTED
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intemplate
            case HTML_PARSER_MODE_IN_TEMPLATE:
                if (is_character || is_comment || is_doctype)
                {
                    NOT_IMPLEMENTED
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
                    NOT_IMPLEMENTED
                }
                else if (is_start && (name_is(CAPTION, CAPTION_SIZE, &t)    ||
                                      name_is(COLGROUP, COLGROUP_SIZE, &t)  ||
                                      name_is(TBODY, TBODY_SIZE, &t)        ||
                                      name_is(TFOOT, TFOOT_SIZE, &t)        ||
                                      name_is(THEAD, THEAD_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(COL, COL_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(TR, TR_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (name_is(TD, TD_SIZE, &t) ||
                                      name_is(TH, TH_SIZE, &t)))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start)
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end)
                {
                    NOT_IMPLEMENTED
                }
                else if (is_eof)
                {
                    NOT_IMPLEMENTED
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-afterbody
            case HTML_PARSER_MODE_AFTER_BODY:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_comment)
                {
                    insert_comment(&t, stack[0]);
                }
                else if (is_doctype)
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && name_is(HTML, HTML_SIZE, &t))
                {
                    // todo: if parser was created using fragment parsing algorithm -> error and ignore token

                    mode                = HTML_PARSER_MODE_AFTER_AFTER_BODY;
                }
                else if (is_eof)
                {
                    stop_parsing();
                }
                else
                {
                    // todo: parse error
                    mode                = HTML_PARSER_MODE_IN_BODY;
                    consume             = false;
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inframeset
            case HTML_PARSER_MODE_IN_FRAMESET:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(FRAMESET, FRAMESET_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && name_is(FRAMESET, FRAMESET_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(FRAME, FRAME_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(NOFRAMES, NOFRAMES_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_eof)
                {
                    NOT_IMPLEMENTED
                }
                else
                {
                    NOT_IMPLEMENTED
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-afterframeset
            case HTML_PARSER_MODE_AFTER_FRAMESET:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(HTML, HTML_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && name_is(HTML, HTML_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(NOFRAMES, NOFRAMES_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_eof)
                {
                    NOT_IMPLEMENTED
                }
                else
                {
                    NOT_IMPLEMENTED
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
                    NOT_IMPLEMENTED
                }
                else if (is_eof)
                {
                    stop_parsing();
                }
                else
                {
                    NOT_IMPLEMENTED
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
                    NOT_IMPLEMENTED
                }
                else if (is_eof)
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && name_is(NOFRAMES, NOFRAMES_SIZE, &t))
                {
                    NOT_IMPLEMENTED
                }
                else
                {
                    NOT_IMPLEMENTED
                }
                break;
            }

            if (use_rules_for)      { use_rules_for = false; mode = original_mode; }
            if (will_use_rules_for) { will_use_rules_for = false; use_rules_for = true; }
            if (consume)            { i++; }
        }
    }

    html_tokenizer_free();

    return document;
}

void html_parser_free()
{
    
}