#include "tokenizer.h"

#include <stddef.h>
#include <string.h>

#include "util/utf8.h"
#include "css/util.h"

/*
 * Notes
 * 
 */

/********************/
/*      defines     */
/********************/

#define MAX_REPR_SIZE 32

/********************/
/* static variables */
/********************/

typedef enum
{
    CSS_NUMBER_INTEGER,
    CSS_NUMBER_FLOAT
} css_number_type_e;

static css_tokenizer_state_e state      = CSS_TOKENIZER_DATA_STATE;
static css_tokenizer_state_e prev_state = CSS_TOKENIZER_DATA_STATE;
static const unsigned char* buf         = NULL;
static uint32_t buf_cur                 = 0;
static uint32_t buf_size                = 0;
static const uint32_t replacement       = 0xfffd;

/********************/
/* static functions */
/********************/


static bool consume(uint32_t* cp, int32_t* cp_len)
{
    *cp_len = -1;
    *cp = 0;

    if (buf_cur >= buf_size) { return true; }

    *cp_len = utf8_decode(buf, buf_size, buf_cur, cp);

    buf_cur += (uint32_t)*cp_len;

    return false;
}

static void reconsume(int32_t cp_len)
{
    if (cp_len <= 0) { return; }

    buf_cur -= (uint32_t)cp_len;
}

static void peek(uint32_t offset, uint32_t* cp, int32_t* cp_len)
{
    *cp_len = 0;
    uint32_t buf_cur_offset = 0;

    for (uint32_t i = 0; i < offset; i++)
    {
        *cp_len = utf8_decode(buf, buf_size, buf_cur + buf_cur_offset, cp);
        buf_cur_offset += (uint32_t)(*cp_len);

        if (*cp_len <= 0)
        {
            *cp = 0;
            break;
        }
    }

    return;
}


static void update_data(css_token_t* t, uint32_t cp)
{
    if (t->data_size + 4 > CSS_TOKEN_MAX_DATA_SIZE) { return; }

    t->data_size += (uint32_t)utf8_encode(cp, &(t->data[t->data_size]));
}


static bool is_whitespace(uint32_t cp)
{
    return cp == '\n' || cp == '\t' || cp == ' ';
}


static void consume_escaped_cp(uint32_t* cp)
{
    uint32_t cp_n = 0;
    int32_t cp_n_len = -1;
    bool is_eof = false;
    uint32_t digits = 0;
    uint32_t result = 0;

    while (true)
    {
        is_eof = consume(&cp_n, &cp_n_len);

        if (is_eof || cp_n == 0)
        {
            *cp = replacement;
            return;
        }
        else if (is_whitespace(cp_n))
        {
            break;
        }
        else if (digits >= 6)
        {
            reconsume(cp_n_len);
            break;
        }
        else if (utf8_is_digit(cp_n))
        {
            result  *= 16;
            result  += cp_n - 0x30;
            digits  += 1;
        }
        else if (utf8_is_upper_hex(cp_n))
        {
            result  *= 16;
            result  += cp_n - 0x37;
            digits  += 1;
        }
        else if (utf8_is_lower_hex(cp_n))
        {
            result  *= 16;
            result  += cp_n - 0x57;
            digits  += 1;
        }
        else
        {
            *cp = cp_n;
            return;
        }
    }

    if (result == 0 || utf8_is_surrogate(result) || result > 0x10ffff)
    {
        *cp = replacement;
    }
    else
    {
        *cp = result;
    }
}


static bool is_nonprintable(uint32_t cp)
{
    return  (cp > '\0' && cp <= '\\') ||
            (cp == '\t') ||
            (cp >= 0x0e && cp <= 0x1f) ||
            (cp == 0x7f);
}


static bool is_id_start(uint32_t cp)
{
    return utf8_is_alpha(cp) || cp >= 0x80 || cp == '_';
}


