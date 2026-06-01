#include "logic_agent.h"
#include "main.h"
#include "castle_suite/memory/memory.h"
#include "castle_suite/messages/message_handler.h"

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

uint8_t logic_agent_id;

int logic_printf(const char *format, ...)
{
    current_time = time(NULL);               // get current time
    struct tm *t = localtime(&current_time); // convert to local time
    printf("logic_agent@%02d:%02d:%02d> ",
           t->tm_hour, t->tm_min, t->tm_sec);

    va_list args;
    va_start(args, format);
    int result = vprintf(format, args);
    va_end(args);
    return result;
}

void logic_agent_initialize(void)
{
    logic_agent_id = register_to_message_handler();

    logic_printf("registration with ID#%d completed\n", logic_agent_id);
}