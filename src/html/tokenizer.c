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
static uint32_t buf_size                                                = 0;
static uint32_t buf_cur                                                 = 0;
static html_token_t* tokens                                             = NULL;
static uint32_t token_idx                                               = 0;
static uint32_t max_tokens                                              = 0;
static html_tokenizer_state_e state                                     = HTML_TOKENIZER_DATA_STATE;
static html_tokenizer_state_e return_state                              = HTML_TOKENIZER_DATA_STATE;
static unsigned char tmp_buf[MAX_TEMP_BUFFER_SIZE]                  = { 0 };
static uint32_t tmp_buf_size                                        = 0;
static hash_str_t last_emitted_start_tag                                = 0;
static int32_t bytes_read                                               = 0;
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


static bool is_noncharacter(uint32_t code_point)
{
    return (code_point >= 0xfdd0 && code_point <= 0xfdef) || 
            code_point == 0xfffe || code_point == 0xffff ||
            code_point == 0x1fffe || code_point == 0x1ffff ||
            code_point == 0x2fffe || code_point == 0x2ffff ||
            code_point == 0x3fffe || code_point == 0x3ffff ||
            code_point == 0x4fffe || code_point == 0x4ffff || 
            code_point == 0x5fffe || code_point == 0x5ffff ||
            code_point == 0x6fffe || code_point == 0x6ffff ||
            code_point == 0x7fffe || code_point == 0x7ffff ||
            code_point == 0x8fffe || code_point == 0x8ffff ||
            code_point == 0x9fffe || code_point == 0x9ffff || 
            code_point == 0xafffe || code_point == 0xaffff || 
            code_point == 0xbfffe || code_point == 0xbffff ||
            code_point == 0xcfffe || code_point == 0xcffff ||
            code_point == 0xdfffe || code_point == 0xdffff ||
            code_point == 0xefffe || code_point == 0xeffff ||
            code_point == 0xffffe || code_point == 0xfffff ||
            code_point == 0x10fffe || code_point == 0x10ffff;
}


static bool is_leading_surrogate(uint32_t code_point)
{
    return code_point >= 0xd800 && code_point <= 0xdbff;
}


static bool is_trailing_surrogate(uint32_t code_point)
{
    return code_point >= 0xdc00 && code_point <= 0xdfff;
}


