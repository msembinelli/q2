/**********************************************************
 * Name:
 *     q2.c
 *
 * Description:
 *     Implementation for power of two queue.
 *
 *********************************************************/
/**********************************************************
 * Includes
 *********************************************************/
#include "q2.h"
#include <string.h>

/**********************************************************
 * Procedures
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
uint32_t q2_init(q2_context_t* const ctx)
{
    q2_return_t ret = Q2_SUCCESS;

    if(NULL == ctx)
    {
        ret = Q2_ERROR_NULL_PARAMETER;
    }

    if(Q2_SUCCESS == ret)
    {
        /* Check for power of two */
        if(!((ctx->max_length & (ctx->max_length - 1)) == 0) || !ctx->max_length)
        {
            ret = Q2_ERROR_LENGTH_NOT_POWER_OF_TWO;
        }
        else
        {
            ctx->initialized = true;
        }
    }

    return ret;
}

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
uint32_t q2_put(q2_context_t* const ctx, void* const input)
{
    q2_return_t ret = Q2_SUCCESS;

    if(NULL == ctx || NULL == input)
    {
        ret = Q2_ERROR_NULL_PARAMETER;
    }
    else if(false == ctx->initialized)
    {
        ret = Q2_ERROR_NOT_INITIALIZED;
    }

    if(Q2_SUCCESS == ret)
    {
        /* Check next location */
        if(((ctx->head + 1) & (ctx->max_length - 1)) == ctx->tail)
        {
            ctx->full = true;
        }
        else if(true == ctx->full)
        {
            ret = Q2_ERROR_FULL;
        }

        if(Q2_SUCCESS == ret)
        {
            ctx->empty = false;
            memcpy((uint8_t*)ctx->data + (ctx->head * ctx->item_length), input, ctx->item_length);
            ctx->head = ((ctx->head + 1) & (ctx->max_length - 1));
        }
    }

    return ret;
}

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
uint32_t q2_get(q2_context_t * const ctx, void* const output)
{
    q2_return_t ret = Q2_SUCCESS;

    if(NULL == ctx || NULL == output)
    {
        ret = Q2_ERROR_NULL_PARAMETER;
    }
    else if(false == ctx->initialized)
    {
        ret = Q2_ERROR_NOT_INITIALIZED;
    }

    if(Q2_SUCCESS == ret)
    {
        if (ctx->empty)
        {
            ret = Q2_ERROR_EMPTY;
        }
        else
        {
            ctx->full = false;
            memcpy(output, (uint8_t*)ctx->data + (ctx->tail * ctx->item_length), ctx->item_length);
            ctx->tail = ((ctx->tail + 1) & (ctx->max_length - 1));
            if (ctx->head == ctx->tail)
            {
                ctx->empty = true;
            }
        }
    }

    return ret;
}

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
uint32_t q2_empty(q2_context_t* const ctx, bool* const empty)
{
    q2_return_t ret = Q2_SUCCESS;

    if(NULL == ctx)
    {
        ret = Q2_ERROR_NULL_PARAMETER;
    }
    else if(false == ctx->initialized)
    {
        ret = Q2_ERROR_NOT_INITIALIZED;
    }

    if(Q2_SUCCESS == ret)
    {
        *empty = ctx->empty;
    }

    return ret;
}

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
uint32_t q2_full(q2_context_t* const ctx, bool* const full)
{
    q2_return_t ret = Q2_SUCCESS;

    if(NULL == ctx)
    {
        ret = Q2_ERROR_NULL_PARAMETER;
    }
    else if(false == ctx->initialized)
    {
        ret = Q2_ERROR_NOT_INITIALIZED;
    }

    if(Q2_SUCCESS == ret)
    {
        *full = ctx->full;
    }

    return ret;
}

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
uint32_t q2_length(q2_context_t* const ctx, uint32_t* const length)
{
    q2_return_t ret = Q2_SUCCESS;

    if(NULL == ctx)
    {
        ret = Q2_ERROR_NULL_PARAMETER;
    }
    else if(false == ctx->initialized)
    {
        ret = Q2_ERROR_NOT_INITIALIZED;
    }

    if(Q2_SUCCESS == ret)
    {
        if(true == ctx->full)
        {
        *length = ctx->max_length;
        }
        else
        {
        *length = (((ctx->head) - (ctx->tail)) & (ctx->max_length - 1));
        }
    }

    return ret;
}

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
uint32_t q2_reset(q2_context_t* const ctx)
{
    q2_return_t ret = Q2_SUCCESS;

    if(NULL == ctx)
    {
        ret = Q2_ERROR_NULL_PARAMETER;
    }
    else if(false == ctx->initialized)
    {
        ret = Q2_ERROR_NOT_INITIALIZED;
    }

    if(Q2_SUCCESS == ret)
    {
        ctx->head = 0;
        ctx->tail = 0;
        ctx->full = false;
        ctx->empty = true;
    }

    return ret;
}
