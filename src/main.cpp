#include "main.h"
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#include "overseer.h"
#include "message_handler.h"
#include "logic_agent.h"

#include "scheduler.h"
#include "hot_loader.h"

time_t current_time;

int main()
{
    initialize_hot_loader();

    initialize_loaded_dlls();

    getchar();

    scheduler_initialize();

    printf("Press any key to exit the program...");
    if (_getch())
        return 0;
}