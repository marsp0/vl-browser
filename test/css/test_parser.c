#include "test_parser.h"
#include "test_utils.h"
#include "test_parser_utils.h"

#include <assert.h>

#include "css/parser.h"
#include "css/parser_types.h"
#include "css/tokenizer_types.h"


static css_parser_node_t* new_data_token_node(hash_str_t data, css_token_type_e type)
{
    css_parser_node_t* n    = css_parser_node_new(data, CSS_PARSER_NODE_TYPE_TOKEN);
    css_token_t* t          = css_token_new();
    t->type                 = type;
    n->token                = t;
    if (data > 0)
    {
        memcpy(t->data, hash_str_get(data), hash_str_get_size(data));    
        t->data_size = hash_str_get_size(data);
    }
    return n;
}


static css_parser_node_t* new_ident_node(hash_str_t name)
{
    return new_data_token_node(name, CSS_TOKEN_IDENT);
}


static css_parser_node_t* new_number_node(float real)
{
    css_parser_node_t* n    = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_TOKEN);
    css_token_t* t          = css_token_new();
    t->type                 = CSS_TOKEN_NUMBER;
    t->integer              = (int32_t)real;
    t->real                 = real;
    n->token                = t;

    return n;
}


static css_parser_node_t* new_token_node(css_token_type_e type)
{
    css_parser_node_t* n    = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_TOKEN);
    css_token_t* t          = css_token_new();
    t->type                 = type;
    n->token                = t;

    return n;
}


static css_parser_node_t* new_hash_node(hash_str_t data, css_token_hash_type_e hash_type)
{
    css_parser_node_t* n = new_data_token_node(data, CSS_TOKEN_HASH);
    n->token->hash_type = hash_type;

    return n;
}


static void test_parser_1()
{
    /*
    {
        css: '@media{ }',
        expected: {
          "type": "STYLESHEET",
          "rules": [
            {
              "type": "AT-RULE",
              "name": "media",
              "prelude": [],
              "declarations": [],
              "rules": []
            }
          ]
        }
      },
    */
    unsigned char data[] = " @media{ }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* actual = css_parser_parse_stylesheet();
    css_parser_free();

    css_parser_node_t* expected = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);
    css_parser_node_t* rule     = css_parser_node_new(hash_str_new("media", 5), CSS_PARSER_NODE_TYPE_AT_RULE);

    css_parser_node_add_rule(expected, rule);

    ASSERT_CSS_PARSER_NODE(actual, expected);
}


static void test_parser_2()
{
    /*
    {
        css: '@media{}',
        expected: {
          "type": "STYLESHEET",
          "rules": [
            {
              "type": "AT-RULE",
              "name": "media",
              "prelude": [],
              "declarations": [],
              "rules": []
            }
          ]
        }
      },
    */
    unsigned char data[] = "@media{}";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* actual = css_parser_parse_stylesheet();
    css_parser_free();

    css_parser_node_t* expected = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);
    css_parser_node_t* rule     = css_parser_node_new(hash_str_new("media", 5), CSS_PARSER_NODE_TYPE_AT_RULE);

    css_parser_node_add_rule(expected, rule);

    ASSERT_CSS_PARSER_NODE(actual, expected);
}


static void test_parser_3()
{
    /*
    source: `@test{p:v}`
    {
      "type": "STYLESHEET",
      "rules": [
        {
          "type": "AT-RULE",
          "name": "test",
          "prelude": [],
          "declarations": [
            {
              "type": "DECLARATION",
              "name": "p",
              "value": [
                {
                  "type": "IDENT",
                  "value": "v"
                }
              ],
              "important": false
            }
          ],
          "rules": []
        }
      ]
    }
    */
    unsigned char data[] = "@test{p:v}";
    uint32_t data_size = sizeof(data) - 1;

    // actual
    css_parser_init(data, data_size);
    css_parser_node_t* actual = css_parser_parse_stylesheet();
    css_parser_free();

    // expected
    hash_str_t test_str             = hash_str_new("test", 4);
    hash_str_t p_str                = hash_str_new("p", 1);
    hash_str_t v_str                = hash_str_new("v", 1);
    css_parser_node_t* expected     = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);
    css_parser_node_t* at_rule      = css_parser_node_new(test_str, CSS_PARSER_NODE_TYPE_AT_RULE);
    css_parser_node_t* p_node       = css_parser_node_new(p_str, CSS_PARSER_NODE_TYPE_DECL);
    css_parser_node_t* v_node       = new_ident_node(v_str);

    css_parser_node_add_rule(expected, at_rule);
    css_parser_node_add_decl(at_rule, p_node);
    css_parser_node_add_comp_val(p_node, v_node);

    ASSERT_CSS_PARSER_NODE(actual, expected);
}


