#include "not_implemented.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void not_implemented(unsigned char* file, uint32_t line)
{
    printf(ANSI_COLOR_RED);
    printf("Section not implemented: %s:%d", file, line);
    printf(ANSI_COLOR_RESET "\n");
    assert(false);
}
