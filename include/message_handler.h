#pragma once
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include "action.h"
#include "scheduler.h"

    /**
     * @struct message_t
     * @brief Represents a message passed between processs in the castle suite system.
     *
     * @member sender_type The type of process sending this message.
     * @member reciever_id The unique identifier of the process receiving this message.
     * @member message_id A unique identifier for this message, used for tracking and correlation.
     * @member action_proposal The proposed action contained within this message.
     */
    typedef struct
    {
        process_t sender_process;

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
    int message_handler_printf(const char *format, ...);

    /**
     * @brief Registers an process with the message handler and returns its unique identifier.
     *
     * This function assigns a unique ID to a new process registering with the message handler.
     * It prints a confirmation message with the assigned ID and increments the counter for
     * the next process registration.
     *
     * @return uint8_t The unique identifier (ID) assigned to the newly registered process.
     *                  The ID is incremented after each registration.
     *
     * @note This function assumes that next_process_id will not overflow beyond UINT8_MAX.
     *       Consider adding overflow protection if registrations exceed 255 processs.
     *
     * @see message_handler_printf()
     */
    void register_to_message_handler(process_t *process);

    /**
     * @brief Retrieves a message from the message box.
     *
     * Fetches a message for a specific process from the message box at the given index.
     *
     * @param process_id The ID of the process whose message box to query.
     * @param index    The index position in the process's message box.
     *
     * @return The message_t structure at the specified location in the message box.
     *
     * @warning No bounds checking is performed. Ensure index is valid for the process.
     */
    message_t get_message(process_t process, uint64_t index);

    /**
     * @brief Prints every field inside a message_t.
     *
     * Includes the embedded action_proposal_t fields.
     *
     * @param message The message to print.
     */
    void print_message(const message_t *message);

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

#ifdef __cplusplus
}
#endif