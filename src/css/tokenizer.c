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
    CSS_TOKENIZER_STATE_NUMBER_SIGN,
    CSS_TOKENIZER_STATE_NUMBER_INTEGER,
    CSS_TOKENIZER_STATE_NUMBER_DOT,
    CSS_TOKENIZER_STATE_NUMBER_FRACTION,
    CSS_TOKENIZER_STATE_NUMBER_E,
    CSS_TOKENIZER_STATE_NUMBER_E_SIGN,
    CSS_TOKENIZER_STATE_NUMBER_E_SIGN_DIGITS,
    CSS_TOKENIZER_STATE_NUMBER_PRE_PROCESS,
    CSS_TOKENIZER_STATE_NUMBER_END,
    CSS_TOKENIZER_STATE_STRING,
    CSS_TOKENIZER_STATE_STRING_ESCAPE_START,
    CSS_TOKENIZER_STATE_STRING_ESCAPE,
    CSS_TOKENIZER_STATE_STRING_ESCAPE_END,
    CSS_TOKENIZER_STATE_AT,
    CSS_TOKENIZER_STATE_AT_COMPLETE,
    CSS_TOKENIZER_STATE_COMMENT,
    CSS_TOKENIZER_STATE_COMMENT_START,
    CSS_TOKENIZER_STATE_DOT,
    CSS_TOKENIZER_STATE_LESS_THAN,
    CSS_TOKENIZER_STATE_PLUS
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


static void t_buf_update(uint32_t cp)
{
    t_buf[t_buf_size] = cp;
    t_buf_size++;
}


