#pragma once
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include "action.h"
#include "scheduler.h"

    typedef struct
    {
        process_t sender_process;

        uint8_t reciever_id;

        uint64_t message_id;

        action_proposal_t action_proposal;
    } message_t;

    int message_handler_printf(const char *format, ...);

    void register_to_message_handler(process_t *process);

    message_t get_message(process_t process, uint64_t index);

    void print_message(const message_t *message);

    uint64_t send_message(message_t *message);

    uint8_t message_handler_initialize(void);

#ifdef __cplusplus
}
#endif