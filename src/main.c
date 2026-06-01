#include <stdio.h>
#include <conio.h>
#include <time.h>

// #include "castle_suite/memory/memory.h"
// #include "castle_suite/messages/message_handler.h"

#include "mania_suite/logic_agent/logic_agent.h"
#include "castle_suite/overseer/overseer.h"

time_t current_time;

int main()
{
    printf("preparing...\n");

    overseer_initialize();
    logic_agent_initialize();

    printf("ready!\n");

    printf("Press any key to exit the program...");
    if (getch())
        return 0;
}