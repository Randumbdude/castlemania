#include "scheduler.hpp"

// C++ libs
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector> // for event

// C libs
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <windows.h>

// C includes
#include "main.h"
#include "message_handler.h"

int scheduler_printf(const char *format, ...)
{
    current_time = time(NULL); // get current time
    struct tm t;

#ifdef _WIN32
    localtime_s(&t, &current_time);
#else
    localtime_r(&current_time, &t);
#endif

    printf(" ~ scheduler@%02d:%02d:%02d> ",
           t.tm_hour, t.tm_min, t.tm_sec);

    va_list args;
    va_start(args, format);
    int result = vprintf(format, args);
    va_end(args);
    return result;
}

#define MAX_PROCESSES 255
process_t scheduled_processes[MAX_PROCESSES];
uint8_t next_process_id = 0;

void scheduler_runtime_method(void)
{
    DWORD tid = GetCurrentThreadId();
    scheduler_printf("scheduler running on thread ID: %lu (0x%lx)\n",
                     (unsigned long)tid,
                     (unsigned long)tid);

    std::vector<std::thread> threads;

    for (uint8_t i = 0; i < next_process_id; i++)
    {
        if (scheduled_processes[i].is_running == 1)
        {
            threads.emplace_back([i]()
                                 {
            DWORD tid2 = GetCurrentThreadId();
            scheduler_printf("process \"%s\" running on thread ID: %lu (0x%lx)\n",
                scheduled_processes[i].process_name,
                (unsigned long)tid2,
                (unsigned long)tid2);
            while (scheduled_processes[i].is_running == 1)
            {
                void (*process_method)() = (void (*)())scheduled_processes[i].runtime_method;
                process_method();
                // scheduler_printf("process_ID#%d executed successfully\n", i);
            } });
        }
    }

    for (auto &thread : threads)
    {
        thread.join();
    }
}

extern "C"
{
    uint8_t register_to_scheduler(process_t *process_to_schedule)
    {
        if ((next_process_id == 0) && (strcmp(process_to_schedule->process_name, "overseer") != 0))
        {
            next_process_id++;
        }
        if (next_process_id >= MAX_PROCESSES)
        {
            scheduler_printf("fatal error: maximum number of scheduled processes (%d) exceeded.\n", MAX_PROCESSES);
            throw std::runtime_error("maximum number of scheduled processes exceeded");
            return -1; // Indicate error
        }
        if (strcmp(process_to_schedule->process_name, "overseer") == 0)
        {
            scheduled_processes[0] = *process_to_schedule;
            scheduler_printf("process \"%s\" registered with process_ID#%d\n", process_to_schedule->process_name, 0);
            process_to_schedule->process_id = 0;
            return 0;
        }
        else
        {
            scheduled_processes[next_process_id] = *process_to_schedule;
            scheduler_printf("process \"%s\" registered with process_ID#%d\n", process_to_schedule->process_name, next_process_id);
            process_to_schedule->process_id = next_process_id;
            return next_process_id++;
        }
    }

    void scheduler_initialize(void)
    {
        scheduler_printf("finalizing...\n");

        std::thread scheduler_runtime_thread(scheduler_runtime_method);

        scheduler_printf("complete.\n");

        while (1)
            ;

        scheduler_runtime_thread.join();
    }
}