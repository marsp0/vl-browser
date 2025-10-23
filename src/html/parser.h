#pragma once

#include <stdint.h>

#include "dom/node.h"

// https://html.spec.whatwg.org/multipage/parsing.html#the-insertion-mode
typedef enum
{
    HTML_PARSER_MODE_INITIAL,
    HTML_PARSER_MODE_BEFORE_HTML,
    HTML_PARSER_MODE_BEFORE_HEAD,
    HTML_PARSER_MODE_IN_HEAD,
    HTML_PARSER_MODE_IN_HEAD_NOSCRIPT,
    HTML_PARSER_MODE_AFTER_HEAD,
    HTML_PARSER_MODE_IN_BODY,
    HTML_PARSER_MODE_TEXT,
    HTML_PARSER_MODE_IN_TABLE,
    HTML_PARSER_MODE_IN_TABLE_TEXT,
    HTML_PARSER_MODE_IN_CAPTION,
    HTML_PARSER_MODE_IN_COLUMN_GROUP,
    HTML_PARSER_MODE_IN_TABLE_BODY,
    HTML_PARSER_MODE_IN_ROW,
    HTML_PARSER_MODE_IN_CELL,
    HTML_PARSER_MODE_IN_TEMPLATE,
    HTML_PARSER_MODE_AFTER_BODY,
    HTML_PARSER_MODE_IN_FRAMESET,
    HTML_PARSER_MODE_AFTER_FRAMESET,
    HTML_PARSER_MODE_AFTER_AFTER_BODY,
    HTML_PARSER_MODE_AFTER_AFTER_FRAMESET,
} html_parser_mode_e;

void            html_parser_init(bool scripting);
dom_node_t*     html_parser_run(const unsigned char* buffer, const uint32_t size);
void            html_parser_free();