static void test_parser_4()
{
    /*
    surce: `@test x, y x(1+2) {p:v}`
    {
      "type": "STYLESHEET",
      "rules": [
        {
          "type": "AT-RULE",
          "name": "test",
          "prelude": [
            {
              "type": "WHITESPACE"
            },
            {
              "type": "IDENT",
              "value": "x"
            },
            {
              "type": "COMMA"
            },
            {
              "type": "WHITESPACE"
            },
            {
              "type": "IDENT",
              "value": "y"
            },
            {
              "type": "WHITESPACE"
            },
            {
              "type": "FUNCTION",
              "name": "x",
              "value": [
                {
                  "type": "NUMBER",
                  "value": 1,
                  "isInteger": true
                },
                {
                  "type": "NUMBER",
                  "value": 2,
                  "isInteger": true,
                  "sign": "+"
                }
              ]
            },
            {
              "type": "WHITESPACE"
            }
          ],
          "declarations": [
            {
              "type": "DECLARATION",
              "name": "p",
              "value": [
                {
                  "type": "IDENT",
                  "value": "v"
                }
              ],
              "important": false
            }
          ],
          "rules": []
        }
      ]
    }
    */

    unsigned char data[] = "@test x, y x(1+2) {p:v}";
    uint32_t data_size = sizeof(data) - 1;

    // actual
    css_parser_init(data, data_size);
    css_parser_node_t* actual = css_parser_parse_stylesheet();
    css_parser_free();

    // // expected
    hash_str_t test_str             = hash_str_new("test", 4);
    hash_str_t x_str                = hash_str_new("x", 1);
    hash_str_t y_str                = hash_str_new("y", 1);
    hash_str_t p_str                = hash_str_new("p", 1);
    hash_str_t v_str                = hash_str_new("v", 1);
    css_parser_node_t* expected     = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);
    css_parser_node_t* at_rule      = css_parser_node_new(test_str, CSS_PARSER_NODE_TYPE_AT_RULE);
    css_parser_node_t* white1       = new_token_node(CSS_TOKEN_WHITESPACE);
    css_parser_node_t* x_node       = new_ident_node(x_str);
    css_parser_node_t* comma        = new_token_node(CSS_TOKEN_COMMA);
    css_parser_node_t* white2       = new_token_node(CSS_TOKEN_WHITESPACE);
    css_parser_node_t* y_node       = new_ident_node(y_str);
    css_parser_node_t* white3       = new_token_node(CSS_TOKEN_WHITESPACE);
    css_parser_node_t* func         = css_parser_node_new(x_str, CSS_PARSER_NODE_TYPE_FUNCTION);
    css_parser_node_t* white4       = new_token_node(CSS_TOKEN_WHITESPACE);
    css_parser_node_t* one          = new_number_node(1);
    css_parser_node_t* two          = new_number_node(2);
    css_parser_node_t* p_node       = css_parser_node_new(p_str, CSS_PARSER_NODE_TYPE_DECL);
    css_parser_node_t* v_node       = new_ident_node(v_str);

    css_parser_node_add_rule(expected, at_rule);
    css_parser_node_add_comp_val(at_rule, white1);
    css_parser_node_add_comp_val(at_rule, x_node);
    css_parser_node_add_comp_val(at_rule, comma);
    css_parser_node_add_comp_val(at_rule, white2);
    css_parser_node_add_comp_val(at_rule, y_node);
    css_parser_node_add_comp_val(at_rule, white3);
    css_parser_node_add_comp_val(at_rule, func);
    css_parser_node_add_comp_val(func, one);
    css_parser_node_add_comp_val(func, two);
    css_parser_node_add_comp_val(at_rule, white4);
    css_parser_node_add_decl(at_rule, p_node);
    css_parser_node_add_comp_val(p_node, v_node);

    ASSERT_CSS_PARSER_NODE(actual, expected);
}


