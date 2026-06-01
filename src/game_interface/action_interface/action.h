#pragma once
#include <stdint.h>

typedef enum
{
    dig,
    flag
} action_type;

typedef struct
{
    action_type type;

    uint8_t x;
    uint8_t y;

    float confidence; // only used for dig actions, ignored for flag actions

} action_proposal_t;