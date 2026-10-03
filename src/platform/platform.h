#pragma once

#include <stdbool.h>

void platform_init();
void platform_process_events();
bool platform_should_run();
void platform_paint();
void platform_free();