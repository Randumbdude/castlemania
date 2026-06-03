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

ex. Dominique Pizzie  
ex. [@DomPizzie](https://twitter.com/dompizzie)

## Version History

* 0.2
    * Various bug fixes and optimizations
    * See [commit change]() or See [release history]()
* 0.1
    * Initial Release

## License

This project is licensed under the [NAME HERE] License - see the LICENSE.md file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [awesome-readme](https://github.com/matiassingers/awesome-readme)
* [PurpleBooth](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2)
* [dbader](https://github.com/dbader/readme-template)
* [zenorocha](https://gist.github.com/zenorocha/4526327)
* [fvcproductions](https://gist.github.com/fvcproductions/1bfc2d4aecb01a834b46)