static bool is_id(uint32_t cp)
{
    return is_id_start(cp) || utf8_is_digit(cp) || cp == '-';
}


static bool is_valid_escape(uint32_t cp, uint32_t cp_n)
{
    if (cp != '\\')     { return false; }
    if (cp_n == '\n')   { return false; }

    return true;
}


static bool is_number_start(uint32_t cp1, uint32_t cp2, uint32_t cp3)
{
    if (cp1 == '+' || cp1 == '-')
    {
        if (utf8_is_digit(cp2))
        {
            return true;
        }
        else if (cp2 == '.' && utf8_is_digit(cp3))
        {
            return true;
        }

        return false;
    }
    else if (cp1 == '.')
    {
        if (utf8_is_digit(cp2))
        {
            return true;
        }

        return false;
    }
    else if (utf8_is_digit(cp1))
    {
        return true;
    }

    return false;
}


static bool is_id_seq_start(uint32_t cp1, uint32_t cp2, uint32_t cp3)
{
    if (cp1 == '-')
    {
        if ((is_id_start(cp2) || cp2 == '-') || (is_valid_escape(cp2, cp3)))
        {
            return true;
        }

        return false;
    }
    else if (is_id_start(cp1))
    {
        return true;
    }
    else if (cp1 == '\\')
    {
        if (is_valid_escape(cp1, cp2))
        {
            return true;
        }

        return false;
    }

    return false;
}

static bool compare_seq(css_token_t* t, unsigned char* data, uint32_t data_size)
{
    if (data_size > CSS_TOKEN_MAX_DATA_SIZE)
    {
        return false;
    }

    unsigned char* t_data = t->data;

    for (uint32_t i = 0; i < data_size; i++)
    {
        unsigned char a = data[i];
        unsigned char b = t_data[i];

        if (a < 'a')    { a += 0x20; }
        if (b < 'a')    { b += 0x20; }

        if (a != b)     { return false; }
    }

    return true;
}


static void consume_string(css_token_t* t, uint32_t end_cp)
{
    uint32_t cp = 0;
    int32_t cp_len = -1;
    bool is_eof = false;

    while (true)
    {
        is_eof = consume(&cp, &cp_len);

        if (is_eof || cp == end_cp)
        {
            return;
        }
        else if (cp == '\n')
        {
            reconsume(cp_len);
            memset(t->data, 0, CSS_TOKEN_MAX_DATA_SIZE);
            t->data_size = 0;
            t->type = CSS_TOKEN_BAD_STRING;
            return;
        }
        else if (cp == '\\')
        {
            uint32_t cp_n = 0;
            int32_t cp_n_len = -1;

            peek(1, &cp_n, &cp_n_len);

            if (is_eof)
            {
                continue;
            }
            else if (cp_n == '\n')
            {
                consume(&cp_n, &cp_n_len);
            }
            else
            {
                consume_escaped_cp(&cp_n);
                update_data(t, cp_n);
            }
        }
        else
        {
            update_data(t, cp);
        }
    }
}


static void consume_id_seq(css_token_t* t)
{
    uint32_t cp = 0;
    uint32_t cp_n = 0;
    int32_t cp_len = -1;
    int32_t cp_n_len = -1;

    while (true)
    {
        consume(&cp, &cp_len);

        peek(1, &cp_n, &cp_n_len);

        if (is_id(cp))
        {
            update_data(t, cp);
        }
        else if (is_valid_escape(cp, cp_n))
        {
            consume_escaped_cp(&cp);
            update_data(t, cp);
        }
        else
        {
            reconsume(cp_len);
            return;
        }
    }
}


