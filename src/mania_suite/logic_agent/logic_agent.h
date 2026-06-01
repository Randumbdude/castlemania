#pragma once
#include <stdint.h>

extern uint8_t logic_agent_id;

int logic_printf(const char *format, ...);

void logic_agent_initialize(void);