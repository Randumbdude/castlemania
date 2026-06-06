#include "main.h"
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#include "overseer.h"
#include "message_handler.h"
#include "logic_agent.h"

#include "scheduler.hpp"

time_t current_time;

int main()
{
    // DWORD tid = GetCurrentThreadId();
    // printf("'main' running on thread ID: %lu (0x%lx)\n",
    //        (unsigned long)tid,
    //        (unsigned long)tid);

    scheduler_initialize();

    printf("Press any key to exit the program...");
    if (_getch())
        return 0;
}