static void consume_bad_url()
{
    uint32_t cp1 = 0;
    int32_t cp1_len = -1;

    uint32_t cp2 = 0;
    int32_t cp2_len = -1;
    
    bool is_eof = false;

    is_eof = consume(&cp1, &cp1_len);

    peek(1, &cp2, &cp2_len);

    while (true)
    {
        if (is_eof || cp2 == ')')
        {
            break;
        }
        else if (is_valid_escape(cp1, cp2))
        {
            consume_escaped_cp(&cp1);
        }

        is_eof = consume(&cp1, &cp1_len);
        peek(1, &cp2, &cp2_len);
    }
}


static void consume_url_token(css_token_t* t)
{
    t->type = CSS_TOKEN_URL;

    memset(t->data, 0, CSS_TOKEN_MAX_DATA_SIZE);
    t->data_size = 0;

    uint32_t cp1 = 0;
    int32_t cp1_len = -1;
    uint32_t cp2 = 0;
    int32_t cp2_len = -1;
    bool is_eof = false;

    peek(1, &cp1, &cp1_len);
    peek(2, &cp2, &cp2_len);

    while (is_whitespace(cp1))
    {
        is_eof = consume(&cp1, &cp1_len);
    }

    // is_eof = consume(&cp1, &cp1_len);

    while (true)
    {
        if (is_eof || cp1 == ')')
        {
            // t->type = CSS_TOKEN_URL;
            return;
        }
        else if (is_whitespace(cp1))
        {
            while(true)
            {
                if (is_whitespace(cp1))
                {
                    // ignore
                }
                else if (is_eof || cp1 == ')')
                {
                    t->type = CSS_TOKEN_URL;
                    return;
                }
                else
                {
                    consume_bad_url();
                    memset(t->data, 0, CSS_TOKEN_MAX_DATA_SIZE);
                    t->data_size = 0;
                    t->type = CSS_TOKEN_BAD_URL;
                    return;
                }

                is_eof = consume(&cp1, &cp1_len);
            }
        }
        else if (cp1 == '"'  ||
                 cp1 == '\'' ||
                 cp1 == '('  ||
                 is_nonprintable(cp1))
        {
            consume_bad_url();
            memset(t->data, 0, CSS_TOKEN_MAX_DATA_SIZE);
            t->data_size = 0;
            t->type = CSS_TOKEN_BAD_URL;
        }
        else if (cp1 == '\\')
        {
            if (is_valid_escape(cp1, cp2))
            {
                consume_escaped_cp(&cp1);
                update_data(t, cp1);
            }
            else
            {
                consume_bad_url();
                memset(t->data, 0, CSS_TOKEN_MAX_DATA_SIZE);
                t->data_size = 0;
                t->type = CSS_TOKEN_BAD_URL;
            }
        }
        else
        {
            update_data(t, cp1);
        }

        is_eof = consume(&cp1, &cp1_len);
    }
}


static void consume_id_token(css_token_t* t)
{
    uint32_t cp1 = 0;
    int32_t cp1_len = -1;

    uint32_t cp2 = 0;
    int32_t cp2_len = -1;

    consume_id_seq(t);
    peek(1, &cp1, &cp1_len);

    if (compare_seq(t, "url", 3) && cp1 == '(')
    {
        consume(&cp1, &cp1_len);

        peek(1, &cp1, &cp1_len);
        peek(2, &cp2, &cp2_len);

        while (is_whitespace(cp1) && is_whitespace(cp2))
        {
            consume(&cp1, &cp1_len);
            peek(1, &cp1, &cp1_len);
            peek(2, &cp2, &cp2_len);
        }

        if ((cp1 == '"' || cp1 == '\'' || is_whitespace(cp1)) && (cp2 == '"' || cp2 == '\''))
        {
            t->type = CSS_TOKEN_FUNCTION;
            return;
        }

        consume_url_token(t);
    }
    else if (cp1 == '(')
    {
        consume(&cp1, &cp1_len);
        t->type = CSS_TOKEN_FUNCTION;
    }
    else
    {
        t->type = CSS_TOKEN_IDENT;
    }
}


