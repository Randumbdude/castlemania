#pragma once

#include <time.h>
#include <stdint.h>
#include "scheduler.h"
#include "message_handler.h"

struct castle_api
{
    time_t *current_time;
    void (*register_to_message_handler)(process_t *process);
    message_t (*get_message)(process_t process, uint64_t index);
    void (*print_message)(const message_t *message);
    uint64_t (*send_message)(message_t *message);
    uint8_t (*register_to_scheduler)(process_t *process_to_schedule);
};