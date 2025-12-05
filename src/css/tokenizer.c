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
    CSS_TOKENIZER_STATE_DATA,
    CSS_TOKENIZER_STATE_WHITESPACE,
    CSS_TOKENIZER_STATE_ID_TOKEN,
    CSS_TOKENIZER_STATE_ID_SEQ,
    CSS_TOKENIZER_STATE_ID_SEQ_ESCAPE_START,
    CSS_TOKENIZER_STATE_ID_SEQ_ESCAPE,
    CSS_TOKENIZER_STATE_ID_SEQ_ESCAPE_END,
    CSS_TOKENIZER_STATE_URL_START,
    CSS_TOKENIZER_STATE_URL,
    CSS_TOKENIZER_STATE_URL_END,
    CSS_TOKENIZER_STATE_URL_ESCAPE_START,
    CSS_TOKENIZER_STATE_URL_ESCAPE,
    CSS_TOKENIZER_STATE_URL_ESCAPE_END,
    CSS_TOKENIZER_STATE_BAD_URL,
    CSS_TOKENIZER_STATE_BAD_URL_ESCAPE_START,
    CSS_TOKENIZER_STATE_BAD_URL_ESCAPE,
    CSS_TOKENIZER_STATE_BAD_URL_ESCAPE_END,
    CSS_TOKENIZER_STATE_ESCAPE_START,
    CSS_TOKENIZER_STATE_MINUS,
    CSS_TOKENIZER_STATE_NUMERIC_TOKEN,
    CSS_TOKENIZER_STATE_NUMERIC_TOKEN_START,
    CSS_TOKENIZER_STATE_NUMERIC_TOKEN_END,
    CSS_TOKENIZER_STATE_NUMBER,
    CSS_TOKENIZER_STATE_NUMBER_END,
    CSS_TOKENIZER_STATE_STRING,
    CSS_TOKENIZER_STATE_STRING_ESCAPE_START,
    CSS_TOKENIZER_STATE_STRING_ESCAPE,
    CSS_TOKENIZER_STATE_STRING_ESCAPE_END,
    CSS_TOKENIZER_STATE_AT,
    CSS_TOKENIZER_STATE_AT_COMPLETE,
    CSS_TOKENIZER_STATE_COMMENT,
    CSS_TOKENIZER_STATE_COMMENT_START
} css_tokenizer_state_e;

static const unsigned char* buf         = NULL;
static uint32_t buf_cur                 = 0;
static uint32_t buf_size                = 0;
static const uint32_t replacement       = 0xfffd;
static uint32_t t_buf[20]              = { 0 };
static uint32_t t_buf_size             = 0;

/********************/
/* static functions */
/********************/


// static bool consume(uint32_t* cp, int32_t* cp_len)
// {
//     *cp_len = -1;
//     *cp = 0;

//     if (buf_cur >= buf_size) { return true; }

//     *cp_len = utf8_decode(buf, buf_size, buf_cur, cp);

//     buf_cur += (uint32_t)*cp_len;

//     return false;
// }

// static void reconsume(int32_t cp_len)
// {
//     if (cp_len <= 0) { return; }

//     buf_cur -= (uint32_t)cp_len;
// }

// static bool peek(uint32_t offset, uint32_t* cp, int32_t* cp_len)
// {
//     *cp_len = 0;
//     uint32_t buf_cur_offset = 0;

//     for (uint32_t i = 0; i < offset; i++)
//     {
//         *cp_len = utf8_decode(buf, buf_size, buf_cur + buf_cur_offset, cp);
//         buf_cur_offset += (uint32_t)(*cp_len);

//         if (*cp_len <= 0)
//         {
//             *cp = 0;
//             return true;
//         }
//     }

//     return false;
// }


static void update_data(css_token_t* t, uint32_t cp)
{
    if (t->data_size + 4 > CSS_TOKEN_MAX_DATA_SIZE) { return; }

    t->data_size += (uint32_t)utf8_encode(cp, &(t->data[t->data_size]));
}


static bool is_whitespace(uint32_t cp)
{
    return cp == '\n' || cp == '\t' || cp == ' ';
}


static void t_buf_clear()
{
    memset(t_buf, 0, sizeof(t_buf));
    t_buf_size = 0;
}


// static void t_buf_update(uint32_t cp)
// {
//     t_buf[t_buf_size] = cp;
//     t_buf_size++;
// }


static bool t_buf_contains(uint32_t cp)
{
    for (uint32_t i = 0; i < t_buf_size; i++)
    {
        if (t_buf[i] == cp)
        {
            return true;
        }
    }

    return false;
}


// static void consume_escaped_cp(uint32_t* cp)
// {
//     uint32_t cp_n = 0;
//     int32_t cp_n_len = -1;
//     bool is_eof = false;
//     uint32_t digits = 0;
//     uint32_t result = 0;

//     while (true)
//     {
//         is_eof = consume(&cp_n, &cp_n_len);

//         if (is_eof || cp_n == 0)
//         {
//             *cp = replacement;
//             return;
//         }
//         else if (is_whitespace(cp_n) && digits > 0)
//         {
//             break;
//         }
//         else if (digits >= 6)
//         {
//             reconsume(cp_n_len);
//             break;
//         }
//         else if (utf8_is_digit(cp_n))
//         {
//             result  *= 16;
//             result  += cp_n - 0x30;
//             digits  += 1;
//         }
//         else if (utf8_is_upper_hex(cp_n))
//         {
//             result  *= 16;
//             result  += cp_n - 0x37;
//             digits  += 1;
//         }
//         else if (utf8_is_lower_hex(cp_n))
//         {
//             result  *= 16;
//             result  += cp_n - 0x57;
//             digits  += 1;
//         }
//         else
//         {
//             *cp = cp_n;
//             return;
//         }
//     }

//     if (result == 0 || utf8_is_surrogate(result) || result > 0x10ffff)
//     {
//         *cp = replacement;
//     }
//     else
//     {
//         *cp = result;
//     }
// }


