
#include "overseer.h"
#include "main.h"
#include "memory.h"
#include "message_handler.h"

#include "matrix.h"
#include "__matrix_.h"

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <windows.h>

#include <chrono>
#include <iostream>
#include <thread>

int overseer_printf(const char *format, ...)
{
    current_time = time(NULL); // get current time
    struct tm t;

#ifdef _WIN32
    localtime_s(&t, &current_time);
#else
    localtime_r(&current_time, &t);
#endif

    printf("$~ overseer@%02d:%02d:%02d> ",
           t.tm_hour, t.tm_min, t.tm_sec);

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

#define TEST 512
int test_gpu()
{
    const int N = TEST;

    overseer_printf("Allocating matrices %dx%d...\n", N, N);

    matrix A(N, N);
    matrix B(N, N);

    // Fill matrices with simple values
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
        {
            A.set(r, c, r + c);
            B.set(r, c, r + c);
            // B.set(r, c, (r * c) % 1000);
        }

    overseer_printf("Running GPU matrix multiplication...\n");

    auto t1 = std::chrono::high_resolution_clock::now();

    matrix C = A * B; // (A * B) = C

    auto t2 = std::chrono::high_resolution_clock::now();

    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();
    overseer_printf("GPU matmul completed in %.3f seconds\n\n", ms / 1000);

    return 0;
}
int test_cpu()
{
    const int N = TEST;

    overseer_printf("Allocating matrices %dx%d...\n", N, N);

    __matrix_ A(N, N);
    __matrix_ B(N, N);

    // Fill matrices with simple values
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
        {
            A.set(r, c, r + c);
            B.set(r, c, r + c);
            // B.set(r, c, (r * c) % 1000);
        }

    overseer_printf("Running CPU matrix multiplication...\n");

    auto t1 = std::chrono::high_resolution_clock::now();

    __matrix_ C = A * B; // (A * B) = C

    auto t2 = std::chrono::high_resolution_clock::now();

    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();
    overseer_printf("CPU matmul completed in %.3f seconds\n\n", ms / 1000);

    return 0;
}

// the runtime method that will be looped by the scheduler
uint8_t incr = 0;
void overseer_runtime_method()
{
    if ((overseer_process.unread_messages != 0) && (incr == 0))
    {
        Sleep(1000);
        message_t message = get_message(overseer_process, 0);
        // overseer_printf("received message_ID#%d successfully\n", message.message_id);
        print_message(&message);

        Sleep(500);
        // int value = 5;
        // gpu_add_10(&value);
        // overseer_printf("%d\n", value);
        test_gpu();

        Sleep(500);
        // int value = 5;
        // cpu_add_10(&value);
        // overseer_printf("%d\n", value);
        test_cpu();

        incr++;
    }
}
// register the process to the scheduler automatically before main() is called
proc_hook void overseer_auto_register(void)
{
    register_to_scheduler(&overseer_process);
    register_to_message_handler(&overseer_process);
}
