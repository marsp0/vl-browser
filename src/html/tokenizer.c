#include "tokenizer.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "util/utf8.h"

/*
 * Notes
 * 
 */

/********************/
/*      defines     */
/********************/

#define MAX_TEMP_BUFFER_SIZE    256

/********************/
/* static variables */
/********************/

// https://html.spec.whatwg.org/multipage/parsing.html#tokenization
typedef enum
{
    DATA_STATE,
    RCDATA_STATE,
    RAWTEXT_STATE,
    SCRIPT_DATA_STATE,
    PLAINTEXT_STATE,
    TAG_OPEN_STATE,
    END_TAG_OPEN_STATE,
    TAG_NAME_STATE,
    RCDATA_LESS_THAN_STATE,
    RCDATA_END_TAG_OPEN_STATE,
    RCDATA_END_TAG_NAME_STATE,
    RAWTEXT_LESS_THAN_STATE,
    RAWTEXT_END_TAG_OPEN_STATE,
    RAWTEXT_END_TAG_NAME_STATE,
    SCRIPT_DATA_LESS_THAN_STATE,
    SCRIPT_DATA_END_TAG_OPEN_STATE,
    SCRIPT_DATA_END_TAG_NAME_STATE,
    SCRIPT_DATA_ESCAPE_START_STATE,
    SCRIPT_DATA_ESCAPE_START_DASH_STATE,
    SCRIPT_DATA_ESCAPED_STATE,
    SCRIPT_DATA_ESCAPED_DASH_STATE,
    SCRIPT_DATA_ESCAPED_DASH_DASH_STATE,
    SCRIPT_DATA_ESCAPED_LESS_THAN_STATE,
    SCRIPT_DATA_ESCAPED_END_TAG_OPEN_STATE,
    SCRIPT_DATA_ESCAPED_END_TAG_NAME_STATE,
    SCRIPT_DATA_DOUBLE_ESCAPE_START_STATE,
    SCRIPT_DATA_DOUBLE_ESCAPED_STATE,
    SCRIPT_DATA_DOUBLE_ESCAPED_DASH_STATE,
    SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH_STATE,
    SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_STATE,
    SCRIPT_DATA_DOUBLE_ESCAPE_END_STATE,
    BEFORE_ATTRIBUTE_NAME_STATE,
    ATTRIBUTE_NAME_STATE,
    AFTER_ATTRIBUTE_NAME_STATE,
    BEFORE_ATTRIBUTE_VALUE_STATE,
    ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE,
    ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE,
    ATTRIBUTE_VALUE_UNQUOTED_STATE,
    AFTER_ATTRIBUTE_VALUE_QUOTED_STATE,
    SELF_CLOSING_START_TAG_STATE,
    BOGUS_COMMENT_STATE,
    MARKUP_DECLARATION_OPEN_STATE,
    COMMENT_START_STATE,
    COMMENT_START_DASH_STATE,
    COMMENT_STATE,
    COMMENT_LESS_THAN_STATE,
    COMMENT_LESS_THAN_BANG_STATE,
    COMMENT_LESS_THAN_BANG_DASH_STATE,
    COMMENT_LESS_THAN_BANG_DASH_DASH_STATE,
    COMMENT_END_DASH_STATE,
    COMMENT_END_STATE,
    COMMENT_END_BANG_STATE,
    DOCTYPE_STATE,
    BEFORE_DOCTYPE_NAME_STATE,
    DOCTYPE_NAME_STATE,
    AFTER_DOCTYPE_NAME_STATE,
    AFTER_DOCTYPE_PUBLIC_KEYWORD_STATE,
    BEFORE_DOCTYPE_PUBLIC_IDENTIFIER_STATE,
    DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED_STATE,
    DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED_STATE,
    AFTER_DOCTYPE_PUBLIC_IDENTIFIER_STATE,
    BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS_STATE,
    AFTER_DOCTYPE_SYSTEM_KEYWORD_STATE,
    BEFORE_DOCTYPE_SYSTEM_IDENTIFIER_STATE,
    DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE,
    DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED_STATE,
    AFTER_DOCTYPE_SYSTEM_IDENTIFIER_STATE,
    BOGUS_DOCTYPE_STATE,
    CDATA_SECTION_STATE,
    CDATA_SECTION_BRACKET_STATE,
    CDATA_SECTION_END_STATE,
    CHARACTER_REFERENCE_STATE,
    NAMED_CHARACTER_REFERENCE_STATE,
    AMBIGUOUS_AMPERSAND_STATE,
    NUMERIC_CHARACTER_REFERENCE_STATE,
    HEXADECIMAL_CHARACTER_REFERENCE_START_STATE,
    DECIMAL_CHARACTER_REFERENCE_START_STATE,
    HEXADECIMAL_CHARACTER_REFERENCE_STATE,
    DECIMAL_CHARACTER_REFERENCE_STATE,
    NUMERIC_CHARACTER_REFERENCE_END_STATE
} state_e;

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

static const unsigned char* buffer                                      = NULL;
static uint32_t size                                                    = 0;
static uint32_t cursor                                                  = 0;
static html_token_t* tokens                                             = NULL;
static uint32_t token_idx                                               = 0;
static uint32_t max_tokens                                              = 0;
static state_e state                                                    = DATA_STATE;
static state_e return_state                                             = DATA_STATE;
static unsigned char temp_buffer[MAX_TEMP_BUFFER_SIZE]                  = { 0 };
static uint32_t temp_buffer_size                                        = 0;
static unsigned char last_emitted_start_tag[HTML_TOKEN_MAX_NAME_LEN]    = { 0 };
static int32_t bytes_read                                               = 0;
static uint32_t character_reference_code                                = 0;
static unsigned char hyphen_segment[]                                   = "--";
static unsigned char doctype_segment[]                                  = "DOCTYPE";
static unsigned char cdata_segment[]                                    = "[CDATA[";
static unsigned char public_segment[]                                   = "PUBLIC";
static unsigned char system_segment[]                                   = "SYSTEM";
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

static int32_t find_numeric_char_ref(uint32_t code_point, uint32_t* out)
{
    int32_t result = -1;
    for (uint32_t i = 0; i < sizeof(numeric_char_refs) / sizeof(numeric_char_ref_t); i++)
    {
        if (numeric_char_refs[i].key == code_point)
        {
            *out = numeric_char_refs[i].value;
            result = (int32_t)i;
        }
    }

    return result;
}

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

static void clear_temp_buffer()
{
    memset(temp_buffer, 0, sizeof(temp_buffer));
    temp_buffer_size = 0;
}

static void clear_tokens()
{
    token_idx = 0;

    for (uint32_t i = 0; i < max_tokens; i++)
    {
        tokens[i].is_valid = false;
    }
}

static void init_char_token()
{
    tokens[token_idx].is_valid  = true;
    tokens[token_idx].type      = HTML_CHARACTER_TOKEN;
    tokens[token_idx].data_size = 0;

    memset(tokens[token_idx].data, 0, sizeof(tokens[token_idx].data));
}

