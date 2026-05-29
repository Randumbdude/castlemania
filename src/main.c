#include <stdio.h>
#include <conio.h>

int main()
{
    printf("Press any key to exit the program...");
    if (getch())
        return 0; // 0 indicates successful termination
}