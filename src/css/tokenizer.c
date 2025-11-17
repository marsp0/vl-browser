#include "tokenizer.h"

#include <stddef.h>

/*
 * Notes
 * 
 */

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/

static css_tokenizer_state_e state      = CSS_TOKENIZER_DATA_STATE;
static const unsigned char* buf         = NULL;
static uint32_t buf_size                = 0;
// static bool is_eof                      = false;
// static uint32_t cp                      = 0;
// static int32_t cp_len                   = 0;
static css_token_t* tokens              = NULL;
static uint32_t token_idx               = 0;
static uint32_t tokens_size             = 0;
static uint32_t max_tokens_size         = 0;
static const uint32_t replacement_char  = 0xfffd;

/********************/
/* static functions */
/********************/

static bool is_whitespace(uint32_t cp)
{
    return cp == '\n' || cp == '\t' || cp == ' ';
}

static bool consume(uint32_t* cp, int32_t* cp_len)
{
    bool is_eof = false;
    *cp_len  = -1;

    if (buf_cur >= buf_size)
    {
        *cp      = 0;
        is_eof  = true;
    }
    else
    {
        cp_len = utf8_decode(buf, buf_size, buf_cur, &cp);

        if (cp_len <= -1) { cp = 0; }
    }

    return is_eof;
}


static void reconsume(int32_t cp_len)
{
    if (cp_len <= 0) { return; }

    buf_cur -= cp_len;
}


static bool peek(uint32_t count, uint32_t* code_point, uint32_t* code_point_len)
{
    uint32_t cur = buf_cur;

    for (uint32_t i = 0; i < count; i++)
    {
        *code_point = 0;
        *code_point_len  = -1;

        if (cur >= buf_size) { return true; }

        *code_point_len = utf8_decode(buf, buf_size, cur, code_point);
        cur += *code_point_len;
    }

    return false;
}


static uint32_t cp_to_number(uint32_t cp)
{
    if      (utf8_is_digit(cp))     { return cp - 0x30; }
    else if (utf8_is_upper_hex(cp)) { return cp - 0x37; }
    else if (utf8_is_lower_hex(cp)) { return cp - 0x57; }

    return 0;
}


static uint32_t consume_escaped_cp()
{
    uint32_t cp     = 0;
    int32_t cp_len  = -1;
    uint32_t i      = 1;

    bool is_eof = consume(&cp, &cp_len);

    if (is_eof) { return replacement_char; }

    if (utf8_is_hex(cp))
    {
        uint32_t digit = cp_to_number(cp);
    
        while (true)
        {
            bool is_eof = peek(&cp, &cp_len);
            if (is_eof) { break; }

            consume(&cp, &cp_len);

            if (is_whitespace(cp)) { break; }

            digit *= 16;
            digit += cp_to_number(cp);
            i++;
    
            if (i >=6) { break; }
        }

        if (digit == 0 || utf8_is_surrogate(digit) || digit > 0x10FFFF)
        {
            return replacement_char;
        }

        return digit;
    }

    return cp;
}


static check_if_two_cp_are_valid_escape(uint32_t offset)
{
    uint32_t cp = 0;
    int32_t cp_len = -1;
    bool is_eof = false;

    is_eof = peek(offset + 1, &cp, &cp_len);

    if (is_eof || cp != '\\') { return false; }

    is_eof = peek(offset + 2, &cp, &cp_len);

    if (is_eof || cp != '\n') { return false; }

    return true;
}


static check_if_three_cp_start_ident()
{
    uint32_t cp = 0;
    int32_t cp_len = -1;
    bool is_eof = false;

    peek(1, &cp, &cp_len);

    if (cp == '-')
    {
        uint32_t cp2 = 0;
        int32_t cp2_len = -1;

        peek(2, &cp2, &cp2_len);
        if (is_ident_start(cp2) || cp2 == '-' || check_if_two_cp_are_valid_escape(1))
        {
            return true;
        }
        return false;
    }
    else if (is_ident_start(cp))
    {
        return true;
    }
    else if (cp == '\\' && check_if_two_cp_are_valid_escape(0))
    {
        return true;
    }

    return false;
}


static bool is_ident_start(uint32_t cp)
{
    return utf8_is_alpha(cp) || cp == '_' || cp > 0x7f;
}


static bool is_ident(uint32_t cp)
{
    return is_ident_start(cp) || utf8_is_digit(cp) || cp == '-';
}


static void update_data(css_token_t* t, uint32_t cp)
{
    if (t->data_size + 4 > CSS_TOKEN_MAX_DATA_SIZE) { return; }

    t->data_size += utf8_encode(cp, &(t->data[t->data_size]))
}


static css_token_t consume_string()
{
    css_token_t result = { 0 };
    result.type = CSS_TOKEN_STRING;
    uint32_t cp = 0;
    uint32_t cp_len = -1;
    uint32_t end_cp = cp;
    

    while (true)
    {
        consume(&cp, &cp_len);

        if (cp == end_cp || is_eof)
        {
            break;
        }
        else if (cp == '\n')
        {
            reconsume(cp_len);
            result.type = CSS_TOKEN_BAD_STRING;
            break;
        }
        else if (cp == '\\')
        {
            uint32_t next_cp = 0;
            uint32_t next_cp_len = 0;
            peek(1, &next_cp, &next_cp_len);

            if (next_cp_len < 0) { continue; }

            if (next_cp == '\n')
            {
                consume(&cp, &cp_len);
            }
            else
            {
                uint32_t cp = consume_escaped_cp();
                update_data(cp, &result);
            }
            
        }
        else
        {
            update_data(cp, &result);
        }
    }

    return result;
}


/********************/
/* public functions */
/********************/


void css_tokenizer_init(unsigned char* buffer, uint32_t buffer_size, css_token_t* tokens_out, uint32_t tokens_out_size)
{
    // is_eof              = false;
    buf                 = buffer;
    buf_size            = buffer_size;
    tokens              = tokens_out;
    tokens_size         = 0;
    max_tokens_size     = tokens_out_size;
    state               = CSS_TOKENIZER_DATA_STATE;
}


css_token_t css_tokenizer_next()
{
    css_token_t result = { 0 };
    uint32_t cp = 0;
    int32_t cp_len = -1;

    consume(&cp, &cp_len);

    if (cp == '\n' || cp == '\t' || cp == ' ')
    {
        result.type = CSS_TOKEN_WHITESPACE;
    }
    else if (cp == '"' || cp == '\'')
    {
        return consume_string();
    }
    else if (cp == '#')
    {
        uint32_t cp1 = 0;
        int32_t cp1_len = -1;

        peek(1, &cp1, &cp1_len);


        if (is_ident(cp1) || check_if_two_cp_are_valid_escape(0))
        {
            result.type = CSS_TOKEN_HASH;
            if (check_if_three_cp_start_ident())
            {
                result.hash_type = CSS_TOKEN_HASH_ID;
            }

            
        }
        else
        {
            result.type = CSS_TOKEN_DELIM;
            update_data(cp, &result);
        }
    }

    return result;
}


void css_tokenizer_free()
{

}
