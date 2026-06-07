
#include "overseer.h"
#include "main.h"
#include "memory.h"
#include "message_handler.h"
// #include "algorithm/algorithm.h"
#include "matrix.h"

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <windows.h>

#include <chrono>
#include <iostream>

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

uint8_t incr = 0;

int test()
{
    const int N = 1024;

    overseer_printf("Allocating matrices %dx%d...\n", N, N);

    matrix A(N, N);
    matrix B(N, N);

    // Fill matrices with simple values
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
        {
            A.set(r, c, r + c);
            B.set(r, c, (r * c) % 1000);
        }

    overseer_printf("Running GPU matrix multiplication...\n");

    auto t1 = std::chrono::high_resolution_clock::now();
    matrix C = A * B;
    auto t2 = std::chrono::high_resolution_clock::now();

    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();
    overseer_printf("GPU matmul completed in %.3f ms\n", ms);

    overseer_printf("Running Hadamard product...\n");
    matrix H = A.hadamard(B);

    overseer_printf("Running sigmoid...\n");
    matrix S = A.sigmoid();

    overseer_printf("Running transpose...\n");
    matrix T = A.transpose();

    overseer_printf("\nSample values:\n");
    overseer_printf("C(0,0) = %f\n", C.get(0, 0));
    overseer_printf("H(10,10) = %f\n", H.get(10, 10));
    overseer_printf("S(500,500) = %f\n", S.get(500, 500));
    overseer_printf("T(0,10) = %f\n", T.get(0, 10));

    return 0;
}

// the runtime method that will be looped by the scheduler
void overseer_runtime_method()
{
    if ((overseer_process.unread_messages != 0) && (incr == 0))
    {
        Sleep(1000);
        message_t message = get_message(overseer_process, 0);
        // overseer_printf("received message_ID#%d successfully\n", message.message_id);
        print_message(&message);
        incr++;

        Sleep(500);
        // int value = 5;
        // gpu_add_10(&value);
        // overseer_printf("%d\n", value);
        test();
    }
}
// register the process to the scheduler automatically before main() is called
proc_hook void overseer_auto_register(void)
{
    register_to_scheduler(&overseer_process);
    register_to_message_handler(&overseer_process);
}
