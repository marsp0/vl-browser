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

    struct timespec start   = { 0 };
    struct timespec end     = { 0 };
    struct timespec diff    = { 0 };
    struct timespec dur     = { 0 };
    struct timespec rem     = { 0 };

    while (platform_should_run())
    {
        clock_gettime(CLOCK_MONOTONIC, &start);

        platform_process_events();

        clock_gettime(CLOCK_MONOTONIC, &end);
        diff = time_diff(start, end);
        if (diff.tv_sec == 0 && diff.tv_nsec < 16000000L)
        {
            dur.tv_nsec = 16000000L - diff.tv_nsec;
            nanosleep(&dur, &rem);
        }
    }

    platform_free();
    global_modules_free();
}