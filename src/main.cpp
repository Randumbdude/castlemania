#include "main.h"
#include <cstdio>
#include <conio.h>
#include <ctime>
#include <windows.h>
#include <iostream>

#include "scheduler.h"

time_t current_time;

int main()
{
    scheduler_initialize();

    printf("Press any key to exit the program...");
    if (_getch())
        return 0;
}