#pragma once
#include <stdint.h>
#include "game_interface/action_interface/action.h"

typedef enum
{
    explorer_type,
    logic_type,
    pattern_type,
    probability_type,
    overseer_type
} agent_type;

typedef struct
{
    agent_type sender_type;

    uint8_t reciever_id;

    uint64_t message_id;

    action_proposal_t action_proposal;
} message_t;

/**
 * @brief Formats and handles a message using printf-style formatting.
 *
 * @param format A printf-style format string specifying how to format the message.
 * @param ... Variable number of arguments corresponding to the format specifiers in the format string.
 *
 * @return The number of characters printed, or a negative value on error.
 *
 * @note This function uses variable arguments (variadic function) to support flexible message formatting.
 *       Ensure that the arguments match the format specifiers to avoid undefined behavior.
 */
// int message_handler_printf(const char *format, ...);

/**
 * @brief Initializes the message handler system.
 *
 * Resets global identifiers for agents and messages to their initial state.
 * This function should be called once during system initialization before
 * any message handling operations begin.
 *
 * @return void
 */
void message_handler_initialize(void);

/**
 * @brief Registers an agent with the message handler and returns its unique identifier.
 *
 * This function assigns a unique ID to a new agent registering with the message handler.
 * It prints a confirmation message with the assigned ID and increments the counter for
 * the next agent registration.
 *
 * @return uint8_t The unique identifier (ID) assigned to the newly registered agent.
 *                  The ID is incremented after each registration.
 *
 * @note This function assumes that next_agent_id will not overflow beyond UINT8_MAX.
 *       Consider adding overflow protection if registrations exceed 255 agents.
 *
 * @see message_handler_printf()
 */
uint8_t register_to_message_handler(void);

/**
 * @brief Retrieves a message from the message box.
 *
 * Fetches a message for a specific agent from the message box at the given index.
 *
 * @param agent_id The ID of the agent whose message box to query.
 * @param index    The index position in the agent's message box.
 *
 * @return The message_t structure at the specified location in the message box.
 *
 * @warning No bounds checking is performed. Ensure index is valid for the agent.
 */
message_t get_message(uint8_t agent_id, uint64_t index);
/**
 * @brief Gets the number of messages in an agent's inbox
 * @param agent_id The ID of the agent whose inbox size is to be retrieved
 * @return The number of messages currently in the agent's inbox
 * @note Iterates through the message box array until a message with ID 0 is found,
 *       assuming messages are stored contiguously and terminated by a zero ID
 */
uint64_t get_inbox_size(uint8_t agent_id);

/**
 * @brief Sends a message and stores it in the message box.
 *
 * Assigns a unique message ID to the message, stores it in the message box
 * for the specified sender, and logs the operation.
 *
 * @param message The message to send. The message_t struct should have
 *                sender_id populated before calling this function.
 *
 * @return The assigned message ID (uint64_t) for tracking the message.
 *
 * @note The message ID is auto-incremented with each call via next_message_id.
 */
uint64_t send_message(message_t *message);