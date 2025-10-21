#include "tokenizer.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/utf8.h"
#include "dom/hash_str.h"
#include "html/named_char_refs.h"

/*
 * Notes
 *  todo - remove all the _segment strings and replace with hash_str_t
 */

/********************/
/*      defines     */
/********************/

#define MAX_TEMP_BUFFER_SIZE    256

/********************/
/* static variables */
/********************/

typedef enum
{
    CASE_SENSITIVE_MATCH,
    CASE_INSENSITIVE_MATCH,
} match_type_e;

typedef struct
{
    uint32_t key;
    uint32_t value;
} numeric_char_ref_t;

static unsigned char* buf                                               = NULL;
static uint32_t buf_cap                                                 = 2048;
static uint32_t buf_size                                                = 0;
static uint32_t buf_cur                                                 = 0;
static html_token_t* tokens                                             = NULL;
static uint32_t token_idx                                               = 0;
static uint32_t max_tokens                                              = 0;
static html_tokenizer_state_e state                                     = HTML_TOKENIZER_DATA_STATE;
static html_tokenizer_state_e return_state                              = HTML_TOKENIZER_DATA_STATE;
static unsigned char tmp_buf[MAX_TEMP_BUFFER_SIZE]                      = { 0 };
static uint32_t tmp_buf_size                                            = 0;
static hash_str_t last_emitted_start_tag                                = 0;
static int32_t cp_len                                                   = 0;
static uint32_t character_reference_code                                = 0;
static unsigned char hyphen_segment[]                                   = "--";
static uint32_t hyphen_segment_size                                     = sizeof(hyphen_segment) - 1;
static unsigned char doctype_segment[]                                  = "DOCTYPE";
static uint32_t doctype_segment_size                                    = sizeof(doctype_segment) - 1;
static unsigned char cdata_segment[]                                    = "[CDATA[";
static uint32_t cdata_segment_size                                      = sizeof(cdata_segment) - 1;
static unsigned char public_segment[]                                   = "PUBLIC";
static uint32_t public_segment_size                                     = sizeof(public_segment) - 1;
static unsigned char system_segment[]                                   = "SYSTEM";
static uint32_t system_segment_size                                     = sizeof(system_segment) - 1;
static const numeric_char_ref_t numeric_char_refs[]                     = { { 0x80, 0x20AC },
                                                                            { 0x82, 0x201A },
                                                                            { 0x83, 0x0192 },
                                                                            { 0x84, 0x201E },
                                                                            { 0x85, 0x2026 },
                                                                            { 0x86, 0x2020 },
                                                                            { 0x87, 0x2021 },
                                                                            { 0x88, 0x02C6 },
                                                                            { 0x89, 0x2030 },
                                                                            { 0x8A, 0x0160 },
                                                                            { 0x8B, 0x2039 },
                                                                            { 0x8C, 0x0152 },
                                                                            { 0x8E, 0x017D },
                                                                            { 0x91, 0x2018 },
                                                                            { 0x92, 0x2019 },
                                                                            { 0x93, 0x201C },
                                                                            { 0x94, 0x201D },
                                                                            { 0x95, 0x2022 },
                                                                            { 0x96, 0x2013 },
                                                                            { 0x97, 0x2014 },
                                                                            { 0x98, 0x02DC },
                                                                            { 0x99, 0x2122 },
                                                                            { 0x9A, 0x0161 },
                                                                            { 0x9B, 0x203A },
                                                                            { 0x9C, 0x0153 },
                                                                            { 0x9E, 0x017E },
                                                                            { 0x9F, 0x0178 } };

/********************/
/* static functions */
/********************/


static bool is_noncharacter(uint32_t cp)
{
    return (cp >= 0xfdd0 && cp <= 0xfdef) || 
            cp == 0xfffe || cp == 0xffff ||
            cp == 0x1fffe || cp == 0x1ffff ||
            cp == 0x2fffe || cp == 0x2ffff ||
            cp == 0x3fffe || cp == 0x3ffff ||
            cp == 0x4fffe || cp == 0x4ffff || 
            cp == 0x5fffe || cp == 0x5ffff ||
            cp == 0x6fffe || cp == 0x6ffff ||
            cp == 0x7fffe || cp == 0x7ffff ||
            cp == 0x8fffe || cp == 0x8ffff ||
            cp == 0x9fffe || cp == 0x9ffff || 
            cp == 0xafffe || cp == 0xaffff || 
            cp == 0xbfffe || cp == 0xbffff ||
            cp == 0xcfffe || cp == 0xcffff ||
            cp == 0xdfffe || cp == 0xdffff ||
            cp == 0xefffe || cp == 0xeffff ||
            cp == 0xffffe || cp == 0xfffff ||
            cp == 0x10fffe || cp == 0x10ffff;
}


static bool is_leading_surrogate(uint32_t cp)
{
    return cp >= 0xd800 && cp <= 0xdbff;
}


static bool is_trailing_surrogate(uint32_t cp)
{
    return cp >= 0xdc00 && cp <= 0xdfff;
}


static bool is_surrogate(uint32_t cp)
{
    return is_leading_surrogate(cp) || is_trailing_surrogate(cp);
}


static void clear_tmp_buf()
{
    memset(tmp_buf, 0, sizeof(tmp_buf));
    tmp_buf_size = 0;
}


static void clear_tokens()
{
    token_idx = 0;
    memset(tokens, 0, max_tokens * sizeof(html_token_t));
}


static void init_token(html_token_type_e type)
{
    memset(&tokens[token_idx], 0, sizeof(html_token_t));

    tokens[token_idx].is_valid  = true;
    tokens[token_idx].type      = type;
}


static void update_data(unsigned char c)
{
    html_token_t* t = &tokens[token_idx];
    t->data[t->data_size] = c;
    t->data_size++;
}


static void update_data_from_buf()
{
    html_token_t* t = &tokens[token_idx];

    for (uint32_t i = buf_cur; i < buf_cur + (uint32_t)cp_len; i++)
    {
        t->data[t->data_size] = buf[i];
        t->data_size++;
    }
}


static void update_name(unsigned char c)
{
    html_token_t* t = &tokens[token_idx];
    t->name[t->name_size] = c;
    t->name_size++;
}


static void update_name_from_buf()
{
    html_token_t* t = &tokens[token_idx];

    for (uint32_t i = buf_cur; i < buf_cur + (uint32_t)cp_len; i++)
    {
        t->name[t->name_size] = buf[i];
        t->name_size++;
    }
}


static void update_public_id(unsigned char c)
{
    html_token_t* t = &tokens[token_idx];
    t->public_id[t->public_id_size] = c;
    t->public_id_size++;
}


static void update_public_id_from_buf()
{
    html_token_t* t = &tokens[token_idx];

    for (uint32_t i = buf_cur; i < buf_cur + (uint32_t)cp_len; i++)
    {
        t->public_id[t->public_id_size] = buf[i];
        t->public_id_size++;
    }
}


static void update_system_id(unsigned char c)
{
    html_token_t* t = &tokens[token_idx];
    t->system_id[t->system_id_size] = c;
    t->system_id_size++;
}


static void update_system_id_from_buf()
{
    html_token_t* t = &tokens[token_idx];

    for (uint32_t i = buf_cur; i < buf_cur + (uint32_t)cp_len; i++)
    {
        t->system_id[t->system_id_size] = buf[i];
        t->system_id_size++;
    }
}


static void init_attr()
{
    html_token_t* token          = &tokens[token_idx];
    html_token_attribute_t* attr = &(token->attributes[token->attributes_size]);
    memset(attr, 0, sizeof(html_token_attribute_t));
}


