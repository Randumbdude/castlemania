#include "overseer.h"
#include "main.h"
#include "castle_suite/memory/memory.h"
#include "castle_suite/messages/message_handler.h"

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

uint8_t overseer_id = 0; // should always be 0 anyway :|

int overseer_printf(const char *format, ...)
{
    current_time = time(NULL);               // get current time
    struct tm *t = localtime(&current_time); // convert to local time

    printf("$~ overseer@%02d:%02d:%02d> ",
           t->tm_hour, t->tm_min, t->tm_sec);

    va_list args;
    va_start(args, format);
    int result = vprintf(format, args);
    va_end(args);
    return result;
}

void overseer_initialize(void)
{
    overseer_id = register_to_message_handler();

    overseer_printf("registration with ID#%d completed\n", overseer_id);
}