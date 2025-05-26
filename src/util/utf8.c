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
bool utf8_validate(unsigned char* buffer, uint32_t size)
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
