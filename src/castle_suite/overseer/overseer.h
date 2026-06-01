#pragma once
#include <stdint.h>

extern uint8_t overseer_id;

int overseer_printf(const char *format, ...);

void overseer_initialize(void);