static void consume_number(css_token_t* token)
{
    uint32_t repr[MAX_REPR_SIZE] = { 0 };
    uint32_t repr_idx = 0;
    // css_number_type_e type = CSS_NUMBER_INTEGER;

    uint32_t cp = 0;
    uint32_t cp2 = 0;
    uint32_t cp3 = 0;
    int32_t cp_len = -1;
    int32_t cp2_len = -1;
    int32_t cp3_len = -1;

    peek(1, &cp, &cp_len);

    if (cp == '+' || cp == '-')
    {
        consume(&cp, &cp_len);
        repr[repr_idx] = cp;
        repr_idx++;
    }

    peek(1, &cp, &cp_len);

    while (utf8_is_digit(cp))
    {
        consume(&cp, &cp_len);
        repr[repr_idx] = cp;
        repr_idx++;
        peek(1, &cp, &cp_len);

        if (repr_idx >= MAX_REPR_SIZE) { break; }
    }

    peek(1, &cp, &cp_len);
    peek(2, &cp2, &cp2_len);

    if (cp == '.' && utf8_is_digit(cp2))
    {
        consume(&cp, &cp_len);
        repr[repr_idx] = cp;
        repr_idx++;

        consume(&cp, &cp_len);
        repr[repr_idx] = cp;
        repr_idx++;

        // type = CSS_NUMBER_FLOAT;

        peek(1, &cp, &cp_len);

        while (utf8_is_digit(cp))
        {
            consume(&cp, &cp_len);
            repr[repr_idx] = cp;
            repr_idx++;
            peek(1, &cp, &cp_len);
    
            if (repr_idx >= MAX_REPR_SIZE) { break; }
        }
    }
    
    peek(3, &cp3, &cp3_len);

    bool esd = (cp == 'e' || cp == 'E') && (cp2 == '+' || cp2 == '-') && utf8_is_digit(cp3);
    bool ed = (cp == 'e' || cp == 'E') && utf8_is_digit(cp2);

    if (esd || ed)
    {
        // type = CSS_NUMBER_FLOAT;

        consume(&cp, &cp_len);
        repr[repr_idx] = cp;
        repr_idx++;

        consume(&cp, &cp_len);
        repr[repr_idx] = cp;
        repr_idx++;

        if (esd)
        {
            consume(&cp, &cp_len);
            repr[repr_idx] = cp;
            repr_idx++;
        }

        peek(1, &cp, &cp_len);

        while (utf8_is_digit(cp))
        {
            consume(&cp, &cp_len);
            repr[repr_idx] = cp;
            repr_idx++;
            peek(1, &cp, &cp_len);
    
            if (repr_idx >= MAX_REPR_SIZE) { break; }
        }
    }

    double res = css_convert_buf_to_num(repr, MAX_REPR_SIZE);
    token->real = (float)res;
    token->integer = (int32_t)res;
}


static void consume_numeric_token(css_token_t* t)
{
    uint32_t cp1 = 0;
    int32_t cp1_len = -1;

    uint32_t cp2 = 0;
    int32_t cp2_len = -1;

    uint32_t cp3 = 0;
    int32_t cp3_len = -1;

    consume_number(t);

    peek(1, &cp1, &cp1_len);
    peek(2, &cp2, &cp2_len);
    peek(3, &cp3, &cp3_len);

    if (is_id_seq_start(cp1, cp2, cp3))
    {
        t->type = CSS_TOKEN_DIMENSION;
        consume_id_seq(t);
    }
    else if (cp1 == '%')
    {
        consume(&cp1, &cp2_len);
        t->type = CSS_TOKEN_PERCENTAGE;
    }
    else
    {
        t->type = CSS_TOKEN_NUMBER;
    }
}