static bool t_buf_contains(uint32_t cp)
{
    if (t_buf_size == 0) { return false; }

    return t_buf[t_buf_size - 1] == cp;
}


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
                t_buf_update(cp);
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
                consume = false;
                state = CSS_TOKENIZER_STATE_PLUS;
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
                consume = false;
                state = CSS_TOKENIZER_STATE_DOT;
            }
            else if (cp == ':')
            {
                t.type = CSS_TOKEN_COLON;
                emit = true;
            }
            else if (cp == ';')
            {
                t.type = CSS_TOKEN_SEMICOLON;
                emit = true;
            }
            else if (cp == '<')
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_LESS_THAN;
            }
            else if (cp == '@')
            {
                state = CSS_TOKENIZER_STATE_AT;
            }
            else if (cp == '[')
            {
                t.type = CSS_TOKEN_OPEN_BRACKET;
                emit = true;
            }
            else if (cp == '\\')
            {
                state = CSS_TOKENIZER_STATE_ESCAPE_START;
                t_buf_update(cp);
            }
            else if (cp == ']')
            {
                t.type = CSS_TOKEN_CLOSED_BRACKET;
                emit = true;
            }
            else if (cp == '{')
            {
                t.type = CSS_TOKEN_OPEN_BRACE;
                emit = true;
            }
            else if (cp == '}')
            {
                t.type = CSS_TOKEN_CLOSED_BRACE;
                emit = true;
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
                t_buf_update(cp);
            }
            else
            {
                memset(t.data, 0, CSS_TOKEN_MAX_DATA_SIZE);
                t.data_size = 0;
                state = CSS_TOKENIZER_STATE_URL;
                t.type = CSS_TOKEN_URL;
                consume = false;
                t_buf_clear();
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

        case CSS_TOKENIZER_STATE_PLUS:
            t_buf_update(cp);

            if (t_buf_size < 3)
            {
                // consume
            }
            else if (is_number_start(t_buf[0], t_buf[1], t_buf[2]))
            {
                rewind = true;
                state = CSS_TOKENIZER_STATE_NUMERIC_TOKEN_START;
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

        case CSS_TOKENIZER_STATE_MINUS:
            t_buf_update(cp);

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
            consume = false;
            state = CSS_TOKENIZER_STATE_NUMBER_SIGN;
            break;

        case CSS_TOKENIZER_STATE_NUMBER_SIGN:
            if (cp == '+' || cp == '-')
            {
                t_buf_update(cp);
                state = CSS_TOKENIZER_STATE_NUMBER_INTEGER;
            }
            else
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_NUMBER_INTEGER;
            }
            break;

        case CSS_TOKENIZER_STATE_NUMBER_INTEGER:
            if (utf8_is_digit(cp))
            {
                t_buf_update(cp);
            }
            else if (cp == '.')
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_NUMBER_DOT;
            }
            else if (cp == 'e' || cp == 'E')
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_NUMBER_E;
            }
            else
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_NUMBER_END;
            }
            break;

        case CSS_TOKENIZER_STATE_NUMBER_DOT:
            if (cp == '.')
            {
                t_buf_update(cp);
                state = CSS_TOKENIZER_STATE_NUMBER_FRACTION;
            }
            else
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_NUMBER_FRACTION;
            }
            break;

        case CSS_TOKENIZER_STATE_NUMBER_FRACTION:
            if (utf8_is_digit(cp))
            {
                t_buf_update(cp);
            }
            else if (cp == 'e' || cp == 'E')
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_NUMBER_E;
            }
            else
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_NUMBER_END;
            }
            break;

        case CSS_TOKENIZER_STATE_NUMBER_E:
            if (cp == 'e' || cp == 'E')
            {
                t_buf_update(cp);
                state = CSS_TOKENIZER_STATE_NUMBER_E_SIGN;
            }
            else
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_NUMBER_E_SIGN;
            }
            break;

        case CSS_TOKENIZER_STATE_NUMBER_E_SIGN:
            if (cp == '+' || cp == '-')
            {
                t_buf_update(cp);
                state = CSS_TOKENIZER_STATE_NUMBER_E_SIGN_DIGITS;
            }
            else
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_NUMBER_E_SIGN_DIGITS;
            }
            break;

        case CSS_TOKENIZER_STATE_NUMBER_E_SIGN_DIGITS:
            if (utf8_is_digit(cp))
            {
                t_buf_update(cp);
            }
            else
            {
                consume = false;
                state = CSS_TOKENIZER_STATE_NUMBER_PRE_PROCESS;
            }
            break;

        case CSS_TOKENIZER_STATE_NUMBER_PRE_PROCESS:
            if (t_buf_contains('+') || t_buf_contains('-'))
            {
                rewind = true;
                rewind_start = t_buf_size - 2;
            }
            consume = false;
            state = CSS_TOKENIZER_STATE_NUMBER_END;
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
            t_buf_update(cp);

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
            t_buf_update(cp);

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
                t_buf_update(cp);
            }
            else if (is_eof || (t_buf[0] == '*' && cp == '/'))
            {
                t.type = CSS_TOKEN_COMMENT;
                emit = true;
            }
            break;

        case CSS_TOKENIZER_STATE_NUMERIC_TOKEN_START:
            consume = false;
            state   = CSS_TOKENIZER_STATE_NUMBER;
            r_state = CSS_TOKENIZER_STATE_NUMERIC_TOKEN;
            break;

        case CSS_TOKENIZER_STATE_NUMERIC_TOKEN:
            t_buf_update(cp);

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
                // rewind_start = 1;
                emit = true;
            }
            break;

        case CSS_TOKENIZER_STATE_NUMERIC_TOKEN_END:
            consume = false;
            emit = true;
            break;

        case CSS_TOKENIZER_STATE_DOT:
            t_buf_update(cp);
            if (!is_eof && t_buf_size < 3)
            {
            
            }
            else if (is_number_start(t_buf[0], t_buf[1], t_buf[2]))
            {
                rewind = true;
                state = CSS_TOKENIZER_STATE_NUMERIC_TOKEN_START;
            }
            else
            {
                t.type = CSS_TOKEN_DELIM;
                update_data(&t, t_buf[0]);
                rewind = true;
                rewind_start = 1;
                emit = true;
            }
            break;

        case CSS_TOKENIZER_STATE_LESS_THAN:
            t_buf_update(cp);
            if (!is_eof && t_buf_size < 4)
            {
            
            }
            else if (t_buf[0] == '<' && t_buf[1] == '!' && t_buf[2] == '-' && t_buf[3] == '-')
            {
                t.type = CSS_TOKEN_CDO;
                emit = true;
            }
            else
            {
                t.type = CSS_TOKEN_DELIM;
                update_data(&t, t_buf[0]);
                rewind = true;
                rewind_start = 1;
                emit = true;
            }
            break;
        }

        if (consume)
        {
            buf_cur += (uint32_t)cp_len;
        }

        if (rewind)
        {
            uint32_t cnt = 0;
            for (uint32_t i = rewind_start; i < t_buf_size; i++)
            {
                unsigned char b[4];
                int32_t len = utf8_encode(t_buf[i], b);
                buf_cur -= (uint32_t)len;

                t_buf[i] = 0;
                cnt++;
            }

            t_buf_size -= cnt;
            if (rewind_start == 0)
            {
                memset(t_buf, 0, sizeof(t_buf));
                t_buf_size = 0;
            }
        }
    }

    return t;
}


void css_tokenizer_free()
{

}
