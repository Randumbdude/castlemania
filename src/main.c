#include "main.h"
#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "castle_suite/overseer/overseer.h"
#include "castle_suite/messages/message_handler.h"
#include "mania_suite/logic_agent/logic_agent.h"

#include "castle_suite/scheduler.hpp"

time_t current_time;

int main()
{
    scheduler_initialize();

    printf("Press any key to exit the program...");
    if (getch())
        return 0;
}