static bool consume_comment()
{
    uint32_t cp1 = 0;
    int32_t cp1_len = -1;

    uint32_t cp2 = 0;
    int32_t cp2_len = -1;
    bool is_eof = false;

    peek(1, &cp1, &cp1_len);
    peek(2, &cp2, &cp2_len);

    if (cp1 == '/' && cp2 == '*')
    {
        is_eof = consume(&cp1, &cp1_len);
        is_eof = consume(&cp2, &cp2_len);

        while (true)
        {
            peek(1, &cp1, &cp1_len);
            peek(2, &cp2, &cp2_len);

            if (is_eof || (cp1 == '*' && cp2 == '/'))
            {
                is_eof = consume(&cp1, &cp1_len);
                is_eof = consume(&cp2, &cp2_len);
                return true;
            }

            is_eof = consume(&cp1, &cp1_len);
        }
    }

    return false;
}

/********************/
/* public functions */
/********************/


void css_tokenizer_init(unsigned char* buffer, uint32_t buffer_size)
{
    buf                 = buffer;
    buf_cur             = 0;
    buf_size            = buffer_size;
    state               = CSS_TOKENIZER_DATA_STATE;
    prev_state          = CSS_TOKENIZER_DATA_STATE;
}


css_token_t css_tokenizer_next()
{
    css_token_t t   = { .type = CSS_TOKEN_EOF };
    uint32_t cp = 0;
    int32_t cp_len = -1;
    bool is_eof = false;

    bool is_comment = consume_comment();

    if (is_comment)
    {
        t.type = CSS_TOKEN_COMMENT;
        return t;
    }

    is_eof = consume(&cp, &cp_len);

    if (is_eof)
    {
        t.type  = CSS_TOKEN_EOF;
    }
    else if (is_whitespace(cp))
    {
        t.type  = CSS_TOKEN_WHITESPACE;
    }
    else if (cp == '\'' || cp == '"')
    {
        t.type  = CSS_TOKEN_STRING;
        consume_string(&t, cp);        
    }
    else if (cp == '#')
    {
        uint32_t cp_n = 0;
        int32_t cp_n_len = -1;

        uint32_t cp_nn = 0;
        int32_t cp_nn_len = -1;

        peek(1, &cp_n, &cp_n_len);
        peek(2, &cp_nn, &cp_nn_len);

        if (is_id(cp_n) || is_valid_escape(cp_n, cp_nn))
        {
            t.type = CSS_TOKEN_HASH;
            // todo: check 3 cps
            consume_id_seq(&t);
        }
        else
        {
            t.type = CSS_TOKEN_DELIM;
        }
    }
    else if (cp == '(')
    {
        t.type  = CSS_TOKEN_OPEN_PARENTHESIS;
    }
    else if (cp == ')')
    {
        t.type  = CSS_TOKEN_CLOSED_PARENTHESIS;
    }
    else if (cp == '+')
    {
        uint32_t cp2 = 0;
        int32_t cp2_len = -1;

        uint32_t cp3 = 0;
        int32_t cp3_len = -1;

        peek(1, &cp2, &cp2_len);
        peek(2, &cp3, &cp3_len);

        if (is_number_start(cp, cp2, cp3))
        {
            reconsume(cp_len);
            consume_numeric_token(&t);
        }
        else
        {
            t.type = CSS_TOKEN_DELIM;
        }
    }
    else if (cp == ',')
    {
        t.type  = CSS_TOKEN_COMMA;
    }
    else if (cp == '-')
    {
        uint32_t cp2 = 0;
        int32_t cp2_len = -1;

        uint32_t cp3 = 0;
        int32_t cp3_len = -1;

        peek(1, &cp2, &cp2_len);
        peek(2, &cp3, &cp3_len);

        if (is_number_start(cp, cp2, cp3))
        {
            reconsume(cp_len);
            consume_numeric_token(&t);
        }
        else if (cp2 == '-' && cp3 == '>')
        {
            consume(&cp2, &cp2_len);
            consume(&cp3, &cp3_len);
            t.type = CSS_TOKEN_CDC;
        }
        else if (is_id_seq_start(cp, cp2, cp3))
        {
            reconsume(cp_len);
            consume_id_token(&t);
        }
        else
        {
            update_data(&t, cp);
            t.type = CSS_TOKEN_DELIM;
        }
    }
    else if (cp == '.')
    {
        uint32_t cp2 = 0;
        int32_t cp2_len = -1;

        uint32_t cp3 = 0;
        int32_t cp3_len = -1;

        peek(1, &cp2, &cp2_len);
        peek(2, &cp3, &cp3_len);

        if (is_number_start(cp, cp2, cp3))
        {
            reconsume(cp_len);
            consume_numeric_token(&t);
        }
        else
        {
            update_data(&t, cp);
            t.type = CSS_TOKEN_DELIM;
        }
    }
    else if (cp == ':')
    {
        t.type  = CSS_TOKEN_COLON;
    }
    else if (cp == ';')
    {
        t.type  = CSS_TOKEN_SEMICOLON;
    }
    else if (cp == '<')
    {
        uint32_t cp2 = 0;
        int32_t cp2_len = -1;

        uint32_t cp3 = 0;
        int32_t cp3_len = -1;

        uint32_t cp4 = 0;
        int32_t cp4_len = -1;

        peek(1, &cp2, &cp2_len);
        peek(2, &cp3, &cp3_len);
        peek(3, &cp4, &cp4_len);

        if (cp2 == '!' && cp3 == '-' && cp4 == '-')
        {
            consume(&cp2, &cp2_len);
            consume(&cp3, &cp3_len);
            consume(&cp4, &cp4_len);
            t.type = CSS_TOKEN_CDO;
        }
        else
        {
            t.type = CSS_TOKEN_DELIM;
        }
    }
    else if (cp == '@')
    {
        uint32_t cp2 = 0;
        int32_t cp2_len = -1;

        uint32_t cp3 = 0;
        int32_t cp3_len = -1;

        uint32_t cp4 = 0;
        int32_t cp4_len = -1;

        peek(1, &cp2, &cp2_len);
        peek(2, &cp3, &cp3_len);
        peek(3, &cp4, &cp4_len);

        if (is_id_seq_start(cp2, cp3, cp4))
        {
            consume_id_seq(&t);
            t.type = CSS_TOKEN_AT_KEYWORD;
        }
        else
        {
            update_data(&t, cp);
            t.type = CSS_TOKEN_DELIM;
        }
    }
    else if (cp == '[')
    {
        t.type  = CSS_TOKEN_OPEN_BRACKET;
    }
    else if (cp == '\\')
    {
        uint32_t cp2 = 0;
        int32_t cp2_len = -1;

        uint32_t cp3 = 0;
        int32_t cp3_len = -1;

        uint32_t cp4 = 0;
        int32_t cp4_len = -1;

        peek(1, &cp2, &cp2_len);
        peek(2, &cp3, &cp3_len);
        peek(3, &cp4, &cp4_len);

        if (is_valid_escape(cp, cp2))
        {
            reconsume(cp_len);
            consume_id_token(&t);
        }
        else
        {
            update_data(&t, cp);
            t.type = CSS_TOKEN_DELIM;
        }
    }
    else if (cp == ']')
    {
        t.type  = CSS_TOKEN_CLOSED_BRACKET;
    }
    else if (cp == '{')
    {
        t.type  = CSS_TOKEN_OPEN_BRACE;
    }
    else if (cp == '}')
    {
        t.type  = CSS_TOKEN_CLOSED_BRACE;
    }
    else if (utf8_is_digit(cp))
    {
        reconsume(cp_len);
        consume_numeric_token(&t);
    }
    else if (is_id_start(cp))
    {
        reconsume(cp_len);
        consume_id_token(&t);
    }
    else
    {
        update_data(&t, cp);
        t.type  = CSS_TOKEN_DELIM;
    }

    return t;
}


void css_tokenizer_free()
{

}
