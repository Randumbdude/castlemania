#pragma once
#include <stdint.h>
#include "game_interface/action_interface/action.h"

typedef enum
{
    explorer_type = 0,
    logic_type,
    pattern_type,
    probability_type,
    overseer_type
} agent_type;

typedef struct
{
    uint8_t sender_id;
    agent_type sender_type;

    uint64_t message_id;

    action_proposal_t action_proposal;
} message_t;

/// @brief Registers an agent with the message handler and returns a unique ID for that agent.
/// @return unique ID for the agent
uint8_t register_to_message_handler(void);

/// @brief Sends a message to the message handler.
/// @param message The message to send
/// @return unique ID for the message
uint64_t send_message(message_t message);

/// @brief Gets a message from the message handler based off the ID & index.
/// @param agent_id The ID of the agent getting the message
/// @param index The index of the message to retrieve from the inbox
/// @return The requested message
message_t get_message(uint8_t agent_id, uint64_t index);
/// @brief Gets the size of the inbox.
/// @return The size of the inbox
uint64_t get_inbox_size(void);