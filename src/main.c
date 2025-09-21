#include <stdio.h>
#include <stdint.h>

#include "dom/option.h"

int main()
{
    printf("Browser compiled\n");
    printf("Option size: %lu \n", sizeof(html_option_t));
}