static void test_parser_5()
{
    /*
    {
      "type": "STYLESHEET",
      "rules": [
        {
          "type": "QUALIFIED-RULE",
          "prelude": [
            {
              "type": "IDENT",
              "value": "foo"
            },
            {
              "type": "WHITESPACE"
            }
          ],
          "declarations": [
            {
              "type": "DECLARATION",
              "name": "bar",
              "value": [
                {
                  "type": "IDENT",
                  "value": "baz"
                }
              ],
              "important": false
            }
          ],
          "rules": []
        }
      ]
    }
    */
    unsigned char data[] = "foo {\nbar: baz;\n}";
    uint32_t data_size = sizeof(data) - 1;

    // actual
    css_parser_init(data, data_size);
    css_parser_node_t* actual = css_parser_parse_stylesheet();
    css_parser_free();

    // // expected
    hash_str_t foo_str              = hash_str_new("foo", 3);
    hash_str_t baz_str              = hash_str_new("baz", 3);
    hash_str_t bar_str              = hash_str_new("bar", 3);
    css_parser_node_t* expected     = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);
    css_parser_node_t* q_rule       = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_Q_RULE);
    css_parser_node_t* foo          = new_ident_node(foo_str);
    css_parser_node_t* whitespace   = new_token_node(CSS_TOKEN_WHITESPACE);
    css_parser_node_t* bar          = css_parser_node_new(bar_str, CSS_PARSER_NODE_TYPE_DECL);
    css_parser_node_t* baz          = new_ident_node(baz_str);

    css_parser_node_add_rule(expected, q_rule);
    css_parser_node_add_comp_val(q_rule, foo);
    css_parser_node_add_comp_val(q_rule, whitespace);
    css_parser_node_add_decl(q_rule, bar);
    css_parser_node_add_comp_val(bar, baz);

    ASSERT_CSS_PARSER_NODE(actual, expected);
}


static void test_parser_6()
{
    /*
    {
      "type": "STYLESHEET",
      "rules": [
        {
          "type": "QUALIFIED-RULE",
          "prelude": [
            {
              "type": "IDENT",
              "value": "foo"
            },
            {
              "type": "WHITESPACE"
            }
          ],
          "declarations": [
            {
              "type": "DECLARATION",
              "name": "bar",
              "value": [
                {
                  "type": "FUNCTION",
                  "name": "rgb",
                  "value": [
                    {
                      "type": "NUMBER",
                      "value": 255,
                      "isInteger": true
                    },
                    {
                      "type": "COMMA"
                    },
                    {
                      "type": "WHITESPACE"
                    },
                    {
                      "type": "NUMBER",
                      "value": 0,
                      "isInteger": true
                    },
                    {
                      "type": "COMMA"
                    },
                    {
                      "type": "WHITESPACE"
                    },
                    {
                      "type": "NUMBER",
                      "value": 127,
                      "isInteger": true
                    }
                  ]
                }
              ],
              "important": false
            }
          ],
          "rules": []
        }
      ]
    }
    */
    unsigned char data[] = "foo { bar: rgb(255, 0, 127); }";
    uint32_t data_size = sizeof(data) - 1;

    css_parser_init(data, data_size);
    css_parser_node_t* actual = css_parser_parse_stylesheet();
    css_parser_free();

    // expected
    hash_str_t foo_str              = hash_str_new("foo", 3);
    hash_str_t rgb_str              = hash_str_new("rgb", 3);
    hash_str_t bar_str              = hash_str_new("bar", 3);
    css_parser_node_t* expected     = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);
    css_parser_node_t* q_rule       = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_Q_RULE);
    css_parser_node_t* foo_node     = new_ident_node(foo_str);
    css_parser_node_t* ws_node1     = new_token_node(CSS_TOKEN_WHITESPACE);
    css_parser_node_t* bar_node     = css_parser_node_new(bar_str, CSS_PARSER_NODE_TYPE_DECL);
    css_parser_node_t* rgb_node     = css_parser_node_new(rgb_str, CSS_PARSER_NODE_TYPE_FUNCTION);
    css_parser_node_t* tff_node     = new_number_node(255.f);
    css_parser_node_t* comma_node1  = new_token_node(CSS_TOKEN_COMMA);
    css_parser_node_t* ws_node2     = new_token_node(CSS_TOKEN_WHITESPACE);
    css_parser_node_t* zero_node    = new_number_node(0.f);
    css_parser_node_t* comma_node2  = new_token_node(CSS_TOKEN_COMMA);
    css_parser_node_t* ws_node3     = new_token_node(CSS_TOKEN_WHITESPACE);
    css_parser_node_t* ots_node     = new_number_node(127.f);

    css_parser_node_add_rule(expected, q_rule);
    css_parser_node_add_comp_val(q_rule, foo_node);
    css_parser_node_add_comp_val(q_rule, ws_node1);
    css_parser_node_add_decl(q_rule, bar_node);
    css_parser_node_add_comp_val(bar_node, rgb_node);
    css_parser_node_add_comp_val(rgb_node, tff_node);
    css_parser_node_add_comp_val(rgb_node, comma_node1);
    css_parser_node_add_comp_val(rgb_node, ws_node2);
    css_parser_node_add_comp_val(rgb_node, zero_node);
    css_parser_node_add_comp_val(rgb_node, comma_node2);
    css_parser_node_add_comp_val(rgb_node, ws_node3);
    css_parser_node_add_comp_val(rgb_node, ots_node);

    ASSERT_CSS_PARSER_NODE(actual, expected);
}


