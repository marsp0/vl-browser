#pragma once

#define NOT_IMPLEMENTED                                         \
do                                                              \
{                                                               \
    printf("Section not implemented: parser.c:%d\n", __LINE__); \
    assert(false);                                              \
} while(0);