static bool is_surrogate(uint32_t code_point)
{
    return is_leading_surrogate(code_point) || is_trailing_surrogate(code_point);
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

static void create_char_token_from_buffer()
{
    assert(bytes_read >= 0);

    init_token(HTML_CHARACTER_TOKEN);
    uint32_t read = (uint32_t)bytes_read;
    uint32_t data_size = tokens[token_idx].data_size;

    for (uint32_t i = 0; i < read; i++)
    {
        tokens[token_idx].data[data_size + i] = buf[buf_cur + i];
    }
    tokens[token_idx].data_size = data_size + read;
}

static void create_replacement_char_token()
{
    // u+FFFD = 0xef 0xbf 0xbd
    init_token(HTML_CHARACTER_TOKEN);

    uint32_t data_size = tokens[token_idx].data_size;
    tokens[token_idx].data[data_size] = 0xef;
    data_size++;
    tokens[token_idx].data[data_size] = 0xbf;
    data_size++;
    tokens[token_idx].data[data_size] = 0xbd;
    data_size++;
    tokens[token_idx].data_size = data_size;
}

static void create_char_token(unsigned char c)
{
    init_token(HTML_CHARACTER_TOKEN);

    uint32_t data_size = tokens[token_idx].data_size;
    tokens[token_idx].data[data_size] = c;
    tokens[token_idx].data_size++;
}

static void create_tag_token(html_token_type_e type)
{
    tokens[token_idx].is_valid          = true;
    tokens[token_idx].type              = type;
    tokens[token_idx].name_size         = 0;

    memset(tokens[token_idx].name, 0, sizeof(tokens[token_idx].name));
}

static void update_tag_token_from_buffer()
{
    assert(bytes_read >= 0);

    uint32_t read = (uint32_t)bytes_read;
    uint32_t name_size = tokens[token_idx].name_size;

    if (name_size + read >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    for (uint32_t i = 0; i < read; i++)
    {
        tokens[token_idx].name[name_size + i] = buf[buf_cur + i];
    }

    tokens[token_idx].name_size = name_size + read;
}

static void update_tag_token(unsigned char c)
{
    uint32_t name_size = tokens[token_idx].name_size;
    if (name_size + 1 >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].name[name_size] = c;
    tokens[token_idx].name_size++;
}

static void update_tag_token_replacement()
{
    uint32_t name_size = tokens[token_idx].name_size;
    if (name_size + 2 >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].name[name_size] = 0xef;
    name_size++;
    tokens[token_idx].name[name_size] = 0xbf;
    name_size++;
    tokens[token_idx].name[name_size] = 0xbd;
    name_size++;
    tokens[token_idx].name_size = name_size;
}

static void init_attribute()
{
    html_token_t* token = &tokens[token_idx];
    html_token_attribute_t* attribute = &(token->attributes[token->attributes_size]);
    memset(attribute->name, 0, sizeof(attribute->name));
    memset(attribute->value, 0, sizeof(attribute->value));
    attribute->name_size = 0;
    attribute->value_size = 0;
}

static void create_attribute_from_buffer()
{
    init_attribute();

    html_token_t* token = &tokens[token_idx];
    html_token_attribute_t* attribute = &(token->attributes[token->attributes_size]);

    assert(bytes_read >= 0);

    uint32_t read = (uint32_t)bytes_read;
    uint32_t name_size = attribute->name_size;

    for (uint32_t i = 0; i < read; i++)
    {
        attribute->name[name_size + i] = buf[buf_cur + i];
    }

    attribute->name_size = name_size + read;
}

static void update_attribute_name(unsigned char c)
{
    html_token_t* token                     = &tokens[token_idx];
    html_token_attribute_t* attribute       = &(token->attributes[token->attributes_size]);
    attribute->name[attribute->name_size]   = c;
    attribute->name_size++;
}

static void update_attribute_name_replacement_char()
{
    html_token_t* token                     = &tokens[token_idx];
    html_token_attribute_t* attribute       = &(token->attributes[token->attributes_size]);
    attribute->name[attribute->name_size]   = 0xef;
    attribute->name_size++;
    attribute->name[attribute->name_size]   = 0xbf;
    attribute->name_size++;
    attribute->name[attribute->name_size]   = 0xbd;
    attribute->name_size++;
}

static void update_attribute_value_replacement_char()
{
    html_token_t* token                     = &tokens[token_idx];
    html_token_attribute_t* attribute       = &(token->attributes[token->attributes_size]);
    attribute->value[attribute->value_size]   = 0xef;
    attribute->value_size++;
    attribute->value[attribute->value_size]   = 0xbf;
    attribute->value_size++;
    attribute->value[attribute->value_size]   = 0xbd;
    attribute->value_size++;
}

static void update_attribute_value_from_buffer()
{
    html_token_t* token = &tokens[token_idx];
    html_token_attribute_t* attribute = &(token->attributes[token->attributes_size]);

    assert(bytes_read >= 0);

    uint32_t read = (uint32_t)bytes_read;
    uint32_t value_size = attribute->value_size;
    
    if (value_size > HTML_TOKEN_MAX_NAME_LEN) { return; }

    for (uint32_t i = 0; i < read; i++)
    {
        attribute->value[value_size + i] = buf[buf_cur + i];
    }

    attribute->value_size = value_size + read;
}

static void update_attribute_value(unsigned char c)
{
    html_token_t* token                     = &tokens[token_idx];
    html_token_attribute_t* attribute       = &(token->attributes[token->attributes_size]);
    attribute->value[attribute->value_size]   = c;
    attribute->value_size++;
}

static void update_attribute_name_from_buffer()
{
    html_token_t* token = &tokens[token_idx];
    html_token_attribute_t* attribute = &(token->attributes[token->attributes_size]);

    assert(bytes_read >= 0);

    uint32_t read = (uint32_t)bytes_read;
    uint32_t name_size = attribute->name_size;
    
    if (name_size + read > HTML_TOKEN_MAX_NAME_LEN) { return; }

    for (uint32_t i = 0; i < read; i++)
    {
        attribute->name[name_size + i] = buf[buf_cur + i];
    }

    attribute->name_size = name_size + read;
}

static void create_comment_token()
{
    tokens[token_idx].is_valid           = true;
    tokens[token_idx].type               = HTML_COMMENT_TOKEN;
    tokens[token_idx].data_size          = 0;

    memset(tokens[token_idx].data, 0, sizeof(tokens[token_idx].data));
}

static void update_comment_token_replacement_char()
{
    uint32_t data_size = tokens[token_idx].data_size;

    if (data_size >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].data[data_size] = 0xef;
    data_size++;
    tokens[token_idx].data[data_size] = 0xbf;
    data_size++;
    tokens[token_idx].data[data_size] = 0xbd;
    data_size++;
    tokens[token_idx].data_size = data_size;
}

static void update_comment_token_from_buffer()
{
    if (bytes_read <= 0) { return; }

    uint32_t read = (uint32_t)bytes_read;
    uint32_t data_size = tokens[token_idx].data_size;

    if (data_size >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    for (uint32_t i = 0; i < read; i++)
    {
        tokens[token_idx].data[data_size + i] = buf[buf_cur + i];
    }

    tokens[token_idx].data_size += read;
}

static void update_comment_token(unsigned char c)
{
    uint32_t data_size = tokens[token_idx].data_size;

    if (data_size >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].data[data_size] = c;
    data_size++;
    tokens[token_idx].data_size = data_size;
}

static void create_doctype_token()
{
    tokens[token_idx].is_valid          = true;
    tokens[token_idx].type              = HTML_DOCTYPE_TOKEN;
    tokens[token_idx].name_size         = 0;
    tokens[token_idx].public_id_size    = 0;
    tokens[token_idx].system_id_size    = 0;
    tokens[token_idx].force_quirks      = false;

    memset(tokens[token_idx].name, 0, sizeof(tokens[token_idx].name));
    memset(tokens[token_idx].public_id, 0, sizeof(tokens[token_idx].public_id));
    memset(tokens[token_idx].system_id, 0, sizeof(tokens[token_idx].system_id));
}

static void update_doctype_token_name(unsigned char c)
{
    uint32_t name_size = tokens[token_idx].name_size;

    if (name_size + 1 >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].name[name_size] = c;
    tokens[token_idx].name_size++;
}

static void update_doctype_token_name_from_buffer()
{
    assert(bytes_read >= 0);

    uint32_t read = (uint32_t)bytes_read;
    uint32_t name_size = tokens[token_idx].name_size;

    if (name_size + read >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    for (uint32_t i = 0; i < read; i++)
    {
        tokens[token_idx].name[name_size + i] = buf[buf_cur + i];
    }

    tokens[token_idx].name_size = name_size + read;
}

static void update_doctype_token_name_replacement_char()
{
    uint32_t name_size = tokens[token_idx].name_size;

    if (name_size + 2 >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].name[name_size] = 0xef;
    name_size++;
    tokens[token_idx].name[name_size] = 0xbf;
    name_size++;
    tokens[token_idx].name[name_size] = 0xbd;
    name_size++;
    tokens[token_idx].name_size = name_size;
}

static void update_doctype_token_public_identifier_replacement_char()
{
    uint32_t public_id_size = tokens[token_idx].public_id_size;

    if (public_id_size + 2 >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].public_id[public_id_size] = 0xef;
    public_id_size++;
    tokens[token_idx].public_id[public_id_size] = 0xbf;
    public_id_size++;
    tokens[token_idx].public_id[public_id_size] = 0xbd;
    public_id_size++;
    tokens[token_idx].public_id_size = public_id_size;
}

static void update_doctype_public_identifier_from_buffer()
{
    assert(bytes_read >= 0);

    uint32_t read = (uint32_t)bytes_read;
    uint32_t public_id_size = tokens[token_idx].public_id_size;

    if (public_id_size + read >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    for (uint32_t i = 0; i < read; i++)
    {
        tokens[token_idx].public_id[public_id_size + i] = buf[buf_cur + i];
    }

    tokens[token_idx].public_id_size += read;
}

static void update_doctype_token_system_identifier_replacement_char()
{
    uint32_t system_id_size = tokens[token_idx].system_id_size;

    if (system_id_size + 2 >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].system_id[system_id_size] = 0xef;
    system_id_size++;
    tokens[token_idx].system_id[system_id_size] = 0xbf;
    system_id_size++;
    tokens[token_idx].system_id[system_id_size] = 0xbd;
    system_id_size++;
    tokens[token_idx].system_id_size = system_id_size;
}

static void update_doctype_token_system_identifier_from_buffer()
{
    assert(bytes_read >= 0);

    uint32_t read = (uint32_t)bytes_read;
    uint32_t system_id_size = tokens[token_idx].system_id_size;

    if (system_id_size + read >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    for (uint32_t i = 0; i < read; i++)
    {
        tokens[token_idx].system_id[system_id_size + i] = buf[buf_cur + i];
    }

    tokens[token_idx].system_id_size += read;
}

static void emit_attribute()
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

    if (is_duplicate)           { init_attribute(); return; }
    if (attr.name_size == 0)    { return; }

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
    uint32_t code_point = 0;
    int32_t bytes = 0;
    while (cur < tmp_buf_size)
    {
        bytes = utf8_decode(tmp_buf, tmp_buf_size, cur, &code_point);

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

static void update_tmp_buf_from_buffer()
{
    assert(bytes_read >= 0);

    uint32_t read = (uint32_t)bytes_read;

    for (uint32_t i = 0; i < read; i++)
    {
        tmp_buf[tmp_buf_size + i] = buf[buf_cur + i];
    }
    tmp_buf_size += read;
}

static void update_tmp_buf(unsigned char c)
{
    tmp_buf[tmp_buf_size] = c;
    tmp_buf_size++;
}

static void flush_tmp_buf_to_attribute_value()
{
    html_token_t* token = &tokens[token_idx];

    html_token_attribute_t* attr = &(token->attributes[token->attributes_size]);
    for (uint32_t i = 0; i < tmp_buf_size; i++)
    {
        attr->value[attr->value_size] = tmp_buf[i];
        attr->value_size++;
    }
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


static void flush_code_points_consumed_as_char_ref(html_tokenizer_state_e s)
{
    if (s == HTML_TOKENIZER_ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE ||
        s == HTML_TOKENIZER_ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE ||
        s == HTML_TOKENIZER_ATTRIBUTE_VALUE_UNQUOTED_STATE)
    {
        for (uint32_t i = 0; i < tmp_buf_size; i++)
        {
            update_attribute_value(tmp_buf[i]);
        }
    }
    else
    {
        emit_tmp_buf();
    }
}

/********************/
/* public functions */
/********************/

void html_tokenizer_init(const unsigned char* new_buffer, const uint32_t new_size, html_token_t* new_tokens, const uint32_t new_max_tokens)
{
    assert(new_buffer);

    buf                      = malloc(new_size + 1);
    buf[new_size]            = 0;
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
    uint32_t code_point;
    html_tokenizer_error_e status = HTML_TOKENIZER_OK;

    while (token_idx == 0)
    {
        assert(token_idx < max_tokens);

        bytes_read  = -1;
        consume     = true;

        if (buf_cur >= buf_size)
        {
            is_eof = true;
        }
        else
        {
            bytes_read = utf8_decode(buf, buf_size, buf_cur, &code_point);
            if (bytes_read <= -1) { return HTML_TOKENIZER_ERROR; }
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

            switch (code_point)
            {
            case '&':
                state                           = HTML_TOKENIZER_CHARACTER_REFERENCE_STATE;
                return_state                    = HTML_TOKENIZER_DATA_STATE;
                break;
            case '<':
                state                           = HTML_TOKENIZER_TAG_OPEN_STATE;
                break;
            default:
                if (code_point == 0) { status   = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER; }
                create_char_token_from_buffer();
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

            switch (code_point)
            {
            case '&':
                return_state                    = HTML_TOKENIZER_RCDATA_STATE;
                state                           = HTML_TOKENIZER_CHARACTER_REFERENCE_STATE;
                break;
            case '<':
                state                           = HTML_TOKENIZER_RCDATA_LESS_THAN_STATE;
                break;
            default:
                if (code_point == 0)
                {
                    create_replacement_char_token();
                    status                      = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                }
                else
                {
                    create_char_token_from_buffer();
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

            switch(code_point)
            {
            case '<':
                state                           = HTML_TOKENIZER_RAWTEXT_LESS_THAN_STATE;
                break;
            default:
                if (code_point == 0)
                {
                    status                      = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                    create_replacement_char_token();
                }
                else
                {
                    create_char_token_from_buffer();
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

            switch (code_point)
            {
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_LESS_THAN_STATE;
                break;
            default:
                if (code_point == 0)
                {
                    create_replacement_char_token();
                    status                      = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                }
                else
                {
                    create_char_token_from_buffer();
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

            if (code_point == 0)
            {
                create_replacement_char_token();
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
            }
            else
            {
                create_char_token_from_buffer();
            }

            emit_token();
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#tag-open-state
        case HTML_TOKENIZER_TAG_OPEN_STATE:
            if (is_eof)
            {
                create_char_token('<');
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_BEFORE_TAG_NAME;
                break;
            }

            if (code_point == '!')
            {
                state                           = HTML_TOKENIZER_MARKUP_DECLARATION_OPEN_STATE;
            }
            else if (code_point == '/')
            {
                state                           = HTML_TOKENIZER_END_TAG_OPEN_STATE;
            }
            else if (utf8_is_alpha(code_point))
            {
                create_tag_token(HTML_START_TOKEN);
                consume                         = false;
                state                           = HTML_TOKENIZER_TAG_NAME_STATE;
            }
            else if (code_point == '?')
            {
                create_comment_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_BOGUS_COMMENT_STATE;
                status                          = HTML_TOKENIZER_UNEXPECTED_QUOESTION_MARK_INSTEAD_OF_TAG_NAME;
            }
            else
            {
                create_char_token('<');
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
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status                          = HTML_TOKENIZER_EOF_BEFORE_TAG_NAME;
                break;
            }

            if (utf8_is_alpha(code_point))
            {
                create_tag_token(HTML_END_TOKEN);
                consume                         = false;
                state                           = HTML_TOKENIZER_TAG_NAME_STATE;
            }
            else if (code_point == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                status                          = HTML_TOKENIZER_MISSING_END_TAG_NAME;
            }
            else
            {
                create_comment_token();
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

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '/')
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
            }
            else if (code_point == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c + 0x20);
            }
            else if (code_point == 0)
            {
                update_tag_token_replacement();
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
            }
            else
            {
                update_tag_token_from_buffer();
            }

            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rcdata-less-than-sign-state
        case HTML_TOKENIZER_RCDATA_LESS_THAN_STATE:
            switch (code_point)
            {
            case '/':
                state                           = HTML_TOKENIZER_RCDATA_END_TAG_OPEN_STATE;
                break;
            default:
                create_char_token('<');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_RCDATA_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rcdata-end-tag-open-state
        case HTML_TOKENIZER_RCDATA_END_TAG_OPEN_STATE:
            if (utf8_is_alpha(code_point))
            {
                create_tag_token(HTML_END_TOKEN);
                consume                         = false;
                state                           = HTML_TOKENIZER_RCDATA_END_TAG_NAME_STATE;
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_RCDATA_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rcdata-end-tag-name-state
        case HTML_TOKENIZER_RCDATA_END_TAG_NAME_STATE:
            if ((code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ') && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '/' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
            }
            else if (code_point == '>' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c + 0x20);
                update_tmp_buf_from_buffer();
            }
            else if (utf8_is_lower_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c);
                update_tmp_buf_from_buffer();
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_RCDATA_STATE;
                emit_tmp_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rawtext-less-than-sign-state
        case HTML_TOKENIZER_RAWTEXT_LESS_THAN_STATE:
            switch (code_point)
            {
            case '/':
                clear_tmp_buf();
                state                           = HTML_TOKENIZER_RAWTEXT_END_TAG_OPEN_STATE;
                break;
            default:
                create_char_token('<');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_RAWTEXT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rawtext-end-tag-open-state
        case HTML_TOKENIZER_RAWTEXT_END_TAG_OPEN_STATE:
            if (utf8_is_alpha(code_point))
            {
                create_tag_token(HTML_END_TOKEN);
                consume                         = false;
                state                           = HTML_TOKENIZER_RAWTEXT_END_TAG_NAME_STATE;
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_RAWTEXT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rawtext-end-tag-name-state
        case HTML_TOKENIZER_RAWTEXT_END_TAG_NAME_STATE:
            if ((code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ') && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '/' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
            }
            else if (code_point == '>' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c + 0x20);
                update_tmp_buf_from_buffer();
            }
            else if (utf8_is_lower_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c);
                update_tmp_buf_from_buffer();
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_RAWTEXT_STATE;
                emit_tmp_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-less-than-sign-state
        case HTML_TOKENIZER_SCRIPT_DATA_LESS_THAN_STATE:
            switch (code_point)
            {
            case '/':
                clear_tmp_buf();
                state                           = HTML_TOKENIZER_SCRIPT_DATA_END_TAG_OPEN_STATE;
                break;
            case '!':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPE_START_STATE;
                create_char_token('<');
                emit_token();
                create_char_token('!');
                emit_token();
                break;
            default:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_STATE;
                consume                         = false;
                create_char_token('<');
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-end-tag-open-state
        case HTML_TOKENIZER_SCRIPT_DATA_END_TAG_OPEN_STATE:
            if (utf8_is_alpha(code_point))
            {
                create_tag_token(HTML_END_TOKEN);
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_END_TAG_NAME_STATE;
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-end-tag-name-state
        case HTML_TOKENIZER_SCRIPT_DATA_END_TAG_NAME_STATE:
            if ((code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ') && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '/' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
            }
            else if (code_point == '>' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c + 0x20);
                update_tmp_buf_from_buffer();
            }
            else if (utf8_is_lower_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c);
                update_tmp_buf_from_buffer();
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_STATE;
                emit_tmp_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escape-start-state
        case HTML_TOKENIZER_SCRIPT_DATA_ESCAPE_START_STATE:
            switch (code_point)
            {
            case '-':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPE_START_DASH_STATE;
                create_char_token('-');
                emit_token();
                break;
            default:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_STATE;
                consume                         = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escape-start-dash-state
        case HTML_TOKENIZER_SCRIPT_DATA_ESCAPE_START_DASH_STATE:
            switch (code_point)
            {
            case '-':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_DASH_DASH_STATE;
                create_char_token('-');
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
            switch (code_point)
            {
            case '-':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_DASH_STATE;
                create_char_token('-');
                emit_token();
                break;
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_LESS_THAN_STATE;
                break;
            case 0:
                create_replacement_char_token();
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                break;
            default:
                create_char_token_from_buffer();
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

            switch (code_point)
            {
            case '-':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_DASH_DASH_STATE;
                create_char_token('-');
                emit_token();
                break;
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_LESS_THAN_STATE;
                break;
            case 0:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                create_replacement_char_token();
                emit_token();
                break;
            default:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                create_char_token_from_buffer();
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

            switch (code_point)
            {
            case '-':
                create_char_token('-');
                emit_token();
                break;
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_LESS_THAN_STATE;
                break;
            case 0:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                create_replacement_char_token();
                emit_token();
                break;
            default:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                create_char_token_from_buffer();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-less-than-sign-state
        case HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_LESS_THAN_STATE:
            if (code_point == '/')
            {
                clear_tmp_buf();
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_END_TAG_OPEN_STATE;
            }
            else if (utf8_is_alpha(code_point))
            {
                clear_tmp_buf();
                create_char_token('<');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPE_START_STATE;
            }
            else
            {
                create_char_token('<');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-end-tag-open-state
        case HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_END_TAG_OPEN_STATE:
            if (utf8_is_alpha(code_point))
            {
                create_tag_token(HTML_END_TOKEN);
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_END_TAG_NAME_STATE;
                consume                         = false;
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-end-tag-name-state
        case HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_END_TAG_NAME_STATE:
            if ((code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ') && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '/' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
            }
            else if (code_point == '>' && is_appropriate_end_tag())
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c + 0x20);
                update_tmp_buf_from_buffer();
            }
            else if (utf8_is_lower_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c);
                update_tmp_buf_from_buffer();
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                emit_tmp_buf();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escape-start-state
        case HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPE_START_STATE:
            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || 
                code_point == ' '  || code_point == '/'  || code_point == '>')
            {
                if (tmp_buf_size == 6 && strncmp(tmp_buf, "script", tmp_buf_size) == 0)
                {
                    state                       = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                }
                else
                {
                    state                       = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                }
                
                create_char_token_from_buffer();
                emit_token();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tmp_buf(c + 0x20);
                create_char_token(c);
                emit_token();
            }
            else if (utf8_is_lower_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tmp_buf(c);
                create_char_token(c);
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

            switch (code_point)
            {
            case '-':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_STATE;
                create_char_token('-');
                emit_token();
                break;
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_STATE;
                create_char_token('<');
                emit_token();
                break;
            case 0:
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                create_replacement_char_token();
                emit_token();
                break;
            default:
                create_char_token_from_buffer();
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

            switch (code_point)
            {
            case '-':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH_STATE;
                create_char_token('-');
                emit_token();
                break;
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_STATE;
                create_char_token('<');
                emit_token();
                break;
            case 0:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                create_replacement_char_token();
                emit_token();
                break;
            default:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                create_char_token_from_buffer();
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

            switch (code_point)
            {
            case '-':
                create_char_token('-');
                emit_token();
                break;
            case '<':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_STATE;
                create_char_token('<');
                emit_token();
                break;
            case '>':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_STATE;
                create_char_token('>');
                emit_token();
                break;
            case 0:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                create_replacement_char_token();
                emit_token();
                break;
            default:
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                create_char_token_from_buffer();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escaped-less-than-sign-state
        case HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_STATE:
            switch (code_point)
            {
            case '/':
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPE_END_STATE;
                clear_tmp_buf();
                create_char_token('/');
                emit_token();
                break;
            default:
                consume                         = false;
                state                           = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escape-end-state
        case HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPE_END_STATE:
            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || 
                code_point == ' '  || code_point == '/'  || code_point == '>')
            {
                if (tmp_buf_size == 6 && strncmp(tmp_buf, "script", tmp_buf_size) == 0)
                {
                    state                       = HTML_TOKENIZER_SCRIPT_DATA_ESCAPED_STATE;
                }
                else
                {
                    state                       = HTML_TOKENIZER_SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                }
                
                create_char_token_from_buffer();
                emit_token();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tmp_buf(c + 0x20);
                create_char_token(c);
                emit_token();
            }
            else if (utf8_is_lower_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tmp_buf(c);
                create_char_token(c);
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
            if (is_eof || code_point == '/'  || code_point == '>')
            {
                consume                         = false;
                state                           = HTML_TOKENIZER_AFTER_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                // ignore
            }
            else if (code_point == '=')
            {
                create_attribute_from_buffer();
                state                           = HTML_TOKENIZER_ATTRIBUTE_NAME_STATE;
                status                          = HTML_TOKENIZER_UNEXPECTED_EQUALS_SIGN_BEFORE_ATTRIBUTE_NAME;
            }
            else
            {
                init_attribute();
                consume                         = false;
                state                           = HTML_TOKENIZER_ATTRIBUTE_NAME_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#attribute-name-state
        case HTML_TOKENIZER_ATTRIBUTE_NAME_STATE:
            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || 
                code_point == ' '  || code_point == '/'  || code_point == '>' || is_eof)
            {
                consume                         = false;
                state                           = HTML_TOKENIZER_AFTER_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '=')
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_VALUE_STATE;
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_attribute_name(c + 0x20);
            }
            else if (code_point == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_attribute_name_replacement_char();
            }
            else
            {
                if (code_point == '"' || code_point == '\'' || code_point == '<')
                {
                    status                      = HTML_TOKENIZER_UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME;
                }
                update_attribute_name_from_buffer();
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

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '/')
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
            }
            else if (code_point == '=')
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_VALUE_STATE;
            }
            else if (code_point == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_attribute();
                emit_token();
            }
            else
            {
                emit_attribute();
                init_attribute();
                consume                         = false;
                state                           = HTML_TOKENIZER_ATTRIBUTE_NAME_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#before-attribute-value-state
        case HTML_TOKENIZER_BEFORE_ATTRIBUTE_VALUE_STATE:
            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '"')
            {
                state                           = HTML_TOKENIZER_ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE;
            }
            else if (code_point == '\'')
            {
                state                           = HTML_TOKENIZER_ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE;
            }
            else if (code_point == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                status                          = HTML_TOKENIZER_MISSING_ATTRIBUTE_VALUE;
                emit_attribute();
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

            if (code_point == '"')
            {
                state                           = HTML_TOKENIZER_AFTER_ATTRIBUTE_VALUE_QUOTED_STATE;
            }
            else if (code_point == '&')
            {
                state                           = HTML_TOKENIZER_CHARACTER_REFERENCE_STATE;
                return_state                    = HTML_TOKENIZER_ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE;
            }
            else if (code_point == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_attribute_value_replacement_char();
            }
            else
            {
                update_attribute_value_from_buffer();
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

            if (code_point == '\'')
            {
                state                           = HTML_TOKENIZER_AFTER_ATTRIBUTE_VALUE_QUOTED_STATE;
            }
            else if (code_point == '&')
            {
                state                           = HTML_TOKENIZER_CHARACTER_REFERENCE_STATE;
                return_state                    = HTML_TOKENIZER_ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE;
            }
            else if (code_point == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_attribute_value_replacement_char();
            }
            else
            {
                update_attribute_value_from_buffer();
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

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
                emit_attribute();
            }
            else if (code_point == '&')
            {
                state                           = HTML_TOKENIZER_CHARACTER_REFERENCE_STATE;
                return_state                    = HTML_TOKENIZER_ATTRIBUTE_VALUE_UNQUOTED_STATE;
            }
            else if (code_point == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_attribute();
                emit_token();
            }
            else if (code_point == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_attribute_value_replacement_char();
            }
            else
            {
                if (code_point == '"' || code_point == '\'' || code_point == '<' || code_point == '=' || code_point == '`')
                {
                    status                      = HTML_TOKENIZER_UNEXPECTED_CHARACTER_IN_UNQUOTED_ATTRIBUTE_VALUE;
                }
                update_attribute_value_from_buffer();
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

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
                emit_attribute();
            }
            else if (code_point == '/')
            {
                state                           = HTML_TOKENIZER_SELF_CLOSING_START_TAG_STATE;
                emit_attribute();
            }
            else if (code_point == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_attribute();
                emit_token();
            }
            else
            {
                status                          = HTML_TOKENIZER_MISSING_WHITESPACE_BETWEEN_ATTRIBUTES;
                consume                         = false;
                state                           = HTML_TOKENIZER_BEFORE_ATTRIBUTE_NAME_STATE;
                emit_attribute();
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

            if (code_point == '>')
            {
                if (tokens[token_idx].type == HTML_START_TOKEN)
                {
                    tokens[token_idx].self_closing  = true;
                }
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_attribute();
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

            if (code_point == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (code_point == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_comment_token_replacement_char();
            }
            else
            {
                update_comment_token_from_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#markup-declaration-open-state
        case HTML_TOKENIZER_MARKUP_DECLARATION_OPEN_STATE:
            if (code_point == '-' && match_segment(hyphen_segment, hyphen_segment_size, CASE_SENSITIVE_MATCH))
            {
                create_comment_token();
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
                create_comment_token();
                consume                         = false;
                state                           = HTML_TOKENIZER_BOGUS_COMMENT_STATE;
                status                          = HTML_TOKENIZER_INCORRECTLY_OPENED_COMMENT;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-start-state
        case HTML_TOKENIZER_COMMENT_START_STATE:
            switch (code_point)
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

            switch (code_point)
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
                update_comment_token('-');
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

            switch (code_point)
            {
            case '<':
                update_comment_token_from_buffer();
                state                           = HTML_TOKENIZER_COMMENT_LESS_THAN_STATE;
                break;
            case '-':
                state                           = HTML_TOKENIZER_COMMENT_END_DASH_STATE;
                break;
            case 0:
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_comment_token_replacement_char();
                break;
            default:
                update_comment_token_from_buffer();
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

            switch (code_point)
            {
            case '!':
                update_comment_token_from_buffer();
                state                           = HTML_TOKENIZER_COMMENT_LESS_THAN_BANG_STATE;
                break;
            case '<':
                update_comment_token_from_buffer();
                break;
            default:
                state                           = HTML_TOKENIZER_COMMENT_STATE;
                consume                         = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-less-than-sign-bang-state
        case HTML_TOKENIZER_COMMENT_LESS_THAN_BANG_STATE:
            switch (code_point)
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
            switch (code_point)
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
            if (!is_eof && code_point != '>')
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

            switch (code_point)
            {
            case '-':
                state                           = HTML_TOKENIZER_COMMENT_END_STATE;
                break;
            default:
                update_comment_token('-');
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

            switch (code_point)
            {
            case '>':
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
                break;
            case '!':
                state                           = HTML_TOKENIZER_COMMENT_END_BANG_STATE;
                break;
            case '-':
                update_comment_token('-');
                break;
            default:
                update_comment_token('-');
                update_comment_token('-');
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

            switch (code_point)
            {
            case '-':
                update_comment_token('-');
                update_comment_token('-');
                update_comment_token('!');
                state                           = HTML_TOKENIZER_COMMENT_END_DASH_STATE;
                break;
            case '>':
                emit_token();
                state                           = HTML_TOKENIZER_DATA_STATE;
                status                          = HTML_TOKENIZER_INCORRECTLY_CLOSED_COMMENT;
                break;
            default:
                update_comment_token('-');
                update_comment_token('-');
                update_comment_token('!');
                consume                         = false;
                state                           = HTML_TOKENIZER_COMMENT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#doctype-state
        case HTML_TOKENIZER_DOCTYPE_STATE:
            if (is_eof)
            {
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                           = HTML_TOKENIZER_BEFORE_DOCTYPE_NAME_STATE;
            }
            else if (code_point == '>')
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
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                init_token(HTML_EOF_TOKEN);
                emit_token();
                status = HTML_TOKENIZER_EOF_IN_DOCTYPE;
                break;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                state                           = HTML_TOKENIZER_DOCTYPE_NAME_STATE;

                create_doctype_token();
                unsigned char c = (unsigned char)code_point;
                update_doctype_token_name(c + 0x20);

            }
            else if (code_point == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                state                           = HTML_TOKENIZER_DOCTYPE_NAME_STATE;

                create_doctype_token();
                update_doctype_token_name_replacement_char();
            }
            else if (code_point == '>')
            {
                create_doctype_token();
                tokens[token_idx].force_quirks  = true;
                state                           = HTML_TOKENIZER_DATA_STATE;
                status                          = HTML_TOKENIZER_MISSING_DOCTYPE_NAME;
                emit_token();
            }
            else
            {
                state                           = HTML_TOKENIZER_DOCTYPE_NAME_STATE;
                create_doctype_token();
                update_doctype_token_name_from_buffer();
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

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                           = HTML_TOKENIZER_AFTER_DOCTYPE_NAME_STATE;
            }
            else if (code_point == '>')
            {
                state                           = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_doctype_token_name(c + 0x20);
            }
            else if (code_point == 0)
            {
                status                          = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_doctype_token_name_replacement_char();
            }
            else
            {
                update_doctype_token_name_from_buffer();
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

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '>')
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

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                               = HTML_TOKENIZER_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER_STATE;
            }
            else if (code_point == '"')
            {
                // todo: missing vs empty identifier
                state                               = HTML_TOKENIZER_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED_STATE;
                status                              = HTML_TOKENIZER_MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD;
            }
            else if (code_point == '\'')
            {
                // todo: missing vs empty identifier
                state                               = HTML_TOKENIZER_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED_STATE;
                status                              = HTML_TOKENIZER_MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD;
            }
            else if (code_point == '>')
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

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '"')
            {
                // todo: missing vs empty identifier
                state                               = HTML_TOKENIZER_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED_STATE;
            }
            else if (code_point == '\'')
            {
                // todo: missing vs empty identifier
                state                               = HTML_TOKENIZER_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED_STATE;
            }
            else if (code_point == '>')
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

            if (code_point == '"')
            {
                state                               = HTML_TOKENIZER_AFTER_DOCTYPE_PUBLIC_IDENTIFIER_STATE;
            }
            else if (code_point == 0)
            {
                status                              = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_doctype_token_public_identifier_replacement_char();
            }
            else if (code_point == '>')
            {
                tokens[token_idx].force_quirks      = true;
                state                               = HTML_TOKENIZER_DATA_STATE;
                status                              = HTML_TOKENIZER_ABRUPT_DOCTYPE_PUBLIC_IDENTIFIER;
                emit_token();
            }
            else
            {
                update_doctype_public_identifier_from_buffer();
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

            if (code_point == '\'')
            {
                state                               = HTML_TOKENIZER_AFTER_DOCTYPE_PUBLIC_IDENTIFIER_STATE;
            }
            else if (code_point == 0)
            {
                status                              = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_doctype_token_public_identifier_replacement_char();
            }
            else if (code_point == '>')
            {
                tokens[token_idx].force_quirks      = true;
                state                               = HTML_TOKENIZER_DATA_STATE;
                status                              = HTML_TOKENIZER_ABRUPT_DOCTYPE_PUBLIC_IDENTIFIER;
                emit_token();
            }
            else
            {
                update_doctype_public_identifier_from_buffer();
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

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                               = HTML_TOKENIZER_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS_STATE;
            }
            else if (code_point == '>')
            {
                state                               = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (code_point == '"')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE;
                status                              = HTML_TOKENIZER_MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS;
            }
            else if (code_point == '\'')
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

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '>')
            {
                state                               = HTML_TOKENIZER_DATA_STATE;
                emit_token();
            }
            else if (code_point == '"')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE;
            }
            else if (code_point == '\'')
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

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                               = HTML_TOKENIZER_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER_STATE;
            }
            else if (code_point == '"')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE;
                status                              = HTML_TOKENIZER_MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD;
            }
            else if (code_point == '\'')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED_STATE;
                status                              = HTML_TOKENIZER_MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD;
            }
            else if (code_point == '>')
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

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '"')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE;
            }
            else if (code_point == '\'')
            {
                // todo: missing vs empty system id
                state                               = HTML_TOKENIZER_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED_STATE;
            }
            else if (code_point == '>')
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

            switch (code_point)
            {
            case '"':
                state                               = HTML_TOKENIZER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER_STATE;
                break;
            case 0:
                status                              = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_doctype_token_system_identifier_replacement_char();
                break;
            case '>':
                tokens[token_idx].force_quirks      = true;
                state                               = HTML_TOKENIZER_DATA_STATE;
                status                              = HTML_TOKENIZER_ABRUPT_DOCTYPE_SYSTEM_IDENTIFIER;
                emit_token();
                break;
            default:
                update_doctype_token_system_identifier_from_buffer();
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

            switch (code_point)
            {
            case '\'':
                state                               = HTML_TOKENIZER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER_STATE;
                break;
            case 0:
                status                              = HTML_TOKENIZER_UNEXPECTED_NULL_CHARACTER;
                update_doctype_token_system_identifier_replacement_char();
                break;
            case '>':
                tokens[token_idx].force_quirks      = true;
                state                               = HTML_TOKENIZER_DATA_STATE;
                status                              = HTML_TOKENIZER_ABRUPT_DOCTYPE_SYSTEM_IDENTIFIER;
                emit_token();
                break;
            default:
                update_doctype_token_system_identifier_from_buffer();
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

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '>')
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

            switch(code_point)
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

            switch(code_point)
            {
            case ']':
                state                               = HTML_TOKENIZER_CDATA_SECTION_BRACKET_STATE;
                break;
            default:
                create_char_token_from_buffer();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#cdata-section-bracket-state
        case HTML_TOKENIZER_CDATA_SECTION_BRACKET_STATE:
            switch(code_point)
            {
            case ']':
                create_char_token(']');
                emit_token();
                break;
            case '>':
                state                               = HTML_TOKENIZER_DATA_STATE;
                break;
            default:
                create_char_token(']');
                emit_token();
                state                               = HTML_TOKENIZER_CDATA_SECTION_END_STATE;
                consume                             = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#cdata-section-end-state
        case HTML_TOKENIZER_CDATA_SECTION_END_STATE:
            switch (code_point)
            {
            case ']':
                create_char_token(']');
                emit_token();
                break;
            case '>':
                state                               = HTML_TOKENIZER_DATA_STATE;
                break;
            default:
                create_char_token(']');
                emit_token();
                create_char_token(']');
                emit_token();
                consume                             = false;
                state                               = HTML_TOKENIZER_CDATA_SECTION_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#character-reference-state
        case HTML_TOKENIZER_CHARACTER_REFERENCE_STATE:
            clear_tmp_buf();
            update_tmp_buf('&');
            if (utf8_is_alphanumeric(code_point))
            {
                consume                             = false;
                state                               = HTML_TOKENIZER_NAMED_CHARACTER_REFERENCE_STATE;
            }
            else if (code_point == '#')
            {
                update_tmp_buf('#');
                state                               = HTML_TOKENIZER_NUMERIC_CHARACTER_REFERENCE_STATE;
            }
            else
            {
                if (return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE || 
                    return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE ||
                    return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_UNQUOTED_STATE)
                {
                    flush_tmp_buf_to_attribute_value();
                }
                else
                {
                    emit_tmp_buf();
                }
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
                    if ((return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE ||
                        return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE ||
                        return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_UNQUOTED_STATE) &&
                        (buf[buf_cur - 1] != ';' && (buf[buf_cur] == '=' || utf8_is_alphanumeric(buf[buf_cur]))))
                    {
                        flush_code_points_consumed_as_char_ref(return_state);
                    }
                    else
                    {
                        clear_tmp_buf();
                        int32_t bytes = utf8_encode(named_cp, tmp_buf);

                        tmp_buf_size    = (uint32_t)bytes;
                        flush_code_points_consumed_as_char_ref(return_state);
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

            flush_code_points_consumed_as_char_ref(return_state);
            buf_cur              = buf_cur + cursor_offset;
            consume             = false;
            state               = return_state;
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#ambiguous-ampersand-state
        case HTML_TOKENIZER_AMBIGUOUS_AMPERSAND_STATE:
            if (utf8_is_alphanumeric(code_point))
            {
                if (return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE || 
                    return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE ||
                    return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_UNQUOTED_STATE)
                {
                    unsigned char c = (unsigned char)code_point;
                    update_attribute_value(c);
                }
                else
                {
                    unsigned char c = (unsigned char)code_point;
                    create_char_token(c);
                    emit_token();
                }
            }
            else if (code_point == ';')
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

            if (code_point == 'x' || code_point == 'X')
            {
                unsigned char c = (unsigned char)code_point;
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
            if (utf8_is_hex(code_point))
            {
                consume                             = false;
                state                               = HTML_TOKENIZER_HEXADECIMAL_CHARACTER_REFERENCE_STATE;
            }
            else
            {
                if (return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE || 
                    return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE ||
                    return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_UNQUOTED_STATE)
                {
                    flush_tmp_buf_to_attribute_value();
                }
                else
                {
                    emit_tmp_buf();
                }
                status                              = HTML_TOKENIZER_ABSENCE_OF_DIGITS_IN_NUMERIC_CHARACTER_REFERENCE;
                consume                             = false;
                state                               = return_state;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#decimal-character-reference-start-state
        case HTML_TOKENIZER_DECIMAL_CHARACTER_REFERENCE_START_STATE:
            if (utf8_is_digit(code_point))
            {
                consume                             = false;
                state                               = HTML_TOKENIZER_DECIMAL_CHARACTER_REFERENCE_STATE;
            }
            else
            {
                if (return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE || 
                    return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE ||
                    return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_UNQUOTED_STATE)
                {
                    flush_tmp_buf_to_attribute_value();
                }
                else
                {
                    emit_tmp_buf();
                }
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
            else if (utf8_is_digit(code_point))
            {
                character_reference_code            *= 16;
                character_reference_code            += code_point - 0x30;
            }
            else if (utf8_is_upper_hex(code_point))
            {
                character_reference_code            *= 16;
                character_reference_code            += code_point - 0x37;
            }
            else if (utf8_is_lower_hex(code_point))
            {
                character_reference_code            *= 16;
                character_reference_code            += code_point - 0x57;
            }
            else if (code_point == ';')
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
            else if (utf8_is_digit(code_point))
            {
                uint32_t old_val                    = character_reference_code;
                character_reference_code           *= 10;
                character_reference_code           += code_point - 0x30;

                if (old_val > character_reference_code)
                {
                    character_reference_code        = 0xFFFFFFFF;
                }
            }
            else if (code_point == ';')
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

            state                               = return_state;
            if (return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE || 
                return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE ||
                return_state == HTML_TOKENIZER_ATTRIBUTE_VALUE_UNQUOTED_STATE)
            {
                flush_tmp_buf_to_attribute_value();
            }
            else
            {
                emit_tmp_buf();
            }

            break;
        }

        if (consume && bytes_read > 0)
        {
            buf_cur += (uint32_t)bytes_read;
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
    if (buf) { free(buf); }
    buf = NULL;
}
