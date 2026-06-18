#pragma once
#define proc_hook __attribute__((constructor))
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
    int scheduler_printf(const char *format, ...);
    void scheduler_initialize(void);
    typedef struct
    {
        char process_name[16];
        uint8_t process_id;
        void *runtime_method;
        uint8_t is_running;
        uint8_t unread_messages;
    } process_t;
    uint8_t register_to_scheduler(process_t *process_to_schedule);
    uint8_t make_process_idle(process_t *process);
#ifdef __cplusplus
}
#endif