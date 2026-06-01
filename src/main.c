#include <stdio.h>
#include <conio.h>

#include "castle_suite/memory/memory.h"

int main()
{
    printf("preparing...\n");

#ifdef _WIN32
    printf("You are using Windows.\n");
#else
    printf("Not a Windows system!\n");
#endif

    printf("ready!\n");

    printf("Press any key to exit the program...");
    if (getch())
        return 0;
}