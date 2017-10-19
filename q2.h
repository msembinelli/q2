/**********************************************************
 * Name:
 *     q2.h
 *
 * Description:
 *     Header for power of two queue.
 *
 * Copyright (c) 2017 Matthew Sembinelli
 *********************************************************/
#ifndef Q2_H
#define Q2_H

/**********************************************************
 * Includes
 *********************************************************/
#include <stdint.h>
#include <stdbool.h>

/**********************************************************
 * Types
 *********************************************************/
typedef enum
{
    Q2_RETURN_BASE                   = (0x00),

    Q2_SUCCESS                       = (Q2_RETURN_BASE + 0),
    Q2_ERROR_FULL                    = (Q2_RETURN_BASE + 1),
    Q2_ERROR_EMPTY                   = (Q2_RETURN_BASE + 2),
    Q2_ERROR_LENGTH_NOT_POWER_OF_TWO = (Q2_RETURN_BASE + 3),
    Q2_ERROR_NULL_PARAMETER          = (Q2_RETURN_BASE + 4),
    Q2_ERROR_NOT_INITIALIZED         = (Q2_RETURN_BASE + 5),

    Q2_RETURN_MAX                    = (0xFF)
} q2_return_t;

typedef struct
{
    bool initialized;

    uint32_t head;
    uint32_t tail;
    bool empty;
    bool full;

    void* data;
    uint32_t max_length;
    uint32_t item_length;
} q2_context_t;

/**********************************************************
 * Macros
 *********************************************************/
#define Q2(context_name, struct_type, queue_size) \
        static struct_type context_name##_array[queue_size]; \
        static q2_context_t context_name = { \
            .initialized = false, \
            .head = 0, \
            .tail = 0, \
            .empty = true, \
            .full = false, \
            .data = context_name##_array, \
            .max_length = queue_size, \
            .item_length = sizeof(struct_type) \
        };


/**********************************************************
 * Prototypes
 *********************************************************/
/**********************************************************
 * Name:
 *    q2_init
 *
 * Description:
 *    Initializes the q2 context. Checks that the queue
 *    length is a power of two.
 *
 * Parameters:
 *    q2_context_t* const ctx - Pointer to the q2 context.
 *
 * Returns:
 *    Q2_ERROR_LENGTH_NOT_POWER_OF_TWO - Buffer size is not
 *                                       a power of two
 *    Q2_ERROR_NULL_PARAMETER - When ctx or input is NULL.
 *    Q2_SUCCESS - Buffer size is a power of two, context
 *                 initialized.
 *********************************************************/
uint32_t q2_init(q2_context_t* const ctx);

/**********************************************************
 * Name:
 *    q2_put
 *
 * Description:
 *    Adds an item to the queue and updates the head index.
 *
 * Parameters:
 *    q2_context_t* const ctx - Pointer to the q2 context.
 *    void* const input - Item to be put in the queue.
 *
 * Returns:
 *    Q2_ERROR_FULL - Queue is full.
 *    Q2_SUCCESS - Successfully added item to queue.
 *    Q2_ERROR_NULL_PARAMETER - When ctx or input is NULL.
 *    Q2_ERROR_NOT_INITIALIZED - When q2 init has not been
 *                               called is NULL.
 **********************************************************/
uint32_t q2_put(q2_context_t* const ctx, void* const input);

/**********************************************************
 * Name:
 *    q2_get
 *
 * Description:
 *    Gets an item from the queue and updates the tail index.
 *
 * Parameters:
 *    q2_context_t* const ctx - Pointer to the q2 context.
 *    void* const output - Item to be put in the queue.
 *
 * Returns:
 *    Q2_ERROR_EMPTY - Queue is empty.
 *    Q2_SUCCESS - Successfully retrieved item from queue.
 *    Q2_ERROR_NULL_PARAMETER - When ctx or output is NULL.
 *    Q2_ERROR_NOT_INITIALIZED - When q2 init has not been
 *                               called is NULL.
 *********************************************************/
 uint32_t q2_get(q2_context_t* const ctx, void* const output);

/**********************************************************
 * Name:
 *    q2_empty
 *
 * Description:
 *    Returns whether the queue is empty or not.
 *
 * Parameters:
 *    q2_context_t* const ctx - Pointer to the q2 context.
 *    bool* const empty - If set to true, queue is empty.
 *
 * Returns:
 *    Q2_SUCCESS - Successfully retrieved empty status.
 *    Q2_ERROR_NULL_PARAMETER - When ctx is NULL.
 *    Q2_ERROR_NOT_INITIALIZED - When q2 init has not been
 *                               called is NULL.
 *********************************************************/
uint32_t q2_empty(q2_context_t* const ctx, bool* const empty);

/**********************************************************
 * Name:
 *    q2_full
 *
 * Description:
 *    Returns whether the queue is full or not.
 *
 * Parameters:
 *    q2_context_t* const ctx - Pointer to the q2 context.
 *    bool* const full - If set to true, queue is full.
 *
 * Returns:
 *    Q2_SUCCESS - Successfully retrieved full status.
 *    Q2_ERROR_NULL_PARAMETER - When ctx is NULL.
 *    Q2_ERROR_NOT_INITIALIZED - When q2 init has not been
 *                               called is NULL.
 *********************************************************/
uint32_t q2_full(q2_context_t* const ctx, bool* const full);

/**********************************************************
 * Name:
 *    q2_length
 *
 * Description:
 *    Returns the current length of the queue.
 *
 * Parameters:
 *    q2_context_t* const ctx - Pointer to the q2 context.
 *    bool* const length - Current length of queue.
 *
 * Returns:
 *    Q2_SUCCESS - Successfully retrieved length.
 *    Q2_ERROR_NULL_PARAMETER - When ctx is NULL.
 *    Q2_ERROR_NOT_INITIALIZED - When q2 init has not been
 *                               called is NULL.
 *********************************************************/
uint32_t q2_length(q2_context_t* const ctx, uint32_t* const length);

/**********************************************************
 * Name:
 *    q2_reset
 *
 * Description:
 *    Resets the queue to the initial state.
 *
 * Parameters:
 *    q2_context_t* const ctx - Pointer to the q2 context.
 *
 * Returns:
 *    Q2_SUCCESS - Successfully reset queue.
 *    Q2_ERROR_NULL_PARAMETER - When ctx is NULL.
 *    Q2_ERROR_NOT_INITIALIZED - When q2 init has not been
 *                               called is NULL.
 *********************************************************/
uint32_t q2_reset(q2_context_t* const ctx);

#endif // Q2_H