static void test_parser_7()
{
    /*
    {
      "type": "STYLESHEET",
      "rules": [
        {
          "type": "QUALIFIED-RULE",
          "prelude": [
            {
              "type": "HASH",
              "value": "foo",
              "isIdent": true
            },
            {
              "type": "WHITESPACE"
            }
          ],
          "declarations": [],
          "rules": []
        }
      ]
    }
    */
    unsigned char data[] = "#foo {}";
    uint32_t data_size = sizeof(data) - 1;

    // actual
    css_parser_init(data, data_size);
    css_parser_node_t* actual = css_parser_parse_stylesheet();
    css_parser_free();

    // expected
    hash_str_t foo_str              = hash_str_new("foo", 3);
    css_parser_node_t* expected     = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);
    css_parser_node_t* q_rule       = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_Q_RULE);
    css_parser_node_t* foo_n        = new_hash_node(foo_str, CSS_TOKEN_HASH_ID);
    css_parser_node_t* ws_n         = new_token_node(CSS_TOKEN_WHITESPACE);

    css_parser_node_add_rule(expected, q_rule);
    css_parser_node_add_comp_val(q_rule, foo_n);
    css_parser_node_add_comp_val(q_rule, ws_n);

    ASSERT_CSS_PARSER_NODE(actual, expected);
}


