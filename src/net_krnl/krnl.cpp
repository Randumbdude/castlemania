// main inc
#include "krnl.h"

#include <conio.h>
#include <cstdio>

// our inc
#include "scheduler.h"

time_t current_time;

uint8_t krnl_initialize(void)
{
    // start by initializing the scheduler
    scheduler_initialize();

    printf("Press any key to exit the program...");
    if (_getch())
        return 0;
}
