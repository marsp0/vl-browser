#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "global_modules.h"

#include "platform/platform.h"
#include "time_utils.h"

int main()
{
    // initialize modules that
    // - preallocate memory
    // - need to run code once
    global_modules_init();
    platform_init();

    struct timespec update_time = { 0 };
    struct timespec dur         = { 0, 5555555L };
    struct timespec rem         = { 0 };
    clock_gettime(CLOCK_MONOTONIC, &update_time);
    bool run = true;

    while (run)
    {
        struct timespec now = { 0 };
        clock_gettime(CLOCK_MONOTONIC, &now);

        struct timespec diff = time_diff(update_time, now);
        if (diff.tv_sec == 0 && diff.tv_nsec < 16666666L)
        {
            nanosleep(&dur, &rem);
            continue;
        }
        update_time = now;

        platform_process_events(&run);

        if (!run) { break; }
    }

    global_modules_free();
}