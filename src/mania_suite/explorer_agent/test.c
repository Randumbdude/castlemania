
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

int test_printf(const char *format, ...)
{
    current_time = time(NULL);               // get current time
    struct tm *t = localtime(&current_time); // convert to local time

    printf(" - test@%02d:%02d:%02d> ",
           t->tm_hour, t->tm_min, t->tm_sec);

    va_list args;
    va_start(args, format);
    int result = vprintf(format, args);
    va_end(args);
    return result;
}

// our includes
#include "main.h"
#include "castle_suite/messages/message_handler.h"
// the runtime method that will be looped by the scheduler
void test_process_runtime_method()
{
    test_printf("test process is running\n");
}

// the struct representing the process to be scheduled
process_t test_process = {
    .process_name = "test_process",
    .runtime_method = (void *)test_process_runtime_method,
    .is_running = 1};

// register the process to the scheduler automatically before main() is called
proc_hook void test_process_auto_register(void)
{
    register_to_scheduler(&test_process);
    register_to_message_handler(&test_process);
}