#include "message_handler.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <inttypes.h>

#include "main.h"
#include "castle_suite/scheduler.hpp"

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
message_t message_box[255][MAX_MESSAGES]; // 255 processs (max uint8_t value) with 256 messages each, should be more than enough for now

void message_handler_initialize(void)
{
    next_message_id = 0;
}

void register_to_message_handler(process_t *process)
{
    message_handler_printf("process \"%s\" registered with process_ID#%d\n", process->process_name, process->process_id);
}

message_t get_message(uint8_t process_id, uint64_t index)
{
    return message_box[process_id][index];
}

uint64_t get_inbox_size(uint8_t process_id)
{
    uint64_t size = 0;
    for (uint64_t i = 0; i < MAX_MESSAGES; i++)
    {
        if (message_box[process_id][i].message_id != 0)
            size++;
        else
            break;
    }
    return size;
}

static const char *process_type_to_string(process_type type)
{
    switch (type)
    {
    case explorer_type:
        return "explorer_type";
    case logic_type:
        return "logic_type";
    case pattern_type:
        return "pattern_type";
    case probability_type:
        return "probability_type";
    case overseer_type:
        return "overseer_type";
    default:
        return "unknown_process_type";
    }
}

static const char *action_type_to_string(action_type type)
{
    switch (type)
    {
    case dig:
        return "dig";
    case flag:
        return "flag";
    default:
        return "unknown_action_type";
    }
}

void print_message(const message_t *message)
{
    if (message == NULL)
    {
        message_handler_printf("message: (null)\n");
        return;
    }

    message_handler_printf("message {\n");
    message_handler_printf("  sender_type: %s (%d)\n",
                           process_type_to_string(message->sender_type),
                           message->sender_type);
    message_handler_printf("  reciever_id: %u\n", (unsigned int)message->reciever_id);
    message_handler_printf("  message_id: %" PRIu64 "\n", message->message_id);
    message_handler_printf("  action_proposal: {\n");
    message_handler_printf("    type: %s (%d)\n",
                           action_type_to_string(message->action_proposal.type),
                           message->action_proposal.type);
    message_handler_printf("    x: %u\n", (unsigned int)message->action_proposal.x);
    message_handler_printf("    y: %u\n", (unsigned int)message->action_proposal.y);
    message_handler_printf("    confidence: %.2f\n", message->action_proposal.confidence);
    message_handler_printf("  }\n");
    message_handler_printf("}\n");
}

uint64_t send_message(message_t *message)
{
    message->message_id = next_message_id++;
    message_box[message->reciever_id][0] = *message;

    message_handler_printf("message sent with message_ID#%" PRIu64 "\n", message->message_id);

    return message->message_id;
}
