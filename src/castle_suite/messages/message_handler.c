#include "message_handler.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "main.h"

uint8_t next_agent_id = 0;

int message_handler_printf(const char *format, ...)
{
    current_time = time(NULL);               // get current time
    struct tm *t = localtime(&current_time); // convert to local time

    printf("message_handler@%02d:%02d:%02d> ",
           t->tm_hour, t->tm_min, t->tm_sec);

    va_list args;
    va_start(args, format);
    int result = vprintf(format, args);
    va_end(args);
    return result;
}

uint8_t register_to_message_handler(void)
{
    message_handler_printf("agent registered with ID: %d\n", next_agent_id);
    return next_agent_id++;
}

uint64_t send_message(message_t message)
{
}

message_t get_message(uint8_t agent_id, uint64_t index)
{
}

uint64_t get_inbox_size(void)
{
}