static void update_attr_name(unsigned char c)
{
    html_token_t* token          = &tokens[token_idx];
    html_token_attribute_t* attr = &(token->attributes[token->attributes_size]);

    if (attr->name_size >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    attr->name[attr->name_size]   = c;
    attr->name_size++;
}


static void update_attr_name_from_buf()
{
    html_token_t* token = &tokens[token_idx];
    html_token_attribute_t* attr = &(token->attributes[token->attributes_size]);

    for (uint32_t i = buf_cur; i < buf_cur + (uint32_t)cp_len; i++)
    {
        attr->name[attr->name_size] = buf[i];
        attr->name_size++;

        if (attr->name_size >= HTML_TOKEN_MAX_NAME_LEN) { return; }
    }
}


static void update_attr_val(unsigned char c)
{
    html_token_t* token          = &tokens[token_idx];
    html_token_attribute_t* attr = &(token->attributes[token->attributes_size]);

    if (attr->value_size >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    attr->value[attr->value_size]   = c;
    attr->value_size++;
}


static void update_attr_val_from_buf()
{
    html_token_t* token = &tokens[token_idx];
    html_token_attribute_t* attr = &(token->attributes[token->attributes_size]);

    for (uint32_t i = buf_cur; i < buf_cur + (uint32_t)cp_len; i++)
    {
        attr->value[attr->value_size] = buf[i];
        attr->value_size++;

        if (attr->value_size >= HTML_TOKEN_MAX_NAME_LEN) { return; }
    }
}


static void emit_attr()
{
    bool is_duplicate           = false;
    html_token_t* token         = &tokens[token_idx];
    html_token_attribute_t attr = token->attributes[token->attributes_size];
    for (uint32_t i = 0; i < token->attributes_size; i++)
    {
        html_token_attribute_t attr_i = token->attributes[i];
        if (attr.name_size == attr_i.name_size && strncmp(attr.name, attr_i.name, attr.name_size) == 0)
        {
            is_duplicate = true;
        }
    }

    if (is_duplicate || attr.name_size == 0)
    {
        init_attr();
        return;
    }

    tokens[token_idx].attributes_size++;
}

static void emit_token()
{
    token_idx++;

    // reset any parsed attributes for an end token
    if (tokens[token_idx - 1].type == HTML_END_TOKEN && tokens[token_idx - 1].attributes_size > 0)
    {
        memset(tokens[token_idx - 1].attributes, 0, tokens[token_idx - 1].attributes_size * sizeof(html_token_attribute_t));
        tokens[token_idx - 1].attributes_size = 0;
    }

    // save the name of start tokens
    if (tokens[token_idx - 1].type != HTML_START_TOKEN) { return; }

    hash_str_t t_name = hash_str_new(tokens[token_idx - 1].name, tokens[token_idx - 1].name_size);
    last_emitted_start_tag = t_name;
}

static bool match_segment(unsigned char* segment, uint32_t segment_size, match_type_e match_type)
{
    if (buf_cur + segment_size > buf_size)
    {
        return false;
    }
    if (match_type == CASE_SENSITIVE_MATCH)
    {
        return strncmp(&buf[buf_cur], segment, segment_size) == 0;
    }

    for (uint32_t i = 0; i < segment_size; i++)
    {
        unsigned char a = buf[buf_cur + i];
        unsigned char b = segment[i];
        if (a < 'a')    { a += 0x20; }
        if (b < 'a')    { b += 0x20; }
        if (a != b)     { return false; }
    }

    return true;
}

static bool is_appropriate_end_tag()
{
    hash_str_t new_name = hash_str_new(tokens[token_idx].name, tokens[token_idx].name_size);
    return last_emitted_start_tag == new_name;
}

static void emit_tmp_buf()
{
    uint32_t cur = 0;
    uint32_t cp = 0;
    int32_t bytes = 0;
    while (cur < tmp_buf_size)
    {
        bytes = utf8_decode(tmp_buf, tmp_buf_size, cur, &cp);

        assert(bytes > -1);

        init_token(HTML_CHARACTER_TOKEN);
        uint32_t read = (uint32_t)bytes;
        uint32_t data_size = tokens[token_idx].data_size;

        for (uint32_t i = 0; i < read; i++)
        {
            tokens[token_idx].data[data_size + i] = tmp_buf[cur + i];
        }

        tokens[token_idx].data_size = data_size + read;

        emit_token();
        
        cur += read;
    }
}

static void update_tmp_buf_from_buf()
{
    if (tmp_buf_size >= MAX_TEMP_BUFFER_SIZE)
    {
        tmp_buf_size = MAX_TEMP_BUFFER_SIZE;
        return;
    }

    assert(cp_len >= 0);

    uint32_t read = (uint32_t)cp_len;

    for (uint32_t i = 0; i < read; i++)
    {
        tmp_buf[tmp_buf_size + i] = buf[buf_cur + i];
    }
    tmp_buf_size += read;
}

static void update_tmp_buf(unsigned char c)
{
    if (tmp_buf_size >= MAX_TEMP_BUFFER_SIZE)
    {
        tmp_buf_size = MAX_TEMP_BUFFER_SIZE;
        return;
    }

    tmp_buf[tmp_buf_size] = c;
    tmp_buf_size++;
}

static void normalize_line_endings()
{
    if (buf_size == 0) { return; }

    uint32_t new_size = buf_size;

    // replace \r\n with \n
    for (uint32_t i = 0; i < buf_size - 1; i++)
    {
        if (buf[i] != '\r' || buf[i + 1] != '\n') { continue; }

        new_size--;

        for (uint32_t j = i + 1; j < buf_size; j++)
        {
            buf[j - 1] = buf[j];
        }
    }

    buf_size = new_size;

    // replace \r with \n
    for (uint32_t i = 0; i < buf_size; i++)
    {
        if (buf[i] == '\r') { buf[i] = '\n'; }
    }
}


static bool in_attribute(html_tokenizer_state_e s)
{
    return  s == HTML_TOKENIZER_ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE || 
            s == HTML_TOKENIZER_ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE ||
            s == HTML_TOKENIZER_ATTRIBUTE_VALUE_UNQUOTED_STATE;
}


static void flush_cps_consumed_as_char_ref(html_tokenizer_state_e s)
{
    if (in_attribute(s))
    {
        for (uint32_t i = 0; i < tmp_buf_size; i++)
        {
            update_attr_val(tmp_buf[i]);
        }
    }
    else
    {
        emit_tmp_buf();
    }
}

void resize_buffer(uint32_t min_size)
{
    uint32_t next_cap = buf_cap;
    while (next_cap < min_size) { next_cap <<= 1; }

    if (next_cap < buf_cap) { assert(false); }
    buf_cap = next_cap;

    if (buf) { free(buf); }

    buf = malloc(buf_cap);
    memset(buf, 0, buf_cap);
}

/********************/
/* public functions */
/********************/

void html_tokenizer_global_init()
{
    buf = malloc(buf_cap);
}

void html_tokenizer_init(const unsigned char* new_buffer, const uint32_t new_size, html_token_t* new_tokens, const uint32_t new_max_tokens)
{
    assert(new_buffer);

    if (buf_cap < new_size + 1)
    {
        resize_buffer(new_size + 1);
    }

    buf_size                 = new_size;
    memcpy(buf, new_buffer, new_size);

    normalize_line_endings();

    buf_cur                      = 0;
    tokens                      = new_tokens;
    max_tokens                  = new_max_tokens;

    state                       = HTML_TOKENIZER_DATA_STATE;
    return_state                = HTML_TOKENIZER_DATA_STATE;

    character_reference_code    = 0;

    last_emitted_start_tag      = 0;
    clear_tmp_buf();
}

html_tokenizer_error_e html_tokenizer_next()
{
    assert(buf);

    clear_tokens();

    bool is_eof = false;
    bool consume = true;
    uint32_t cp;
    html_tokenizer_error_e status = HTML_TOKENIZER_OK;

    while (token_idx == 0)
    {
        assert(token_idx < max_tokens);

        cp_len  = -1;
        consume     = true;

        if (buf_cur >= buf_size)
        {
            is_eof = true;
        }
        else
        {
            cp_len = utf8_decode(buf, buf_size, buf_cur, &cp);
            if (cp_len <= -1) { return HTML_TOKENIZER_ERROR; }
        }

        switch (state)
        {
        // https://html.spec.whatwg.org/multipage/parsing.html#data-state
        case HTML_TOKENIZER_DATA_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                break;
            }

            switch (cp)
            {
            case '&':
                state                           = HTML_TOKENIZER_CHARACTER_REFERENCE_STATE;
                return_state                    = HTML_TOKENIZER_DATA_STATE;
                break;
            case '<':
                state                           = HTML_TOKENIZER_TAG_OPEN_STATE;
                break;
            default:
                if (cp == 0) { status   = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER; }
                init_token(HTML_CHARACTER_TOKEN);
                update_data_from_buf();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rcdata-state
        case HTML_TOKENIZER_RCDATA_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_OK;
                break;
            }

            switch (cp)
            {
            case '&':
                return_state                    = HTML_TOKENIZER_RCDATA_STATE;
                state                           = HTML_TOKENIZER_CHARACTER_REFERENCE_STATE;
                break;
            case '<':
                state                           = HTML_TOKENIZER_RCDATA_LESS_THAN_STATE;
                break;
            default:
                if (cp == 0)
                {
                    init_token(HTML_CHARACTER_TOKEN);
                    update_data(0xef);
                    update_data(0xbf);
                    update_data(0xbd);
                    status                      = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                }
                else
                {
                    init_token(HTML_CHARACTER_TOKEN);
                    update_data_from_buf();
                }

                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rawtext-state
        case HTML_TOKENIZER_RAWTEXT_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_OK;
                break;
            }

            switch(cp)
            {
            case '<':
                state                           = HTML_TOKENIZER_RAWTEXT_LESS_THAN_STATE;
                break;
            default:
                if (cp == 0)
                {
                    status                      = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                    init_token(HTML_CHARACTER_TOKEN);
                    update_data(0xef);
                    update_data(0xbf);
                    update_data(0xbd);
                }
                else
                {
                    init_token(HTML_CHARACTER_TOKEN);
                    update_data_from_buf();
                }

                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-state
        case HTML_TOKENIZER_SCRIPT_DATA_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_OK;
                break;
            }

            switch (cp)
            {
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_LESS_THAN_STATE;
                break;
            default:
                if (cp == 0)
                {
                    init_token(HTML_CHARACTER_TOKEN);
                    update_data(0xef);
                    update_data(0xbf);
                    update_data(0xbd);
                    status                      = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                }
                else
                {
                    init_token(HTML_CHARACTER_TOKEN);
                    update_data_from_buf();
                }
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#plaintext-state
        case HTML_TOKENIZER_PLAINTEXT_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_OK;
                break;
            }

            if (cp == 0)
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data(0xef);
                update_data(0xbf);
                update_data(0xbd);
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
            }
            else
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data_from_buf();
            }

            emit_token();
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#tag-open-state
        case HTML_TOKENIZER_TAG_OPEN_STATE:
            if (is_eof)
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_BEFORE_TAG_NAME;
                break;
            }

            if (cp == '!')
            {
                state                           = HTML_TOKENIZER_MARKUP_DECLARATION_OPEN_STATE;
            }
            else if (cp == '/')
            {
                state                           = HTML_TOKENIZER_END_TAG_OPEN_STATE;
            }
            else if (utf8_is_alpha(cp))
            {
                init_token(HTML_START_TOKEN);
                consume                         = false;
                state                           = HTML_TOKENIZER_TAG_NAME_STATE;
            }
            else if (cp == '?')
            {
                init_token(HTML_COMMENT_TOKEN);
                consume                         = false;
                state                           = HTML_TOKENIZER_BOGUS_COMMENT_STATE;
                status                          = HTML_TOKENIZER_UNEXPECTED_QUOESTION_MARK_INSTEAD_OF_TAG_NAME;
            }
            else
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_DATA_STATE;
                status                          = HTML_TOKENIZER_INVALID_FIRST_CHARACTER_OF_TAG_NAME;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#end-tag-open-state
        case HTML_TOKENIZER_END_TAG_OPEN_STATE:
            if (is_eof)
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                init_token(HTML_CHARACTER_TOKEN);
                update_data('/');
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_BEFORE_TAG_NAME;
                break;
            }

            if (utf8_is_alpha(cp))
            {
                init_token(HTML_END_TOKEN);
                consume                         = false;
                state                           = HTML_TOKENIZER_TAG_NAME_STATE;
            }
            else if (cp == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                status                          = HTML_TOKENIZER_MISSING_END_TAG_NAME;
            }
            else
            {
                init_token(HTML_COMMENT_TOKEN);
                consume                         = false;
                state                           = HTML_TOKENIZER_BOGUS_COMMENT_STATE;
                status                          = HTML_TOKENIZER_INVALID_FIRST_CHARACTER_OF_TAG_NAME;
            }

            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#tag-name-state
        case HTML_TOKENIZER_TAG_NAME_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_TAG;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (cp == '/')
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
            }
            else if (cp == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(cp))
            {
                unsigned char c = (unsigned char)cp;
                update_name(c + 0x20);
            }
            else if (cp == 0)
            {
                update_name(0xef);
                update_name(0xbf);
                update_name(0xbd);
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
            }
            else
            {
                update_name_from_buf();
            }

            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rcdata-less-than-sign-state
        case HTML_TOKENIZER_RCDATA_LESS_THAN_STATE:
            switch (cp)
            {
            case '/':
                state                           = HTML_TOKENIZER_RCDATA_END_TAG_OPEN_STATE;
                break;
            default:
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_RCDATA_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rcdata-end-tag-open-state
        case HTML_TOKENIZER_RCDATA_END_TAG_OPEN_STATE:
            if (utf8_is_alpha(cp))
            {
                init_token(HTML_END_TOKEN);
                consume                         = false;
                state                           = HTML_TOKENIZER_RCDATA_END_TAG_NAME_STATE;
            }
            else
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                init_token(HTML_CHARACTER_TOKEN);
                update_data('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_RCDATA_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rcdata-end-tag-name-state
        case HTML_TOKENIZER_RCDATA_END_TAG_NAME_STATE:
            if ((cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ') && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (cp == '/' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
            }
            else if (cp == '>' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(cp))
            {
                unsigned char c = (unsigned char)cp;
                update_name(c + 0x20);
                update_tmp_buf_from_buf();
            }
            else if (utf8_is_lower_alpha(cp))
            {
                unsigned char c = (unsigned char)cp;
                update_name(c);
                update_tmp_buf_from_buf();
            }
            else
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                init_token(HTML_CHARACTER_TOKEN);
                update_data('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_RCDATA_STATE;
                emit_tmp_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rawtext-less-than-sign-state
        case HTML_TOKENIZER_RAWTEXT_LESS_THAN_STATE:
            switch (cp)
            {
            case '/':
                clear_tmp_buf();
                state                           = HTML_TOKENIZER_RAWTEXT_END_TAG_OPEN_STATE;
                break;
            default:
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_RAWTEXT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rawtext-end-tag-open-state
        case HTML_TOKENIZER_RAWTEXT_END_TAG_OPEN_STATE:
            if (utf8_is_alpha(cp))
            {
                init_token(HTML_END_TOKEN);
                consume                         = false;
                state                           = HTML_TOKENIZER_RAWTEXT_END_TAG_NAME_STATE;
            }
            else
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                init_token(HTML_CHARACTER_TOKEN);
                update_data('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_RAWTEXT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rawtext-end-tag-name-state
        case HTML_TOKENIZER_RAWTEXT_END_TAG_NAME_STATE:
            if ((cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ') && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (cp == '/' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
            }
            else if (cp == '>' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(cp))
            {
                unsigned char c = (unsigned char)cp;
                update_name(c + 0x20);
                update_tmp_buf_from_buf();
            }
            else if (utf8_is_lower_alpha(cp))
            {
                unsigned char c = (unsigned char)cp;
                update_name(c);
                update_tmp_buf_from_buf();
            }
            else
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                init_token(HTML_CHARACTER_TOKEN);
                update_data('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_RAWTEXT_STATE;
                emit_tmp_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-less-than-sign-state
        case HTML_TOKENIZER_SCRIPT_DATA_LESS_THAN_STATE:
            switch (cp)
            {
            case '/':
                clear_tmp_buf();
                state                           = HTML_TOKENIZER_SCRIPT_DATA_END_TAG_OPEN_STATE;
                break;
            case '!':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPE_START_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                init_token(HTML_CHARACTER_TOKEN);
                update_data('!');
                emit_token();
                break;
            default:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_STATE;
                consume                         = false;
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-end-tag-open-state
        case HTML_TOKENIZER_SCRIPT_DATA_END_TAG_OPEN_STATE:
            if (utf8_is_alpha(cp))
            {
                init_token(HTML_END_TOKEN);
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_END_TAG_NAME_STATE;
            }
            else
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                init_token(HTML_CHARACTER_TOKEN);
                update_data('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-end-tag-name-state
        case HTML_TOKENIZER_SCRIPT_DATA_END_TAG_NAME_STATE:
            if ((cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ') && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (cp == '/' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
            }
            else if (cp == '>' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(cp) && !is_eof)
            {
                unsigned char c = (unsigned char)cp;
                update_name(c + 0x20);
                update_tmp_buf(c + 0x20);
            }
            else if (utf8_is_lower_alpha(cp) && !is_eof)
            {
                unsigned char c = (unsigned char)cp;
                update_name(c);
                update_tmp_buf(c);
            }
            else
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                init_token(HTML_CHARACTER_TOKEN);
                update_data('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_STATE;
                emit_tmp_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escape-start-state
        case HTML_TOKENIZER_SCRIPT_DATA_ESCAPE_START_STATE:
            switch (cp)
            {
            case '-':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPE_START_DASH_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data('-');
                emit_token();
                break;
            default:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_STATE;
                consume                         = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escape-start-dash-state
        case HTML_TOKENIZER_SCRIPT_DATA_ESCAPE_START_DASH_STATE:
            switch (cp)
            {
            case '-':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_DASH_DASH_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data('-');
                emit_token();
                break;
            default:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_STATE;
                consume                         = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-state
        case HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT;
                break;
            }
            switch (cp)
            {
            case '-':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_DASH_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data('-');
                emit_token();
                break;
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_LESS_THAN_STATE;
                break;
            case 0:
                init_token(HTML_CHARACTER_TOKEN);
                update_data(0xef);
                update_data(0xbf);
                update_data(0xbd);
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                break;
            default:
                init_token(HTML_CHARACTER_TOKEN);
                update_data_from_buf();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-dash-state
        case HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_DASH_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT;
                break;
            }

            switch (cp)
            {
            case '-':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_DASH_DASH_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data('-');
                emit_token();
                break;
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_LESS_THAN_STATE;
                break;
            case 0:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                init_token(HTML_CHARACTER_TOKEN);
                update_data(0xef);
                update_data(0xbf);
                update_data(0xbd);
                emit_token();
                break;
            default:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data_from_buf();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-dash-dash-state
        case HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_DASH_DASH_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT;
                break;
            }

            switch (cp)
            {
            case '-':
                init_token(HTML_CHARACTER_TOKEN);
                update_data('-');
                emit_token();
                break;
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_LESS_THAN_STATE;
                break;
            case 0:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                init_token(HTML_CHARACTER_TOKEN);
                update_data(0xef);
                update_data(0xbf);
                update_data(0xbd);
                emit_token();
                break;
            default:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data_from_buf();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-less-than-sign-state
        case HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_LESS_THAN_STATE:
            if (cp == '/')
            {
                clear_tmp_buf();
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_END_TAG_OPEN_STATE;
            }
            else if (utf8_is_alpha(cp))
            {
                clear_tmp_buf();
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPE_START_STATE;
            }
            else
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-end-tag-open-state
        case HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_END_TAG_OPEN_STATE:
            if (utf8_is_alpha(cp))
            {
                init_token(HTML_END_TOKEN);
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_END_TAG_NAME_STATE;
                consume                         = false;
            }
            else
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                init_token(HTML_CHARACTER_TOKEN);
                update_data('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-end-tag-name-state
        case HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_END_TAG_NAME_STATE:
            if ((cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ') && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (cp == '/' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
            }
            else if (cp == '>' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(cp))
            {
                unsigned char c = (unsigned char)cp;
                update_name(c + 0x20);
                update_tmp_buf_from_buf();
            }
            else if (utf8_is_lower_alpha(cp))
            {
                unsigned char c = (unsigned char)cp;
                update_name(c);
                update_tmp_buf_from_buf();
            }
            else
            {
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                init_token(HTML_CHARACTER_TOKEN);
                update_data('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                emit_tmp_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escape-start-state
        case HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPE_START_STATE:
            if (cp == '\t' || cp == '\n' || cp == '\f' || 
                cp == ' '  || cp == '/'  || cp == '>')
            {
                if (tmp_buf_size == 6 && strncmp(tmp_buf, "script", tmp_buf_size) == 0)
                {
                    state                       = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                }
                else
                {
                    state                       = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                }
                
                init_token(HTML_CHARACTER_TOKEN);
                update_data_from_buf();
                emit_token();
            }
            else if (utf8_is_upper_alpha(cp))
            {
                unsigned char c = (unsigned char)cp;
                update_tmp_buf(c + 0x20);
                init_token(HTML_CHARACTER_TOKEN);
                update_data(c);
                emit_token();
            }
            else if (utf8_is_lower_alpha(cp))
            {
                unsigned char c = (unsigned char)cp;
                update_tmp_buf(c);
                init_token(HTML_CHARACTER_TOKEN);
                update_data(c);
                emit_token();
            }
            else
            {
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
            }
            break;

        //https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escaped-state
        case HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT;
                break;
            }

            switch (cp)
            {
            case '-':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data('-');
                emit_token();
                break;
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                break;
            case 0:
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                init_token(HTML_CHARACTER_TOKEN);
                update_data(0xef);
                update_data(0xbf);
                update_data(0xbd);
                emit_token();
                break;
            default:
                init_token(HTML_CHARACTER_TOKEN);
                update_data_from_buf();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escaped-dash-state
        case HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT;
                break;
            }

            switch (cp)
            {
            case '-':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data('-');
                emit_token();
                break;
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                break;
            case 0:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                init_token(HTML_CHARACTER_TOKEN);
                update_data(0xef);
                update_data(0xbf);
                update_data(0xbd);
                emit_token();
                break;
            default:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data_from_buf();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escaped-dash-dash-state
        case HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT;
                break;
            }

            switch (cp)
            {
            case '-':
                init_token(HTML_CHARACTER_TOKEN);
                update_data('-');
                emit_token();
                break;
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data('<');
                emit_token();
                break;
            case '>':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data('>');
                emit_token();
                break;
            case 0:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                init_token(HTML_CHARACTER_TOKEN);
                update_data(0xef);
                update_data(0xbf);
                update_data(0xbd);
                emit_token();
                break;
            default:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                init_token(HTML_CHARACTER_TOKEN);
                update_data_from_buf();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escaped-less-than-sign-state
        case HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_STATE:
            switch (cp)
            {
            case '/':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPE_END_STATE;
                clear_tmp_buf();
                init_token(HTML_CHARACTER_TOKEN);
                update_data('/');
                emit_token();
                break;
            default:
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escape-end-state
        case HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPE_END_STATE:
            if (cp == '\t' || cp == '\n' || cp == '\f' || 
                cp == ' '  || cp == '/'  || cp == '>')
            {
                if (tmp_buf_size == 6 && strncmp(tmp_buf, "script", tmp_buf_size) == 0)
                {
                    state                       = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                }
                else
                {
                    state                       = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                }
                
                init_token(HTML_CHARACTER_TOKEN);
                update_data_from_buf();
                emit_token();
            }
            else if (utf8_is_upper_alpha(cp))
            {
                unsigned char c = (unsigned char)cp;
                update_tmp_buf(c + 0x20);
                init_token(HTML_CHARACTER_TOKEN);
                update_data(c);
                emit_token();
            }
            else if (utf8_is_lower_alpha(cp))
            {
                unsigned char c = (unsigned char)cp;
                update_tmp_buf(c);
                init_token(HTML_CHARACTER_TOKEN);
                update_data(c);
                emit_token();
            }
            else
            {
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#before-attribute-name-state
        case HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE:
            if (is_eof || cp == '/'  || cp == '>')
            {
                consume                         = false;
                state                           = HTML_TOKENIZER_AFTER_ATTRIBUTE_NAME_STATE;
            }
            else if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {
                // ignore
            }
            else if (cp == '=')
            {
                init_attr();
                update_attr_name_from_buf();
                state                           = HTML_TOKENIZER_ATTRIBUTE_NAME_STATE;
                status                          = HTML_TOKENIZER_UNEXPECTED_EQUALS_SIGN_BEFORE_ATTRIBUTE_NAME;
            }
            else
            {
                init_attr();
                consume                         = false;
                state                           = HTML_TOKENIZER_ATTRIBUTE_NAME_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#attribute-name-state
        case HTML_TOKENIZER_ATTRIBUTE_NAME_STATE:
            if (cp == '\t' || cp == '\n' || cp == '\f' || 
                cp == ' '  || cp == '/'  || cp == '>' || is_eof)
            {
                consume                         = false;
                state                           = HTML_TOKENIZER_AFTER_ATTRIBUTE_NAME_STATE;
            }
            else if (cp == '=')
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_VALUE_STATE;
            }
            else if (utf8_is_upper_alpha(cp))
            {
                unsigned char c = (unsigned char)cp;
                update_attr_name(c + 0x20);
            }
            else if (cp == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_attr_name(0xef);
                update_attr_name(0xbf);
                update_attr_name(0xbd);
            }
            else
            {
                if (cp == '"' || cp == '\'' || cp == '<')
                {
                    status                      = HTML_TOKENIZER_UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME;
                }
                update_attr_name_from_buf();
            }

            // todo: parser error - duplicate-attribute that we are not handling atm
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-attribute-name-state
        case HTML_TOKENIZER_AFTER_ATTRIBUTE_NAME_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_TAG;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {

            }
            else if (cp == '/')
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
            }
            else if (cp == '=')
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_VALUE_STATE;
            }
            else if (cp == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_attr();
                emit_token();
            }
            else
            {
                emit_attr();
                init_attr();
                consume                         = false;
                state                           = HTML_TOKENIZER_ATTRIBUTE_NAME_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#before-attribute-value-state
        case HTML_TOKENIZER_BEFORE_ATTRIBUTE_VALUE_STATE:
            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {

            }
            else if (cp == '"')
            {
                state                           = HTML_TOKENIZER_ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE;
            }
            else if (cp == '\'')
            {
                state                           = HTML_TOKENIZER_ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE;
            }
            else if (cp == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                status                          = HTML_TOKENIZER_MISSING_ATTRIBUTE_VALUE;
                emit_attr();
                emit_token();
            }
            else
            {
                consume                         = false;
                state                           = HTML_TOKENIZER_ATTRIBUTE_VALUE_UNQUOTED_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#attribute-value-(double-quoted)-state
        case HTML_TOKENIZER_ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_TAG;
                break;
            }

            if (cp == '"')
            {
                state                           = HTML_TOKENIZER_AFTER_ATTRIBUTE_VALUE_QUOTED_STATE;
            }
            else if (cp == '&')
            {
                state                           = HTML_TOKENIZER_CHARACTER_REFERENCE_STATE;
                return_state                    = HTML_TOKENIZER_ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE;
            }
            else if (cp == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_attr_val(0xef);
                update_attr_val(0xbf);
                update_attr_val(0xbd);
            }
            else
            {
                update_attr_val_from_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#attribute-value-(single-quoted)-state
        case HTML_TOKENIZER_ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_TAG;
                break;
            }

            if (cp == '\'')
            {
                state                           = HTML_TOKENIZER_AFTER_ATTRIBUTE_VALUE_QUOTED_STATE;
            }
            else if (cp == '&')
            {
                state                           = HTML_TOKENIZER_CHARACTER_REFERENCE_STATE;
                return_state                    = HTML_TOKENIZER_ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE;
            }
            else if (cp == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_attr_val(0xef);
                update_attr_val(0xbf);
                update_attr_val(0xbd);
            }
            else
            {
                update_attr_val_from_buf();
            }
            break;


        // https://html.spec.whatwg.org/multipage/parsing.html#attribute-value-(unquoted)-state
        case HTML_TOKENIZER_ATTRIBUTE_VALUE_UNQUOTED_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_TAG;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
                emit_attr();
            }
            else if (cp == '&')
            {
                state                           = HTML_TOKENIZER_CHARACTER_REFERENCE_STATE;
                return_state                    = HTML_TOKENIZER_ATTRIBUTE_VALUE_UNQUOTED_STATE;
            }
            else if (cp == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_attr();
                emit_token();
            }
            else if (cp == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_attr_val(0xef);
                update_attr_val(0xbf);
                update_attr_val(0xbd);
            }
            else
            {
                if (cp == '"' || cp == '\'' || cp == '<' || cp == '=' || cp == '`')
                {
                    status                      = HTML_TOKENIZER_UNEXPECTED_CHARACTER_IN_UNQUOTED_ATTRIBUTE_VALUE;
                }
                update_attr_val_from_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-attribute-value-(quoted)-state
        case HTML_TOKENIZER_AFTER_ATTRIBUTE_VALUE_QUOTED_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_TAG;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
                emit_attr();
            }
            else if (cp == '/')
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
                emit_attr();
            }
            else if (cp == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_attr();
                emit_token();
            }
            else
            {
                status                          = HTML_TOKENIZER_MISSING_WHITESPACE_BETWEEN_ATTRIBUTES;
                consume                         = false;
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
                emit_attr();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#self-closing-start-tag-state
        case HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_TAG;
                break;
            }

            if (cp == '>')
            {
                if (tokens[token_idx].type == HTML_START_TOKEN)
                {
                    tokens[token_idx].self_closing  = true;
                }
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_attr();
                emit_token();
            }
            else
            {
                consume                         = false;
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
                status                          = HTML_TOKENIZER_UNEXPECTED_SOLIDUS_IN_TAG;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#bogus-comment-state
        case HTML_TOKENIZER_BOGUS_COMMENT_STATE:
            if (is_eof)
            {
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                break;
            }

            if (cp == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (cp == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_data(0xef);
                update_data(0xbf);
                update_data(0xbd);
            }
            else
            {
                update_data_from_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#markup-declaration-open-state
        case HTML_TOKENIZER_MARKUP_DECLARATION_OPEN_STATE:
            if (cp == '-' && match_segment(hyphen_segment, hyphen_segment_size, CASE_SENSITIVE_MATCH))
            {
                init_token(HTML_COMMENT_TOKEN);
                buf_cur                        += hyphen_segment_size;
                state                           = HTML_TOKENIZER_COMMENT_START_STATE;
                consume                         = false;
            }
            else if (match_segment(doctype_segment, doctype_segment_size, CASE_INSENSITIVE_MATCH))
            {
                buf_cur                        += doctype_segment_size;
                consume                         = false;
                state                           = HTML_TOKENIZER_DOCTYPE_STATE;
            }
            else if (match_segment(cdata_segment, cdata_segment_size, CASE_SENSITIVE_MATCH))
            {
                buf_cur                        += cdata_segment_size;
                consume                         = false;
                // todo: handle once implementation on parser has started
                assert(false);
            }
            else
            {
                init_token(HTML_COMMENT_TOKEN);
                consume                         = false;
                state                           = HTML_TOKENIZER_BOGUS_COMMENT_STATE;
                status                          = HTML_TOKENIZER_INCORRECTLY_OPENED_COMMENT;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-start-state
        case HTML_TOKENIZER_COMMENT_START_STATE:
            switch (cp)
            {
            case '-':
                state                           = HTML_TOKENIZER_COMMENT_START_DASH_STATE;
                break;
            case '>':
                status                          = HTML_TOKENIZER_ABRUPT_CLOSING_OF_EMPTY_COMMENT;
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
                break;
            default:
                consume                         = false;
                state                           = HTML_TOKENIZER_COMMENT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-start-dash-state
        case HTML_TOKENIZER_COMMENT_START_DASH_STATE:
            if (is_eof)
            {
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_COMMENT;
                break;
            }

            switch (cp)
            {
            case '-':
                state                           = HTML_TOKENIZER_COMMENT_END_STATE;
                break;
            case '>':
                status                          = HTML_TOKENIZER_ABRUPT_CLOSING_OF_EMPTY_COMMENT;
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
                break;
            default:
                update_data('-');
                consume                         = false;
                state                           = HTML_TOKENIZER_COMMENT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-state
        case HTML_TOKENIZER_COMMENT_STATE:
            if (is_eof)
            {
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_COMMENT;
                break;
            }

            switch (cp)
            {
            case '<':
                update_data_from_buf();
                state                           = HTML_TOKENIZER_COMMENT_LESS_THAN_STATE;
                break;
            case '-':
                state                           = HTML_TOKENIZER_COMMENT_END_DASH_STATE;
                break;
            case 0:
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_data(0xef);
                update_data(0xbf);
                update_data(0xbd);
                break;
            default:
                update_data_from_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-less-than-sign-state
        case HTML_TOKENIZER_COMMENT_LESS_THAN_STATE:
            if (is_eof)
            {
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_COMMENT;
                break;
            }

            switch (cp)
            {
            case '!':
                update_data_from_buf();
                state                           = HTML_TOKENIZER_COMMENT_LESS_THAN_BANG_STATE;
                break;
            case '<':
                update_data_from_buf();
                break;
            default:
                state                           = HTML_TOKENIZER_COMMENT_STATE;
                consume                         = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-less-than-sign-bang-state
        case HTML_TOKENIZER_COMMENT_LESS_THAN_BANG_STATE:
            switch (cp)
            {
            case '-':
                state                           = HTML_TOKENIZER_COMMENT_LESS_THAN_BANG_DASH_STATE;
                break;
            default:
                state                           = HTML_TOKENIZER_COMMENT_STATE;
                consume                         = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-less-than-sign-bang-dash-state
        case HTML_TOKENIZER_COMMENT_LESS_THAN_BANG_DASH_STATE:
            switch (cp)
            {
            case '-':
                state                           = HTML_TOKENIZER_COMMENT_LESS_THAN_BANG_DASH_DASH_STATE;
                break;
            default:
                state                           = HTML_TOKENIZER_COMMENT_END_DASH_STATE;
                consume                         = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-less-than-sign-bang-dash-dash-state
        case HTML_TOKENIZER_COMMENT_LESS_THAN_BANG_DASH_DASH_STATE:
            if (!is_eof && cp != '>')
            {
                status                          = HTML_TOKENIZER_NESTED_COMMENT;
            }
            state                               = HTML_TOKENIZER_COMMENT_END_STATE;
            consume                             = false;
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-end-dash-state
        case HTML_TOKENIZER_COMMENT_END_DASH_STATE:
            if (is_eof)
            {
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_COMMENT;
                break;
            }

            switch (cp)
            {
            case '-':
                state                           = HTML_TOKENIZER_COMMENT_END_STATE;
                break;
            default:
                update_data('-');
                consume                         = false;
                state                           = HTML_TOKENIZER_COMMENT_STATE;
                
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-end-state
        case HTML_TOKENIZER_COMMENT_END_STATE:
            if (is_eof)
            {
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_COMMENT;
                break;
            }

            switch (cp)
            {
            case '>':
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
                break;
            case '!':
                state                           = HTML_TOKENIZER_COMMENT_END_BANG_STATE;
                break;
            case '-':
                update_data('-');
                break;
            default:
                update_data('-');
                update_data('-');
                consume                         = false;
                state                           = HTML_TOKENIZER_COMMENT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-end-bang-state
        case HTML_TOKENIZER_COMMENT_END_BANG_STATE:
            if (is_eof)
            {
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_IN_COMMENT;
                break;
            }

            switch (cp)
            {
            case '-':
                update_data('-');
                update_data('-');
                update_data('!');
                state                           = HTML_TOKENIZER_COMMENT_END_DASH_STATE;
                break;
            case '>':
                emit_token();
                state                           = HTML_TOKENIZER_DATA_STATE;
                status                          = HTML_TOKENIZER_INCORRECTLY_CLOSED_COMMENT;
                break;
            default:
                update_data('-');
                update_data('-');
                update_data('!');
                consume                         = false;
                state                           = HTML_TOKENIZER_COMMENT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#doctype-state
        case HTML_TOKENIZER_DOCTYPE_STATE:
            if (is_eof)
            {
                init_token(HTML_DOCTYPE_TOKEN);
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {
                state                           = HTML_TOKENIZER_BEFORE_DOCTYPE_NAME_STATE;
            }
            else if (cp == '>')
            {
                consume                         = false;
                state                           = HTML_TOKENIZER_BEFORE_DOCTYPE_NAME_STATE;
            }
            else
            {
                consume                         = false;
                state                           = HTML_TOKENIZER_BEFORE_DOCTYPE_NAME_STATE;
                status                          = HTML_TOKENIZER_MISSING_WHITESPACE_BEFORE_DOCTYPE_NAME;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#before-doctype-name-state
        case HTML_TOKENIZER_BEFORE_DOCTYPE_NAME_STATE:
            if (is_eof)
            {
                init_token(HTML_DOCTYPE_TOKEN);
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {
                
            }
            else if (utf8_is_upper_alpha(cp))
            {
                state                           = HTML_TOKENIZER_DOCTYPE_NAME_STATE;

                init_token(HTML_DOCTYPE_TOKEN);
                unsigned char c = (unsigned char)cp;
                update_name(c + 0x20);

            }
            else if (cp == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                state                           = HTML_TOKENIZER_DOCTYPE_NAME_STATE;

                init_token(HTML_DOCTYPE_TOKEN);
                update_name(0xef);
                update_name(0xbf);
                update_name(0xbd);
            }
            else if (cp == '>')
            {
                init_token(HTML_DOCTYPE_TOKEN);
                tokens[token_idx].force_quirks  = true;
                state                           = HTML_TOKENIZER_DATA_STATE;
                status                          = HTML_TOKENIZER_MISSING_DOCTYPE_NAME;
                emit_token();
            }
            else
            {
                state                           = HTML_TOKENIZER_DOCTYPE_NAME_STATE;
                init_token(HTML_DOCTYPE_TOKEN);
                update_name_from_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#doctype-name-state
        case HTML_TOKENIZER_DOCTYPE_NAME_STATE:
            if (is_eof)
            {
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {
                state                           = HTML_TOKENIZER_AFTER_DOCTYPE_NAME_STATE;
            }
            else if (cp == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(cp))
            {
                unsigned char c = (unsigned char)cp;
                update_name(c + 0x20);
            }
            else if (cp == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_name(0xef);
                update_name(0xbf);
                update_name(0xbd);
            }
            else
            {
                update_name_from_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-name-state
        case HTML_TOKENIZER_AFTER_DOCTYPE_NAME_STATE:
            if (is_eof)
            {
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {

            }
            else if (cp == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else
            {
                if (match_segment(public_segment, public_segment_size, CASE_INSENSITIVE_MATCH))
                {
                    buf_cur                        += public_segment_size;
                    state                           = HTML_TOKENIZER_AFTER_DOCTYPE_PUBLIC_KEYWORD_STATE;
                    consume                         = false;
                }
                else if (match_segment(system_segment, system_segment_size, CASE_INSENSITIVE_MATCH))
                {
                    buf_cur                        += system_segment_size;
                    state                           = HTML_TOKENIZER_AFTER_DOCTYPE_SYSTEM_KEYWORD_STATE;
                    consume                         = false;
                }
                else
                {
                    tokens[token_idx].force_quirks  = true;
                    consume                         = false;
                    state                           = HTML_TOKENIZER_BOGUS_DOCTYPE_STATE;
                    status                          = HTML_TOKENIZER_INVALID_CHARACTER_SEQUENCE_AFTER_DOCTYPE_NAME;
                }
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-public-keyword-state
        case HTML_TOKENIZER_AFTER_DOCTYPE_PUBLIC_KEYWORD_STATE:
            if (is_eof)
            {
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {
                state                               = HTML_TOKENIZER_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER_STATE;
            }
            else if (cp == '"')
            {
                // todo: missing vs empty identifier
                state                               = HTML_TOKENIZER_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED_STATE;
                status                              = HTML_TOKENIZER_MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD;
            }
            else if (cp == '\'')
            {
                // todo: missing vs empty identifier
                state                               = HTML_TOKENIZER_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED_STATE;
                status                              = HTML_TOKENIZER_MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD;
            }
            else if (cp == '>')
            {
                tokens[token_idx].force_quirks      = true;
                state                               = HTML_TOKENIZER_DATA_STATE;
                status                              = HTML_TOKENIZER_MISSING_DOCTYPE_PUBLIC_IDENTIFIER;
                emit_token();
            }
            else
            {
                tokens[token_idx].force_quirks      = true;
                consume                             = false;
                state                               = HTML_TOKENIZER_BOGUS_DOCTYPE_STATE;
                status                              = HTML_TOKENIZER_MISSING_QUOTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#before-doctype-public-identifier-state
        case HTML_TOKENIZER_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER_STATE:
            if (is_eof)
            {
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {

            }
            else if (cp == '"')
            {
                // todo: missing vs empty identifier
                state                               = HTML_TOKENIZER_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED_STATE;
            }
            else if (cp == '\'')
            {
                // todo: missing vs empty identifier
                state                               = HTML_TOKENIZER_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED_STATE;
            }
            else if (cp == '>')
            {
                tokens[token_idx].force_quirks      = true;
                state                               = HTML_TOKENIZER_DATA_STATE;
                status                              = HTML_TOKENIZER_MISSING_DOCTYPE_PUBLIC_IDENTIFIER;
                emit_token();
            }
            else
            {
                tokens[token_idx].force_quirks      = true;
                consume                             = false;
                state                               = HTML_TOKENIZER_BOGUS_DOCTYPE_STATE;
                status                              = HTML_TOKENIZER_MISSING_QUOTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#doctype-public-identifier-(double-quoted)-state
        case HTML_TOKENIZER_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED_STATE:
            if (is_eof)
            {
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (cp == '"')
            {
                state                               = HTML_TOKENIZER_AFTER_DOCTYPE_PUBLIC_IDENTIFIER_STATE;
            }
            else if (cp == 0)
            {
                status                              = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_public_id(0xef);
                update_public_id(0xbf);
                update_public_id(0xbd);
            }
            else if (cp == '>')
            {
                tokens[token_idx].force_quirks      = true;
                state                               = HTML_TOKENIZER_DATA_STATE;
                status                              = HTML_TOKENIZER_ABRUPT_DOCTYPE_PUBLIC_IDENTIFIER;
                emit_token();
            }
            else
            {
                update_public_id_from_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#doctype-public-identifier-(single-quoted)-state
        case HTML_TOKENIZER_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED_STATE:
            if (is_eof)
            {
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (cp == '\'')
            {
                state                               = HTML_TOKENIZER_AFTER_DOCTYPE_PUBLIC_IDENTIFIER_STATE;
            }
            else if (cp == 0)
            {
                status                              = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_public_id(0xef);
                update_public_id(0xbf);
                update_public_id(0xbd);
            }
            else if (cp == '>')
            {
                tokens[token_idx].force_quirks      = true;
                state                               = HTML_TOKENIZER_DATA_STATE;
                status                              = HTML_TOKENIZER_ABRUPT_DOCTYPE_PUBLIC_IDENTIFIER;
                emit_token();
            }
            else
            {
                update_public_id_from_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-public-identifier-state
        case HTML_TOKENIZER_AFTER_DOCTYPE_PUBLIC_IDENTIFIER_STATE:
            if (is_eof)
            {
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {
                state                               = HTML_TOKENIZER_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS_STATE;
            }
            else if (cp == '>')
            {
                state                               = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (cp == '"')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE;
                status                              = HTML_TOKENIZER_MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS;
            }
            else if (cp == '\'')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED_STATE;
                status                              = HTML_TOKENIZER_MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS;
            }
            else
            {
                tokens[token_idx].force_quirks      = true;
                consume                             = false;
                state                               = HTML_TOKENIZER_BOGUS_DOCTYPE_STATE;
                status                              = HTML_TOKENIZER_MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#between-doctype-public-and-system-identifiers-state
        case HTML_TOKENIZER_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS_STATE:
            if (is_eof)
            {
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {

            }
            else if (cp == '>')
            {
                state                               = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (cp == '"')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE;
            }
            else if (cp == '\'')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED_STATE;
            }
            else
            {
                tokens[token_idx].force_quirks      = true;
                consume                             = false;
                state                               = HTML_TOKENIZER_BOGUS_DOCTYPE_STATE;
                status                              = HTML_TOKENIZER_MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-system-keyword-state
        case HTML_TOKENIZER_AFTER_DOCTYPE_SYSTEM_KEYWORD_STATE:
            if (is_eof)
            {
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                              = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {
                state                               = HTML_TOKENIZER_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER_STATE;
            }
            else if (cp == '"')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE;
                status                              = HTML_TOKENIZER_MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD;
            }
            else if (cp == '\'')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED_STATE;
                status                              = HTML_TOKENIZER_MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD;
            }
            else if (cp == '>')
            {
                tokens[token_idx].force_quirks      = true;
                state                               = HTML_TOKENIZER_DATA_STATE;
                status                              = HTML_TOKENIZER_MISSING_DOCTYPE_SYSTEM_IDENTIFIER;
                emit_token();
            }
            else
            {
                tokens[token_idx].force_quirks      = true;
                consume                             = false;
                state                               = HTML_TOKENIZER_BOGUS_DOCTYPE_STATE;
                status                              = HTML_TOKENIZER_MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#before-doctype-system-identifier-state
        case HTML_TOKENIZER_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER_STATE:
            if (is_eof)
            {
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {

            }
            else if (cp == '"')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE;
            }
            else if (cp == '\'')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED_STATE;
            }
            else if (cp == '>')
            {
                tokens[token_idx].force_quirks      = true;
                state                               = HTML_TOKENIZER_DATA_STATE;
                status                              = HTML_TOKENIZER_MISSING_DOCTYPE_SYSTEM_IDENTIFIER;
                emit_token();
            }
            else
            {
                tokens[token_idx].force_quirks      = true;
                consume                             = false;
                state                               = HTML_TOKENIZER_BOGUS_DOCTYPE_STATE;
                status                              = HTML_TOKENIZER_MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#doctype-system-identifier-(double-quoted)-state
        case HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE:
            if (is_eof)
            {
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            switch (cp)
            {
            case '"':
                state                               = HTML_TOKENIZER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER_STATE;
                break;
            case 0:
                status                              = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_system_id(0xef);
                update_system_id(0xbf);
                update_system_id(0xbd);
                break;
            case '>':
                tokens[token_idx].force_quirks      = true;
                state                               = HTML_TOKENIZER_DATA_STATE;
                status                              = HTML_TOKENIZER_ABRUPT_DOCTYPE_SYSTEM_IDENTIFIER;
                emit_token();
                break;
            default:
                update_system_id_from_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#doctype-system-identifier-(single-quoted)-state
        case HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED_STATE:
            if (is_eof)
            {
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            switch (cp)
            {
            case '\'':
                state                               = HTML_TOKENIZER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER_STATE;
                break;
            case 0:
                status                              = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_system_id(0xef);
                update_system_id(0xbf);
                update_system_id(0xbd);
                break;
            case '>':
                tokens[token_idx].force_quirks      = true;
                state                               = HTML_TOKENIZER_DATA_STATE;
                status                              = HTML_TOKENIZER_ABRUPT_DOCTYPE_SYSTEM_IDENTIFIER;
                emit_token();
                break;
            default:
                update_system_id_from_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-system-identifier-state
        case HTML_TOKENIZER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER_STATE:
            if (is_eof)
            {
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (cp == '\t' || cp == '\n' || cp == '\f' || cp == ' ')
            {

            }
            else if (cp == '>')
            {
                state                               = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else
            {
                state                               = HTML_TOKENIZER_BOGUS_DOCTYPE_STATE;
                status                              = HTML_TOKENIZER_UNEXPECTED_CHARACTER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
                consume                             = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#bogus-doctype-state
        case HTML_TOKENIZER_BOGUS_DOCTYPE_STATE:
            if (is_eof)
            {
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                break;
            }

            switch(cp)
            {
            case '>':
                state                               = HTML_TOKENIZER_DATA_STATE;
                emit_token();
                break;
            case 0:
                status                              = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                break;
            default:
                break;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#cdata-section-state
        case HTML_TOKENIZER_CDATA_SECTION_STATE:
            if (is_eof)
            {
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                              = HTML_TOKENIZER_EOF_IN_CDATA;
                break;
            }

            switch(cp)
            {
            case ']':
                state                               = HTML_TOKENIZER_CDATA_SECTION_BRACKET_STATE;
                break;
            default:
                init_token(HTML_CHARACTER_TOKEN);
                update_data_from_buf();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#cdata-section-bracket-state
        case HTML_TOKENIZER_CDATA_SECTION_BRACKET_STATE:
            switch(cp)
            {
            case ']':
                init_token(HTML_CHARACTER_TOKEN);
                update_data(']');
                emit_token();
                break;
            case '>':
                state                               = HTML_TOKENIZER_DATA_STATE;
                break;
            default:
                init_token(HTML_CHARACTER_TOKEN);
                update_data(']');
                emit_token();
                state                               = HTML_TOKENIZER_CDATA_SECTION_END_STATE;
                consume                             = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#cdata-section-end-state
        case HTML_TOKENIZER_CDATA_SECTION_END_STATE:
            switch (cp)
            {
            case ']':
                init_token(HTML_CHARACTER_TOKEN);
                update_data(']');
                emit_token();
                break;
            case '>':
                state                               = HTML_TOKENIZER_DATA_STATE;
                break;
            default:
                init_token(HTML_CHARACTER_TOKEN);
                update_data(']');
                emit_token();
                init_token(HTML_CHARACTER_TOKEN);
                update_data(']');
                emit_token();
                consume                             = false;
                state                               = HTML_TOKENIZER_CDATA_SECTION_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#character-reference-state
        case HTML_TOKENIZER_CHARACTER_REFERENCE_STATE:
            clear_tmp_buf();
            update_tmp_buf('&');
            if (utf8_is_alphanumeric(cp))
            {
                consume                             = false;
                state                               = HTML_TOKENIZER_NAMED_CHARACTER_REFERENCE_STATE;
            }
            else if (cp == '#')
            {
                update_tmp_buf('#');
                state                               = HTML_TOKENIZER_NUMERIC_CHARACTER_REFERENCE_STATE;
            }
            else
            {
                flush_cps_consumed_as_char_ref(return_state);
                consume                             = false;
                state                               = return_state;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#named-character-reference-state
        case HTML_TOKENIZER_NAMED_CHARACTER_REFERENCE_STATE:
            ;
            // maximum possible size of named chars
            uint32_t max_size   = buf_cur + 33;
            uint32_t cursor_offset = 0;
            max_size            = max_size > buf_size ? buf_size : max_size;
            bool found          = false;

            for (uint32_t i = buf_cur; i < max_size; i++)
            {
                update_tmp_buf(buf[i]);
                cursor_offset += 1;
                if (buf[i] == ';') { break; }
            }

            while (!found)
            {
                if (tmp_buf_size == 1) { break; }

                hash_str_t hash_val = hash_str_compute(tmp_buf, tmp_buf_size);
                uint32_t named_cp = html_get_named_char_ref(hash_val);

                if (named_cp > 0)
                {
                    buf_cur  = buf_cur + cursor_offset;
                    consume = false;
                    state   = return_state;
                    found   = true;

                    // todo: this is ugly af
                    if (in_attribute(return_state) && (buf[buf_cur - 1] != ';' && (buf[buf_cur] == '=' || utf8_is_alphanumeric(buf[buf_cur]))))
                    {
                        flush_cps_consumed_as_char_ref(return_state);
                    }
                    else
                    {
                        clear_tmp_buf();
                        int32_t bytes = utf8_encode(named_cp, tmp_buf);

                        tmp_buf_size    = (uint32_t)bytes;
                        flush_cps_consumed_as_char_ref(return_state);
                    }

                    break;
                }

                cursor_offset--;
                tmp_buf_size--;
            }
            
            if (found) { break; }

            cursor_offset = 0;

            for (uint32_t i = buf_cur; i < max_size; i++)
            {
                update_tmp_buf(buf[i]);
                cursor_offset += 1;
                if (buf[i] == ';') { break; }
            }

            flush_cps_consumed_as_char_ref(return_state);
            buf_cur              = buf_cur + cursor_offset;
            consume             = false;
            state               = return_state;
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#ambiguous-ampersand-state
        case HTML_TOKENIZER_AMBIGUOUS_AMPERSAND_STATE:
            if (utf8_is_alphanumeric(cp))
            {
                if (in_attribute(return_state))
                {
                    unsigned char c = (unsigned char)cp;
                    update_attr_val(c);
                }
                else
                {
                    unsigned char c = (unsigned char)cp;
                    init_token(HTML_CHARACTER_TOKEN);
                    update_data(c);
                    emit_token();
                }
            }
            else if (cp == ';')
            {
                // todo: parse errors
                consume                             = false;
                state                               = return_state;
            }
            else
            {
                consume                             = false;
                state                               = return_state;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#numeric-character-reference-state
        case HTML_TOKENIZER_NUMERIC_CHARACTER_REFERENCE_STATE:

            character_reference_code = 0;

            if (cp == 'x' || cp == 'X')
            {
                unsigned char c = (unsigned char)cp;
                update_tmp_buf(c);
                state                               = HTML_TOKENIZER_HEXADECIMAL_CHARACTER_REFERENCE_START_STATE;
            }
            else
            {
                consume                             = false;
                state                               = HTML_TOKENIZER_DECIMAL_CHARACTER_REFERENCE_START_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#hexadecimal-character-reference-start-state
        case HTML_TOKENIZER_HEXADECIMAL_CHARACTER_REFERENCE_START_STATE:
            if (utf8_is_hex(cp))
            {
                consume                             = false;
                state                               = HTML_TOKENIZER_HEXADECIMAL_CHARACTER_REFERENCE_STATE;
            }
            else
            {
                flush_cps_consumed_as_char_ref(return_state);

                status                              = HTML_TOKENIZER_ABSENCE_OF_DIGITS_IN_NUMERIC_CHARACTER_REFERENCE;
                consume                             = false;
                state                               = return_state;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#decimal-character-reference-start-state
        case HTML_TOKENIZER_DECIMAL_CHARACTER_REFERENCE_START_STATE:
            if (utf8_is_digit(cp))
            {
                consume                             = false;
                state                               = HTML_TOKENIZER_DECIMAL_CHARACTER_REFERENCE_STATE;
            }
            else
            {
                flush_cps_consumed_as_char_ref(return_state);

                status                              = HTML_TOKENIZER_ABSENCE_OF_DIGITS_IN_NUMERIC_CHARACTER_REFERENCE;
                consume                             = false;
                state                               = return_state;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#hexadecimal-character-reference-state
        case HTML_TOKENIZER_HEXADECIMAL_CHARACTER_REFERENCE_STATE:
            ;
            uint32_t old_hex_val                        = character_reference_code;
            if (is_eof)
            {
                consume                             = false;
                state                               = HTML_TOKENIZER_NUMERIC_CHARACTER_REFERENCE_END_STATE;
                status                              = HTML_TOKENIZER_MISSING_SEMICOLON_AFTER_CHARACTER_REFERENCE;
            }
            else if (utf8_is_digit(cp))
            {
                character_reference_code            *= 16;
                character_reference_code            += cp - 0x30;
            }
            else if (utf8_is_upper_hex(cp))
            {
                character_reference_code            *= 16;
                character_reference_code            += cp - 0x37;
            }
            else if (utf8_is_lower_hex(cp))
            {
                character_reference_code            *= 16;
                character_reference_code            += cp - 0x57;
            }
            else if (cp == ';')
            {
                state                               = HTML_TOKENIZER_NUMERIC_CHARACTER_REFERENCE_END_STATE;
            }
            else
            {
                consume                             = false;
                state                               = HTML_TOKENIZER_NUMERIC_CHARACTER_REFERENCE_END_STATE;
                status                              = HTML_TOKENIZER_MISSING_SEMICOLON_AFTER_CHARACTER_REFERENCE;
            }

            if (old_hex_val > character_reference_code)
            {
                character_reference_code        = 0xFFFFFFFF;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#decimal-character-reference-state
        case HTML_TOKENIZER_DECIMAL_CHARACTER_REFERENCE_STATE:
            if (is_eof)
            {
                consume                             = false;
                state                               = HTML_TOKENIZER_NUMERIC_CHARACTER_REFERENCE_END_STATE;
                status                              = HTML_TOKENIZER_MISSING_SEMICOLON_AFTER_CHARACTER_REFERENCE;
            }
            else if (utf8_is_digit(cp))
            {
                uint32_t old_val                    = character_reference_code;
                character_reference_code           *= 10;
                character_reference_code           += cp - 0x30;

                if (old_val > character_reference_code)
                {
                    character_reference_code        = 0xFFFFFFFF;
                }
            }
            else if (cp == ';')
            {
                state                               = HTML_TOKENIZER_NUMERIC_CHARACTER_REFERENCE_END_STATE;
            }
            else
            {
                consume                             = false;
                state                               = HTML_TOKENIZER_NUMERIC_CHARACTER_REFERENCE_END_STATE;
                status                              = HTML_TOKENIZER_MISSING_SEMICOLON_AFTER_CHARACTER_REFERENCE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#numeric-character-reference-end-state
        case HTML_TOKENIZER_NUMERIC_CHARACTER_REFERENCE_END_STATE:
            if (character_reference_code == 0)
            {
                status                              = HTML_TOKENIZER_NULL_CHARACTER_REFERENCE;
                character_reference_code            = 0xfffd;
            }
            else if (character_reference_code > 0x10ffff)
            {
                status                              = HTML_TOKENIZER_CHARACTER_REFERENCE_OUTSIDE_UNICODE_RANGE;
                character_reference_code            = 0xfffd;
            }
            else if (is_surrogate(character_reference_code))
            {
                status                              = HTML_TOKENIZER_SURROGATE_CHARACTER_REFERENCE;
                character_reference_code            = 0xfffd;
            }
            else if (is_noncharacter(character_reference_code))
            {
                status                              = HTML_TOKENIZER_NONCHARACTER_CHARACTER_REFERENCE;
            }
            else if (character_reference_code == 0x0d || (utf8_is_control(character_reference_code) && !utf8_is_whitespace(character_reference_code)))
            {
                status                              = HTML_TOKENIZER_CONTROL_CHARACTER_REFERENCE;
                uint32_t result                     = 0;
                int32_t char_idx                    = -1;

                for (uint32_t k = 0; k < sizeof(numeric_char_refs) / sizeof(numeric_char_ref_t); k++)
                {
                    if (numeric_char_refs[k].key == character_reference_code)
                    {
                        result = numeric_char_refs[k].value;
                        char_idx = (int32_t)k;
                    }
                }

                if (char_idx != -1) { character_reference_code = result; }
            }

            clear_tmp_buf();
            consume                         = false;
            unsigned char char_ref_buf[4]   = { 0 };
            int32_t bytes                   = utf8_encode(character_reference_code, char_ref_buf);
            for (int32_t i = 0; i < bytes; i++)
            {
                update_tmp_buf(char_ref_buf[i]);
            }

            state                           = return_state;

            flush_cps_consumed_as_char_ref(return_state);

            break;
        }

        if (consume && cp_len > 0)
        {
            buf_cur += (uint32_t)cp_len;
        }
    }

    return status;
}


void html_tokenizer_set_last_emitted_start_tag(hash_str_t tag)
{
    last_emitted_start_tag = tag;
}


void html_tokenizer_set_state(html_tokenizer_state_e new_state)
{
    state           = new_state;
    return_state    = new_state;
}


void html_tokenizer_free()
{
    
}


void html_tokenizer_global_free()
{
    if (buf) { free(buf); }
    buf = NULL;
}
