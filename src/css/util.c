#include "util.h"

#include <stdbool.h>
#include <math.h>

#include "util/utf8.h"

double css_convert_buf_to_num(uint32_t* buf, uint32_t buf_size)
{
    bool is_real = false;
    bool is_e = false;
    uint32_t pos = 0;
    int32_t s = 1;
    int32_t i = 0;
    int32_t f = 0;
    int32_t t = 1;
    int32_t e = 0;
    int32_t d = 0;

    for (uint32_t j = 0; j < buf_size; j++)
    {
        if (buf[j] == '-')
        {
            s = -1;
            pos = j + 1;
            break;
        }
    }

    for (uint32_t j = 0; j < buf_size; j++)
    {
        if (buf[j] == '-' || buf[j] == '+')     { continue; }
        if (!utf8_is_digit(buf[j]))    { break; }

        i *= 10;
        i += (int32_t)(buf[j] - 0x30);
        pos = j + 1;
    }

    if (buf[pos] == '.')
    {
        is_real = true;
        pos++;
    }

    if (is_real)
    {
        for (uint32_t j = pos; j < buf_size; j++)
        {
            if (!utf8_is_digit(buf[j]))    { break; }
    
            f *= 10;
            f += (int32_t)(buf[j] - 0x30);
            d++;
            pos = j + 1;
        }
    }

    if (buf[pos] == 'e' || buf[pos] == 'E')
    {
        pos++;
        is_e = true;
    }

    if (buf[pos] == '-')
    {
        t = -1;
        pos++;
    }

    if (buf[pos] == '+')
    {
        pos++;
    }

    if (is_e)
    {
        for (uint32_t j = pos; j < buf_size; j++)
        {
            if (!utf8_is_digit(buf[j]))    { break; }
    
            e *= 10;
            e += (int32_t)(buf[j] - 0x30);
        }
    }

    double p1 = f * pow(10, -d);
    double p2 = pow(10, t * e);
    double result = s * (i + p1 ) * p2;

    return result;
}