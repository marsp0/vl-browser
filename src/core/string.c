#include "string.h"

#include <stdlib.h>
#include <string.h>

#include "util/utf8.h"

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


/********************/
/* static functions */
/********************/


/********************/
/* public functions */
/********************/

string_t string_new(unsigned char* data, uint32_t size)
{
    string_t str;

    if(size == 0) { return str; }

    str.data = malloc(sizeof(unsigned char) * size);
    str.size = size;
    str.len = utf8_get_len(data, size);
    memcpy(str.data, data, size);

    return str;
}


bool string_compare(const string_t first, const string_t second)
{
    if (first.size != second.size) { return false; }

    return strncmp(first.data, second.data, first.size) == 0;
}


bool string_compare_buffer(const string_t first, unsigned char* buffer, uint32_t buffer_size)
{
    if (first.size != buffer_size) { return false; }

    return strncmp(first.data, buffer, buffer_size);
}


void string_free(string_t str)
{
    if (str.size == 0) { return; }

    free(str.data);
}
