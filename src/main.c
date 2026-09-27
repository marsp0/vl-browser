#include <stdio.h>
#include <stdint.h>

#include "global_modules.h"

#include "platform/platform.h"

int main()
{
    printf("Browser compiled\n");
    // printf("Option size: %lu \n", sizeof(html_option_t));
    global_modules_init();

    platform_init();

    bool run = true;

    while (run)
    {
        platform_process_events(&run);

        if (!run) { break; }
    }

    global_modules_free();
}