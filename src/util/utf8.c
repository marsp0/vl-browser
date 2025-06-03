#include "utf8.h"

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

typedef enum
{
    STATE_START,
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
} state_e;

/********************/
/* static functions */
/********************/


/********************/
/* public functions */
/********************/

// http://unicode.org/mail-arch/unicode-ml/y2003-m02/att-0467/01-The_Algorithm_to_Valide_an_UTF-8_String
bool utf8_validate(const unsigned char* buffer, const uint32_t size)
{
    unsigned char c;
    bool is_valid   = true;
    state_e state   = STATE_START;

    for (uint32_t i = 0; i < size; i++)
    {
        c = buffer[i];

        if(state == STATE_START)
        {
            if      (c <= 0x7f)                 { continue;                 }
            else if (c >= 0xC2 && c <= 0xDF)    { state = STATE_A;          }
            else if (c >= 0xE1 && c <= 0xEC)    { state = STATE_B;          }
            else if (c >= 0xEE && c <= 0xEF)    { state = STATE_B;          }
            else if (c == 0xE0)                 { state = STATE_C;          }
            else if (c == 0xED)                 { state = STATE_D;          }
            else if (c >= 0xF1 && c <= 0xF3)    { state = STATE_E;          }
            else if (c == 0xF0)                 { state = STATE_F;          }
            else if (c == 0xF4)                 { state = STATE_G;          }
            else                                { is_valid = false; break;  }
        }
        else if (state == STATE_A)
        {
            if (c >= 0x80 && c <= 0xBF )        { state = STATE_START;      }
            else                                { is_valid = false; break;  }
        }
        else if (state == STATE_B)
        {
            if (c >= 0x80 && c <= 0xBF )        { state = STATE_A;          }
            else                                { is_valid = false; break;  }
        }
        else if (state == STATE_C)
        {
            if (c >= 0xA0 && c <= 0xBF )        { state = STATE_A;          }
            else                                { is_valid = false; break;  }
        }
        else if (state == STATE_D)
        {
            if (c >= 0x80 && c <= 0x9F )        { state = STATE_A;          }
            else                                { is_valid = false; break;  }
        }
        else if (state == STATE_E)
        {
            if (c >= 0x80 && c <= 0xBF )        { state = STATE_B;          }
            else                                { is_valid = false; break;  }
        }
        else if (state == STATE_F)
        {
            if (c >= 0x90 && c <= 0x9F )        { state = STATE_B;          }
            else                                { is_valid = false; break;  }
        }
        else if (state == STATE_G)
        {
            if (c >= 0x80 && c <= 0x8F )        { state = STATE_B;          }
            else                                { is_valid = false; break;  }
        }
    }

    return is_valid;
}

int32_t utf8_code_point(const unsigned char* buf, const uint32_t size, const uint32_t cur, uint32_t* val)
{
    if (cur >= size) { return -1; }

    uint32_t c = (uint32_t)buf[cur];
    if (c < 0x80)
    {
        *val = c;
        return 1;
    }
    else if ((c & 0xE0) == 0xC0)                                        // ensure prefix of 110 - 2 byte codepoint
    {
        if (cur + 1 >= size) { return -1; }

        uint32_t c1 = buf[cur + 1];
        if ((c1 & 0xC0) != 0x80) { return -1; }                         // ensure 2nd byte prefix

        *val = ((c & 0x1F) << 6) |                                      // remove prefix -> shift left to make space for second byte -> 
                (c1 & 0x3f);                                            // remove prefix from second byte -> add together
        return 2;
    }
    else if ((c & 0xF0) == 0xE0)                                        // 1110 - 3 byte codepoint
    {
        if (cur + 2 >= size) { return -1; }

        uint32_t c1 = buf[cur + 1];
        uint32_t c2 = buf[cur + 2];
        if (((c1 & 0xC0) != 0x80) || ((c2 & 0xC0) != 0x80)) { return -1; } // ensure 2nd and 3rd byte prefix

        *val =  ((c & 0x0F) << 12) |                                    // remove prefix -> shift left
                ((c1 & 0x3f) << 6) |                                    // remove prefix -> shift left -> add to first byte
                 (c2 & 0x3f);                                           // remove prefix -> shift left -> add to total
        return 3;
    }
    else if ((c & 0xF8) == 0xF0)                                        // 11110 - 4 byte codepoint
    {
        if (cur + 3 >= size) { return -1; }

        uint32_t c1 = buf[cur + 1];
        uint32_t c2 = buf[cur + 2];
        uint32_t c3 = buf[cur + 3];
        if (((c1 & 0xC0) != 0x80) || ((c2 & 0xC0) != 0x80) || ((c3 & 0xC0) != 0x80)) { return -1; } // ensure 2nd, 3rd and 4th byte prefix

        *val =  ((c & 0x0F) << 18) |                                    // remove prefix -> shift left
                ((c1 & 0x3f) << 12) |                                   // remove prefix -> shift left -> add to first byte
                ((c2 & 0x3f) << 6) |                                    // remove prefix -> shift left -> add to total
                 (c3 & 0x3f);                                           // remove prefix -> shift left -> add to total
        return 4;
    }
    else
    {
        return -1;
    }
}

bool utf8_is_alpha(uint32_t code_point)
{
    return utf8_is_lower_alpha(code_point) || utf8_is_upper_alpha(code_point);
}

bool utf8_is_lower_alpha(uint32_t code_point)
{
    return code_point >= 'a' && code_point <= 'z';
}

bool utf8_is_upper_alpha(uint32_t code_point)
{
    return code_point >= 'A' && code_point <= 'Z';
}