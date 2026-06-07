#include "message_handler.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <inttypes.h>
#include <windows.h>

#include "main.h"
#include "scheduler.h"

uint64_t next_message_id = 0;

int message_handler_printf(const char *format, ...)
{
    current_time = time(NULL); // get current time

    struct tm t;

#ifdef _WIN32
    localtime_s(&t, &current_time);
#else
    localtime_r(&current_time, &t);
#endif

    printf(" = message_handler@%02d:%02d:%02d> ",
           t.tm_hour, t.tm_min, t.tm_sec);

    va_list args;
    va_start(args, format);
    int result = vprintf(format, args);
    va_end(args);
    return result;
}

#define MAX_MESSAGES 255
message_t message_box[255][MAX_MESSAGES]; // 255 processs (max uint8_t value) with 256 messages each, should be more than enough for now
process_t *process_indexes[255];

void register_to_message_handler(process_t *process)
{
    process->unread_messages = 0;
    process_indexes[process->process_id] = process;
    message_handler_printf("process \"%s\" registered with process_ID#%d\n", process->process_name, process->process_id);
}

message_t get_message(process_t process, uint64_t index)
{
    return message_box[process.process_id][index];
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

    message_handler_printf(
        "message {\n"
        "                               sender_type: %s (%d)\n"
        "                               reciever_id: %u\n"
        "                               message_id: %" PRIu64 "\n"
        "                               action_proposal: {\n"
        "                                 type: %s (%d)\n"
        "                                 x: %u\n"
        "                                 y: %u\n"
        "                                 confidence: %.2f\n"
        "                               }\n"
        "                             }\n",
        message->sender_process.process_name, message->sender_process.process_id, (unsigned int)message->reciever_id, message->message_id, action_type_to_string(message->action_proposal.type), message->action_proposal.type, (unsigned int)message->action_proposal.x, (unsigned int)message->action_proposal.y, message->action_proposal.confidence);
}

uint64_t send_message(message_t *message)
{
    // set messages message_id tag and increment for the next one
    message->message_id = next_message_id++;
    // this is a biggin.
    message_box[message->reciever_id][(process_indexes[message->reciever_id]->unread_messages >= MAX_MESSAGES) ? 0 : process_indexes[message->reciever_id]->unread_messages] = *message;
    message_handler_printf("message sent with message_ID#%" PRIu64 "\n", message->message_id);
    // check if the message box is full if so = 0;
    if ((process_indexes[message->reciever_id]->unread_messages++ >= MAX_MESSAGES))
    {
        process_indexes[message->reciever_id]->unread_messages = 0;
    }
    // print the message
    // return.
    return message->message_id;
}

// defining runtime
void message_handler_runtime_method(void);

// the struct representing the process to be scheduled
process_t message_handler_process = {
    .process_name = "message_handler",
    .runtime_method = (void *)message_handler_runtime_method,
    .is_running = 1};

// the runtime method that will be looped by the scheduler
void message_handler_runtime_method()
{
    Sleep(1000);
}

// register the process to the scheduler automatically before main() is called
proc_hook void message_handler_auto_register(void)
{
    register_to_scheduler(&message_handler_process);
}