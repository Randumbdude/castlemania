#pragma once
#define proc_hook __attribute__((constructor))
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
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
    int scheduler_printf(const char *format, ...);
#ifdef __cplusplus
}
#endif