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
#include "dom/attribute.h"
#include "html/select.h"
#include "util/not_implemented.h"
#include "html/tag_constants.h"

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

typedef enum
{
    GENERIC_SCOPE,
    BUTTON_SCOPE,
    TABLE_SCOPE,
} dom_element_scope_e;

typedef struct
{
    dom_node_t* parent;
    dom_node_t* child;
} dom_insertion_location_t;

static html_parser_mode_e mode                      = HTML_PARSER_MODE_INITIAL;
static html_parser_mode_e original_mode             = HTML_PARSER_MODE_INITIAL;
static html_parser_mode_e replacement_mode          = HTML_PARSER_MODE_INITIAL;
static bool use_rules_for                           = false;

static html_token_t pending_tokens[MAX_TOKENS]      = { 0 };
static uint32_t pending_tokens_size                 = 0;

static html_token_t tokens[MAX_TOKENS]              = { 0 };
static dom_node_t* stack[OPEN_STACK_MAX_SIZE]      = { 0 };
static uint32_t stack_idx                           = 0;
static uint32_t stack_size                          = 0;
static dom_node_t* document                        = NULL;
static bool stop                                    = false;
static bool foster_parenting                        = false;
static bool will_use_foster_parenting               = false;
static dom_node_t* head_pointer                    = NULL;

static dom_node_t* formatting_elements[10]         = { 0 };
static bool formatting_elements_m[10]               = { 0 };
static html_token_t formatting_elements_t[10]       = { 0 };
static uint32_t formatting_elements_size            = 0;

/********************/
/* static functions */
/********************/

// // todo: remove
// static void print_document_tree(dom_node_t* node, uint32_t level)
// {
//     for (uint32_t i = 0; i < level; i++)
//     {
//         printf("  ");
//     }

//     if (dom_node_is_element(node))
//     {
//         dom_element_t* element = dom_element_from_node(node);
//         const unsigned char* name = hash_str_get(element->local_name);
//         const uint32_t name_size = hash_str_get_size(element->local_name);
//         printf("%.*s\n", name_size, name);

//         dom_attr_t* attr = element->attr;

//         for (uint32_t i = 0; i < element->attr_size; i++)
//         {
//             for (uint32_t j = 0; j < level; j++)
//             {
//                 printf("  ");
//             }
//             printf("  ");

//             const unsigned char* attr_name = hash_str_get(attr->name);
//             const uint32_t attr_name_size = hash_str_get_size(attr->name);
//             printf("#attr - %.*s\n", attr_name_size, attr_name);
//             attr = attr->next;
//         }
//     }
//     else if (dom_node_is_document(node))
//     {
//         printf("#document\n");
//     }
//     else if (dom_node_is_text(node))
//     {
//         dom_text_t* text = dom_text_from_node(node);
//         printf("#text - %s\n", text->data);
//     }
//     else if (dom_node_is_comment(node))
//     {
//         dom_comment_t* comment = dom_comment_from_node(node);
//         printf("<!-- %s -->\n", comment->data);
//     }

//     dom_node_t* child = node->first;
//     while (child)
//     {
//         print_document_tree(child, level + 1);
//         child = child->next;
//     }
// }

// static bool string_compare(const unsigned char* first, const uint32_t first_size, const unsigned char* second, const uint32_t second_size)
// {
//     if (first_size != second_size) { return false; }

//     return strncmp(first, second, first_size) == 0;
// }

static uint32_t get_tokens_size()
{
    uint32_t result = 0;
    for (uint32_t i = 0; i < MAX_TOKENS; i++) { if (tokens[i].is_valid) { result += 1; } }

    return result;
}


static void stack_push(dom_node_t* node)
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

static void remove_from_stack(dom_node_t* node)
{
    for (uint32_t i = 0; i < stack_size; i++)
    {
        if (node != stack[i]) { continue; }

        for (uint32_t j = i; j < stack_size - 1; j++)
        {
            stack[j] = stack[j + 1];
        }

        stack_idx = stack_idx > 0 ? stack_idx - 1 : 0;
        stack_size--;
        return;
    }
}


static void stack_insert(uint32_t index, dom_node_t* node)
{
    if (index >= stack_size) { return; }

    for (uint32_t i = stack_size; i > index; i--)
    {
        stack[i] = stack[i-1];
    }

    stack[index] = node;
    stack_size++;
}


// static bool name_is(const unsigned char* name, const uint32_t name_size, const html_token_t* token)
// {
//     if (token->name_size != name_size) { return false; }

//     return string_compare(name, name_size, token->name, token->name_size);
// }


static bool stack_contains_element(hash_str_t name)
{
    for (uint32_t i = 0; i < stack_size; i++)
    {
        dom_node_t* node = stack[i];

        if (!node)                      { return false; }
        if (!dom_node_is_element(node)) { continue; }
        if (node->name == name)         { return true; }

        // todo: figure out which name to use and perform case insensitive comparison
    }

    return false;
}


static bool stack_contains_node(dom_node_t* node)
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


static bool in_scope(const hash_str_t name, dom_element_scope_e scope)
{
    dom_node_t* node = stack[stack_idx];

    int32_t i = (int32_t)stack_idx;

    while (i >= 0)
    {
        const hash_str_t node_name = node->name;

        if (node_name == name) { return true; }

        // applies to all scopes
        if (node_name == html_tag_html() || node_name == html_tag_table() || node_name == html_tag_template())
        {
            return false;
        }

        // generic, button and list scope
        if ((scope != TABLE_SCOPE) && (node_name == html_tag_applet() ||
                                       node_name == html_tag_caption() ||
                                       node_name == html_tag_td() ||
                                       node_name == html_tag_th() ||
                                       node_name == html_tag_marquee() ||
                                       node_name == html_tag_select() ||
                                       node_name == html_tag_object()))
        {
            return false;
        }

        if (scope == BUTTON_SCOPE && node_name == html_tag_button())
        {
            return false;
        }

        i--;
        node = stack[i];
    }

    return false;
}


static dom_node_t* find_last_stack_element(const hash_str_t name)
{
    int32_t i = (int32_t)stack_size - 1;

    while (i >= 0)
    {
        dom_node_t* node = stack[i];

        if (node->name == name) { return node; }

        i--;
    }

    return NULL;
}


static uint32_t find_node_index(dom_node_t** list, uint32_t list_size, dom_node_t* node)
{
    for (int32_t i = (int32_t)list_size - 1; i >= 0; i--)
    {
        if (list[i] == node) { return (uint32_t)i; }
    }

    NOT_IMPLEMENTED
    return 0;
}


static dom_insertion_location_t get_appropriate_insertion_location(dom_node_t* override)
{
    dom_insertion_location_t location  = { NULL, NULL };
    dom_node_t* target                 = stack[stack_idx];

    if (override) { target = override; }

    if (foster_parenting && dom_node_is_element(target) && (target->name == html_tag_table() ||
                                                            target->name == html_tag_tbody() ||
                                                            target->name == html_tag_thead() ||
                                                            target->name == html_tag_tfoot() ||
                                                            target->name == html_tag_tr()))
    {
        dom_node_t* last_template = find_last_stack_element(html_tag_template());
        dom_node_t* last_table = find_last_stack_element(html_tag_table());

        if (last_template)
        {
            NOT_IMPLEMENTED
        }
        else if (!last_table)
        {
            location.parent = stack[0];
        }
        else if (last_table->parent)
        {
            location.parent = last_table->parent;
            location.child = last_table;
        }
        else
        {
            uint32_t table_i = find_node_index(stack, stack_size, last_table);
            dom_node_t* prev = stack[table_i - 1];
            location.parent = prev;
        }
    }
    else
    {
        location.parent = target;
    }

    if (false) // inside template element
    {
    
    }

    return location;
}


static void insert_comment(html_token_t* token, dom_node_t* position)
{
    dom_insertion_location_t location   = get_appropriate_insertion_location(position);
    dom_node_t* comment                 = dom_comment_new(document, token->data, token->data_size);

    dom_node_insert_before(location.parent, comment, location.child);
}


static dom_node_t* create_appropriate_element(dom_node_t* doc, hash_str_t name)
{
    if (name == html_tag_select())  { return html_select_new(doc);              }
    else                            { return html_element_new(doc, name);       }
}

