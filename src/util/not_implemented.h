#pragma once

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define NOT_IMPLEMENTED                                             \
do                                                                  \
{                                                                   \
    printf(ANSI_COLOR_RED);                                         \
    printf("Section not implemented: %s:%d", __FILE__, __LINE__);   \
    printf(ANSI_COLOR_RESET "\n");                                  \
    assert(false);                                                  \
} while(0);


#define INCOMPLETE_IMPLEMENTATION(msg)                                              \
do                                                                                  \
{                                                                                   \
    /*printf("Section not fully implemented: %s:%d - %s\n", __func__, __LINE__, msg);*/ \
} while(0);