static void test_parser_8()
{
    unsigned char data[] = ".foo {color: red; @media { foo: bar } color: green }";
    uint32_t data_size = sizeof(data) - 1;

    // actual
    css_parser_init(data, data_size);
    css_parser_node_t* actual = css_parser_parse_stylesheet();
    css_parser_free();

    // expected
    hash_str_t foo_str              = hash_str_new("foo", 3);
    hash_str_t color_str            = hash_str_new("color", 5);
    hash_str_t red_str              = hash_str_new("red", 3);
    hash_str_t green_str            = hash_str_new("green", 5);
    hash_str_t media_str            = hash_str_new("media", 5);
    hash_str_t bar_str              = hash_str_new("bar", 3);
    hash_str_t dot_str              = hash_str_new(".", 1);

    css_parser_node_t* expected     = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);
    css_parser_node_t* q_rule       = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_Q_RULE);
    css_parser_node_t* delim        = new_data_token_node(dot_str, CSS_TOKEN_DELIM);
    css_parser_node_t* foo_nt       = new_ident_node(foo_str);
    css_parser_node_t* ws_n1        = new_token_node(CSS_TOKEN_WHITESPACE);
    css_parser_node_t* color_n1     = css_parser_node_new(color_str, CSS_PARSER_NODE_TYPE_DECL);
    css_parser_node_t* red_n        = new_ident_node(red_str);
    css_parser_node_t* color_n2     = css_parser_node_new(color_str, CSS_PARSER_NODE_TYPE_DECL);
    css_parser_node_t* green_n      = new_ident_node(green_str);
    css_parser_node_t* at_rule      = css_parser_node_new(media_str, CSS_PARSER_NODE_TYPE_AT_RULE);
    css_parser_node_t* ws_n2        = new_token_node(CSS_TOKEN_WHITESPACE);
    css_parser_node_t* foo_nd       = css_parser_node_new(foo_str, CSS_PARSER_NODE_TYPE_DECL);
    css_parser_node_t* bar_n        = new_ident_node(bar_str);
    

    css_parser_node_add_rule(expected, q_rule);
    css_parser_node_add_comp_val(q_rule, delim);
    css_parser_node_add_comp_val(q_rule, foo_nt);
    css_parser_node_add_comp_val(q_rule, ws_n1);
    css_parser_node_add_decl(q_rule, color_n1);
    css_parser_node_add_comp_val(color_n1, red_n);
    css_parser_node_add_decl(q_rule, color_n2);
    css_parser_node_add_comp_val(color_n2, green_n);
    css_parser_node_add_rule(q_rule, at_rule);
    css_parser_node_add_comp_val(at_rule, ws_n2);
    css_parser_node_add_decl(at_rule, foo_nd);
    css_parser_node_add_comp_val(foo_nd, bar_n);

    ASSERT_CSS_PARSER_NODE(actual, expected);
}


static void test_parser_9()
{
    unsigned char data[] = "foo{div:hover; color:red{};}";
    uint32_t data_size = sizeof(data) - 1;

    // actual
    css_parser_init(data, data_size);
    css_parser_node_t* actual = css_parser_parse_stylesheet();
    css_parser_free();

    // expected
    hash_str_t foo_str              = hash_str_new("foo", 3);
    hash_str_t color_str            = hash_str_new("color", 5);
    hash_str_t red_str              = hash_str_new("red", 3);
    hash_str_t div_str              = hash_str_new("div", 3);
    hash_str_t hover_str            = hash_str_new("hover", 5);

    css_parser_node_t* expected     = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);
    css_parser_node_t* q_rule1      = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_Q_RULE);
    css_parser_node_t* q_rule2      = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_Q_RULE);
    css_parser_node_t* foo_n        = new_ident_node(foo_str);
    css_parser_node_t* hover_n      = new_ident_node(hover_str);
    css_parser_node_t* div_n        = css_parser_node_new(div_str, CSS_PARSER_NODE_TYPE_DECL);
    css_parser_node_t* color_n      = new_ident_node(color_str);
    css_parser_node_t* red_n        = new_ident_node(red_str);
    css_parser_node_t* colon_n      = new_token_node(CSS_TOKEN_COLON);

    css_parser_node_add_rule(expected, q_rule1);
    css_parser_node_add_comp_val(q_rule1, foo_n);
    css_parser_node_add_decl(q_rule1, div_n);
    css_parser_node_add_comp_val(div_n, hover_n);
    css_parser_node_add_rule(q_rule1, q_rule2);
    css_parser_node_add_comp_val(q_rule2, color_n);
    css_parser_node_add_comp_val(q_rule2, colon_n);
    css_parser_node_add_comp_val(q_rule2, red_n);

    ASSERT_CSS_PARSER_NODE(actual, expected);
}


static void test_parser_10()
{
    unsigned char data[] = "@foo;;foo {}";
    uint32_t data_size = sizeof(data) - 1;

    // actual
    css_parser_init(data, data_size);
    css_parser_node_t* actual = css_parser_parse_stylesheet();
    css_parser_free();

    hash_str_t foo_str          = hash_str_new("foo", 3);
    css_parser_node_t* expected = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);
    css_parser_node_t* rule     = css_parser_node_new(foo_str, CSS_PARSER_NODE_TYPE_AT_RULE);

    css_parser_node_add_rule(expected, rule);

    ASSERT_CSS_PARSER_NODE(actual, expected);
}


