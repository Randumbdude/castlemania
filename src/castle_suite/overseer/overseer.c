
#include "overseer.h"
#include "main.h"
#include "castle_suite/memory/memory.h"
#include "castle_suite/messages/message_handler.h"

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <windows.h>

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

// defining runtime
void overseer_runtime_method(void);
// the struct representing the process to be scheduled
process_t overseer_process = {
    .process_name = "overseer",
    .runtime_method = (void *)overseer_runtime_method,
    .is_running = 1};
// the runtime method that will be looped by the scheduler
void overseer_runtime_method()
{
    overseer_printf("overseer process is running\n");
    Sleep(1000);

    if (get_inbox_size(overseer_process.process_id) > 0)
    {
        message_t message = get_message(overseer_process.process_id, 0);
        overseer_printf("received message_ID#%d successfully\n", message.message_id);
    }
}
// register the process to the scheduler automatically before main() is called
proc_hook void aoverseer_auto_register(void)
{
    register_to_scheduler(&overseer_process);
    register_to_message_handler(&overseer_process);
}