// https://html.spec.whatwg.org/multipage/parsing.html#create-an-element-for-the-token
static dom_node_t* create_element(hash_str_t name, html_token_t* token, dom_node_t* doc)
{
    // todo: step 1
    // todo: step 2

    // dom_node_t* doc = parent->document;

    // todo: step 5
    // todo: step 6
    // todo: step 7
    // todo: step 8
    // todo: step 9

    // dom_node_t* doc        = parent->document;
    dom_node_t* element    = create_appropriate_element(doc, name);
    if (token)
    {
        for (uint32_t i = 0; i < token->attributes_size; i++)
        {
            html_token_attribute_t t_attr = token->attributes[i];
            hash_str_t a_name = hash_str_new(t_attr.name, t_attr.name_size);
            hash_str_t a_value = hash_str_new(t_attr.value, t_attr.value_size);
            dom_node_t* attr = dom_attr_new(a_name, a_value, element);
            dom_element_append_attr(dom_element_from_node(element), dom_attr_from_node(attr));
        }
    }

    // todo: step 11
    // todo: step 12
    // todo: step 13
    // todo: step 14
    // todo: step 15

    return element;
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-a-foreign-element
static dom_node_t* insert_foreign_element(hash_str_t name, html_token_t* token, bool only_add_to_stack)
{
    dom_insertion_location_t location  = get_appropriate_insertion_location(NULL);
    dom_node_t* node                   = create_element(name, token, document);

    if (!only_add_to_stack)
    {
        // todo: step 2
        // todo: step 3
        dom_node_insert_before(location.parent, node, location.child);
        // todo: step 5
    }

    stack_push(node);

    return node;
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-an-html-element
static dom_node_t* insert_html_element(hash_str_t name, html_token_t* token)
{
    return insert_foreign_element(name, token, false);
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-a-character
static void insert_character(unsigned char* data, uint32_t data_size)
{
    dom_insertion_location_t insertion_position = get_appropriate_insertion_location(NULL);
    dom_node_t* location   = insertion_position.parent;
    dom_node_t* child      = insertion_position.child;

    if (!child) { child = location->last; }
    else        { child = child->prev; }

    if (dom_node_is_document(location)) { return; }

    if (child && dom_node_is_text(child))
    {
        dom_text_append_data(child, data, data_size);
    }
    else
    {
        dom_node_t* node = dom_text_new(document, data, data_size);
        dom_node_insert_before(insertion_position.parent, node, insertion_position.child);
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


static void generate_implied_end_tags(const hash_str_t name)
{
    while (stack_size > 0)
    {
        dom_node_t* node = stack[stack_idx];
        const hash_str_t node_name = node->name;

        if (name != 0 && node_name == name)
        {
            return;
        }

        if (node_name == html_tag_dd()        ||
            node_name == html_tag_dt()        ||
            node_name == html_tag_li()        ||
            node_name == html_tag_optgroup()  ||
            node_name == html_tag_option()    ||
            node_name == html_tag_p()         ||
            node_name == html_tag_rb()        ||
            node_name == html_tag_rp()        ||
            node_name == html_tag_rt()        ||
            node_name == html_tag_rtc())
        {
            stack_pop();
        }
        else
        {
            return;
        }
    }
}


static void clear_formatting_elements()
{
    INCOMPLETE_IMPLEMENTATION("remove magic constat");

    while (formatting_elements_size < 10)
    {
        bool is_marker = formatting_elements_m[formatting_elements_size - 1];
        formatting_elements_size--;

        if (is_marker) { return; }
    }
}


static void close_p_element()
{
    generate_implied_end_tags(html_tag_p());

    dom_node_t* node = stack[stack_idx];
    hash_str_t node_name = node->name;

    if (node_name != html_tag_p())
    {
        INCOMPLETE_IMPLEMENTATION("parse error");
    }

    bool run = true;
    // todo: when do we stop ? 
    while (run)
    {
        node_name = node->name;

        if (node_name == html_tag_p()) { run = false; }
        stack_pop();

        node = stack[stack_idx];
    }
}


static void close_cell()
{
    generate_implied_end_tags(0);

    dom_node_t* node = stack[stack_idx];
    hash_str_t node_name = node->name;

    if (node_name != html_tag_td() && node_name != html_tag_th())
    {
        INCOMPLETE_IMPLEMENTATION("parse error");
    }

    while (stack_size > 0 && node_name != html_tag_th() && node_name != html_tag_td())
    {
        stack_pop();
        node = stack[stack_idx];
        node_name = node->name;
    }

    stack_pop();
    clear_formatting_elements();

    mode = HTML_PARSER_MODE_IN_ROW;
}


static void pop_all_including(dom_node_t* node)
{
    dom_node_t* current = stack[stack_idx];
    while (current != node)
    {
        stack_pop();
        current = stack[stack_idx];
    }

    stack_pop();
}


static void pop_elements_until_name_included(const hash_str_t name)
{
    dom_node_t* node = stack[stack_idx];

    while (true)
    {
        if (node->name == name) { break; }

        stack_pop();
        node = stack[stack_idx];
    }
    stack_pop();
}


static bool is_special(dom_node_t* node)
{
    hash_str_t name = node->name;

    return  name == html_tag_address()    || name == html_tag_applet()      ||
            name == html_tag_area()       || name == html_tag_article()     ||
            name == html_tag_aside()      || name == html_tag_base()        ||
            name == html_tag_basefont()   || name == html_tag_bgsound()     ||
            name == html_tag_blockquote() || name == html_tag_body()        ||
            name == html_tag_br()         || name == html_tag_button()      ||
            name == html_tag_caption()    || name == html_tag_center()      ||
            name == html_tag_col()        || name == html_tag_colgroup()    ||
            name == html_tag_dd()         || name == html_tag_details()     ||
            name == html_tag_dir()        || name == html_tag_div()         ||
            name == html_tag_dl()         || name == html_tag_dt()          ||
            name == html_tag_embed()      || name == html_tag_fieldset()    ||
            name == html_tag_figcaption() || name == html_tag_figure()      ||
            name == html_tag_footer()     || name == html_tag_form()        ||
            name == html_tag_frame()      || name == html_tag_frameset()    ||
            name == html_tag_h1()         || name == html_tag_h2()          ||
            name == html_tag_h3()         || name == html_tag_h4()          ||
            name == html_tag_h5()         || name == html_tag_h6()          ||
            name == html_tag_head()       || name == html_tag_header()      ||
            name == html_tag_hgroup()     || name == html_tag_hr()          ||
            name == html_tag_html()       || name == html_tag_iframe()      ||
            name == html_tag_img()        || name == html_tag_input()       ||
            name == html_tag_keygen()     || name == html_tag_li()          ||
            name == html_tag_link()       || name == html_tag_listing()     ||
            name == html_tag_main()       || name == html_tag_marquee()     ||
            name == html_tag_menu()       || name == html_tag_meta()        ||
            name == html_tag_nav()        || name == html_tag_noembed()     ||
            name == html_tag_noframes()   || name == html_tag_noscript()    ||
            name == html_tag_object()     || name == html_tag_ol()          ||
            name == html_tag_p()          || name == html_tag_param()       ||
            name == html_tag_plaintext()  || name == html_tag_pre()         ||
            name == html_tag_script()     || name == html_tag_search()      ||
            name == html_tag_section()    || name == html_tag_select()      ||
            name == html_tag_source()     || name == html_tag_style()       ||
            name == html_tag_summary()    || name == html_tag_table()       ||
            name == html_tag_tbody()      || name == html_tag_td()          ||
            name == html_tag_template()   || name == html_tag_textarea()    ||
            name == html_tag_tfoot()      || name == html_tag_th()          ||
            name == html_tag_thead()      || name == html_tag_title()       ||
            name == html_tag_tr()         || name == html_tag_track()       ||
            name == html_tag_ul()         || name == html_tag_wbr();
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

static void push_formatting_element(dom_node_t* node, html_token_t* token)
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


static void insert_formatting_element(dom_node_t* node, html_token_t* token, uint32_t idx)
{
    for (uint32_t i = formatting_elements_size; i > idx; i--)
    {
        formatting_elements[i] = formatting_elements[i - 1];
        formatting_elements_m[i] = formatting_elements_m[i - 1];
        memcpy(&formatting_elements_t[i], &formatting_elements_t[i - 1], sizeof(html_token_t));
    }

    formatting_elements[idx] = node;
    formatting_elements_m[idx] = false;
    memcpy(&formatting_elements_t[idx], token, sizeof(html_token_t));
    formatting_elements_size++;
}


static void remove_formatting_element(dom_node_t* node)
{
    if (formatting_elements_size == 0) { return; }

    int32_t idx = -1;
    int32_t size = (int32_t) formatting_elements_size;
    for (int32_t i = 0; i < size; i++)
    {
        if (formatting_elements[i] == node)
        {
            idx = i;
            break;
        }
    }

    if (idx < 0) { return; }

    for (int32_t i = idx; i < size - 1; i++)
    {
        formatting_elements[i] = formatting_elements[i + 1];
        formatting_elements_m[i] = formatting_elements_m[i + 1];
        memcpy(&formatting_elements_t[i], &formatting_elements_t[i + 1], sizeof(html_token_t));
    }

    formatting_elements_size--;
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
    dom_node_t* entry = formatting_elements[idx];

    uint32_t step    = 4;
    dom_node_t* new_element                = NULL;
    bool run                                = true;

    while (run)
    {
        switch (step)
        {
        case 4:
            if (idx - 1 < 0)
            {
                step = 8;
            }
            else
            {
                step = 5;
            }
            break;

        case 5:
            idx--;
            entry = formatting_elements[idx];
            step = 6;
            break;

        case 6:
            if (formatting_elements_m[idx] || stack_contains_node(entry))
            {
                step = 7;
            }
            else
            {
                step = 4;
            }
            break;

        case 7:
            idx++;
            entry = formatting_elements[idx];
            step = 8;
            break;

        case 8:
            ;
            html_token_t* t = &(formatting_elements_t[idx]);
            hash_str_t t_name = hash_str_new(t->name, t->name_size);
            new_element = insert_html_element(t_name, t);
            step = 9;
            break;

        case 9:
            formatting_elements[idx] = new_element;
            step = 10;
            break;

        case 10:
            if (idx + 1 != (int32_t)formatting_elements_size)
            {
                step = 7;
            }
            else
            {
                run = false;
            }
            break;
        }
    }
}


static bool formatting_elements_contains(dom_node_t* node)
{
    for (uint32_t i = 0; i < formatting_elements_size; i++)
    {
        if (formatting_elements[i] == node)
        {
            return true;
        }
    }

    return false;
}


static dom_node_t* find_appropriate_formatting_element(const hash_str_t name)
{
    int32_t last_marker = -1;
    for (int32_t i = 0; i < (int32_t)formatting_elements_size; i++)
    {
        if (formatting_elements_m[i])
        {
            last_marker = i;
        }
    }

    int32_t start = last_marker >= 0 ? last_marker + 1 : 0;

    for (int32_t i = (int32_t)formatting_elements_size - 1; i >= start; i--)
    {
        dom_node_t* node = formatting_elements[i];

        if (node->name == name) { return node; }
    }

    return NULL;
}


static void formatting_elements_replace(dom_node_t* old, dom_node_t* new)
{
    for (uint32_t i = 0; i < formatting_elements_size; i++)
    {
        if (formatting_elements[i] == old) { formatting_elements[i] = new; }
    }
}


static void stack_replace(dom_node_t* old, dom_node_t* new)
{
    for (uint32_t i = 0; i < stack_size; i++)
    {
        if (stack[i] == old) { stack[i] = new; }
    }
}


static dom_node_t* find_furthest_block(dom_node_t* node)
{
    uint32_t node_idx = 0;
    for (uint32_t i = 0; i < stack_size; i++)
    {
        if (stack[i] == node) { node_idx = i; break; }
    }

    for (uint32_t i = node_idx + 1; i < stack_size; i++)
    {
        if (is_special(stack[i])) { return stack[i]; }
    }

    return NULL;
}

static dom_node_t* find_common_ancestor(dom_node_t* node)
{
    for (uint32_t i = stack_size - 1; i > 0; i--)
    {
        if (stack[i] == node) { return stack[i - 1]; }
    }

    NOT_IMPLEMENTED

    return NULL;
}


// https://html.spec.whatwg.org/multipage/parsing.html#adoption-agency-algorithm
static bool run_adoption_procedure(const hash_str_t t_name)
{
    // turn this bool into OPERATION_RESULT or something similar
    INCOMPLETE_IMPLEMENTATION("operation_result_t instead of bool");
    dom_node_t* current = stack[stack_idx];

    // step 2
    if (t_name == current->name && !formatting_elements_contains(current))
    {
        stack_pop();
        return true;
    }

    // step 3
    uint32_t outer_i = 0;

    // step 4
    while (true)
    {
        // step 4.1
        if (outer_i >= 8)
        {
            return true;
        }

        // step 4.2
        outer_i++;

        // step 4.3
        dom_node_t* formatting_node = find_appropriate_formatting_element(t_name);

        if (!formatting_node) { return false; }

        // step 4.4
        if (!stack_contains_node(formatting_node))
        {
            // todo: parse error
            remove_formatting_element(formatting_node);
            return true;
        }

        // step 4.5
        if (stack_contains_node(formatting_node) && !in_scope(formatting_node->name, GENERIC_SCOPE))
        {
            // todo: parse error
            return true;
        }

        // step 4.6
        if (stack[stack_idx] != formatting_node)
        {
            // todo: parse error
        }

        // step 4.7
        dom_node_t* furthest = find_furthest_block(formatting_node);

        // step 4.8
        if (!furthest)
        {
            pop_all_including(formatting_node);
            remove_formatting_element(formatting_node);
            return true;
        }

        // step 4.9
        dom_node_t* common_ancestor = find_common_ancestor(formatting_node);

        // step 4.10
        uint32_t bookmark = 0;
        for (uint32_t i = 0; i < formatting_elements_size; i++)
        {
            if (formatting_elements[i] == formatting_node) { bookmark = i; }
        }

        // step 4.11
        dom_node_t* node = furthest;
        dom_node_t* last_node = furthest;

        // step 4.12
        uint32_t inner_i = 0;
        uint32_t node_i = find_node_index(stack, stack_size, node);

        // step 4.13
        while (true)
        {
            // step 4.13.1
            inner_i++;

            // step 4.13.2
            node_i--;
            node = stack[node_i];

            // step 4.13.3
            if (node == formatting_node) { break; }

            // step 4.13.4
            if (inner_i > 3 && formatting_elements_contains(node))
            {
                // todo: decrease bookmark if node is before bookmark
                remove_formatting_element(node);
            }

            // step 4.13.5
            if (!formatting_elements_contains(node))
            {
                remove_from_stack(node);
                continue;
            }

            // step 4.13.6
            uint32_t node_formatting_index = find_node_index(formatting_elements, formatting_elements_size, node);
            html_token_t* node_t = &formatting_elements_t[node_formatting_index];
            hash_str_t node_t_name = hash_str_new(node_t->name, node_t->name_size);
            dom_node_t* new_node = create_element(node_t_name, node_t, document);
            dom_node_append(common_ancestor, new_node);
            formatting_elements_replace(node, new_node);
            stack_replace(node, new_node);
            node = new_node;

            // step 4.13.7
            if (last_node == furthest)
            {
                for (uint32_t i = 0; i < formatting_elements_size; i++)
                {
                    if (formatting_elements[i] == new_node) { bookmark = i + 1; }
                }
            }

            // step 4.13.8
            dom_node_remove(last_node->parent, last_node);
            dom_node_append(node, last_node);

            // step 4.13.9
            last_node = node;
        }

        // step 4.14
        dom_insertion_location_t location = get_appropriate_insertion_location(common_ancestor);
        dom_node_remove(last_node->parent, last_node);
        dom_node_insert_before(location.parent, last_node, location.child);

        // step 4.15
        uint32_t formatting_node_i = find_node_index(formatting_elements, formatting_elements_size, formatting_node);
        html_token_t* formatting_node_t = &formatting_elements_t[formatting_node_i];
        hash_str_t formatting_node_t_name = hash_str_new(formatting_node_t->name, formatting_node_t->name_size);
        dom_node_t* new_element = create_element(formatting_node_t_name, formatting_node_t, document);

        // step 4.16
        dom_node_t* child = furthest->first;
        while (child)
        {
            dom_node_t* next = child->next;
            dom_node_remove(furthest, child);
            dom_node_append(new_element, child);
            child = next;
        }

        // step 4.17
        dom_node_append(furthest, new_element);

        // step 4.18
        remove_formatting_element(formatting_node);
        insert_formatting_element(new_element, formatting_node_t, bookmark);

        // step 4.19
        remove_from_stack(formatting_node);
        uint32_t furthest_i = find_node_index(stack, stack_size, furthest);
        stack_insert(furthest_i + 1, new_element);
    }
}

static void handle_end_tag_in_body(const hash_str_t t_name)
{
    uint32_t idx = stack_idx;
    dom_node_t* node = stack[idx];

    while (true)
    {
        if (node->name == t_name)
        {
            generate_implied_end_tags(t_name);
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
    }
}


static void clear_stack_back_to_table_row()
{
    dom_node_t* node = stack[stack_idx];

    while (node->name != html_tag_tr() && node->name != html_tag_html() && node->name != html_tag_template())
    {
        stack_pop();
        node = stack[stack_idx];
    }
}

static void clear_stack_back_to_table()
{
    dom_node_t* node = stack[stack_idx];

    while (node->name != html_tag_table() && node->name != html_tag_html() && node->name != html_tag_template())
    {
        stack_pop();
        node = stack[stack_idx];
    }
}


static void clear_stack_back_to_table_body()
{
    dom_node_t* node = stack[stack_idx];

    while (node->name != html_tag_tfoot() && 
           node->name != html_tag_tbody() && 
           node->name != html_tag_thead() && 
           node->name != html_tag_html() && 
           node->name != html_tag_template())
    {
        stack_pop();
        node = stack[stack_idx];
    }
}


static void reset_insertion_mode_appropriately()
{
    uint32_t idx = stack_idx;
    bool last = false;
    dom_node_t* node = stack[idx];

    INCOMPLETE_IMPLEMENTATION("fragment parsing logic");

    while (!last)
    {
        const hash_str_t name = node->name;

        if (idx == 0) { last = true; }

        if ((name == html_tag_td() || name == html_tag_th()) && !last)
        {
            mode = HTML_PARSER_MODE_IN_CELL;
            return;
        }
        else if (name == html_tag_tr())
        {
            mode = HTML_PARSER_MODE_IN_ROW;
            return;
        }
        else if (name == html_tag_tbody() || name == html_tag_thead() || name == html_tag_tfoot())
        {
            mode = HTML_PARSER_MODE_IN_TABLE_BODY;
            return;
        }
        else if (name == html_tag_caption())
        {
            mode = HTML_PARSER_MODE_IN_CAPTION;
            return;
        }
        else if (name == html_tag_colgroup())
        {
            mode = HTML_PARSER_MODE_IN_COLUMN_GROUP;
            return;
        }
        else if (name == html_tag_table())
        {
            mode = HTML_PARSER_MODE_IN_TABLE;
            return;
        }
        else if (name == html_tag_template())
        {
            NOT_IMPLEMENTED
        }
        else if (name == html_tag_head() && !false)
        {
            mode = HTML_PARSER_MODE_IN_HEAD;
            return;
        }
        else if (name == html_tag_body())
        {
            mode = HTML_PARSER_MODE_IN_BODY;
            return;
        }
        else if (name == html_tag_frameset())
        {
            mode = HTML_PARSER_MODE_IN_FRAMESET;
            return;
        }
        else if (name == html_tag_html())
        {
            if (!head_pointer)
            {
                mode = HTML_PARSER_MODE_BEFORE_HEAD;
                return;
            }
            else
            {
                mode = HTML_PARSER_MODE_AFTER_HEAD;
                return;
            }
        }

        if (last)
        {
            mode = HTML_PARSER_MODE_IN_BODY;
            return;
        }

        idx--;
        node = stack[idx];
    }
}


static dom_node_t* find_first_in_stack(hash_str_t name)
{
    for (uint32_t i = 0; i < stack_size; i++)
    {
        if (stack[i]->name == name)
        {
            return stack[i];
        }
    }

    return NULL;
}


static dom_node_t* find_nearest_ancestor(dom_node_t* node, hash_str_t name)
{
    dom_node_t* parent = node->parent;

    while (parent && !dom_node_is_document(parent))
    {
        if (parent->name == name) { return parent; }

        parent = parent->parent;
    }

    return NULL;
}

static void maybe_clone_option_into_selected_content(dom_node_t* option)
{
    dom_node_t* select = find_nearest_ancestor(option, html_tag_select());

    if (!select) { return; }

}


/********************/
/* public functions */
/********************/


void html_parser_init()
{
    mode                        = HTML_PARSER_MODE_INITIAL;
    original_mode               = HTML_PARSER_MODE_INITIAL;
    replacement_mode            = HTML_PARSER_MODE_INITIAL;
    use_rules_for               = false;
    stack_idx                   = 0;
    stack_size                  = 0;
    document                    = NULL;
    stop                        = false;
    foster_parenting            = false;
    will_use_foster_parenting   = false;
    head_pointer                = NULL;

    formatting_elements_size = 0;
    memset(formatting_elements, 0, sizeof(formatting_elements));
    memset(formatting_elements_m, 0, sizeof(formatting_elements_m));
    memset(formatting_elements_t, 0, sizeof(formatting_elements_t));
}


dom_node_t* html_parser_run(const unsigned char* buffer, const uint32_t size)
{
    html_tokenizer_init(buffer, size, tokens, MAX_TOKENS);

    document                    = dom_document_new();

    dom_node_t* form_element   = NULL;
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
            bool consume                    = true;
            html_token_t t                  = tokens[i];
            hash_str_t t_name               = hash_str_new(t.name, t.name_size);
            html_parser_mode_e current_mode = mode;

            bool is_doctype                 = t.type == HTML_DOCTYPE_TOKEN;
            bool is_start                   = t.type == HTML_START_TOKEN;
            bool is_end                     = t.type == HTML_END_TOKEN;
            bool is_comment                 = t.type == HTML_COMMENT_TOKEN;
            bool is_character               = t.type == HTML_CHARACTER_TOKEN;
            bool is_eof                     = t.type == HTML_EOF_TOKEN;

            if (use_rules_for)
            {
                use_rules_for = false;
                current_mode = replacement_mode;
            }

            switch (current_mode)
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

                    // dom_node_document_t* document_data = document->document_data;

                    // if (!document_data->parser_cannot_change_mode)
                    // {
                    //     document_data->compat_mode = string_new("quirks", 6);
                    // }

                    mode = HTML_PARSER_MODE_BEFORE_HTML;
                    consume                 = false;
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
                else if (is_start && t_name == html_tag_html())
                {
                    dom_node_t* element = create_element(t_name, &t, document);
                    dom_node_append(document, element);
                    stack_push(element);

                    mode = HTML_PARSER_MODE_BEFORE_HEAD;
                }
                else if (is_end && !(t_name == html_tag_html() || t_name == html_tag_head() || t_name == html_tag_body() || t_name == html_tag_br()))
                {
                    // todo: parse error, ignore token
                }
                else
                {
                    dom_node_t* element    = create_element(html_tag_html(), NULL, document);

                    dom_node_append(document, element);
                    stack_push(element);
                    mode = HTML_PARSER_MODE_BEFORE_HEAD;

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
                    INCOMPLETE_IMPLEMENTATION("parse error");
                }
                else if (is_start && t_name == html_tag_html())
                {
                    replacement_mode    = HTML_PARSER_MODE_IN_BODY;
                    consume             = false;
                    mode                = HTML_PARSER_MODE_BEFORE_HEAD;
                    use_rules_for       = true;
                }
                else if (is_start && t_name == html_tag_head())
                {
                    mode = HTML_PARSER_MODE_IN_HEAD;
                    head_pointer        = insert_html_element(t_name, &t);
                }
                else if (is_end && !(t_name == html_tag_html() || t_name == html_tag_head() || t_name == html_tag_body() || t_name == html_tag_br()))
                {
                    INCOMPLETE_IMPLEMENTATION("parse error");
                }
                else
                {
                    mode = HTML_PARSER_MODE_IN_HEAD;
                    head_pointer        = insert_html_element(html_tag_head(), NULL);
                    consume             = false;
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inhead
            case HTML_PARSER_MODE_IN_HEAD:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    insert_character(t.data, t.data_size);
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                    INCOMPLETE_IMPLEMENTATION("parse error");
                }
                else if (is_start && t_name == html_tag_html())
                {
                    replacement_mode    = HTML_PARSER_MODE_IN_BODY;
                    consume             = false;
                    mode                = HTML_PARSER_MODE_IN_HEAD;
                    use_rules_for       = true;
                }
                else if (is_start && (t_name == html_tag_base() || t_name == html_tag_basefont() || t_name == html_tag_bgsound() || t_name == html_tag_link()))
                {
                    insert_html_element(t_name, &t);
                    stack_pop();
                    INCOMPLETE_IMPLEMENTATION("ack self closing tag");
                }
                else if (is_start && t_name == html_tag_meta())
                {
                    insert_html_element(t_name, &t);
                    stack_pop();

                    INCOMPLETE_IMPLEMENTATION("ack self closing tag");
                    INCOMPLETE_IMPLEMENTATION("speculative parsing logic");
                }
                else if (is_start && t_name == html_tag_title())
                {
                    insert_html_element(t_name, &t);
                    html_tokenizer_set_state(HTML_TOKENIZER_RCDATA_STATE);

                    original_mode       = mode;
                    mode = HTML_PARSER_MODE_TEXT;
                }
                else if ((is_start && t_name == html_tag_noscript() && scripting_enabled) || 
                         (is_start && (t_name == html_tag_noframes() || t_name == html_tag_style())))
                {
                    insert_html_element(t_name, &t);
                    html_tokenizer_set_state(HTML_TOKENIZER_RAWTEXT_STATE);

                    original_mode       = mode;
                    mode = HTML_PARSER_MODE_TEXT;
                }
                else if (is_start && t_name == html_tag_noscript() && !scripting_enabled)
                {
                    insert_html_element(t_name, &t);
                    mode = HTML_PARSER_MODE_IN_HEAD_NOSCRIPT;
                }
                else if (is_start && t_name == html_tag_script())
                {
                    dom_insertion_location_t location   = get_appropriate_insertion_location(NULL);
                    dom_node_t* element    = create_element(t_name, &t, document);

                    INCOMPLETE_IMPLEMENTATION("missing steps: 3/4/5");

                    dom_node_insert_before(location.parent, element, location.child);
                    stack_push(element);
                    html_tokenizer_set_state(HTML_TOKENIZER_SCRIPT_DATA_STATE);

                    original_mode           = mode;
                    mode = HTML_PARSER_MODE_TEXT;
                }
                else if (is_end && t_name == html_tag_head())
                {
                    stack_pop();
                    mode = HTML_PARSER_MODE_AFTER_HEAD;
                }
                else if (is_start && t_name == html_tag_template())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && t_name == html_tag_template())
                {
                    NOT_IMPLEMENTED
                }
                else if ((is_start && t_name == html_tag_head()) || 
                         (is_end && !(t_name == html_tag_body() || t_name == html_tag_html() || t_name == html_tag_br())))
                {
                    INCOMPLETE_IMPLEMENTATION("parse error");
                }
                else
                {
                    stack_pop();
                    mode = HTML_PARSER_MODE_AFTER_HEAD;
                    consume             = false; 
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inheadnoscript
            case HTML_PARSER_MODE_IN_HEAD_NOSCRIPT:
                if (is_doctype)
                {
                    // todo: parse error, ignore token
                }
                else if (is_start && t_name == html_tag_html())
                {
                    replacement_mode    = HTML_PARSER_MODE_IN_BODY;
                    consume             = false;
                    mode                = HTML_PARSER_MODE_IN_HEAD_NOSCRIPT;
                    use_rules_for       = true;
                }
                else if (is_end && t_name == html_tag_noscript())
                {
                    stack_pop();
                    mode = HTML_PARSER_MODE_IN_HEAD;
                }
                else if ((is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' ') ) ||
                         (is_comment) ||
                         (is_start && (t_name == html_tag_basefont()  ||
                                       t_name == html_tag_bgsound()   ||
                                       t_name == html_tag_link()      ||
                                       t_name == html_tag_meta()      ||
                                       t_name == html_tag_noframes()  ||
                                       t_name == html_tag_style())))
                {
                    replacement_mode    = HTML_PARSER_MODE_IN_HEAD;
                    consume             = false;
                    mode                = HTML_PARSER_MODE_IN_HEAD_NOSCRIPT;
                    use_rules_for       = true;
                }
                else if ((is_start && (t_name == html_tag_head() || t_name == html_tag_noscript())) ||
                         (is_end && !(t_name == html_tag_br())))
                {
                    // todo: parse error, ignore token
                }
                else
                {
                    INCOMPLETE_IMPLEMENTATION("parse error");
                    stack_pop();
                    mode = HTML_PARSER_MODE_IN_HEAD;
                    consume             = false; 
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#the-after-head-insertion-mode
            case HTML_PARSER_MODE_AFTER_HEAD:
                if (is_character && (t.data[0] == '\t' || t.data[0] == '\n' || t.data[0] == '\f' || t.data[0] == '\r' || t.data[0] == ' '))
                {
                    insert_character(t.data, t.data_size);
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                    // todo: parse error, ignore token
                }
                else if (is_start && t_name == html_tag_html())
                {
                    replacement_mode    = HTML_PARSER_MODE_IN_BODY;
                    consume             = false;
                    mode                = HTML_PARSER_MODE_AFTER_HEAD;
                    use_rules_for       = true;
                }
                else if (is_start && t_name == html_tag_body())
                {
                    insert_html_element(t_name, &t);
                    mode = HTML_PARSER_MODE_IN_BODY;
                    INCOMPLETE_IMPLEMENTATION("ack self-closing tag");
                }
                else if (is_start && t_name == html_tag_frameset())
                {
                    insert_html_element(t_name, &t);
                    mode = HTML_PARSER_MODE_IN_FRAMESET;
                }
                else if (is_start && (t_name == html_tag_base()       ||
                                      t_name == html_tag_basefont()   ||
                                      t_name == html_tag_bgsound()    ||
                                      t_name == html_tag_link()       ||
                                      t_name == html_tag_meta()       ||
                                      t_name == html_tag_noframes()   ||
                                      t_name == html_tag_script()     || 
                                      t_name == html_tag_template()   ||
                                      t_name == html_tag_title()      ||
                                      t_name == html_tag_style()))
                {
                    INCOMPLETE_IMPLEMENTATION("parse error");
                    assert(head_pointer);
                    stack_push(head_pointer);

                    replacement_mode    = HTML_PARSER_MODE_IN_HEAD;
                    consume             = false;
                    mode                = HTML_PARSER_MODE_AFTER_HEAD;
                    use_rules_for       = true;

                    remove_from_stack(head_pointer);
                }
                else if (is_end && t_name == html_tag_template())
                {
                    replacement_mode    = HTML_PARSER_MODE_IN_HEAD;
                    consume             = false;
                    mode                = HTML_PARSER_MODE_AFTER_HEAD;
                    use_rules_for       = true;
                }
                else if ( (is_start && t_name == html_tag_head())||
                          (is_end && !(t_name == html_tag_body() ||
                                       t_name == html_tag_html() ||
                                       t_name == html_tag_br())))
                {
                    INCOMPLETE_IMPLEMENTATION("parse error");
                }
                else
                {
                    insert_html_element(html_tag_body(), NULL);
                    mode = HTML_PARSER_MODE_IN_BODY;
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
                    insert_character(t.data, t.data_size);
                }
                else if (is_character)
                {
                    reconstruct_formatting_elements();
                    insert_character(t.data, t.data_size);
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
                else if (is_start && t_name == html_tag_html())
                {
                    // todo: parse error
                    if (stack_contains_element(html_tag_template())) { break; }
                    // todo: handle attributes logic
                }
                else if ((is_start && (t_name == html_tag_base()      ||
                                       t_name == html_tag_basefont()  ||
                                       t_name == html_tag_bgsound()   ||
                                       t_name == html_tag_link()      ||
                                       t_name == html_tag_meta()      ||
                                       t_name == html_tag_noframes()  ||
                                       t_name == html_tag_script()    ||
                                       t_name == html_tag_template()  ||
                                       t_name == html_tag_title()     ||
                                       t_name == html_tag_style()))   ||
                        (is_end && t_name == html_tag_template()))
                {
                    replacement_mode    = HTML_PARSER_MODE_IN_HEAD;
                    consume             = false;
                    mode                = HTML_PARSER_MODE_IN_BODY;
                    use_rules_for       = true;
                }
                else if (is_start && t_name == html_tag_body())
                {
                    // todo: parse error

                    // If the stack of open elements has only one node on it, 
                    // or if the second element on the stack of open elements is not a body element, 
                    // or if there is a template element on the stack of open elements, 
                    // then ignore the token. (fragment case or there is a template element on the stack)

                    if ((stack_size == 0) ||
                        (stack_size > 1 && stack[1]->name != html_tag_body()) ||
                        (stack_contains_element(html_tag_template())))
                    {
                        // ignore
                    }
                    else
                    {
                        // todo: frameset-ok 
                        // todo: handle attribute logic
                    }
                }
                else if (is_start && t_name == html_tag_frameset())
                {
                    // todo: parse error

                    if ((stack_size == 1) || (stack_size > 1 && stack[1]->name != html_tag_body()))
                    {
                        // ignore token
                    }

                    // todo: frameset-ok flag logic

                    // todo: step 1 
                    // todo: step 2
                    insert_html_element(t_name, &t);
                    mode = HTML_PARSER_MODE_IN_FRAMESET;
                }
                else if (is_eof)
                {
                    // todo: handle stack of open template insertion modes
                    if (false)
                    {
                        NOT_IMPLEMENTED
                    }
                    else
                    {
                        if (!(stack_contains_element(html_tag_dd())         ||
                              stack_contains_element(html_tag_dt())         ||
                              stack_contains_element(html_tag_li())         ||
                              stack_contains_element(html_tag_optgroup())   ||
                              stack_contains_element(html_tag_option())     ||
                              stack_contains_element(html_tag_p())          ||
                              stack_contains_element(html_tag_rb())         ||
                              stack_contains_element(html_tag_rt())         ||
                              stack_contains_element(html_tag_rtc())        ||
                              stack_contains_element(html_tag_tbody())      ||
                              stack_contains_element(html_tag_td())         ||
                              stack_contains_element(html_tag_tfoot())      ||
                              stack_contains_element(html_tag_th())         ||
                              stack_contains_element(html_tag_thead())      ||
                              stack_contains_element(html_tag_tr())         ||
                              stack_contains_element(html_tag_body())       ||
                              stack_contains_element(html_tag_html())))
                        {
                            // todo: parse error
                        }
                        stop_parsing();
                    }
                }
                else if (is_end && t_name == html_tag_body())
                {
                    // todo: handle scope logic
                    // If the stack of open elements does not have a body element in scope, this is a parse error; ignore the token.
                    if (!in_scope(html_tag_body(), GENERIC_SCOPE))
                    {
                        // todo: parse error
                        NOT_IMPLEMENTED
                    }

                    if (!(stack_contains_element(html_tag_dd())               ||
                          stack_contains_element(html_tag_dt())               ||
                          stack_contains_element(html_tag_li())               ||
                          stack_contains_element(html_tag_optgroup())   ||
                          stack_contains_element(html_tag_option())       ||
                          stack_contains_element(html_tag_p())                 ||
                          stack_contains_element(html_tag_rb())               ||
                          stack_contains_element(html_tag_rt())               ||
                          stack_contains_element(html_tag_rtc())             ||
                          stack_contains_element(html_tag_tbody())         ||
                          stack_contains_element(html_tag_td())               ||
                          stack_contains_element(html_tag_tfoot())         ||
                          stack_contains_element(html_tag_th())               ||
                          stack_contains_element(html_tag_thead())         ||
                          stack_contains_element(html_tag_tr())               ||
                          stack_contains_element(html_tag_body())           ||
                          stack_contains_element(html_tag_html())))
                    {
                        // todo: parse error
                        NOT_IMPLEMENTED
                    }

                    mode = HTML_PARSER_MODE_AFTER_BODY;
                }
                else if (is_end && t_name == html_tag_html())
                {
                    // todo: handle scope logic
                    if (!(stack_contains_element(html_tag_dd())               ||
                          stack_contains_element(html_tag_dt())               ||
                          stack_contains_element(html_tag_li())               ||
                          stack_contains_element(html_tag_optgroup())   ||
                          stack_contains_element(html_tag_option())       ||
                          stack_contains_element(html_tag_p())                 ||
                          stack_contains_element(html_tag_rb())               ||
                          stack_contains_element(html_tag_rt())               ||
                          stack_contains_element(html_tag_rtc())             ||
                          stack_contains_element(html_tag_tbody())         ||
                          stack_contains_element(html_tag_td())               ||
                          stack_contains_element(html_tag_tfoot())         ||
                          stack_contains_element(html_tag_th())               ||
                          stack_contains_element(html_tag_thead())         ||
                          stack_contains_element(html_tag_tr())               ||
                          stack_contains_element(html_tag_body())           ||
                          stack_contains_element(html_tag_html())))
                    {
                        // todo: parse error
                    }

                    mode = HTML_PARSER_MODE_AFTER_BODY;
                    consume             = false;
                }
                else if (is_start && (t_name == html_tag_address()    ||
                                      t_name == html_tag_article()    ||
                                      t_name == html_tag_aside()      ||
                                      t_name == html_tag_blockquote() ||
                                      t_name == html_tag_center()     ||
                                      t_name == html_tag_details()    ||
                                      t_name == html_tag_dialog()     ||
                                      t_name == html_tag_dir()        ||
                                      t_name == html_tag_div()        ||
                                      t_name == html_tag_dl()         ||
                                      t_name == html_tag_fieldset()   ||
                                      t_name == html_tag_figcaption() ||
                                      t_name == html_tag_figure()     ||
                                      t_name == html_tag_footer()     ||
                                      t_name == html_tag_header()     ||
                                      t_name == html_tag_hgroup()     ||
                                      t_name == html_tag_main()       ||
                                      t_name == html_tag_menu()       ||
                                      t_name == html_tag_nav()        ||
                                      t_name == html_tag_ol()         ||
                                      t_name == html_tag_p()          ||
                                      t_name == html_tag_search()     ||
                                      t_name == html_tag_section()    ||
                                      t_name == html_tag_summary()    ||
                                      t_name == html_tag_ul()))
                {
                    if (in_scope(html_tag_p(), BUTTON_SCOPE))
                    {
                        close_p_element();
                    }

                    insert_html_element(t_name, &t);
                }
                else if (is_start && (t_name == html_tag_h1() ||
                                      t_name == html_tag_h2() ||
                                      t_name == html_tag_h3() ||
                                      t_name == html_tag_h4() ||
                                      t_name == html_tag_h5() ||
                                      t_name == html_tag_h6()))
                {
                    // todo: scope logic

                    dom_node_t* node = stack[stack_idx];

                    if (node->name == html_tag_h1() ||
                        node->name == html_tag_h2() ||
                        node->name == html_tag_h3() ||
                        node->name == html_tag_h4() ||
                        node->name == html_tag_h5() ||
                        node->name == html_tag_h6())
                    {
                        // todo: parse error
                        stack_pop();
                    }
                    insert_html_element(t_name, &t);
                }
                else if (is_start && (t_name == html_tag_pre() || t_name == html_tag_listing()))
                {
                    // todo: scope logic
                    insert_html_element(t_name, &t);
                    // todo: check if next token is \n
                    // todo: frameset-ok flag
                }
                else if (is_start && t_name == html_tag_form())
                {
                    if (form_element && !stack_contains_element(html_tag_template()))
                    {
                        // todo: parse error
                        // ignore token
                    }
                    else
                    {
                        // todo: scope logic
                        dom_node_t* element = insert_html_element(t_name, &t);
                        if (!stack_contains_element(html_tag_template()))
                        {
                            form_element = element;
                        }
                    }
                }
                else if (is_start && t_name == html_tag_li())
                {
                    INCOMPLETE_IMPLEMENTATION("set frameset flag to not-ok");

                    dom_node_t* node    = stack[stack_idx];
                    uint32_t idx        = stack_idx;
                    uint32_t step       = 1;
                    bool run            = true;

                    while (run)
                    {
                        switch (step)
                        {
                        case 1:
                            if (node->name == html_tag_li())
                            {
                                generate_implied_end_tags(html_tag_li());
    
                                node = stack[stack_idx];

                                if (node->name != html_tag_li())
                                {
                                    INCOMPLETE_IMPLEMENTATION("parse error");
                                }
    
                                pop_elements_until_name_included(html_tag_li());
                                step = 4;
                            }
                            else
                            {
                                step = 2;
                            }
                            break;

                        case 2:
                            if (is_special(node) && (node->name != html_tag_address() && node->name != html_tag_div() && node->name != html_tag_p()))
                            {
                                step = 4;
                            }
                            else
                            {
                                step = 3;
                            }
                            break;

                        case 3:
                            idx--;
                            node = stack[idx];
                            step = 1;
                            break;

                        case 4:
                            if (in_scope(html_tag_p(), BUTTON_SCOPE))
                            {
                                close_p_element();
                            }
                            run = false;
                            break;
                        }
                    }

                    insert_html_element(t_name, &t);
                }
                else if (is_start && (t_name == html_tag_dt() || t_name == html_tag_dd()))
                {
                    // todo: implement
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_plaintext())
                {
                    // todo: handle scope logic - If the stack of open elements has a p element in button scope, then close a p element.
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_button())
                {
                    if (in_scope(html_tag_button(), BUTTON_SCOPE))
                    {
                        generate_implied_end_tags(0);

                        dom_node_t* node = stack[stack_idx];

                        while (node->name != html_tag_button())
                        {
                            stack_pop();
                            node = stack[stack_idx];
                        }

                        stack_pop();
                    }

                    // todo: reconstruct the active formatting elements
                    insert_html_element(t_name, &t);
                    // set frameset-ok flag to not ok
                }
                else if (is_end && (t_name == html_tag_address()      || t_name == html_tag_article()       || 
                                    t_name == html_tag_aside()        || t_name == html_tag_blockquote()    ||
                                    t_name == html_tag_button()       || t_name == html_tag_center()        ||
                                    t_name == html_tag_details()      || t_name == html_tag_dialog()        ||
                                    t_name == html_tag_dir()          || t_name == html_tag_div()           ||
                                    t_name == html_tag_dl()           || t_name == html_tag_fieldset()      ||
                                    t_name == html_tag_figcaption()   || t_name == html_tag_figure()        ||
                                    t_name == html_tag_footer()       || t_name == html_tag_header()        ||
                                    t_name == html_tag_hgroup()       || t_name == html_tag_listing()       ||
                                    t_name == html_tag_main()         || t_name == html_tag_menu()          ||
                                    t_name == html_tag_nav()          || t_name == html_tag_ol()            ||
                                    t_name == html_tag_pre()          || t_name == html_tag_search()        ||
                                    t_name == html_tag_section()      || t_name == html_tag_summary()       ||
                                    t_name == html_tag_select()       || t_name == html_tag_ul() ))
                {
                    if (!in_scope(t_name, GENERIC_SCOPE))
                    {
                        // todo: parse error
                    }
                    else
                    {
                        generate_implied_end_tags(0);

                        dom_node_t* node = stack[stack_idx];

                        if (node->name != t_name)
                        {
                            // todo: parse error
                        }

                        pop_elements_until_name_included(t_name);
                    }
                }
                else if (is_end && t_name == html_tag_form())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && t_name == html_tag_p())
                {
                    if (!in_scope(html_tag_p(), BUTTON_SCOPE))
                    {
                        // todo: parse error
                        insert_html_element(html_tag_p(), NULL);
                    }
                    
                    close_p_element();
                }
                else if (is_end && t_name == html_tag_li())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (t_name == html_tag_dd() || t_name == html_tag_dt() ))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (t_name == html_tag_h1() || t_name == html_tag_h2() || t_name == html_tag_h3() ||
                                    t_name == html_tag_h4() || t_name == html_tag_h5() || t_name == html_tag_h6()))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_a())
                {
                    dom_node_t* node = find_appropriate_formatting_element(html_tag_a());
                    if (node)
                    {
                        INCOMPLETE_IMPLEMENTATION("parse error");

                        bool success = run_adoption_procedure(t_name);

                        if (!success) { handle_end_tag_in_body(t_name); }

                        remove_from_stack(node);
                        remove_formatting_element(node);
                    }

                    reconstruct_formatting_elements();
                    node = insert_html_element(t_name, &t);
                    push_formatting_element(node, &t);
                }
                else if (is_start && (t_name == html_tag_b()      || 
                                      t_name == html_tag_big()    ||
                                      t_name == html_tag_code()   ||
                                      t_name == html_tag_em()     ||
                                      t_name == html_tag_font()   ||
                                      t_name == html_tag_i()      ||
                                      t_name == html_tag_s()      ||
                                      t_name == html_tag_small()  ||
                                      t_name == html_tag_strike() ||
                                      t_name == html_tag_strong() ||
                                      t_name == html_tag_tt()     ||
                                      t_name == html_tag_u()) )
                {
                    reconstruct_formatting_elements();
                    dom_node_t* node = insert_html_element(t_name, &t);
                    push_formatting_element(node, &t);
                }
                else if (is_start && t_name == html_tag_nobr())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (t_name == html_tag_a()        ||
                                    t_name == html_tag_b()        ||
                                    t_name == html_tag_big()      ||
                                    t_name == html_tag_code()     ||
                                    t_name == html_tag_em()       ||
                                    t_name == html_tag_font()     ||
                                    t_name == html_tag_i()        ||
                                    t_name == html_tag_nobr()     ||
                                    t_name == html_tag_s()        ||
                                    t_name == html_tag_small()    ||
                                    t_name == html_tag_strike()   ||
                                    t_name == html_tag_strong()   ||
                                    t_name == html_tag_tt()       ||
                                    t_name == html_tag_u()))
                {
                    bool success = run_adoption_procedure(t_name);

                    if (!success) { handle_end_tag_in_body(t_name); }
                }
                else if (is_start && (t_name == html_tag_applet() || t_name == html_tag_marquee() || t_name == html_tag_object()) )
                {
                    reconstruct_formatting_elements();
                    insert_html_element(t_name, &t);
                    insert_marker();
                    // todo: set frameset-ok flag to not ok
                }
                else if (is_end && (t_name == html_tag_applet() || t_name == html_tag_marquee() || t_name == html_tag_object()) )
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_table())
                {
                    INCOMPLETE_IMPLEMENTATION("quirk modes additional logic");

                    insert_html_element(t_name, &t);

                    INCOMPLETE_IMPLEMENTATION("Set the frameset-ok flag to not-ok.");

                    mode = HTML_PARSER_MODE_IN_TABLE;
                }
                else if (is_end && t_name == html_tag_br())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (t_name == html_tag_area() || t_name == html_tag_br() || t_name == html_tag_embed() ||
                                      t_name == html_tag_img() || t_name == html_tag_keygen() || t_name == html_tag_wbr()))
                {
                    // todo: reconstruct the active formatting elements

                    insert_html_element(t_name, &t);
                    stack_pop();

                    // todo: acknowledge self closing tag
                    // todo: set frameset-ok flag to not ok
                }
                else if (is_start && t_name == html_tag_input())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (t_name == html_tag_param() || t_name == html_tag_source() || t_name == html_tag_track() ))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_hr())
                {
                    if (in_scope(html_tag_p(), BUTTON_SCOPE))
                    {
                        close_p_element();
                    }
                    if (in_scope(html_tag_select(), GENERIC_SCOPE))
                    {
                        generate_implied_end_tags(0);
                        if (in_scope(html_tag_option(), GENERIC_SCOPE) || in_scope(html_tag_optgroup(), GENERIC_SCOPE))
                        {
                            // todo: parse error
                        }
                    }
                    insert_html_element(t_name, &t);
                    stack_pop();
                    // todo: ack self closing tag if set
                    // todo: set frameset-ok to "not ok"
                }
                else if (is_start && t_name == html_tag_image())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_textarea())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_xmp())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_iframe() )
                {
                    NOT_IMPLEMENTED
                }
                else if ( (is_start && t_name == html_tag_noembed() ) ||
                          (is_start && t_name == html_tag_noscript() && scripting_enabled) )
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_select() )
                {
                    // todo: fragment parsing logic
                    if (in_scope(html_tag_select(), GENERIC_SCOPE))
                    {
                        // todo: parse error
                        pop_elements_until_name_included(html_tag_select());
                    }
                    else
                    {
                        reconstruct_formatting_elements();
                        insert_html_element(t_name, &t);
                        // todo: set frameset-ok to "not ok"
                    }
                }
                else if (is_start && t_name == html_tag_option())
                {
                    if (in_scope(html_tag_select(), GENERIC_SCOPE))
                    {
                        generate_implied_end_tags(html_tag_optgroup());
                        if (in_scope(html_tag_option(), GENERIC_SCOPE))
                        {
                            INCOMPLETE_IMPLEMENTATION("parse error");
                        }
                    }
                    else
                    {
                        dom_node_t* current = stack[stack_idx];
                        if (current->name == html_tag_option())
                        {
                            stack_pop();
                        }
                    }

                    reconstruct_formatting_elements();
                    insert_html_element(t_name, &t);
                }
                else if (is_start && t_name == html_tag_optgroup())
                {
                    if (in_scope(html_tag_select(), GENERIC_SCOPE))
                    {
                        generate_implied_end_tags(0);
                        if (in_scope(html_tag_option(), GENERIC_SCOPE) || in_scope(html_tag_optgroup(), GENERIC_SCOPE))
                        {
                            INCOMPLETE_IMPLEMENTATION("parse error");
                        }
                    }
                    else
                    {
                        dom_node_t* current = stack[stack_idx];
                        if (current->name == html_tag_option()) { stack_pop(); }
                    }

                    reconstruct_formatting_elements();
                    insert_html_element(t_name, &t);
                }
                else if (is_end && t_name == html_tag_option())
                {
                    dom_node_t* option = find_first_in_stack(html_tag_option());

                    handle_end_tag_in_body(t_name);

                    if (option && !stack_contains_node(option))
                    {
                        maybe_clone_option_into_selected_content(option);
                    }
                }
                else if (is_start && (t_name == html_tag_rb() || t_name == html_tag_rtc()))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (t_name == html_tag_rp() || t_name == html_tag_rt()))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_math())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_svg())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (t_name == html_tag_caption()    ||
                                      t_name == html_tag_col()        ||
                                      t_name == html_tag_colgroup()   || 
                                      t_name == html_tag_frame()      ||
                                      t_name == html_tag_head()       ||
                                      t_name == html_tag_tbody()      ||
                                      t_name == html_tag_td()         ||
                                      t_name == html_tag_tfoot()      ||
                                      t_name == html_tag_th()         ||
                                      t_name == html_tag_thead()      ||
                                      t_name == html_tag_tr() ))
                {
                        NOT_IMPLEMENTED
                }
                else if (is_start)
                {
                    // todo: Reconstruct the active formatting elements, if any.
                    insert_html_element(t_name, &t);
                }
                else if (is_end)
                {
                    handle_end_tag_in_body(t_name);
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incdata
            case HTML_PARSER_MODE_TEXT:
                if (is_character)
                {
                    insert_character(t.data, t.data_size);
                }
                else if (is_eof)
                {
                    INCOMPLETE_IMPLEMENTATION("parse error");
                    dom_node_t* current = stack[stack_idx];
                    if (current->name == html_tag_script())
                    {
                        INCOMPLETE_IMPLEMENTATION("set already_started to true");
                    }

                    stack_pop();
                    consume = false;
                    mode = original_mode;
                }
                else if (is_end && t_name == html_tag_script())
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
                dom_node_t* current_node = stack[stack_idx];
                const hash_str_t name = current_node->name;

                if (is_character && (name == html_tag_table() || name == html_tag_tbody() ||
                                     name == html_tag_template() || name == html_tag_tfoot() ||
                                     name == html_tag_thead() || name == html_tag_tr()))
                {
                    memset(pending_tokens, 0, MAX_TOKENS * sizeof(html_token_t));
                    pending_tokens_size = 0;

                    original_mode       = mode;
                    mode                = HTML_PARSER_MODE_IN_TABLE_TEXT;
                    consume             = false;
                }
                else if (is_comment)
                {
                    insert_comment(&t, NULL);
                }
                else if (is_doctype)
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_caption())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_colgroup())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_col())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (t_name == html_tag_tbody() || t_name == html_tag_tfoot() || t_name == html_tag_thead()))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (t_name == html_tag_td() || t_name == html_tag_th() || t_name == html_tag_tr()))
                {
                    clear_stack_back_to_table();
                    insert_html_element(html_tag_tbody(), NULL);
                    mode = HTML_PARSER_MODE_IN_TABLE_BODY;

                    consume             = false;
                }
                else if (is_start && t_name == html_tag_table())
                {
                        NOT_IMPLEMENTED
                }
                else if (is_end && t_name == html_tag_table())
                {
                    if (!in_scope(html_tag_table(), TABLE_SCOPE))
                    {
                        INCOMPLETE_IMPLEMENTATION("parse error, ignore token");
                    }
                    else
                    {
                        pop_elements_until_name_included(html_tag_table());
                        reset_insertion_mode_appropriately();
                    }
                }
                else if (is_end && (t_name == html_tag_body()     ||
                                    t_name == html_tag_caption()  ||
                                    t_name == html_tag_col()      ||
                                    t_name == html_tag_colgroup() ||
                                    t_name == html_tag_html()     ||
                                    t_name == html_tag_tbody()    ||
                                    t_name == html_tag_td()       ||
                                    t_name == html_tag_tfoot()    ||
                                    t_name == html_tag_th()       ||
                                    t_name == html_tag_thead()    ||
                                    t_name == html_tag_tr()))
                {
                    NOT_IMPLEMENTED
                }
                else if ((is_start && (t_name == html_tag_style() || t_name == html_tag_script() || t_name == html_tag_template())) ||
                         (is_end && t_name == html_tag_template()))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_input())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_form())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_eof)
                {
                    replacement_mode    = HTML_PARSER_MODE_IN_BODY;
                    mode                = HTML_PARSER_MODE_IN_TABLE;
                    use_rules_for       = true;
                }
                else
                {
                    INCOMPLETE_IMPLEMENTATION("parse error");

                    replacement_mode            = HTML_PARSER_MODE_IN_BODY;
                    will_use_foster_parenting   = true;
                    mode                        = HTML_PARSER_MODE_IN_TABLE;
                    use_rules_for               = true;
                    consume                     = false;
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
                    if (pending_tokens_size == MAX_TOKENS) { break; }

                    pending_tokens[pending_tokens_size] = t;
                    pending_tokens_size++;
                }
                else
                {

                    INCOMPLETE_IMPLEMENTATION("handle whitespace ascii chars");

                    // NOTE: this is not ideal, but should be fine for now. This section is copying w/e is in the IN BODY section.

                    for (uint32_t j = 0; j < pending_tokens_size; j++)
                    {
                        html_token_t token = pending_tokens[j];
                        foster_parenting = true;
                        reconstruct_formatting_elements();
                        insert_character(token.data, token.data_size);
                        // todo: frameset-ok flag
                        foster_parenting = false;
                    }

                    pending_tokens_size = 0;
                    mode = original_mode;
                    consume = false;
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incaption
            case HTML_PARSER_MODE_IN_CAPTION:
                if (is_end && t_name == html_tag_caption())
                {
                    NOT_IMPLEMENTED
                }
                else if ((is_start && (t_name == html_tag_caption()   ||
                                       t_name == html_tag_col()       ||
                                       t_name == html_tag_colgroup()  || 
                                       t_name == html_tag_tbody()     ||
                                       t_name == html_tag_td()        ||
                                       t_name == html_tag_tfoot()     ||
                                       t_name == html_tag_th()        ||
                                       t_name == html_tag_thead()     ||
                                       t_name == html_tag_tr()))      ||
                         (is_end && t_name == html_tag_table()))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (t_name == html_tag_body()     ||
                                    t_name == html_tag_col()      ||
                                    t_name == html_tag_colgroup() ||
                                    t_name == html_tag_html()     ||
                                    t_name == html_tag_tbody()    ||
                                    t_name == html_tag_td()       ||
                                    t_name == html_tag_tfoot()    ||
                                    t_name == html_tag_th()       ||
                                    t_name == html_tag_thead()    ||
                                    t_name == html_tag_tr()))
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
                else if (is_start && t_name == html_tag_html())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_col())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && t_name == html_tag_colgroup())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && t_name == html_tag_col())
                {
                    NOT_IMPLEMENTED
                }
                else if ((is_start || is_end) && (t_name == html_tag_template()))
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
                if (is_start && t_name == html_tag_tr())
                {
                    clear_stack_back_to_table_body();
                    insert_html_element(t_name, &t);
                    mode                = HTML_PARSER_MODE_IN_ROW;
                }
                else if (is_start && (t_name == html_tag_th() || t_name == html_tag_td()))
                {
                    INCOMPLETE_IMPLEMENTATION("parse error");
                    clear_stack_back_to_table_body();

                    insert_html_element(html_tag_tr(), NULL);
                    mode = HTML_PARSER_MODE_IN_ROW;

                    consume             = false;
                }
                else if (is_end && (t_name == html_tag_tbody() || t_name == html_tag_tfoot() || t_name == html_tag_thead() ))
                {
                    NOT_IMPLEMENTED
                }
                else if ((is_start && (t_name == html_tag_caption()   ||
                                       t_name == html_tag_col()       ||
                                       t_name == html_tag_colgroup()  ||
                                       t_name == html_tag_tbody()     ||
                                       t_name == html_tag_tfoot()     ||
                                       t_name == html_tag_thead()))   ||
                         (is_end && t_name == html_tag_table()))
                {
                    if (!in_scope(html_tag_tbody(), TABLE_SCOPE) &&
                        !in_scope(html_tag_thead(), TABLE_SCOPE) &&
                        !in_scope(html_tag_tfoot(), TABLE_SCOPE))
                    {
                        INCOMPLETE_IMPLEMENTATION("parse error");
                    }
                    else
                    {
                        clear_stack_back_to_table_body();
                        stack_pop();
                        mode = HTML_PARSER_MODE_IN_TABLE;
                        consume         = false;
                    }
                }
                else if (is_end && (t_name == html_tag_body()     ||
                                    t_name == html_tag_caption()  ||
                                    t_name == html_tag_col()      ||
                                    t_name == html_tag_colgroup() ||
                                    t_name == html_tag_html()     ||
                                    t_name == html_tag_td()       ||
                                    t_name == html_tag_th()       ||
                                    t_name == html_tag_tr()))
                {
                    NOT_IMPLEMENTED
                }
                else
                {
                    replacement_mode    = HTML_PARSER_MODE_IN_TABLE;
                    consume             = false;
                    mode                = HTML_PARSER_MODE_IN_TABLE_BODY;
                    use_rules_for       = true;
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intr
            case HTML_PARSER_MODE_IN_ROW:
                if (is_start && (t_name == html_tag_th() || t_name == html_tag_td()))
                {
                    clear_stack_back_to_table_row();
                    insert_html_element(t_name, &t);
                    insert_marker();
                    mode = HTML_PARSER_MODE_IN_CELL;
                }
                else if (is_end && t_name == html_tag_tr())
                {
                    if (!in_scope(html_tag_tr(), TABLE_SCOPE))
                    {
                        // todo: parse error
                        // ignore token
                    }
                    else
                    {
                        clear_stack_back_to_table_row();
                        stack_pop();
                        mode = HTML_PARSER_MODE_IN_TABLE_BODY;
                    }
                }
                else if ((is_start && (t_name == html_tag_caption()   ||
                                       t_name == html_tag_col()       ||
                                       t_name == html_tag_colgroup()  ||
                                       t_name == html_tag_tbody()     ||
                                       t_name == html_tag_tfoot()     ||
                                       t_name == html_tag_thead()     ||
                                       t_name == html_tag_tr()))      ||
                         (is_end && t_name == html_tag_table()))
                {
                    if (!in_scope(html_tag_tr(), TABLE_SCOPE))
                    {
                        INCOMPLETE_IMPLEMENTATION("parse error");
                    }
                    else
                    {
                        clear_stack_back_to_table_row();
                        stack_pop();
                        mode = HTML_PARSER_MODE_IN_TABLE_BODY;
                        consume             = false;
                    }
                }
                else if (is_end && (t_name == html_tag_tbody() || t_name == html_tag_thead() || t_name == html_tag_tfoot()))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (t_name == html_tag_body()     ||
                                    t_name == html_tag_caption()  ||
                                    t_name == html_tag_col()      ||
                                    t_name == html_tag_colgroup() ||
                                    t_name == html_tag_html()     ||
                                    t_name == html_tag_td()       ||
                                    t_name == html_tag_th()))
                {
                    NOT_IMPLEMENTED
                }
                else
                {
                    consume             = false;
                    replacement_mode    = HTML_PARSER_MODE_IN_TABLE;
                    mode                = HTML_PARSER_MODE_IN_ROW;
                    use_rules_for       = true;
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intd
            case HTML_PARSER_MODE_IN_CELL:
                if (is_end && (t_name == html_tag_td() || t_name == html_tag_th()))
                {
                    if (!in_scope(t_name, TABLE_SCOPE))
                    {
                        INCOMPLETE_IMPLEMENTATION("parse error");
                    }
                    else
                    {
                        generate_implied_end_tags(0);

                        dom_node_t* node = stack[stack_idx];

                        if (node->name != t_name)
                        {
                            INCOMPLETE_IMPLEMENTATION("parse error");
                        }

                        pop_elements_until_name_included(t_name);
                        clear_formatting_elements();
                        mode            = HTML_PARSER_MODE_IN_ROW;
                    }
                }
                else if (is_start && (t_name == html_tag_caption()    ||
                                      t_name == html_tag_col()        ||
                                      t_name == html_tag_colgroup()   ||
                                      t_name == html_tag_tbody()      ||
                                      t_name == html_tag_tfoot()      ||
                                      t_name == html_tag_thead()      ||
                                      t_name == html_tag_tr()         ||
                                      t_name == html_tag_th()         ||
                                      t_name == html_tag_td()))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (t_name == html_tag_body()     ||
                                    t_name == html_tag_caption()  ||
                                    t_name == html_tag_col()      ||
                                    t_name == html_tag_colgroup() ||
                                    t_name == html_tag_html()))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && (t_name == html_tag_table() || t_name == html_tag_tfoot() || t_name == html_tag_thead() ||
                                    t_name == html_tag_tbody() || t_name == html_tag_tr()))
                {
                    if (!in_scope(t_name, TABLE_SCOPE))
                    {
                        INCOMPLETE_IMPLEMENTATION("parse error");
                    }
                    else
                    {
                        close_cell();
                        consume                 = false;
                    }
                }
                else
                {
                    replacement_mode    = HTML_PARSER_MODE_IN_BODY;
                    consume             = false;
                    mode                = HTML_PARSER_MODE_IN_CELL;
                    use_rules_for       = true;
                }
                break;

            // https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intemplate
            case HTML_PARSER_MODE_IN_TEMPLATE:
                if (is_character || is_comment || is_doctype)
                {
                    NOT_IMPLEMENTED
                }
                else if ((is_start && (t_name == html_tag_base()      ||
                                       t_name == html_tag_basefont()  ||
                                       t_name == html_tag_bgsound()   ||
                                       t_name == html_tag_link()      ||
                                       t_name == html_tag_meta()      ||
                                       t_name == html_tag_noframes()  ||
                                       t_name == html_tag_script()    ||
                                       t_name == html_tag_style()     ||
                                       t_name == html_tag_template()  ||
                                       t_name == html_tag_title()))   ||
                         (is_end && t_name == html_tag_template()))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (t_name == html_tag_caption()    ||
                                      t_name == html_tag_colgroup()   ||
                                      t_name == html_tag_tbody()      ||
                                      t_name == html_tag_tfoot()      ||
                                      t_name == html_tag_thead()))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_col())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_tr())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && (t_name == html_tag_td() || t_name == html_tag_th()))
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
                else if (is_start && t_name == html_tag_html())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && t_name == html_tag_html())
                {
                    // todo: if parser was created using fragment parsing algorithm -> error and ignore token

                    mode = HTML_PARSER_MODE_AFTER_AFTER_BODY;
                }
                else if (is_eof)
                {
                    stop_parsing();
                }
                else
                {
                    // todo: parse error
                    mode = HTML_PARSER_MODE_IN_BODY;
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
                else if (is_start && t_name == html_tag_html())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_frameset())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && t_name == html_tag_frameset())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_frame())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_noframes())
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
                else if (is_start && t_name == html_tag_html())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_end && t_name == html_tag_html())
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_noframes())
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
                         (is_start && t_name == html_tag_html()))
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
                         (is_start && t_name == html_tag_html()))
                {
                    NOT_IMPLEMENTED
                }
                else if (is_eof)
                {
                    NOT_IMPLEMENTED
                }
                else if (is_start && t_name == html_tag_noframes())
                {
                    NOT_IMPLEMENTED
                }
                else
                {
                    NOT_IMPLEMENTED
                }
                break;
            }

            if (foster_parenting)
            {
                foster_parenting = false;
            }

            if (will_use_foster_parenting)
            {
                will_use_foster_parenting = false;
                foster_parenting = true;
            }

            if (consume)
            {
                i++;
            }
        }
    }

    html_tokenizer_free();

    return document;
}

void html_parser_free()
{
    
}