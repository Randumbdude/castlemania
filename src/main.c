#include "main.h"
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#include "castle_suite/overseer/overseer.h"
#include "castle_suite/messages/message_handler.h"
#include "mania_suite/logic_agent/logic_agent.h"

#include "castle_suite/scheduler.hpp"

time_t current_time;

int main()
{
    // DWORD tid = GetCurrentThreadId();
    // printf("'main' running on thread ID: %lu (0x%lx)\n",
    //        (unsigned long)tid,
    //        (unsigned long)tid);

    scheduler_initialize();

    printf("Press any key to exit the program...");
    if (getch())
        return 0;
}