static bool is_nonprintable(uint32_t cp)
{
    return  (cp > '\0' && cp <= 0x08) ||
            (cp == '\t') ||
            (cp >= 0x0e && cp <= 0x1f) ||
            (cp == 0x7f);
}


static bool is_non_ascii_ident_cp(uint32_t cp)
{
    return  (cp == 0xb7) ||
            (cp >= 0xc0 && cp <= 0xd6) ||
            (cp >= 0xd8 && cp <= 0xf6) ||
            (cp >= 0xf8 && cp <= 0x037d) ||
            (cp >= 0x037f && cp <= 0x1fff) ||
            (cp == 0x200c) ||
            (cp == 0x200d) ||
            (cp == 0x203f) ||
            (cp == 0x2040) ||
            (cp >= 0x2070 && cp <= 0x218f) ||
            (cp >= 0x2c00 && cp <= 0x2fef) ||
            (cp >= 0x3001 && cp <= 0xd7ff) ||
            (cp >= 0xf900 && cp <= 0xfdcf) ||
            (cp >= 0xfdf0 && cp <= 0xfffd) ||
            (cp >= 0x10000);
}


static bool is_id_start(uint32_t cp)
{
    return utf8_is_alpha(cp) || is_non_ascii_ident_cp(cp) || cp == '_';
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


// static void consume_string(css_token_t* t, uint32_t end_cp)
// {
//     uint32_t cp = 0;
//     int32_t cp_len = -1;
//     bool is_eof = false;

//     while (true)
//     {
//         is_eof = consume(&cp, &cp_len);

//         if (is_eof || cp == end_cp)
//         {
//             return;
//         }
//         else if (cp == '\n')
//         {
//             reconsume(cp_len);
//             memset(t->data, 0, CSS_TOKEN_MAX_DATA_SIZE);
//             t->data_size = 0;
//             t->type = CSS_TOKEN_BAD_STRING;
//             return;
//         }
//         else if (cp == '\\')
//         {
//             uint32_t cp_n = 0;
//             int32_t cp_n_len = -1;

//             is_eof = peek(1, &cp_n, &cp_n_len);

//             if (is_eof)
//             {
//                 continue;
//             }
//             else if (cp_n == '\n')
//             {
//                 consume(&cp_n, &cp_n_len);
//             }
//             else
//             {
//                 consume_escaped_cp(&cp_n);
//                 update_data(t, cp_n);
//             }
//         }
//         else
//         {
//             update_data(t, cp);
//         }
//     }
// }


// static void consume_id_seq(css_token_t* t)
// {
//     uint32_t cp = 0;
//     uint32_t cp_n = 0;
//     int32_t cp_len = -1;
//     int32_t cp_n_len = -1;

//     while (true)
//     {
//         consume(&cp, &cp_len);

//         peek(1, &cp_n, &cp_n_len);

//         if (is_id(cp))
//         {
//             update_data(t, cp);
//         }
//         else if (is_valid_escape(cp, cp_n))
//         {
//             consume_escaped_cp(&cp);
//             update_data(t, cp);
//         }
//         else
//         {
//             reconsume(cp_len);
//             return;
//         }
//     }
// }


// static void consume_bad_url()
// {
//     uint32_t cp1 = 0;
//     int32_t cp1_len = -1;

//     uint32_t cp2 = 0;
//     int32_t cp2_len = -1;
    
//     bool is_eof = false;

//     is_eof = consume(&cp1, &cp1_len);

//     peek(1, &cp2, &cp2_len);

//     while (true)
//     {
//         if (is_eof || cp2 == ')')
//         {
//             break;
//         }
//         else if (is_valid_escape(cp1, cp2))
//         {
//             consume_escaped_cp(&cp1);
//         }

//         is_eof = consume(&cp1, &cp1_len);
//         peek(1, &cp2, &cp2_len);
//     }
// }


// static void consume_url_token(css_token_t* t)
// {
//     t->type = CSS_TOKEN_URL;

//     memset(t->data, 0, CSS_TOKEN_MAX_DATA_SIZE);
//     t->data_size = 0;

//     uint32_t cp1 = 0;
//     int32_t cp1_len = -1;
//     uint32_t cp2 = 0;
//     int32_t cp2_len = -1;
//     bool is_eof = false;

//     is_eof = consume(&cp1, &cp1_len);

//     while (is_whitespace(cp1))
//     {
//         is_eof = consume(&cp1, &cp1_len);
//     }

//     peek(1, &cp2, &cp2_len);

//     while (true)
//     {
//         if (is_eof || cp1 == ')')
//         {
//             // t->type = CSS_TOKEN_URL;
//             return;
//         }
//         else if (is_whitespace(cp1))
//         {
//             while(true)
//             {
//                 if (is_whitespace(cp1))
//                 {
//                     // ignore
//                 }
//                 else if (is_eof || cp1 == ')')
//                 {
//                     t->type = CSS_TOKEN_URL;
//                     return;
//                 }
//                 else
//                 {
//                     consume_bad_url();
//                     memset(t->data, 0, CSS_TOKEN_MAX_DATA_SIZE);
//                     t->data_size = 0;
//                     t->type = CSS_TOKEN_BAD_URL;
//                     return;
//                 }

//                 is_eof = consume(&cp1, &cp1_len);
//             }
//         }
//         else if (cp1 == '"'  ||
//                  cp1 == '\'' ||
//                  cp1 == '('  ||
//                  is_nonprintable(cp1))
//         {
//             consume_bad_url();
//             memset(t->data, 0, CSS_TOKEN_MAX_DATA_SIZE);
//             t->data_size = 0;
//             t->type = CSS_TOKEN_BAD_URL;
//         }
//         else if (cp1 == '\\')
//         {
//             if (is_valid_escape(cp1, cp2))
//             {
//                 consume_escaped_cp(&cp1);
//                 update_data(t, cp1);
//             }
//             else
//             {
//                 consume_bad_url();
//                 memset(t->data, 0, CSS_TOKEN_MAX_DATA_SIZE);
//                 t->data_size = 0;
//                 t->type = CSS_TOKEN_BAD_URL;
//             }
//         }
//         else
//         {
//             update_data(t, cp1);
//         }

//         is_eof = consume(&cp1, &cp1_len);
//     }
// }


// static void consume_id_token(css_token_t* t)
// {
//     uint32_t cp1 = 0;
//     int32_t cp1_len = -1;

//     uint32_t cp2 = 0;
//     int32_t cp2_len = -1;

//     consume_id_seq(t);
//     peek(1, &cp1, &cp1_len);

//     if (compare_seq(t, "url", 3) && cp1 == '(')
//     {
//         consume(&cp1, &cp1_len);

//         peek(1, &cp1, &cp1_len);
//         peek(2, &cp2, &cp2_len);

//         while (is_whitespace(cp1) && is_whitespace(cp2))
//         {
//             consume(&cp1, &cp1_len);
//             peek(1, &cp1, &cp1_len);
//             peek(2, &cp2, &cp2_len);
//         }

//         if ((cp1 == '"' || cp1 == '\'') || (is_whitespace(cp1) && (cp2 == '"' || cp2 == '\'')))
//         {
//             t->type = CSS_TOKEN_FUNCTION;
//             return;
//         }

//         consume_url_token(t);
//     }
//     else if (cp1 == '(')
//     {
//         consume(&cp1, &cp1_len);
//         t->type = CSS_TOKEN_FUNCTION;
//     }
//     else
//     {
//         t->type = CSS_TOKEN_IDENT;
//     }
// }


// static void consume_number(css_token_t* token)
// {
//     uint32_t repr[MAX_REPR_SIZE] = { 0 };
//     uint32_t repr_idx = 0;

//     uint32_t cp = 0;
//     uint32_t cp2 = 0;
//     uint32_t cp3 = 0;
//     int32_t cp_len = -1;
//     int32_t cp2_len = -1;
//     int32_t cp3_len = -1;

//     peek(1, &cp, &cp_len);

//     if (cp == '+' || cp == '-')
//     {
//         consume(&cp, &cp_len);
//         repr[repr_idx] = cp;
//         repr_idx++;
//     }

//     peek(1, &cp, &cp_len);

//     while (utf8_is_digit(cp))
//     {
//         consume(&cp, &cp_len);
//         repr[repr_idx] = cp;
//         repr_idx++;
//         peek(1, &cp, &cp_len);

//         if (repr_idx >= MAX_REPR_SIZE) { break; }
//     }

//     peek(1, &cp, &cp_len);
//     peek(2, &cp2, &cp2_len);

//     if (cp == '.' && utf8_is_digit(cp2))
//     {
//         consume(&cp, &cp_len);
//         repr[repr_idx] = cp;
//         repr_idx++;

//         consume(&cp, &cp_len);
//         repr[repr_idx] = cp;
//         repr_idx++;

//         // type = CSS_NUMBER_FLOAT;

//         peek(1, &cp, &cp_len);

//         while (utf8_is_digit(cp))
//         {
//             consume(&cp, &cp_len);
//             repr[repr_idx] = cp;
//             repr_idx++;
//             peek(1, &cp, &cp_len);
    
//             if (repr_idx >= MAX_REPR_SIZE) { break; }
//         }
//     }
    
//     peek(3, &cp3, &cp3_len);

//     bool esd = (cp == 'e' || cp == 'E') && (cp2 == '+' || cp2 == '-') && utf8_is_digit(cp3);
//     bool ed = (cp == 'e' || cp == 'E') && utf8_is_digit(cp2);

//     if (esd || ed)
//     {
//         // type = CSS_NUMBER_FLOAT;

//         consume(&cp, &cp_len);
//         repr[repr_idx] = cp;
//         repr_idx++;

//         consume(&cp, &cp_len);
//         repr[repr_idx] = cp;
//         repr_idx++;

//         if (esd)
//         {
//             consume(&cp, &cp_len);
//             repr[repr_idx] = cp;
//             repr_idx++;
//         }

//         peek(1, &cp, &cp_len);

//         while (utf8_is_digit(cp))
//         {
//             consume(&cp, &cp_len);
//             repr[repr_idx] = cp;
//             repr_idx++;
//             peek(1, &cp, &cp_len);
    
//             if (repr_idx >= MAX_REPR_SIZE) { break; }
//         }
//     }

//     double res = css_convert_buf_to_num(repr, MAX_REPR_SIZE);
//     token->real = (float)res;
//     token->integer = (int32_t)res;
// }


// static void consume_numeric_token(css_token_t* t)
// {
//     uint32_t cp1 = 0;
//     int32_t cp1_len = -1;

//     uint32_t cp2 = 0;
//     int32_t cp2_len = -1;

//     uint32_t cp3 = 0;
//     int32_t cp3_len = -1;

//     consume_number(t);

//     peek(1, &cp1, &cp1_len);
//     peek(2, &cp2, &cp2_len);
//     peek(3, &cp3, &cp3_len);

//     if (is_id_seq_start(cp1, cp2, cp3))
//     {
//         t->type = CSS_TOKEN_DIMENSION;
//         consume_id_seq(t);
//     }
//     else if (cp1 == '%')
//     {
//         consume(&cp1, &cp2_len);
//         t->type = CSS_TOKEN_PERCENTAGE;
//     }
//     else
//     {
//         t->type = CSS_TOKEN_NUMBER;
//     }
// }


// static bool consume_comment()
// {
//     uint32_t cp1 = 0;
//     int32_t cp1_len = -1;

//     uint32_t cp2 = 0;
//     int32_t cp2_len = -1;
//     bool is_eof = false;

//     peek(1, &cp1, &cp1_len);
//     peek(2, &cp2, &cp2_len);

//     if (cp1 == '/' && cp2 == '*')
//     {
//         is_eof = consume(&cp1, &cp1_len);
//         is_eof = consume(&cp2, &cp2_len);

//         while (true)
//         {
//             peek(1, &cp1, &cp1_len);
//             peek(2, &cp2, &cp2_len);

//             if (is_eof || (cp1 == '*' && cp2 == '/'))
//             {
//                 is_eof = consume(&cp1, &cp1_len);
//                 is_eof = consume(&cp2, &cp2_len);
//                 return true;
//             }

//             is_eof = consume(&cp1, &cp1_len);
//         }
//     }

//     return false;
// }

/********************/
/* public functions */
/********************/


void css_tokenizer_init(unsigned char* buffer, uint32_t buffer_size)
{
    buf                 = buffer;
    buf_cur             = 0;
    buf_size            = buffer_size;
}


css_token_t css_tokenizer_next()
{
    css_token_t t                   = { .type = CSS_TOKEN_EOF };
    uint32_t end_cp                 = 0;
    uint32_t cp                     = 0;
    int32_t cp_len                  = -1;
    uint32_t escaped_cp             = 0;
    uint32_t escaped_cp_digits      = 0;
    bool emit                       = false;
    css_tokenizer_state_e state     = CSS_TOKENIZER_STATE_DATA;
    css_tokenizer_state_e r_state   = CSS_TOKENIZER_STATE_DATA;

    t_buf_clear();

    while (!emit)
    {
        bool consume = true;
        bool rewind = false;
        uint32_t rewind_start = 0;
        bool is_eof = false;

        if (buf_cur >= buf_size)
        {
            cp      = 0;
            is_eof  = true;
        }
        else
        {
            cp_len = utf8_decode(buf, buf_size, buf_cur, &cp);
            if (cp_len < 0)
            {
                cp = 0;
                is_eof = true;
            }
        }

        switch (state)
        {
        case CSS_TOKENIZER_STATE_DATA:
            if (is_whitespace(cp))
            {
                state = CSS_TOKENIZER_STATE_WHITESPACE;
                t.type = CSS_TOKEN_WHITESPACE;
            }
            else if (cp == '/')
            {
                t_buf[t_buf_size] = cp;
                t_buf_size++;
                state = CSS_TOKENIZER_STATE_COMMENT_START;
            }
            else if (cp == '#')
            {
            
            }
            else if (cp == '\'' || cp == '"')
            {
                t.type = CSS_TOKEN_STRING;
                state = CSS_TOKENIZER_STATE_STRING;
                end_cp = cp;
            }
            else if (cp == '(')
            {
                t.type = CSS_TOKEN_OPEN_PARENTHESIS;
                emit = true;
            }
            else if (cp == ')')
            {
                t.type = CSS_TOKEN_CLOSED_PARENTHESIS;
                emit = true;
            }
            else if (cp == '+')
            {
            
            }
            else if (cp == ',')
            {
                t.type = CSS_TOKEN_COMMA;
                emit = true;
            }
            else if (cp == '-')
            {
                state = CSS_TOKENIZER_STATE_MINUS;
                consume = false;
            }
            else if (cp == '.')
            {
            
            }
            else if (cp == ':')
            {
                t.type = CSS_TOKEN_COLON;
                emit = true;
            }
            else if (cp == ';')
            {
            
            }
            else if (cp == '<')
            {
            
            }
            else if (cp == '@')
            {
                state = CSS_TOKENIZER_STATE_AT;
            }
            else if (cp == '[')
            {
            
            }
            else if (cp == '\\')
            {
                state = CSS_TOKENIZER_STATE_ESCAPE_START;

                t_buf[t_buf_size] = cp;
                t_buf_size++;
            }
            else if (cp == ']')
            {
            
            }
            else if (cp == '{')
            {
            
            }
            else if (cp == '}')
            {
            
            }
            else if (utf8_is_digit(cp))
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_NUMERIC_TOKEN_START;
            }
            else if (is_id_start(cp))
            {
                consume = false;
                state   = CSS_TOKENIZER_STATE_ID_SEQ;
                r_state = CSS_TOKENIZER_STATE_ID_TOKEN;
            }
            else
            {
                t.type = CSS_TOKEN_DELIM;
                update_data(&t, cp);
                emit = true;
            }
            break;

        case CSS_TOKENIZER_STATE_WHITESPACE:
            if (!is_whitespace(cp))
            {
                state   = CSS_TOKENIZER_STATE_DATA;
                consume = false;
                emit    = true;
            }
            break;

        case CSS_TOKENIZER_STATE_ID_SEQ:
            if (is_id(cp))
            {
                update_data(&t, cp);
            }
            else if (cp == '\\')
            {
                state = CSS_TOKENIZER_STATE_ID_SEQ_ESCAPE_START;
            }
            else
            {
                consume = false;
                state = r_state;
            }
            break;

        case CSS_TOKENIZER_STATE_ID_SEQ_ESCAPE_START:
            if (cp == '\n')
            {
                consume     = false;
                state       = CSS_TOKENIZER_STATE_ID_SEQ;
            }
            else
            {
                escaped_cp          = 0;
                escaped_cp_digits   = 0;
                state               = CSS_TOKENIZER_STATE_ID_SEQ_ESCAPE;
                consume             = false;
            }
            break;

        case CSS_TOKENIZER_STATE_ID_SEQ_ESCAPE:
            if (is_eof)
            {
                escaped_cp = replacement;
                state = CSS_TOKENIZER_STATE_ID_SEQ_ESCAPE_END;
            }
            else if (escaped_cp_digits > 0 && is_whitespace(cp))
            {
                state = CSS_TOKENIZER_STATE_ID_SEQ_ESCAPE_END;
            }
            else if (escaped_cp_digits >= 6)
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_ID_SEQ_ESCAPE_END;
            }
            else if (utf8_is_digit(cp))
            {
                escaped_cp  *= 16;
                escaped_cp  += cp - 0x30;
                escaped_cp_digits  += 1;
            }
            else if (utf8_is_upper_hex(cp))
            {
                escaped_cp  *= 16;
                escaped_cp  += cp - 0x37;
                escaped_cp_digits  += 1;
            }
            else if (utf8_is_lower_hex(cp))
            {
                escaped_cp  *= 16;
                escaped_cp  += cp - 0x57;
                escaped_cp_digits  += 1;
            }
            else
            {
                escaped_cp = cp;
                state = CSS_TOKENIZER_STATE_ID_SEQ_ESCAPE_END;
            }
            break;

        case CSS_TOKENIZER_STATE_ID_SEQ_ESCAPE_END:
            if (escaped_cp == 0 || utf8_is_surrogate(escaped_cp) || escaped_cp > 0x10ffff)
            {
                update_data(&t, replacement);
            }
            else
            {
                update_data(&t, escaped_cp);
            }
            state = CSS_TOKENIZER_STATE_ID_SEQ;
            consume = false;
            break;

        case CSS_TOKENIZER_STATE_ID_TOKEN:
            if (compare_seq(&t, "url", 3) && cp == '(')
            {
                state = CSS_TOKENIZER_STATE_URL_START;
            }
            else if (cp == '(')
            {
                emit = true;
                t.type = CSS_TOKEN_FUNCTION;
            }
            else
            {
                emit = true;
                consume = false;
                t.type = CSS_TOKEN_IDENT;
            }
            break;

        case CSS_TOKENIZER_STATE_URL_START:
            if (cp == '\'' || cp == '"')
            {
                t.type = CSS_TOKEN_FUNCTION;
                emit = true;
                consume = false;
                rewind = true;
            }
            else if (is_whitespace(cp))
            {
                t_buf[t_buf_size] = cp;
                t_buf_size++;
            }
            else
            {
                memset(t.data, 0, CSS_TOKEN_MAX_DATA_SIZE);
                t.data_size = 0;
                state = CSS_TOKENIZER_STATE_URL;
                t.type = CSS_TOKEN_URL;
                consume = false;
            }
            break;

        case CSS_TOKENIZER_STATE_URL:
            if (is_whitespace(cp))
            {
                state = CSS_TOKENIZER_STATE_URL_END;
            }
            else if (is_eof || cp == ')')
            {
                t.type = CSS_TOKEN_URL;
                emit = true;
            }
            else if (cp == '"' || cp == '\'' || cp == '(' || is_nonprintable(cp))
            {
                state = CSS_TOKENIZER_STATE_BAD_URL;
            }
            else if (cp == '\\')
            {
                state = CSS_TOKENIZER_STATE_URL_ESCAPE_START;
            }
            else
            {
                update_data(&t, cp);
            }
            break;

        case CSS_TOKENIZER_STATE_URL_END:
            if (is_whitespace(cp))
            {
            
            }
            else if (is_eof || cp == ')')
            {
                t.type = CSS_TOKEN_URL;
                emit = true;
            }
            else
            {
                state = CSS_TOKENIZER_STATE_BAD_URL;
            }
            break;

        case CSS_TOKENIZER_STATE_URL_ESCAPE_START:
            if (cp == '\n')
            {
                t.type  = CSS_TOKEN_BAD_URL;
                emit    = true;
                consume = false;
            }
            else
            {
                consume = false;
                rewind = true;
                state   = CSS_TOKENIZER_STATE_URL_ESCAPE;
                r_state = CSS_TOKENIZER_STATE_URL;
            }
            break;

        case CSS_TOKENIZER_STATE_URL_ESCAPE:
            if (is_eof)
            {
                escaped_cp = replacement;
            }
            else if (escaped_cp_digits > 0 && is_whitespace(cp))
            {
                state = CSS_TOKENIZER_STATE_URL_ESCAPE_END;
            }
            else if (escaped_cp_digits >= 6)
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_URL_ESCAPE_END;
            }
            else if (utf8_is_digit(cp))
            {
                escaped_cp  *= 16;
                escaped_cp  += cp - 0x30;
                escaped_cp_digits  += 1;
            }
            else if (utf8_is_upper_hex(cp))
            {
                escaped_cp  *= 16;
                escaped_cp  += cp - 0x37;
                escaped_cp_digits  += 1;
            }
            else if (utf8_is_lower_hex(cp))
            {
                escaped_cp  *= 16;
                escaped_cp  += cp - 0x57;
                escaped_cp_digits  += 1;
            }
            else
            {
                escaped_cp = cp;
                state = CSS_TOKENIZER_STATE_URL_ESCAPE_END;
            }
            break;

        case CSS_TOKENIZER_STATE_URL_ESCAPE_END:
            if (escaped_cp == 0 || utf8_is_surrogate(escaped_cp) || escaped_cp > 0x10ffff)
            {
                update_data(&t, replacement);
            }
            else
            {
                update_data(&t, escaped_cp);
            }
            state = CSS_TOKENIZER_STATE_URL;
            consume = false;
            break;

        case CSS_TOKENIZER_STATE_BAD_URL:
            if (is_eof || cp == ')')
            {
                t.type = CSS_TOKEN_BAD_URL;
                t.data_size = 0;
                memset(t.data, 0, CSS_TOKEN_MAX_DATA_SIZE);
                emit = true;
            }
            else if (cp == '\\')
            {
                state = CSS_TOKENIZER_STATE_BAD_URL_ESCAPE_START;
            }
            break;

        case CSS_TOKENIZER_STATE_BAD_URL_ESCAPE_START:
            if (cp == '\n')
            {
                update_data(&t, t_buf[0]);
                t.type  = CSS_TOKEN_DELIM;
                emit    = true;
                consume = false;
            }
            else
            {
                consume = false;
                rewind = true;
                state   = CSS_TOKENIZER_STATE_BAD_URL_ESCAPE;
                r_state = CSS_TOKENIZER_STATE_BAD_URL;
            }
            break;

        case CSS_TOKENIZER_STATE_BAD_URL_ESCAPE:
            if (is_eof)
            {
                escaped_cp = replacement;
            }
            else if (escaped_cp_digits > 0 && is_whitespace(cp))
            {
                state = CSS_TOKENIZER_STATE_BAD_URL_ESCAPE_END;
            }
            else if (escaped_cp_digits >= 6)
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_BAD_URL_ESCAPE_END;
            }
            else if (utf8_is_digit(cp))
            {
                escaped_cp  *= 16;
                escaped_cp  += cp - 0x30;
                escaped_cp_digits  += 1;
            }
            else if (utf8_is_upper_hex(cp))
            {
                escaped_cp  *= 16;
                escaped_cp  += cp - 0x37;
                escaped_cp_digits  += 1;
            }
            else if (utf8_is_lower_hex(cp))
            {
                escaped_cp  *= 16;
                escaped_cp  += cp - 0x57;
                escaped_cp_digits  += 1;
            }
            else
            {
                escaped_cp = cp;
                state = CSS_TOKENIZER_STATE_BAD_URL_ESCAPE_END;
            }
            break;

        case CSS_TOKENIZER_STATE_BAD_URL_ESCAPE_END:
            if (escaped_cp == 0 || utf8_is_surrogate(escaped_cp) || escaped_cp > 0x10ffff)
            {
                update_data(&t, replacement);
            }
            else
            {
                update_data(&t, escaped_cp);
            }
            state = CSS_TOKENIZER_STATE_BAD_URL;
            consume = false;
            break;

        case CSS_TOKENIZER_STATE_ESCAPE_START:
            if (cp == '\n')
            {
                update_data(&t, t_buf[0]);
                t.type  = CSS_TOKEN_DELIM;
                emit    = true;
                consume = false;
            }
            else
            {
                consume = false;
                rewind = true;
                state   = CSS_TOKENIZER_STATE_ID_SEQ;
                r_state = CSS_TOKENIZER_STATE_ID_TOKEN;
                t.type = CSS_TOKEN_IDENT;
            }
            break;

        case CSS_TOKENIZER_STATE_MINUS:
            t_buf[t_buf_size] = cp;
            t_buf_size++;

            if (t_buf_size < 3)
            {
                // consume
            }
            else if (is_number_start(t_buf[0], t_buf[1], t_buf[2]))
            {
                rewind = true;
                state = CSS_TOKENIZER_STATE_NUMERIC_TOKEN_START;
            }
            else if (t_buf[0] == '-' && t_buf[1] == '-' && t_buf[2] == '>')
            {
                t.type = CSS_TOKEN_CDC;
                emit = true;
            }
            else if (is_id_seq_start(t_buf[0], t_buf[1], t_buf[2]))
            {
                rewind = true;
                state   = CSS_TOKENIZER_STATE_ID_SEQ;
                r_state = CSS_TOKENIZER_STATE_ID_TOKEN;
                t.type = CSS_TOKEN_IDENT;
            }
            else
            {
                update_data(&t, t_buf[0]);
                emit = true;
                rewind = true;
                rewind_start = 1;
                t.type = CSS_TOKEN_DELIM;
            }
            break;

        case CSS_TOKENIZER_STATE_NUMBER:
            if (t_buf_size > 20)
            {
                
            }
            else if (t_buf_size == 0 && (cp == '+' || cp == '-'))
            {
                t_buf[t_buf_size] = cp;
                t_buf_size++;
            }
            else if (utf8_is_digit(cp))
            {
                t_buf[t_buf_size] = cp;
                t_buf_size++;
            }
            else if (cp == '.' && !t_buf_contains(cp))
            {
                t_buf[t_buf_size] = cp;
                t_buf_size++;
            }
            else if ((cp == 'e' || cp == 'E'))
            {
                t_buf[t_buf_size] = cp;
                t_buf_size++;
            }
            else
            {
                state = CSS_TOKENIZER_STATE_NUMBER_END;
                consume = false;
            }
            break;

        case CSS_TOKENIZER_STATE_NUMBER_END:
            t.real = (float)css_convert_buf_to_num(t_buf, t_buf_size);
            t.integer = (int32_t)t.real;
            state = r_state;
            consume = false;
            t_buf_clear();
            break;

        case CSS_TOKENIZER_STATE_STRING:
            if (is_eof || cp == end_cp)
            {
                emit = true;
            }
            else if (cp == '\n')
            {
                consume = false;
                memset(t.data, 0, CSS_TOKEN_MAX_DATA_SIZE);
                t.data_size = 0;
                t.type = CSS_TOKEN_BAD_STRING;
                emit = true;
            }
            else if (cp == '\\')
            {
                state = CSS_TOKENIZER_STATE_STRING_ESCAPE_START;
            }
            else
            {
                update_data(&t, cp);
            }
            break;

        case CSS_TOKENIZER_STATE_STRING_ESCAPE_START:
            if (is_eof || cp == '\n')
            {
                state = CSS_TOKENIZER_STATE_STRING;
            }
            else
            {
                state = CSS_TOKENIZER_STATE_STRING_ESCAPE;
                escaped_cp          = 0;
                escaped_cp_digits   = 0;
                state               = CSS_TOKENIZER_STATE_STRING_ESCAPE;
                consume             = false;
            }
            break;

        case CSS_TOKENIZER_STATE_STRING_ESCAPE:
            if (is_eof)
            {
                escaped_cp = replacement;
            }
            else if (escaped_cp_digits > 0 && is_whitespace(cp))
            {
                state = CSS_TOKENIZER_STATE_STRING_ESCAPE_END;
            }
            else if (escaped_cp_digits >= 6)
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_STRING_ESCAPE_END;
            }
            else if (utf8_is_digit(cp))
            {
                escaped_cp  *= 16;
                escaped_cp  += cp - 0x30;
                escaped_cp_digits  += 1;
            }
            else if (utf8_is_upper_hex(cp))
            {
                escaped_cp  *= 16;
                escaped_cp  += cp - 0x37;
                escaped_cp_digits  += 1;
            }
            else if (utf8_is_lower_hex(cp))
            {
                escaped_cp  *= 16;
                escaped_cp  += cp - 0x57;
                escaped_cp_digits  += 1;
            }
            else
            {
                escaped_cp = cp;
                state = CSS_TOKENIZER_STATE_STRING_ESCAPE_END;
            }
            break;

        case CSS_TOKENIZER_STATE_STRING_ESCAPE_END:
            if (escaped_cp == 0 || utf8_is_surrogate(escaped_cp) || escaped_cp > 0x10ffff)
            {
                update_data(&t, replacement);
            }
            else
            {
                update_data(&t, escaped_cp);
            }
            state = CSS_TOKENIZER_STATE_STRING;
            consume = false;
            break;

        case CSS_TOKENIZER_STATE_AT:
            t_buf[t_buf_size] = cp;
            t_buf_size++;

            if (t_buf_size < 3)
            {
            
            }
            else if (is_id_seq_start(t_buf[0], t_buf[1], t_buf[2]))
            {
                rewind = true;
                t.type = CSS_TOKEN_AT_KEYWORD;
                state = CSS_TOKENIZER_STATE_ID_SEQ;
                r_state = CSS_TOKENIZER_STATE_AT_COMPLETE;
            }
            else
            {
                t.type = CSS_TOKEN_DELIM;
                update_data(&t, '@');
                emit = true;
                rewind = true;
            }
            break;

        case CSS_TOKENIZER_STATE_AT_COMPLETE:
            emit = true;
            memset(t_buf, 0, sizeof(t_buf));
            break;

        case CSS_TOKENIZER_STATE_COMMENT_START:
            t_buf[t_buf_size] = cp;
            t_buf_size++;

            if (t_buf_size < 2)
            {
            
            }
            else if (t_buf[0] == '/' && t_buf[1] == '*')
            {
                state = CSS_TOKENIZER_STATE_COMMENT;
                memset(t_buf, 0, sizeof(t_buf));
                t_buf_size = 0;
            }
            else
            {
                t.type = CSS_TOKEN_DELIM;
                update_data(&t, '/');
                emit = true;
                rewind = true;
                rewind_start = 1;
            }
            break;

        case CSS_TOKENIZER_STATE_COMMENT:
            if (cp == '*')
            {
                t_buf[t_buf_size] = cp;
                t_buf_size++;
            }
            else if (is_eof || (t_buf[0] == '*' && cp == '/'))
            {
                t.type = CSS_TOKEN_COMMENT;
                emit = true;
            }
            break;

        case CSS_TOKENIZER_STATE_NUMERIC_TOKEN_START:
            consume = false;
            state = CSS_TOKENIZER_STATE_NUMBER;
            r_state = CSS_TOKENIZER_STATE_NUMERIC_TOKEN;
            break;

        case CSS_TOKENIZER_STATE_NUMERIC_TOKEN:
            t_buf[t_buf_size] = cp;
            t_buf_size++;

            if (t_buf_size < 3)
            {
            
            }
            else if (is_id_seq_start(t_buf[0], t_buf[1], t_buf[2]))
            {
                t.type = CSS_TOKEN_DIMENSION;
                rewind = true;
                state = CSS_TOKENIZER_STATE_ID_SEQ;
                r_state = CSS_TOKENIZER_STATE_NUMERIC_TOKEN_END;
            }
            else if (t_buf[0] == '%')
            {
                t.type = CSS_TOKEN_PERCENTAGE;
                rewind = true;
                rewind_start = 1;
                emit = true;
            }
            else
            {
                t.type = CSS_TOKEN_NUMBER;
                rewind = true;
                rewind_start = 1;
                emit = true;
            }
            break;

        case CSS_TOKENIZER_STATE_NUMERIC_TOKEN_END:
            emit = true;
            break;

        }

        if (consume)
        {
            buf_cur += (uint32_t)cp_len;
        }

        if (rewind)
        {
            for (uint32_t i = rewind_start; i < t_buf_size; i++)
            {
                unsigned char b[4];
                int32_t len = utf8_encode(t_buf[i], b);
                buf_cur -= (uint32_t)len;

                t_buf[i] = 0;
            }

            memset(t_buf, 0, sizeof(t_buf));
            t_buf_size = 0;
        }
    }
    

    // bool is_comment = consume_comment();

    // if (is_comment)
    // {
    //     t.type = CSS_TOKEN_COMMENT;
    //     return t;
    // }

    // is_eof = consume(&cp, &cp_len);

    // if (is_eof)
    // {
    //     t.type  = CSS_TOKEN_EOF;
    // }
    // else if (is_whitespace(cp))
    // {
    //     peek(1, &cp, &cp_len);

    //     while (is_whitespace(cp))
    //     {
    //         is_eof = consume(&cp, &cp_len);
    //         peek(1, &cp, &cp_len);
    //     }
    //     t.type  = CSS_TOKEN_WHITESPACE;
    // }
    // else if (cp == '\'' || cp == '"')
    // {
    //     t.type  = CSS_TOKEN_STRING;
    //     consume_string(&t, cp);        
    // }
    // else if (cp == '#')
    // {
    //     uint32_t cp_n = 0;
    //     int32_t cp_n_len = -1;

    //     uint32_t cp_nn = 0;
    //     int32_t cp_nn_len = -1;

    //     peek(1, &cp_n, &cp_n_len);
    //     peek(2, &cp_nn, &cp_nn_len);

    //     if (is_id(cp_n) || is_valid_escape(cp_n, cp_nn))
    //     {
    //         t.type = CSS_TOKEN_HASH;
    //         // todo: check 3 cps
    //         consume_id_seq(&t);
    //     }
    //     else
    //     {
    //         t.type = CSS_TOKEN_DELIM;
    //     }
    // }
    // else if (cp == '(')
    // {
    //     t.type  = CSS_TOKEN_OPEN_PARENTHESIS;
    // }
    // else if (cp == ')')
    // {
    //     t.type  = CSS_TOKEN_CLOSED_PARENTHESIS;
    // }
    // else if (cp == '+')
    // {
    //     uint32_t cp2 = 0;
    //     int32_t cp2_len = -1;

    //     uint32_t cp3 = 0;
    //     int32_t cp3_len = -1;

    //     peek(1, &cp2, &cp2_len);
    //     peek(2, &cp3, &cp3_len);

    //     if (is_number_start(cp, cp2, cp3))
    //     {
    //         reconsume(cp_len);
    //         consume_numeric_token(&t);
    //     }
    //     else
    //     {
    //         update_data(&t, cp);
    //         t.type = CSS_TOKEN_DELIM;
    //     }
    // }
    // else if (cp == ',')
    // {
    //     t.type  = CSS_TOKEN_COMMA;
    // }
    // else if (cp == '-')
    // {
    //     uint32_t cp2 = 0;
    //     int32_t cp2_len = -1;

    //     uint32_t cp3 = 0;
    //     int32_t cp3_len = -1;

    //     peek(1, &cp2, &cp2_len);
    //     peek(2, &cp3, &cp3_len);

    //     if (is_number_start(cp, cp2, cp3))
    //     {
    //         reconsume(cp_len);
    //         consume_numeric_token(&t);
    //     }
    //     else if (cp2 == '-' && cp3 == '>')
    //     {
    //         consume(&cp2, &cp2_len);
    //         consume(&cp3, &cp3_len);
    //         t.type = CSS_TOKEN_CDC;
    //     }
    //     else if (is_id_seq_start(cp, cp2, cp3))
    //     {
    //         reconsume(cp_len);
    //         consume_id_token(&t);
    //     }
    //     else
    //     {
    //         update_data(&t, cp);
    //         t.type = CSS_TOKEN_DELIM;
    //     }
    // }
    // else if (cp == '.')
    // {
    //     uint32_t cp2 = 0;
    //     int32_t cp2_len = -1;

    //     uint32_t cp3 = 0;
    //     int32_t cp3_len = -1;

    //     peek(1, &cp2, &cp2_len);
    //     peek(2, &cp3, &cp3_len);

    //     if (is_number_start(cp, cp2, cp3))
    //     {
    //         reconsume(cp_len);
    //         consume_numeric_token(&t);
    //     }
    //     else
    //     {
    //         update_data(&t, cp);
    //         t.type = CSS_TOKEN_DELIM;
    //     }
    // }
    // else if (cp == ':')
    // {
    //     t.type  = CSS_TOKEN_COLON;
    // }
    // else if (cp == ';')
    // {
    //     t.type  = CSS_TOKEN_SEMICOLON;
    // }
    // else if (cp == '<')
    // {
    //     uint32_t cp2 = 0;
    //     int32_t cp2_len = -1;

    //     uint32_t cp3 = 0;
    //     int32_t cp3_len = -1;

    //     uint32_t cp4 = 0;
    //     int32_t cp4_len = -1;

    //     peek(1, &cp2, &cp2_len);
    //     peek(2, &cp3, &cp3_len);
    //     peek(3, &cp4, &cp4_len);

    //     if (cp2 == '!' && cp3 == '-' && cp4 == '-')
    //     {
    //         consume(&cp2, &cp2_len);
    //         consume(&cp3, &cp3_len);
    //         consume(&cp4, &cp4_len);
    //         t.type = CSS_TOKEN_CDO;
    //     }
    //     else
    //     {
    //         update_data(&t, cp);
    //         t.type = CSS_TOKEN_DELIM;
    //     }
    // }
    // else if (cp == '@')
    // {
    //     uint32_t cp2 = 0;
    //     int32_t cp2_len = -1;

    //     uint32_t cp3 = 0;
    //     int32_t cp3_len = -1;

    //     uint32_t cp4 = 0;
    //     int32_t cp4_len = -1;

    //     peek(1, &cp2, &cp2_len);
    //     peek(2, &cp3, &cp3_len);
    //     peek(3, &cp4, &cp4_len);

    //     if (is_id_seq_start(cp2, cp3, cp4))
    //     {
    //         consume_id_seq(&t);
    //         t.type = CSS_TOKEN_AT_KEYWORD;
    //     }
    //     else
    //     {
    //         update_data(&t, cp);
    //         t.type = CSS_TOKEN_DELIM;
    //     }
    // }
    // else if (cp == '[')
    // {
    //     t.type  = CSS_TOKEN_OPEN_BRACKET;
    // }
    // else if (cp == '\\')
    // {
    //     uint32_t cp2 = 0;
    //     int32_t cp2_len = -1;

    //     uint32_t cp3 = 0;
    //     int32_t cp3_len = -1;

    //     uint32_t cp4 = 0;
    //     int32_t cp4_len = -1;

    //     peek(1, &cp2, &cp2_len);
    //     peek(2, &cp3, &cp3_len);
    //     peek(3, &cp4, &cp4_len);

    //     if (is_valid_escape(cp, cp2))
    //     {
    //         reconsume(cp_len);
    //         consume_id_token(&t);
    //     }
    //     else
    //     {
    //         update_data(&t, cp);
    //         t.type = CSS_TOKEN_DELIM;
    //     }
    // }
    // else if (cp == ']')
    // {
    //     t.type  = CSS_TOKEN_CLOSED_BRACKET;
    // }
    // else if (cp == '{')
    // {
    //     t.type  = CSS_TOKEN_OPEN_BRACE;
    // }
    // else if (cp == '}')
    // {
    //     t.type  = CSS_TOKEN_CLOSED_BRACE;
    // }
    // else if (utf8_is_digit(cp))
    // {
    //     reconsume(cp_len);
    //     consume_numeric_token(&t);
    // }
    // else if (is_id_start(cp))
    // {
    //     reconsume(cp_len);
    //     consume_id_token(&t);
    // }
    // else
    // {
    //     update_data(&t, cp);
    //     t.type  = CSS_TOKEN_DELIM;
    // }

    return t;
}


void css_tokenizer_free()
{

}
