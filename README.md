# castlemania

Minesweeper AI

## Description

A multi-agent AI built to solve minesweeper.

## Getting Started

### Dependencies

* Windows 10

### Installing

* How/where to download your program
* Any modifications needed to be made to files/folders

### Executing program

* below is example code for an agent.
```
#include "main.h"
#include "castle_suite/messages/message_handler.h"

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

// defining runtime
void test_runtime_method(void);

// the struct representing the process to be scheduled
process_t test_process = {
    .process_name = "test",
    .runtime_method = (void *)test_runtime_method,
    .is_running = 1};

// the runtime method that will be looped by the scheduler
void test_runtime_method()
{
    test_printf("test process is running\n");
    Sleep(1000);
}

// register the process to the scheduler automatically before main() is called
proc_hook void atest_auto_register(void)
{
    register_to_scheduler(&test_process);
    register_to_message_handler(&test_process);
}
```

## Help

Any advise for common problems or issues.
```
command to run if program contains helper info
```

## Authors

Contributors names and contact info

O_Riley

## Version History

* 0.1
    * Various bug fixes and optimizations
    * See [commit change]() or See [release history]()
* 0.0
    * Development start.

## License

This project is licensed under the MIT License - see the LICENSE file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [awesome-readme](https://github.com/matiassingers/awesome-readme)
* [PurpleBooth](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2)
* [dbader](https://github.com/dbader/readme-template)
* [zenorocha](https://gist.github.com/zenorocha/4526327)
* [fvcproductions](https://gist.github.com/fvcproductions/1bfc2d4aecb01a834b46)