static void test_parser_11()
{
    unsigned char data[] = "foo{@foo;;foo {}}";
    uint32_t data_size = sizeof(data) - 1;

    // actual
    css_parser_init(data, data_size);
    css_parser_node_t* actual = css_parser_parse_stylesheet();
    css_parser_free();

    hash_str_t foo_str          = hash_str_new("foo", 3);
    css_parser_node_t* expected = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);
    css_parser_node_t* q_rule1  = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_Q_RULE);
    css_parser_node_t* q_rule2  = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_Q_RULE);
    css_parser_node_t* at_rule  = css_parser_node_new(foo_str, CSS_PARSER_NODE_TYPE_AT_RULE);
    css_parser_node_t* foo_n1   = new_ident_node(foo_str);
    css_parser_node_t* foo_n2   = new_ident_node(foo_str);
    css_parser_node_t* ws_n     = new_token_node(CSS_TOKEN_WHITESPACE);

    css_parser_node_add_rule(expected, q_rule1);
    css_parser_node_add_rule(q_rule1, at_rule);
    css_parser_node_add_rule(q_rule1, q_rule2);
    css_parser_node_add_comp_val(q_rule1, foo_n1);
    css_parser_node_add_comp_val(q_rule2, foo_n2);
    css_parser_node_add_comp_val(q_rule2, ws_n);

    ASSERT_CSS_PARSER_NODE(actual, expected);
}

static void test_parser_12()
{
    unsigned char data[] = "foo { --div:hover{}}";
    uint32_t data_size = sizeof(data) - 1;

    // actual
    css_parser_init(data, data_size);
    css_parser_node_t* actual = css_parser_parse_stylesheet();
    css_parser_free();

    hash_str_t foo_str          = hash_str_new("foo", 3);
    hash_str_t div_str          = hash_str_new("--div", 5);
    hash_str_t hover_str        = hash_str_new("hover", 5);

    css_parser_node_t* expected = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);
    css_parser_node_t* q_rule1  = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_Q_RULE);
    css_parser_node_t* foo_n    = new_ident_node(foo_str);
    css_parser_node_t* ws_n     = new_token_node(CSS_TOKEN_WHITESPACE);
    css_parser_node_t* q_rule2  = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_Q_RULE);
    css_parser_node_t* div_n    = new_ident_node(div_str);
    css_parser_node_t* colon_n  = new_token_node(CSS_TOKEN_COLON);
    css_parser_node_t* hover_n  = new_ident_node(hover_str);

    css_parser_node_add_rule(expected, q_rule1);
    css_parser_node_add_rule(q_rule1, q_rule2);
    css_parser_node_add_comp_val(q_rule1, foo_n);
    css_parser_node_add_comp_val(q_rule1, ws_n);
    css_parser_node_add_comp_val(q_rule2, div_n);
    css_parser_node_add_comp_val(q_rule2, colon_n);
    css_parser_node_add_comp_val(q_rule2, hover_n);

    ASSERT_CSS_PARSER_NODE(actual, expected);
}


static void test_parser_13()
{
    // triggers close bracket handling inside consume_q_rule
    unsigned char data[] = "}";
    uint32_t data_size = sizeof(data) - 1;

    // actual
    css_parser_init(data, data_size);
    css_parser_node_t* actual = css_parser_parse_stylesheet();
    css_parser_free();

    css_parser_node_t* expected = css_parser_node_new(0, CSS_PARSER_NODE_TYPE_STYLESHEET);

    ASSERT_CSS_PARSER_NODE(actual, expected);
}


void css_parser_test()
{
    TEST_CASE(test_parser_1);
    TEST_CASE(test_parser_2);
    TEST_CASE(test_parser_3);
    TEST_CASE(test_parser_4);
    TEST_CASE(test_parser_5);
    TEST_CASE(test_parser_6);
    TEST_CASE(test_parser_7);
    TEST_CASE(test_parser_8);
    TEST_CASE(test_parser_9);
    TEST_CASE(test_parser_10);
    TEST_CASE(test_parser_11);
    TEST_CASE(test_parser_12);
    TEST_CASE(test_parser_13);
}