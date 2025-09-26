#include "runner.h"

#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "test_utils.h"
#include "html/test_parser_utils.h"

#include "dom/node.h"
#include "dom/document.h"
#include "dom/element.h"
#include "dom/comment.h"
#include "dom/attribute.h"
#include "dom/text.h"
#include "util/utf8.h"

typedef enum
{
    STATE_DATA,
    STATE_ERRORS,
    STATE_DOCUMENT,
    STATE_SCRIPT
} html5lib_parse_state_e;

static unsigned char line[2048] = { 0 };
static uint32_t line_size = 0;
static uint32_t is_eof = false;
static FILE* file = NULL;
static uint32_t line_num = 0;
static uint32_t test_line = 0;
static uint32_t level = 0;

// test data
static unsigned char buffer[2048] = { 0 };
static uint32_t buffer_size = 0;
static dom_node_t* document = NULL;

static void read_line()
{
    memset(line, 0, 2048);
    line_size = 0;
    int32_t c = 0;

    while (c != '\n')
    {
        c = fgetc(file);
        if (c == EOF)
        {
            is_eof = true;
            return;
        }

        line[line_size++] = (unsigned char)c;
    }

    line[--line_size] = '\0';
    line_num++;
}


static uint32_t find_level()
{
    for (uint32_t i = 0; i < line_size; i++)
    {
        if (line[i] == '|' || line[i] == ' ') { continue; }

        assert(i % 2 == 0);

        return i / 2;
    }

    return 0;
}


static bool line_is_comment()
{
    uint32_t i = level * 2;
    return line[i] == '<' && line[i + 1] == '!' && line[i + 2] == '-' && line[i + 3] == '-';
}


static bool line_is_text()
{
    return line[level * 2] == '"';
}


static bool line_is_element()
{
    uint32_t i = level * 2;
    return line[i] == '<' && utf8_is_alpha(line[i + 1]);
}


static bool line_is_attr()
{
    uint32_t i = level * 2;
    return utf8_is_alpha(line[i]);
}


static bool line_is_doctype()
{
    uint32_t i = level * 2;
    return line[i] == '<' && line[i + 1] == '!' && line[i + 2] == 'D';
}


static dom_node_t* parse_comment()
{
    uint32_t start = level * 2 + 5;
    uint32_t size = line_size - 4 - start;
    return dom_comment_new(document, &line[start], size);
}


static dom_node_t* parse_element()
{
    uint32_t start = level * 2 + 1;
    uint32_t size = line_size - 1 - start;
    hash_str_t name = hash_str_new(&line[start], size);
    return dom_element_new(document, name);
}


static dom_node_t* parse_text()
{
    uint32_t start = level * 2 + 1;
    uint32_t size = line_size - 1 - start;
    return dom_text_new(document, &line[start], size);
}


static dom_node_t* parse_attr(dom_node_t* element)
{
    uint32_t name = 0;
    uint32_t name_start = level * 2;
    uint32_t name_size = line_size;
    uint32_t name_end = name_start;
    for (uint32_t i = name_start; i < line_size; i++)
    {
        if (line[i] == '=')
        {
            name_end = i;
            break;
        }
    }
    name_size = name_end - name_start;
    name_size = name_size > 64 ? 64 : name_size;
    name = hash_str_new(&line[name_start], name_size);

    uint32_t val = 0;
    uint32_t val_start = name_end + 2;
    uint32_t val_end = line_size - 1;
    uint32_t val_size = val_end - val_start;
    val_size = val_size > 64 ? 64 : val_size;

    if (val_size > 0)
    {
        val = hash_str_new(&line[val_start], val_size);
    }

    return dom_attr_new(name, val, element);
}


static dom_node_t* parse_doctype()
{
    uint32_t start = level * 2 + 10;
    uint32_t size = line_size - 1 - start;
    return dom_doctype_new(document, &line[start], size, NULL, 0, NULL, 0);
}


static void run_test()
{
    memset(buffer, 0, 2048);
    buffer_size = 0;

    // #data
    read_line();
    assert(strncmp("#data", line, 5) == 0);
    test_line = line_num;

    // #test-data
    read_line();
    memcpy(buffer, line, line_size);
    buffer_size = line_size;

    // next header
    read_line();
    html5lib_parse_state_e state    = STATE_DATA;
    document                        = NULL;
    dom_node_t* last                = NULL;
    uint32_t last_level             = 0;

    while (line_size > 0)
    {
        if (strncmp(line, "#errors", 7) == 0 || strncmp(line, "#new-errors", 7) == 0)
        {
            state       = STATE_ERRORS;
            read_line();
        }

        if (strncmp(line, "#document", 9) == 0)
        {
            state       = STATE_DOCUMENT;
            document    = dom_document_new();
            last        = document;

            read_line();
        }

        if (state == STATE_ERRORS)
        {
            // pass - not sure yet what to do with these errors
        }
        else if (state == STATE_DOCUMENT)
        {
            level = find_level();
            dom_node_t* node = NULL;

            if (line_is_comment())      { node = parse_comment(); }
            else if (line_is_element()) { node = parse_element(); }
            else if (line_is_text())    { node = parse_text(); }
            else if (line_is_attr())    { node = parse_attr(last); }
            else if (line_is_doctype()) { node = parse_doctype(); }
            else                        { printf("%s\n", line); assert(false); }

            assert(node);

            if (dom_node_is_attr(node))
            {
                dom_element_append_attr(dom_element_from_node(last), dom_attr_from_node(node));
            }
            else if (dom_node_is_doctype(node))
            {
                dom_document_set_doctype(dom_document_from_node(document), dom_doctype_from_node(node));
            }
            else
            {
                if (last_level < level)
                {
                    dom_node_append(last, node);
                }
                else
                {
                    uint32_t parent_level = last_level;
                    dom_node_t* parent = last;
                    while (parent_level >= level)
                    {
                        parent_level--;
                        parent = parent->parent;
                    }

                    dom_node_append(parent, node);
                }

                if (dom_node_is_element(node))
                {
                    last = node;
                    last_level = level;
                }
            }
        }
        else
        {
            printf("unhandled state\n");
            assert(false);
        }

        read_line();
    }

    html_parser_init();
    dom_node_t* actual = html_parser_run(buffer, buffer_size);
    ASSERT_NODE(actual, document);

    if (!TEST_SUCCEEDED())
    {
        printf("\n========== Test %u ==========\n", test_line);
        printf("%s\n", buffer);
        printf("\n-- Actual Tree --\n");
        print_document_tree(document, 0);
        printf("\n-- Expected Tree --\n");
        print_document_tree(actual, 0);
    }
    
    dom_node_free(document);
    dom_node_free(actual);
    html_parser_free();
}


void html_parser_test()
{
    is_eof = false;
    // const unsigned char* files[] = { "./test/html/parser/data/debug.data" };
    const unsigned char* files[] = { "./test/html/parser/data/tests1.data" };
    uint32_t len = sizeof(files) / sizeof(char*);

    for (uint32_t i = 0; i < len; i++)
    {
        file = fopen(files[i], "r");
        if (!file)
        {
            printf("Cannot open file %s\n", files[i]);
            return;
        }

        while (!is_eof) { TEST_CASE(run_test) }
    }
}