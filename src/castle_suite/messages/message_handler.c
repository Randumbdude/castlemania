#include "message_handler.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "main.h"

uint8_t next_agent_id;
uint64_t next_message_id;

int message_handler_printf(const char *format, ...)
{
    current_time = time(NULL);               // get current time
    struct tm *t = localtime(&current_time); // convert to local time

    printf(" = message_handler@%02d:%02d:%02d> ",
           t->tm_hour, t->tm_min, t->tm_sec);

    va_list args;
    va_start(args, format);
    int result = vprintf(format, args);
    va_end(args);
    return result;
}

#define MAX_MESSAGES 256
message_t message_box[255][MAX_MESSAGES]; // 255 agents (max uint8_t value) with 256 messages each, should be more than enough for now

void message_handler_initialize(void)
{
    next_agent_id = 0;
    next_message_id = 0;
}

uint8_t register_to_message_handler(void)
{
    message_handler_printf("agent registered with ID#%d\n", next_agent_id);
    return next_agent_id++;
}

message_t get_message(uint8_t agent_id, uint64_t index)
{
    return message_box[agent_id][index];
}

uint64_t get_inbox_size(uint8_t agent_id)
{
    uint64_t size = 0;
    for (uint64_t i = 0; i < MAX_MESSAGES; i++)
    {
        if (message_box[agent_id][i].message_id != 0)
            size++;
        else
            break;
    }
    return size;
}

uint64_t send_message(message_t *message)
{
    message->message_id = next_message_id++;
    message_box[message->reciever_id][0] = *message;

    message_handler_printf("message sent with message_ID#%d\n", message->message_id);

    return message->message_id;
}