static void create_char_token_from_buffer()
{
    assert(bytes_read >= 0);

    init_char_token();
    uint32_t read = (uint32_t)bytes_read;
    uint32_t data_size = tokens[token_idx].data_size;

    for (uint32_t i = 0; i < read; i++)
    {
        tokens[token_idx].data[data_size + i] = buffer[cursor + i];
    }
    tokens[token_idx].data_size = data_size + read;
}

static void create_replacement_char_token()
{
    init_char_token();

    uint32_t data_size = tokens[token_idx].data_size;
    tokens[token_idx].data[data_size] = 0xff;
    data_size++;
    tokens[token_idx].data[data_size] = 0xfd;
    data_size++;
    tokens[token_idx].data_size = data_size;
}

static void create_char_token(unsigned char c)
{
    init_char_token();

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

    if (name_size >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    for (uint32_t i = 0; i < read; i++)
    {
        tokens[token_idx].name[name_size + i] = buffer[cursor + i];
    }

    tokens[token_idx].name_size = name_size + read;
}

static void update_tag_token(unsigned char c)
{
    uint32_t name_size = tokens[token_idx].name_size;
    if (name_size >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].name[name_size] = c;
    tokens[token_idx].name_size++;
}

static void update_tag_token_replacement()
{
    uint32_t name_size = tokens[token_idx].name_size;
    if (name_size >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].name[name_size] = 0xff;
    name_size++;
    tokens[token_idx].name[name_size] = 0xfd;
    name_size++;
    tokens[token_idx].name_size = name_size;
}

static void init_attribute()
{
    html_token_t* token = &tokens[token_idx];
    token->attributes_size++;
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
        attribute->name[name_size + i] = buffer[cursor + i];
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
    attribute->name[attribute->name_size]   = 0xff;
    attribute->name_size++;
    attribute->name[attribute->name_size]   = 0xfd;
    attribute->name_size++;
}

static void update_attribute_value_replacement_char()
{
    html_token_t* token                     = &tokens[token_idx];
    html_token_attribute_t* attribute       = &(token->attributes[token->attributes_size]);
    attribute->value[attribute->value_size]   = 0xff;
    attribute->value_size++;
    attribute->value[attribute->value_size]   = 0xfd;
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
        attribute->value[value_size + i] = buffer[cursor + i];
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
    
    if (name_size > HTML_TOKEN_MAX_NAME_LEN) { return; }

    for (uint32_t i = 0; i < read; i++)
    {
        attribute->name[name_size + i] = buffer[cursor + i];
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

    tokens[token_idx].data[data_size] = 0xff;
    data_size++;
    tokens[token_idx].data[data_size] = 0xfd;
    data_size++;
    tokens[token_idx].data_size = data_size;
}

static void update_comment_token_from_buffer()
{
    assert(bytes_read >= 0);

    uint32_t read = (uint32_t)bytes_read;
    uint32_t data_size = tokens[token_idx].data_size;

    if (data_size >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    for (uint32_t i = 0; i < read; i++)
    {
        tokens[token_idx].data[data_size + i] = buffer[cursor + i];
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
    tokens[token_idx].type              = HTML_COMMENT_TOKEN;
    memset(tokens[token_idx].public_id, 0, sizeof(tokens[token_idx].public_id));
    tokens[token_idx].public_id_size    = 0;
    memset(tokens[token_idx].system_id, 0, sizeof(tokens[token_idx].system_id));
    tokens[token_idx].system_id_size    = 0;
    tokens[token_idx].force_quirks      = false;
}

static void update_doctype_token_name(unsigned char c)
{
    uint32_t name_size = tokens[token_idx].name_size;

    if (name_size >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].name[name_size] = c;
    tokens[token_idx].name_size++;
}

static void update_doctype_token_name_from_buffer()
{
    assert(bytes_read >= 0);

    uint32_t read = (uint32_t)bytes_read;
    uint32_t name_size = tokens[token_idx].name_size;

    if (name_size >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    for (uint32_t i = 0; i < read; i++)
    {
        tokens[token_idx].name[name_size + i] = buffer[cursor + i];
    }

    tokens[token_idx].name_size = name_size + read;
}

static void update_doctype_token_name_replacement_char()
{
    uint32_t name_size = tokens[token_idx].name_size;

    if (name_size + 2 >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].name[name_size] = 0xff;
    name_size++;
    tokens[token_idx].name[name_size] = 0xff;
    name_size++;
    tokens[token_idx].name_size = name_size;
}

static void update_doctype_token_public_identifier_replacement_char()
{
    uint32_t public_id_size = tokens[token_idx].public_id_size;

    if (public_id_size + 2 >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].public_id[public_id_size] = 0xff;
    public_id_size++;
    tokens[token_idx].public_id[public_id_size] = 0xff;
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
        tokens[token_idx].public_id[public_id_size + i] = buffer[cursor + i];
    }

    tokens[token_idx].public_id_size += read;
}

static void update_doctype_token_system_identifier_replacement_char()
{
    uint32_t system_id_size = tokens[token_idx].system_id_size;

    if (system_id_size + 2 >= HTML_TOKEN_MAX_NAME_LEN) { return; }

    tokens[token_idx].system_id[system_id_size] = 0xff;
    system_id_size++;
    tokens[token_idx].system_id[system_id_size] = 0xff;
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
        tokens[token_idx].system_id[system_id_size + i] = buffer[cursor + i];
    }

    tokens[token_idx].system_id_size += read;
}

static void create_eof_token()
{
    tokens[token_idx].is_valid          = true;
    tokens[token_idx].type              = HTML_EOF_TOKEN;
}

static void emit_token()
{
    // note: should this be here????
    if (tokens[token_idx].attributes_size > 0) { tokens[token_idx].attributes_size++; }

    token_idx++;

    // save the name of start tokens
    if (tokens[token_idx - 1].type != HTML_START_TOKEN) { return; }

    memcpy(last_emitted_start_tag, tokens[token_idx - 1].name, sizeof(tokens[token_idx - 1].name));
}

static bool match_segment(unsigned char* segment, uint32_t segment_size, match_type_e match_type)
{
    if (cursor + segment_size >= size)
    {
        return false;
    }
    if (match_type == CASE_SENSITIVE_MATCH)
    {
        return strncmp(&buffer[cursor], segment, segment_size) == 0;
    }

    for (uint32_t i = 0; i < segment_size; i++)
    {
        unsigned char a = buffer[cursor + i];
        unsigned char b = segment[i];
        if (a < 'a')    { a += 0x20; }
        if (b < 'a')    { b += 0x20; }
        if (a != b)     { return false; }
    }

    return true;
}

static bool is_appropriate_end_tag()
{
    int32_t result = strncmp(tokens[token_idx].name, last_emitted_start_tag, sizeof(tokens[token_idx].name));
    return result == 0;
}

static void emit_temp_buffer()
{
    uint32_t cur = 0;
    uint32_t code_point = 0;
    int32_t bytes = 0;
    while (cur < temp_buffer_size)
    {
        bytes = utf8_decode(temp_buffer, temp_buffer_size, cur, &code_point);
        assert(bytes > -1);

        init_char_token();
        uint32_t read = (uint32_t)bytes;
        uint32_t data_size = tokens[token_idx].data_size;

        for (uint32_t i = 0; i < read; i++)
        {
            tokens[token_idx].data[data_size + i] = temp_buffer[cur + i];
        }
        tokens[token_idx].data_size = data_size + read;

        emit_token();
        
        cur += read;
    }
}

static void update_temp_buffer_from_buffer()
{
    assert(bytes_read >= 0);

    uint32_t read = (uint32_t)bytes_read;

    for (uint32_t i = 0; i < read; i++)
    {
        temp_buffer[temp_buffer_size + i] = buffer[cursor + i];
    }
    temp_buffer_size += read;
}

static void update_temp_buffer(unsigned char c)
{
    temp_buffer[temp_buffer_size] = c;
    temp_buffer_size++;
}

static void flush_temp_buffer_to_attribute_value()
{
    html_token_t* token = &tokens[token_idx];

    html_token_attribute_t* attr = &(token->attributes[token->attributes_size]);
    for (uint32_t i = 0; i < temp_buffer_size; i++)
    {
        attr->value[attr->value_size] = temp_buffer[i];
        attr->value_size++;
    }
}

/********************/
/* public functions */
/********************/

void html_tokenizer_init(const unsigned char* new_buffer, const uint32_t new_size, html_token_t* new_tokens, const uint32_t new_max_tokens)
{
    // assert(new_buffer);

    buffer                      = new_buffer;
    cursor                      = 0;
    size                        = new_size;
    tokens                      = new_tokens;
    max_tokens                  = new_max_tokens;

    state                       = DATA_STATE;
    return_state                = DATA_STATE;

    character_reference_code    = 0;

    clear_temp_buffer();
}

html_tokenizer_status_e html_tokenizer_next()
{
    assert(buffer);

    clear_tokens();

    bool is_eof = false;
    bool consume = true;
    uint32_t code_point;

    while (token_idx == 0)
    {
        assert(token_idx < max_tokens);
        bytes_read = -1;

        if (cursor >= size)
        {
            is_eof = true;
        }
        else
        {
            bytes_read = utf8_decode(buffer, size, cursor, &code_point);
            if (bytes_read <= -1) { return HTML_TOKENIZER_ERROR; }
        }

        switch (state)
        {
        // https://html.spec.whatwg.org/multipage/parsing.html#data-state
        case DATA_STATE:
            if (is_eof)
            {
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '&':
                state                           = CHARACTER_REFERENCE_STATE;
                return_state                    = DATA_STATE;
                break;
            case '<':
                state                           = TAG_OPEN_STATE;
                break;
            default:
                create_char_token_from_buffer();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rcdata-state
        case RCDATA_STATE:
            if (is_eof)
            {
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '&':
                return_state                    = RCDATA_STATE;
                state                           = CHARACTER_REFERENCE_STATE;
                break;
            case '<':
                state                           = RCDATA_LESS_THAN_STATE;
                break;
            default:
                if (code_point == 0) { create_replacement_char_token(); }
                else                 { create_char_token_from_buffer(); }
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rawtext-state
        case RAWTEXT_STATE:
            if (is_eof)
            {
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch(code_point)
            {
            case '<':
                state                           = RAWTEXT_LESS_THAN_STATE;
                break;
            default:
                if (code_point == 0) { create_replacement_char_token(); }
                else                 { create_char_token_from_buffer(); }
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-state
        case SCRIPT_DATA_STATE:
            if (is_eof)
            {
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '<':
                state                           = SCRIPT_DATA_LESS_THAN_STATE;
                break;
            default:
                if (code_point == 0) { create_replacement_char_token(); }
                else                 { create_char_token_from_buffer(); }
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#plaintext-state
        case PLAINTEXT_STATE:
            if (is_eof)
            {
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == 0) { create_replacement_char_token(); }
            else                 { create_char_token_from_buffer(); }
            emit_token();
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#tag-open-state
        case TAG_OPEN_STATE:
            if (is_eof)
            {
                create_char_token('<');
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '!')
            {
                state                           = MARKUP_DECLARATION_OPEN_STATE;
            }
            else if (code_point == '/')
            {
                state                           = END_TAG_OPEN_STATE;
            }
            else if (utf8_is_alpha(code_point))
            {
                create_tag_token(HTML_START_TOKEN);
                consume                         = false;
                state                           = TAG_NAME_STATE;
            }
            else if (code_point == '?')
            {
                create_comment_token();
                consume                         = false;
                state                           = BOGUS_COMMENT_STATE;
            }
            else
            {
                create_char_token('<');
                emit_token();
                consume                         = false;
                state                           = DATA_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#end-tag-open-state
        case END_TAG_OPEN_STATE:
            if (is_eof)
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (utf8_is_alpha(code_point))
            {
                create_tag_token(HTML_END_TOKEN);
                consume                         = false;
                state                           = TAG_NAME_STATE;
            }
            else if (code_point == '>')
            {
                state                           = DATA_STATE;
            }
            else
            {
                create_comment_token();
                consume                         = false;
                state                           = BOGUS_COMMENT_STATE;
            }

            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#tag-name-state
        case TAG_NAME_STATE:
            if (is_eof)
            {
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                           = BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '/')
            {
                state                           = SELF_CLOSING_START_TAG_STATE;
            }
            else if (code_point == '>')
            {
                state                           = DATA_STATE;
                emit_token();
            }
            else if (utf8_is_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c + 0x20);
            }
            else
            {
                if (code_point == 0) { update_tag_token_replacement(); }
                else                 { update_tag_token_from_buffer(); }
            }

            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rcdata-less-than-sign-state
        case RCDATA_LESS_THAN_STATE:
            switch (code_point)
            {
            case '/':
                state                           = RCDATA_END_TAG_OPEN_STATE;
                break;
            default:
                create_char_token('<');
                emit_token();
                consume                         = false;
                state                           = RCDATA_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rcdata-end-tag-open-state
        case RCDATA_END_TAG_OPEN_STATE:
            if (utf8_is_alpha(code_point))
            {
                create_tag_token(HTML_END_TOKEN);
                consume                         = false;
                state                           = RCDATA_END_TAG_NAME_STATE;
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = RCDATA_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rcdata-end-tag-name-state
        case RCDATA_END_TAG_NAME_STATE:
            if ((code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ') && is_appropriate_end_tag())
            {
                state                           = BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '/' && is_appropriate_end_tag())
            {
                state                           = SELF_CLOSING_START_TAG_STATE;
            }
            else if (code_point == '>' && is_appropriate_end_tag())
            {
                state                           = DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c + 0x20);
                update_temp_buffer_from_buffer();
            }
            else if (utf8_is_lower_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c);
                update_temp_buffer_from_buffer();
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = RCDATA_STATE;
                emit_temp_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rawtext-less-than-sign-state
        case RAWTEXT_LESS_THAN_STATE:
            switch (code_point)
            {
            case '/':
                clear_temp_buffer();
                state                           = RAWTEXT_END_TAG_OPEN_STATE;
                break;
            default:
                create_char_token('<');
                emit_token();
                consume                         = false;
                state                           = RAWTEXT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rawtext-end-tag-open-state
        case RAWTEXT_END_TAG_OPEN_STATE:
            if (utf8_is_alpha(code_point))
            {
                create_tag_token(HTML_END_TOKEN);
                consume                         = false;
                state                           = RAWTEXT_END_TAG_NAME_STATE;
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = RAWTEXT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#rawtext-end-tag-name-state
        case RAWTEXT_END_TAG_NAME_STATE:
            if ((code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ') && is_appropriate_end_tag())
            {
                state                           = BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '/' && is_appropriate_end_tag())
            {
                state                           = SELF_CLOSING_START_TAG_STATE;
            }
            else if (code_point == '>' && is_appropriate_end_tag())
            {
                state                           = DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c + 0x20);
                update_temp_buffer_from_buffer();
            }
            else if (utf8_is_lower_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c);
                update_temp_buffer_from_buffer();
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = RAWTEXT_STATE;
                emit_temp_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-less-than-sign-state
        case SCRIPT_DATA_LESS_THAN_STATE:
            switch (code_point)
            {
            case '/':
                clear_temp_buffer();
                state                           = SCRIPT_DATA_END_TAG_OPEN_STATE;
                break;
            case '!':
                state                           = SCRIPT_DATA_ESCAPE_START_STATE;
                create_char_token('<');
                emit_token();
                create_char_token('!');
                emit_token();
                break;
            default:
                state                           = SCRIPT_DATA_STATE;
                consume                         = false;
                create_char_token('<');
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-end-tag-open-state
        case SCRIPT_DATA_END_TAG_OPEN_STATE:
            if (utf8_is_alpha(code_point))
            {
                create_tag_token(HTML_END_TOKEN);
                consume                         = false;
                state                           = SCRIPT_DATA_END_TAG_NAME_STATE;
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = SCRIPT_DATA_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-end-tag-name-state
        case SCRIPT_DATA_END_TAG_NAME_STATE:
            if ((code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ') && is_appropriate_end_tag())
            {
                state                           = BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '/' && is_appropriate_end_tag())
            {
                state                           = SELF_CLOSING_START_TAG_STATE;
            }
            else if (code_point == '>' && is_appropriate_end_tag())
            {
                state                           = DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c + 0x20);
                update_temp_buffer_from_buffer();
            }
            else if (utf8_is_lower_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c);
                update_temp_buffer_from_buffer();
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = SCRIPT_DATA_STATE;
                emit_temp_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escape-start-state
        case SCRIPT_DATA_ESCAPE_START_STATE:
            switch (code_point)
            {
            case '-':
                state                           = SCRIPT_DATA_ESCAPE_START_DASH_STATE;
                create_char_token('-');
                emit_token();
                break;
            default:
                state                           = SCRIPT_DATA_STATE;
                consume                         = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escape-start-dash-state
        case SCRIPT_DATA_ESCAPE_START_DASH_STATE:
            switch (code_point)
            {
            case '-':
                state                           = SCRIPT_DATA_ESCAPED_DASH_DASH_STATE;
                create_char_token('-');
                emit_token();
                break;
            default:
                state                           = SCRIPT_DATA_STATE;
                consume                         = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-state
        case SCRIPT_DATA_ESCAPED_STATE:
            if (is_eof)
            {
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }
            switch (code_point)
            {
            case '-':
                state                           = SCRIPT_DATA_ESCAPED_DASH_STATE;
                create_char_token('-');
                emit_token();
                break;
            case '<':
                state                           = SCRIPT_DATA_ESCAPED_LESS_THAN_STATE;
                break;
            default:
                if (code_point == 0) { create_replacement_char_token(); }
                else                 { create_char_token_from_buffer(); }
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-dash-state
        case SCRIPT_DATA_ESCAPED_DASH_STATE:
            if (is_eof)
            {
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '-':
                state                           = SCRIPT_DATA_ESCAPED_DASH_DASH_STATE;
                create_char_token('-');
                emit_token();
                break;
            case '<':
                state                           = SCRIPT_DATA_ESCAPED_LESS_THAN_STATE;
                break;
            default:
                state                           = SCRIPT_DATA_ESCAPED_STATE;
                if (code_point == 0) { create_replacement_char_token(); }
                else                 { create_char_token_from_buffer(); }
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-dash-dash-state
        case SCRIPT_DATA_ESCAPED_DASH_DASH_STATE:
            if (is_eof)
            {
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '-':
                create_char_token('-');
                emit_token();
                break;
            case '<':
                state                           = SCRIPT_DATA_ESCAPED_LESS_THAN_STATE;
                break;
            default:
                state                           = SCRIPT_DATA_ESCAPED_STATE;
                if (code_point == 0) { create_replacement_char_token(); }
                else                 { create_char_token_from_buffer(); }
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-less-than-sign-state
        case SCRIPT_DATA_ESCAPED_LESS_THAN_STATE:
            if (code_point == '/')
            {
                clear_temp_buffer();
                state                           = SCRIPT_DATA_ESCAPED_END_TAG_OPEN_STATE;
            }
            else if (utf8_is_alpha(code_point))
            {
                clear_temp_buffer();
                create_char_token('<');
                emit_token();
                consume                         = false;
                state                           = SCRIPT_DATA_DOUBLE_ESCAPE_START_STATE;
            }
            else
            {
                create_char_token('<');
                emit_token();
                consume                         = false;
                state                           = SCRIPT_DATA_ESCAPED_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-end-tag-open-state
        case SCRIPT_DATA_ESCAPED_END_TAG_OPEN_STATE:
            if (utf8_is_alpha(code_point))
            {
                create_tag_token(HTML_END_TOKEN);
                state                           = SCRIPT_DATA_ESCAPED_END_TAG_NAME_STATE;
                consume                         = false;
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = SCRIPT_DATA_ESCAPED_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-end-tag-name-state
        case SCRIPT_DATA_ESCAPED_END_TAG_NAME_STATE:
            if ((code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ') && is_appropriate_end_tag())
            {
                state                           = BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '/' && is_appropriate_end_tag())
            {
                state                           = SELF_CLOSING_START_TAG_STATE;
            }
            else if (code_point == '>' && is_appropriate_end_tag())
            {
                state                           = DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c + 0x20);
                update_temp_buffer_from_buffer();
            }
            else if (utf8_is_lower_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_tag_token(c);
                update_temp_buffer_from_buffer();
            }
            else
            {
                create_char_token('<');
                emit_token();
                create_char_token('/');
                emit_token();
                consume                         = false;
                state                           = SCRIPT_DATA_ESCAPED_STATE;
                emit_temp_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escape-start-state
        case SCRIPT_DATA_DOUBLE_ESCAPE_START_STATE:
            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || 
                code_point == ' '  || code_point == '/'  || code_point == '>')
            {
                if (temp_buffer_size == 6 && strncmp(temp_buffer, "script", temp_buffer_size) == 0)
                {
                    state                       = SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                }
                else
                {
                    state                       = SCRIPT_DATA_ESCAPED_STATE;
                }
                
                create_char_token_from_buffer();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_temp_buffer(c + 0x20);
                create_char_token(c);
                emit_token();
            }
            else if (utf8_is_lower_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_temp_buffer(c);
                create_char_token(c);
                emit_token();
            }
            else
            {
                consume                         = false;
                state                           = SCRIPT_DATA_ESCAPED_STATE;
            }
            break;

        //https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escaped-state
        case SCRIPT_DATA_DOUBLE_ESCAPED_STATE:
            if (is_eof)
            {
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '-':
                state                           = SCRIPT_DATA_DOUBLE_ESCAPED_DASH_STATE;
                create_char_token('-');
                emit_token();
                break;
            case '<':
                state                           = SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_STATE;
                create_char_token('<');
                emit_token();
                break;
            case 0:
                create_replacement_char_token();
                emit_token();
                break;
            default:
                create_char_token_from_buffer();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escaped-dash-state
        case SCRIPT_DATA_DOUBLE_ESCAPED_DASH_STATE:
            if (is_eof)
            {
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '-':
                state                           = SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH_STATE;
                create_char_token('-');
                emit_token();
                break;
            case '<':
                state                           = SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_STATE;
                create_char_token('<');
                emit_token();
                break;
            case 0:
                state                           = SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                create_replacement_char_token();
                emit_token();
                break;
            default:
                state                           = SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                create_char_token_from_buffer();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escaped-dash-dash-state
        case SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH_STATE:
            if (is_eof)
            {
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '-':
                create_char_token('-');
                emit_token();
                break;
            case '<':
                state                           = SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_STATE;
                create_char_token('<');
                emit_token();
                break;
            case '>':
                state                           = SCRIPT_DATA_STATE;
                create_char_token('>');
                emit_token();
                break;
            case 0:
                state                           = SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                create_replacement_char_token();
                emit_token();
                break;
            default:
                state                           = SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                create_char_token_from_buffer();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escaped-less-than-sign-state
        case SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_STATE:
            switch (code_point)
            {
            case '/':
                clear_temp_buffer();
                state                           = SCRIPT_DATA_DOUBLE_ESCAPE_END_STATE;
                create_char_token('/');
                emit_token();
                break;
            default:
                consume                         = false;
                state                           = SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escape-end-state
        case SCRIPT_DATA_DOUBLE_ESCAPE_END_STATE:
            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || 
                code_point == ' '  || code_point == '/'  || code_point == '>')
            {
                if (temp_buffer_size == 6 && strncmp(temp_buffer, "script", temp_buffer_size) == 0)
                {
                    state                       = SCRIPT_DATA_ESCAPED_STATE;
                }
                else
                {
                    state                       = SCRIPT_DATA_DOUBLE_ESCAPED_STATE;
                }
                
                create_char_token_from_buffer();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_temp_buffer(c + 0x20);
                create_char_token(c);
                emit_token();
            }
            else if (utf8_is_lower_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_temp_buffer(c);
                create_char_token(c);
                emit_token();
            }
            else
            {
                consume                         = false;
                state                           = SCRIPT_DATA_ESCAPED_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#before-attribute-name-state
        case BEFORE_ATTRIBUTE_NAME_STATE:
            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                // ignore
            }
            else if (code_point == '/'  || code_point == '>' || is_eof)
            {
                consume                         = false;
                state                           = AFTER_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '=')
            {
                create_attribute_from_buffer();
                state                           = ATTRIBUTE_NAME_STATE;
            }
            else
            {
                init_attribute();
                consume                         = false;
                state                           = ATTRIBUTE_NAME_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#attribute-name-state
        case ATTRIBUTE_NAME_STATE:
            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || 
                code_point == ' '  || code_point == '/'  || code_point == '>' || is_eof)
            {
                consume                         = false;
                state                           = AFTER_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '=')
            {
                state                           = BEFORE_ATTRIBUTE_VALUE_STATE;
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_attribute_name(c + 0x20);
            }
            else if (code_point == 0)
            {
                update_attribute_name_replacement_char();
            }
            else
            {
                // todo: parser error
                update_attribute_name_from_buffer();
            }

            // todo: parser error + remove duplicate attribute

            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-attribute-name-state
        case AFTER_ATTRIBUTE_NAME_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '/')
            {
                state                           = SELF_CLOSING_START_TAG_STATE;
            }
            else if (code_point == '=')
            {
                state                           = BEFORE_ATTRIBUTE_VALUE_STATE;
            }
            else if (code_point == '>')
            {
                state                           = DATA_STATE;
                emit_token();
            }
            else
            {
                init_attribute();
                consume                         = false;
                state                           = ATTRIBUTE_NAME_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#before-attribute-value-state
        case BEFORE_ATTRIBUTE_VALUE_STATE:
            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '"')
            {
                state                           = ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE;
            }
            else if (code_point == '\'')
            {
                state                           = ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE;
            }
            else if (code_point == '>')
            {
                // todo: parse error
                state                           = DATA_STATE;
                emit_token();
            }
            else
            {
                consume                         = false;
                state                           = ATTRIBUTE_VALUE_UNQUOTED_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#attribute-value-(double-quoted)-state
        case ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '"')
            {
                state                           = AFTER_ATTRIBUTE_VALUE_QUOTED_STATE;
            }
            else if (code_point == '&')
            {
                state                           = CHARACTER_REFERENCE_STATE;
                return_state                    = ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE;
            }
            else if (code_point == 0)
            {
                // todo: parse error
                update_attribute_value_replacement_char();
            }
            else
            {
                update_attribute_value_from_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#attribute-value-(single-quoted)-state
        case ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\'')
            {
                state                           = AFTER_ATTRIBUTE_VALUE_QUOTED_STATE;
            }
            else if (code_point == '&')
            {
                state                           = CHARACTER_REFERENCE_STATE;
                return_state                    = ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE;
            }
            else if (code_point == 0)
            {
                // todo: parse error
                update_attribute_value_replacement_char();
            }
            else
            {
                update_attribute_value_from_buffer();
            }
            break;


        // https://html.spec.whatwg.org/multipage/parsing.html#attribute-value-(unquoted)-state
        case ATTRIBUTE_VALUE_UNQUOTED_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                           = BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '&')
            {
                state                           = CHARACTER_REFERENCE_STATE;
                return_state                    = ATTRIBUTE_VALUE_UNQUOTED_STATE;
            }
            else if (code_point == '>')
            {
                state                           = DATA_STATE;
                emit_token();
            }
            else if (code_point == 0)
            {
                // todo: parse error
                update_attribute_value_replacement_char();
            }
            else
            {
                // todo: parse error
                update_attribute_value_from_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-attribute-value-(quoted)-state
        case AFTER_ATTRIBUTE_VALUE_QUOTED_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                           = BEFORE_ATTRIBUTE_NAME_STATE;
            }
            else if (code_point == '/')
            {
                state                           = SELF_CLOSING_START_TAG_STATE;
            }
            else if (code_point == '>')
            {
                state                           = DATA_STATE;
                emit_token();
            }
            else
            {
                // todo: parse error
                consume                         = false;
                state                           = BEFORE_ATTRIBUTE_NAME_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#self-closing-start-tag-state
        case SELF_CLOSING_START_TAG_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '>')
            {
                tokens[token_idx].self_closing  = true;
                state                           = DATA_STATE;
                emit_token();
            }
            else
            {
                // todo: parse error
                consume                         = false;
                state                           = BEFORE_ATTRIBUTE_NAME_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#bogus-comment-state
        case BOGUS_COMMENT_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '>')
            {
                state                           = DATA_STATE;
                emit_token();
            }
            else if (code_point == 0)
            {
                // todo: parse error
                update_comment_token_replacement_char();
            }
            else
            {
                update_comment_token_from_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#markup-declaration-open-state
        case MARKUP_DECLARATION_OPEN_STATE:
            if (code_point == '-' && match_segment(hyphen_segment, sizeof(hyphen_segment), CASE_SENSITIVE_MATCH))
            {
                create_comment_token();
                cursor                         += sizeof(hyphen_segment);
                state                           = COMMENT_START_STATE;
                consume                         = false;
            }
            else if (match_segment(doctype_segment, sizeof(doctype_segment), CASE_INSENSITIVE_MATCH))
            {
                cursor                         += sizeof(doctype_segment);
                consume                         = false;
                state                           = DOCTYPE_STATE;
            }
            else if (match_segment(cdata_segment, sizeof(cdata_segment), CASE_SENSITIVE_MATCH))
            {
                cursor                         += sizeof(cdata_segment);
                consume                         = false;
                // todo: handle once implementation on parser has started
                assert(false);
            }
            else
            {
                // todo: parse error
                create_comment_token();
                consume                         = false;
                state                           = BOGUS_COMMENT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-start-state
        case COMMENT_START_STATE:
            switch (code_point)
            {
            case '-':
                state                           = COMMENT_START_DASH_STATE;
                break;
            case '>':
                // todo: parse error
                state                           = DATA_STATE;
                emit_token();
                break;
            default:
                consume                         = false;
                state                           = COMMENT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-start-dash-state
        case COMMENT_START_DASH_STATE:
            if (is_eof)
            {
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '-':
                state                           = COMMENT_END_STATE;
                break;
            case '>':
                // todo: parse error
                state                           = DATA_STATE;
                emit_token();
                break;
            default:
                update_comment_token('-');
                consume                         = false;
                state                           = COMMENT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-state
        case COMMENT_STATE:
            if (is_eof)
            {
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '<':
                update_comment_token_from_buffer();
                state                           = COMMENT_LESS_THAN_STATE;
                break;
            case '-':
                state                           = COMMENT_END_DASH_STATE;
                break;
            case 0:
                // todo: parse error
                update_comment_token_replacement_char();
                break;
            default:
                update_comment_token_from_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-less-than-sign-state
        case COMMENT_LESS_THAN_STATE:
            switch (code_point)
            {
            case '!':
                update_comment_token_from_buffer();
                state                           = COMMENT_LESS_THAN_BANG_STATE;
                break;
            case '<':
                update_comment_token_from_buffer();
                break;
            default:
                state                           = COMMENT_STATE;
                consume                         = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-less-than-sign-bang-state
        case COMMENT_LESS_THAN_BANG_STATE:
            switch (code_point)
            {
            case '-':
                state                           = COMMENT_LESS_THAN_BANG_DASH_STATE;
                break;
            default:
                state                           = COMMENT_STATE;
                consume                         = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-less-than-sign-bang-dash-state
        case COMMENT_LESS_THAN_BANG_DASH_STATE:
            switch (code_point)
            {
            case '-':
                state                           = COMMENT_LESS_THAN_BANG_DASH_DASH_STATE;
                break;
            default:
                state                           = COMMENT_END_DASH_STATE;
                consume                         = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-less-than-sign-bang-dash-dash-state
        case COMMENT_LESS_THAN_BANG_DASH_DASH_STATE:
            state                               = COMMENT_END_STATE;
            consume                             = false;
            // todo: parse error
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-end-dash-state
        case COMMENT_END_DASH_STATE:
            if (is_eof)
            {
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '-':
                state                           = COMMENT_END_STATE;
                break;
            default:
                update_comment_token('-');
                consume                         = false;
                state                           = COMMENT_STATE;
                
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-end-state
        case COMMENT_END_STATE:
            if (is_eof)
            {
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '>':
                state                           = DATA_STATE;
                emit_token();
                break;
            case '!':
                state                           = COMMENT_END_BANG_STATE;
                break;
            case '-':
                update_comment_token('-');
                break;
            default:
                update_comment_token('-');
                update_comment_token('-');
                consume                         = false;
                state                           = COMMENT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#comment-end-bang-state
        case COMMENT_END_BANG_STATE:
            if (is_eof)
            {
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '-':
                update_comment_token('-');
                update_comment_token('-');
                update_comment_token('!');
                state                           = COMMENT_END_DASH_STATE;
                break;
            case '>':
                // todo: parse error
                emit_token();
                state                           = DATA_STATE;
                break;
            default:
                update_comment_token('-');
                update_comment_token('-');
                update_comment_token('!');
                consume                         = false;
                state                           = COMMENT_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#doctype-state
        case DOCTYPE_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                           = BEFORE_DOCTYPE_NAME_STATE;
            }
            else if (code_point == '>')
            {
                consume                         = false;
                state                           = BEFORE_DOCTYPE_NAME_STATE;
            }
            else
            {
                // todo: parse error
                consume                         = false;
                state                           = BEFORE_DOCTYPE_NAME_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#before-doctype-name-state
        case BEFORE_DOCTYPE_NAME_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                state                           = DOCTYPE_NAME_STATE;

                create_doctype_token();
                unsigned char c = (unsigned char)code_point;
                update_doctype_token_name(c + 0x20);

            }
            else if (code_point == 0)
            {
                // todo: parse error
                state                           = DOCTYPE_NAME_STATE;

                create_doctype_token();
                update_doctype_token_name_replacement_char();
            }
            else if (code_point == '>')
            {
                // todo: parse error
                create_doctype_token();
                tokens[token_idx].force_quirks  = true;
                state                           = DATA_STATE;
                emit_token();
            }
            else
            {
                state                           = DATA_STATE;
                create_doctype_token();
                update_doctype_token_name_from_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#doctype-name-state
        case DOCTYPE_NAME_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                           = AFTER_DOCTYPE_NAME_STATE;
            }
            else if (code_point == '>')
            {
                state                           = DATA_STATE;
                emit_token();
            }
            else if (utf8_is_upper_alpha(code_point))
            {
                unsigned char c = (unsigned char)code_point;
                update_doctype_token_name(c + 0x20);
            }
            else if (code_point == 0)
            {
                // todo: parse error

                create_doctype_token();
                update_doctype_token_name_replacement_char();
            }
            else
            {
                update_doctype_token_name_from_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-name-state
        case AFTER_DOCTYPE_NAME_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '>')
            {
                state                           = DATA_STATE;
                emit_token();
            }
            else
            {
                if (match_segment(public_segment, sizeof(public_segment), CASE_INSENSITIVE_MATCH))
                {
                    cursor                         += sizeof(public_segment);
                    state                           = AFTER_DOCTYPE_PUBLIC_KEYWORD_STATE;
                    consume                         = false;
                }
                else if (match_segment(system_segment, sizeof(system_segment), CASE_INSENSITIVE_MATCH))
                {
                    cursor                         += sizeof(system_segment);
                    state                           = AFTER_DOCTYPE_SYSTEM_KEYWORD_STATE;
                    consume                         = false;
                }
                else
                {
                    // todo: parse error
                    tokens[token_idx].force_quirks  = true;
                    consume                         = false;
                    state                           = BOGUS_DOCTYPE_STATE;
                }
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-public-keyword-state
        case AFTER_DOCTYPE_PUBLIC_KEYWORD_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                               = BEFORE_DOCTYPE_PUBLIC_IDENTIFIER_STATE;
            }
            else if (code_point == '"')
            {
                // todo: parse error
                // todo: missing vs empty identifier
                state                               = DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED_STATE;
            }
            else if (code_point == '\'')
            {
                // todo: parse error
                // todo: missing vs empty identifier
                state                               = DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED_STATE;
            }
            else if (code_point == '>')
            {
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                state                               = DATA_STATE;
                emit_token();
            }
            else
            {
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                consume                             = false;
                state                               = BOGUS_DOCTYPE_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#before-doctype-public-identifier-state
        case BEFORE_DOCTYPE_PUBLIC_IDENTIFIER_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '"')
            {
                // todo: parse error
                // todo: missing vs empty identifier
                state                               = DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED_STATE;
            }
            else if (code_point == '\'')
            {
                // todo: parse error
                // todo: missing vs empty identifier
                state                               = DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED_STATE;
            }
            else if (code_point == '>')
            {
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                state                               = DATA_STATE;
                emit_token();
            }
            else
            {
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                consume                             = false;
                state                               = BOGUS_DOCTYPE_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#doctype-public-identifier-(double-quoted)-state
        case DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '"')
            {
                state                               = AFTER_DOCTYPE_PUBLIC_IDENTIFIER_STATE;
            }
            else if (code_point == 0)
            {
                // todo: parse error
                update_doctype_token_public_identifier_replacement_char();
            }
            else if (code_point == '>')
            {
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                state                               = DATA_STATE;
                emit_token();
            }
            else
            {
                update_doctype_public_identifier_from_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#doctype-public-identifier-(single-quoted)-state
        case DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '"')
            {
                state                               = AFTER_DOCTYPE_PUBLIC_IDENTIFIER_STATE;
            }
            else if (code_point == 0)
            {
                // todo: parse error
                update_doctype_token_public_identifier_replacement_char();
            }
            else if (code_point == '>')
            {
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                state                               = DATA_STATE;
                emit_token();
            }
            else
            {
                update_doctype_public_identifier_from_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-public-identifier-state
        case AFTER_DOCTYPE_PUBLIC_IDENTIFIER_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                               = BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS_STATE;
            }
            else if (code_point == '>')
            {
                state                               = DATA_STATE;
                emit_token();
            }
            else if (code_point == '"')
            {
                // todo: parse error
                // todo: missing vs empty system id
                state                               = DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE;
            }
            else if (code_point == '\'')
            {
                // todo: parse error
                // todo: missing vs empty system id
                state                               = DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED_STATE;
            }
            else
            {
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                consume                             = false;
                state                               = BOGUS_DOCTYPE_STATE;
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#between-doctype-public-and-system-identifiers-state
        case BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '>')
            {
                state                               = DATA_STATE;
                emit_token();
            }
            else if (code_point == '"')
            {
                // todo: parse error
                // todo: missing vs empty system id
                state                               = DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE;
            }
            else if (code_point == '\'')
            {
                // todo: parse error
                // todo: missing vs empty system id
                state                               = DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED_STATE;
            }
            else
            {
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                consume                             = false;
                state                               = BOGUS_DOCTYPE_STATE;
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-system-keyword-state
        case AFTER_DOCTYPE_SYSTEM_KEYWORD_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {
                state                               = BEFORE_DOCTYPE_SYSTEM_IDENTIFIER_STATE;
            }
            else if (code_point == '"')
            {
                // todo: parse error
                // todo: missing vs empty system id
                state                               = DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE;
            }
            else if (code_point == '\'')
            {
                // todo: parse error
                // todo: missing vs empty system id
                state                               = DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED_STATE;
            }
            else if (code_point == '>')
            {
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                state                               = DATA_STATE;
                emit_token();
            }
            else
            {
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                consume                             = false;
                state                               = BOGUS_DOCTYPE_STATE;
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#before-doctype-system-identifier-state
        case BEFORE_DOCTYPE_SYSTEM_IDENTIFIER_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_doctype_token();
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '"')
            {
                // todo: parse error
                // todo: missing vs empty system id
                state                               = DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE;
            }
            else if (code_point == '\'')
            {
                // todo: parse error
                // todo: missing vs empty system id
                state                               = DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED_STATE;
            }
            else if (code_point == '>')
            {
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                state                               = DATA_STATE;
                emit_token();
            }
            else
            {
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                consume                             = false;
                state                               = BOGUS_DOCTYPE_STATE;
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#doctype-system-identifier-(double-quoted)-state
        case DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED_STATE:
            if (is_eof)
            {
                // todo: parse error
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '"':
                state                               = AFTER_DOCTYPE_SYSTEM_IDENTIFIER_STATE;
                break;
            case 0:
                update_doctype_token_system_identifier_replacement_char();
                break;
            case '>':
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                state                               = DATA_STATE;
                emit_token();
                break;
            default:
                update_doctype_token_system_identifier_from_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#doctype-system-identifier-(single-quoted)-state
        case DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED_STATE:
            if (is_eof)
            {
                // todo: parse error
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch (code_point)
            {
            case '\'':
                state                               = AFTER_DOCTYPE_SYSTEM_IDENTIFIER_STATE;
                break;
            case 0:
                update_doctype_token_system_identifier_replacement_char();
                break;
            case '>':
                // todo: parse error
                tokens[token_idx].force_quirks      = true;
                state                               = DATA_STATE;
                emit_token();
                break;
            default:
                update_doctype_token_system_identifier_from_buffer();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-system-identifier-state
        case AFTER_DOCTYPE_SYSTEM_IDENTIFIER_STATE:
            if (is_eof)
            {
                // todo: parse error
                tokens[token_idx].force_quirks = true;
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            if (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == ' ')
            {

            }
            else if (code_point == '>')
            {
                state                               = DATA_STATE;
                emit_token();
            }
            else
            {
                // todo: parse error
                state                               = BOGUS_DOCTYPE_STATE;
                consume                             = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#bogus-doctype-state
        case BOGUS_DOCTYPE_STATE:
            if (is_eof)
            {
                emit_token();
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch(code_point)
            {
            case '>':
                state                               = DATA_STATE;
                emit_token();
                break;
            case 0:
                // todo: parse error
                break;
            default:
                break;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#cdata-section-state
        case CDATA_SECTION_STATE:
            if (is_eof)
            {
                // todo: parse error
                create_eof_token();
                emit_token();
                return HTML_TOKENIZER_OK;
            }

            switch(code_point)
            {
            case ']':
                state                               = CDATA_SECTION_BRACKET_STATE;
                break;
            default:
                create_char_token_from_buffer();
                emit_token();
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#cdata-section-bracket-state
        case CDATA_SECTION_BRACKET_STATE:
            switch(code_point)
            {
            case ']':
                create_char_token(']');
                emit_token();
                break;
            case '>':
                state                               = DATA_STATE;
                break;
            default:
                create_char_token(']');
                emit_token();
                state                               = CDATA_SECTION_STATE;
                consume                             = false;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#cdata-section-end-state
        case CDATA_SECTION_END_STATE:
            switch (code_point)
            {
            case ']':
                create_char_token(']');
                emit_token();
                break;
            case '>':
                state                               = DATA_STATE;
                break;
            default:
                create_char_token(']');
                emit_token();
                create_char_token(']');
                emit_token();
                consume                             = false;
                state                               = CDATA_SECTION_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#character-reference-state
        case CHARACTER_REFERENCE_STATE:
            clear_temp_buffer();
            update_temp_buffer('&');
            if (utf8_is_alphanumeric(code_point))
            {
                consume                             = false;
                state                               = NAMED_CHARACTER_REFERENCE_STATE;
            }
            else if (code_point == '#')
            {
                update_temp_buffer('#');
                state                               = NUMERIC_CHARACTER_REFERENCE_STATE;
            }
            else
            {
                if (return_state == ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE || 
                    return_state == ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE ||
                    return_state == ATTRIBUTE_VALUE_UNQUOTED_STATE)
                {
                    flush_temp_buffer_to_attribute_value();
                }
                else
                {
                    emit_temp_buffer();
                }
                consume                             = false;
                state                               = return_state;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#named-character-reference-state
        case NAMED_CHARACTER_REFERENCE_STATE:
            // todo: add support - https://trello.com/c/r9A2c0oE/25-tokenizer-add-support-for-named-character-reference-state
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#ambiguous-ampersand-state
        case AMBIGUOUS_AMPERSAND_STATE:
            if (utf8_is_alphanumeric(code_point))
            {
                if (return_state == ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE || 
                    return_state == ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE ||
                    return_state == ATTRIBUTE_VALUE_UNQUOTED_STATE)
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
        case NUMERIC_CHARACTER_REFERENCE_STATE:
            if (code_point == 'x' || code_point == 'X')
            {
                unsigned char c = (unsigned char)code_point;
                update_temp_buffer(c);
                state                               = HEXADECIMAL_CHARACTER_REFERENCE_START_STATE;
            }
            else
            {
                consume                             = false;
                state                               = DECIMAL_CHARACTER_REFERENCE_START_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#hexadecimal-character-reference-start-state
        case HEXADECIMAL_CHARACTER_REFERENCE_START_STATE:
            if (utf8_is_hex(code_point))
            {
                consume                             = false;
                state                               = HEXADECIMAL_CHARACTER_REFERENCE_START_STATE;
            }
            else
            {
                // todo: parse error
                if (return_state == ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE || 
                    return_state == ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE ||
                    return_state == ATTRIBUTE_VALUE_UNQUOTED_STATE)
                {
                    flush_temp_buffer_to_attribute_value();
                }
                else
                {
                    emit_temp_buffer();
                }
                consume                             = false;
                state                               = return_state;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#decimal-character-reference-start-state
        case DECIMAL_CHARACTER_REFERENCE_START_STATE:
            if (utf8_is_digit(code_point))
            {
                consume                             = false;
                state                               = DECIMAL_CHARACTER_REFERENCE_STATE;
            }
            else
            {
                // todo: parse error
                if (return_state == ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE || 
                    return_state == ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE ||
                    return_state == ATTRIBUTE_VALUE_UNQUOTED_STATE)
                {
                    flush_temp_buffer_to_attribute_value();
                }
                else
                {
                    emit_temp_buffer();
                }
                consume                             = false;
                state                               = return_state;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#hexadecimal-character-reference-state
        case HEXADECIMAL_CHARACTER_REFERENCE_STATE:
            if (utf8_is_digit(code_point))
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
                state                               = NUMERIC_CHARACTER_REFERENCE_END_STATE;
            }
            else
            {
                // todo: parse error
                consume                             = false;
                state                               = NUMERIC_CHARACTER_REFERENCE_END_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#decimal-character-reference-state
        case DECIMAL_CHARACTER_REFERENCE_STATE:
            if (utf8_is_digit(code_point))
            {
                character_reference_code           *= 10;
                character_reference_code           += code_point - 0x30;
            }
            else if (code_point == ';')
            {
                state                               = NUMERIC_CHARACTER_REFERENCE_END_STATE;
            }
            else
            {
                // todo: parse error
                consume                             = false;
                state                               = NUMERIC_CHARACTER_REFERENCE_END_STATE;
            }
            break;

        // https://html.spec.whatwg.org/multipage/parsing.html#numeric-character-reference-end-state
        case NUMERIC_CHARACTER_REFERENCE_END_STATE:
            if (character_reference_code == 0)
            {
                // todo: parse error
                character_reference_code            = 0xfffd;
            }
            else if (character_reference_code >= 0x10ffff)
            {
                // todo: parse error
                character_reference_code            = 0xfffd;
            }
            else if (is_surrogate(character_reference_code))
            {
                // todo: parse error
                character_reference_code            = 0xfffd;
            }
            else if (is_noncharacter(character_reference_code))
            {
                // todo: parse error
            }
            else if (code_point == 0x0d || (utf8_is_control(code_point) && !utf8_is_whitespace(code_point)))
            {
                // todo: parse error
                unsigned char char_ref_buf[4]       = { 0 };
                uint32_t result                     = 0;
                int32_t char_idx                    = find_numeric_char_ref(code_point, &result);
                if (char_idx != -1) { character_reference_code = result; }

                clear_temp_buffer();
                int32_t bytes = utf8_encode(character_reference_code, char_ref_buf);
                for (int32_t i = 0; i < bytes; i++)
                {
                    update_temp_buffer(char_ref_buf[i]);
                }

                state                               = return_state;
                if (return_state == ATTRIBUTE_VALUE_DOUBLE_QUOTED_STATE || 
                    return_state == ATTRIBUTE_VALUE_SINGLE_QUOTED_STATE ||
                    return_state == ATTRIBUTE_VALUE_UNQUOTED_STATE)
                {
                    flush_temp_buffer_to_attribute_value();
                }
                else
                {
                    emit_temp_buffer();
                }
            }
            break;
        }

        if (consume)
        {
            cursor += (uint32_t)bytes_read;
        }
    }

    return HTML_TOKENIZER_OK;
}

void html_tokenizer_free()
{

}
