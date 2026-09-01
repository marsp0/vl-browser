#pragma once

#include <stdint.h>

void not_implemented(unsigned char* file, uint32_t line);

#define NOT_IMPLEMENTED                                             \
do                                                                  \
{                                                                   \
    not_implemented(__FILE__, __LINE__);                            \
} while(0);


#define INCOMPLETE_IMPLEMENTATION(msg)                                              \
do                                                                                  \
{                                                                                   \
    /*printf("Section not fully implemented: %s:%d - %s\n", __func__, __LINE__, msg);*/ \
} while(0);
