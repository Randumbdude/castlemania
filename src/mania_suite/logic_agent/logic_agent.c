#include "logic_agent.h"
#include "main.h"

#include "memory.h"
#include "message_handler.h"

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <windows.h>

uint8_t logic_agent_id;

int logic_printf(const char *format, ...)
{
    current_time = time(NULL);               // get current time
    struct tm *t = localtime(&current_time); // convert to local time
    printf(" - logic_agent@%02d:%02d:%02d> ",
           t->tm_hour, t->tm_min, t->tm_sec);

    va_list args;
    va_start(args, format);
    int result = vprintf(format, args);
    va_end(args);
    return result;
}

// defining runtime
void logic_agent_runtime_method(void);
// the struct representing the process to be scheduled
process_t logic_agent_process = {
    .process_name = "logic_agent",
    .runtime_method = (void *)logic_agent_runtime_method,
    .is_running = 1};
int incr_1;
// the runtime method that will be looped by the scheduler
void logic_agent_runtime_method()
{
    if (incr_1 == 0)
    {
        Sleep(1000);
        message_t message = {
            .sender_process = logic_agent_process,
            .reciever_id = 0,
            .message_id = -1,
            .action_proposal = {
                .type = dig,
                .x = (WIDTH / 2),
                .y = (HEIGHT / 2),
                .confidence = 1.0f}};
        // message_handler will auto assign a unique message_id when sending
        send_message(&message);
        // logic_printf("sent message_ID#%d successfully\n", message.message_id);
        incr_1++;
    }
}
// register the process to the scheduler automatically before main() is called
proc_hook void logic_agent_auto_register(void)
{
    register_to_scheduler(&logic_agent_process);
    register_to_message_handler(&logic_agent_process);
}