/**********************************************************
 * Name:
 *     q2_tests.c
 *
 * Description:
 *     Unity tests for power of two queue.
 *
 * Copyright (c) 2017 Matthew Sembinelli
 *********************************************************/
/**********************************************************
 * Includes
 *********************************************************/
#include "unity.h"
#include "q2.h"
#include <stdio.h>
#include <string.h>

/**********************************************************
 * Defines
 *********************************************************/

/**********************************************************
 * Types
 *********************************************************/
 typedef struct
 {
     uint32_t var1;
     uint8_t  arr[4];
     uint16_t var2;
     uint8_t  var3;
 } custom_struct_t;

/**********************************************************
 * Macros
 *********************************************************/
Q2(q2_ctx1, custom_struct_t, 2);
Q2(q2_ctx2, uint32_t, 4);
Q2(q2_ctx3, uint8_t, 32);

// Invalid size initializer (not power of two)
Q2(q2_ctx4, uint32_t, 3);
Q2(q2_ctx5, uint32_t, 0);

/**********************************************************
 * Procedures
 *********************************************************/
void test_helper_q2_context_clear(q2_context_t* const ctx)
{
    memset(ctx->data, 0x00, ctx->item_length * ctx->max_length);
    ctx->initialized = false;
    ctx->full = false;
    ctx->empty = true;
    ctx->head = 0;
    ctx->tail = 0;
}

void setUp(void)
{
    test_helper_q2_context_clear(&q2_ctx1);
    test_helper_q2_context_clear(&q2_ctx2);
    test_helper_q2_context_clear(&q2_ctx3);
    test_helper_q2_context_clear(&q2_ctx4);
    test_helper_q2_context_clear(&q2_ctx5);
}

void test_q2_init_should_InitializeContext(void)
{
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx1), Q2_SUCCESS);
}

void test_q2_init_should_NotInitializeContext(void)
{
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx4), Q2_ERROR_LENGTH_NOT_POWER_OF_TWO);
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx5), Q2_ERROR_LENGTH_NOT_POWER_OF_TWO);
    TEST_ASSERT_EQUAL(q2_init(NULL), Q2_ERROR_NULL_PARAMETER);
}

void test_q2_reset_should_Reset(void)
{
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx1), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_reset(&q2_ctx1), Q2_SUCCESS);
}

void test_q2_reset_should_NotReset(void)
{
    TEST_ASSERT_EQUAL(q2_reset(&q2_ctx1), Q2_ERROR_NOT_INITIALIZED);
    TEST_ASSERT_EQUAL(q2_reset(NULL), Q2_ERROR_NULL_PARAMETER);
}

void test_q2_put_should_NotPut(void)
{
    uint32_t input = 0x12345678;
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input), Q2_ERROR_NOT_INITIALIZED);
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx2), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_put(NULL, &input), Q2_ERROR_NULL_PARAMETER);
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, NULL), Q2_ERROR_NULL_PARAMETER);
}

void test_q2_get_should_NotGet(void)
{
    uint32_t output;
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx2, &output), Q2_ERROR_NOT_INITIALIZED);
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx2), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx2, NULL), Q2_ERROR_NULL_PARAMETER);
    TEST_ASSERT_EQUAL(q2_get(NULL, &output), Q2_ERROR_NULL_PARAMETER);
}

void test_q2_put_should_PutUntilFull(void)
{
    uint32_t input = 0x12345678;
    bool full;
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx2), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input), Q2_ERROR_FULL);
    TEST_ASSERT_EQUAL(q2_full(&q2_ctx2, &full), Q2_SUCCESS);
    TEST_ASSERT_TRUE(full);
}

void test_q2_get_should_BeEmpty(void)
{
    uint32_t output;
    bool empty;
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx2), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx2, &output), Q2_ERROR_EMPTY);
    TEST_ASSERT_EQUAL(q2_empty(&q2_ctx2, &empty), Q2_SUCCESS);
    TEST_ASSERT_TRUE(empty);
}

void test_q2_empty_should_NotGetEmpty(void)
{
    bool empty;
    TEST_ASSERT_EQUAL(q2_empty(&q2_ctx2, &empty), Q2_ERROR_NOT_INITIALIZED);
    TEST_ASSERT_EQUAL(q2_empty(NULL, &empty), Q2_ERROR_NULL_PARAMETER);
    TEST_ASSERT_EQUAL(q2_empty(&q2_ctx2, NULL), Q2_ERROR_NULL_PARAMETER);
}

void test_q2_full_should_NotGetFull(void)
{
    bool full;
    TEST_ASSERT_EQUAL(q2_full(&q2_ctx2, &full), Q2_ERROR_NOT_INITIALIZED);
    TEST_ASSERT_EQUAL(q2_full(NULL, &full), Q2_ERROR_NULL_PARAMETER);
    TEST_ASSERT_EQUAL(q2_full(&q2_ctx2, NULL), Q2_ERROR_NULL_PARAMETER);
}

void test_q2_length_should_NotGetLength(void)
{
    uint32_t length;
    TEST_ASSERT_EQUAL(q2_length(&q2_ctx2, &length), Q2_ERROR_NOT_INITIALIZED);
    TEST_ASSERT_EQUAL(q2_length(NULL, &length), Q2_ERROR_NULL_PARAMETER);
    TEST_ASSERT_EQUAL(q2_length(&q2_ctx2, NULL), Q2_ERROR_NULL_PARAMETER);
}

void test_q2_should_FillAndEmptyCustomStruct(void)
{
    custom_struct_t input1 = {
        .var1 = 0x0,
        .arr = { 0, 1, 2, 3 },
        .var2 = 0xBEEF,
        .var3 = 0xFF
    };

    custom_struct_t input2 = {
        .var1 = 0x12345678,
        .arr = { 0, 1, 2, 3 },
        .var2 = 0xBEEF,
        .var3 = 0xFF
    };
    custom_struct_t output;
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx1), Q2_SUCCESS);

    /* Fill up the queue and get length */
    uint32_t length;
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx1, &input1), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_length(&q2_ctx1, &length), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(1, length);

    /* Test that queue is not full after adding 1 entry */
    bool full;
    TEST_ASSERT_EQUAL(q2_full(&q2_ctx1, &full), Q2_SUCCESS);
    TEST_ASSERT_FALSE(full);

    /* Get length when the queue is full */
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx1, &input2), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_length(&q2_ctx1, &length), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(2, length);

    /* Should now be full */
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx1, &input1), Q2_ERROR_FULL);
    TEST_ASSERT_EQUAL(q2_full(&q2_ctx1, &full), Q2_SUCCESS);
    TEST_ASSERT_TRUE(full);

    /* Check outputs */
    bool empty;
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx1, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL_MEMORY(&input1, &output, sizeof(custom_struct_t));

    /* Test that it is not empty yet */
    TEST_ASSERT_EQUAL(q2_empty(&q2_ctx1, &empty), Q2_SUCCESS);
    TEST_ASSERT_FALSE(empty);

    /* Get last item */
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx1, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL_MEMORY(&input2, &output, sizeof(custom_struct_t));

    /* Should now be empty */
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx1, &output), Q2_ERROR_EMPTY);
    TEST_ASSERT_EQUAL(q2_empty(&q2_ctx1, &empty), Q2_SUCCESS);
    TEST_ASSERT_TRUE(empty);
}

void test_q2_should_FillAndEmptyUint32(void)
{
    uint32_t input1 = 0xAAAAAAAA;
    uint32_t input2 = 0xBBBBBBBB;
    uint32_t input3 = 0xCCCCCCCC;
    uint32_t input4 = 0xDDDDDDDD;
    uint32_t input5 = 0xEEEEEEEE;
    uint32_t output;

    TEST_ASSERT_EQUAL(q2_init(&q2_ctx2), Q2_SUCCESS);
    /* Fill up the queue */
    uint32_t length;
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input1), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input2), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input3), Q2_SUCCESS);

    /* Get length */
    TEST_ASSERT_EQUAL(q2_length(&q2_ctx2, &length), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(3, length);

    /* Test that queue is not full after adding 3 entries */
    bool full;
    TEST_ASSERT_EQUAL(q2_full(&q2_ctx2, &full), Q2_SUCCESS);
    TEST_ASSERT_FALSE(full);

    /* Put last item */
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input4), Q2_SUCCESS);

    /* Should now be full */
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input5), Q2_ERROR_FULL);
    TEST_ASSERT_EQUAL(q2_full(&q2_ctx2, &full), Q2_SUCCESS);
    TEST_ASSERT_TRUE(full);

    /* Check output */
    bool empty;
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx2, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(input1, output);
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx2, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(input2, output);

    /* Test that it is not empty yet */
    TEST_ASSERT_EQUAL(q2_empty(&q2_ctx2, &empty), Q2_SUCCESS);
    TEST_ASSERT_FALSE(empty);

    /* Get the rest of the items */
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx2, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(input3, output);
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx2, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(input4, output);

    /* Should now be empty */
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx2, &output), Q2_ERROR_EMPTY);
    TEST_ASSERT_EQUAL(q2_empty(&q2_ctx2, &empty), Q2_SUCCESS);
    TEST_ASSERT_TRUE(empty);
}

void test_q2_should_FillAndEmptyUint8(void)
{
    uint8_t input = 0;
    uint8_t output;
    uint32_t i = 0;
    uint32_t length;
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx3), Q2_SUCCESS);

    for(i = 0; i < 32; i++)
    {
        TEST_ASSERT_EQUAL(q2_put(&q2_ctx3, &input), Q2_SUCCESS);
        input++;
    }

    /* Should now be full */
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx3, &input), Q2_ERROR_FULL);
    bool full;
    TEST_ASSERT_EQUAL(q2_full(&q2_ctx3, &full), Q2_SUCCESS);
    TEST_ASSERT_TRUE(full);

    /* Get length */
    TEST_ASSERT_EQUAL(q2_length(&q2_ctx3, &length), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(32, length);

    /* Check first output */
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx3, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(0, output); // First item put was a 0

    /* Reset */
    TEST_ASSERT_EQUAL(q2_reset(&q2_ctx3), Q2_SUCCESS);

    /* Get length */
    TEST_ASSERT_EQUAL(q2_length(&q2_ctx3, &length), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(0, length);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_q2_init_should_InitializeContext);
    RUN_TEST(test_q2_init_should_NotInitializeContext);
    RUN_TEST(test_q2_reset_should_Reset);
    RUN_TEST(test_q2_reset_should_NotReset);
    RUN_TEST(test_q2_put_should_NotPut);
    RUN_TEST(test_q2_get_should_NotGet);
    RUN_TEST(test_q2_put_should_PutUntilFull);
    RUN_TEST(test_q2_get_should_BeEmpty);
    RUN_TEST(test_q2_empty_should_NotGetEmpty);
    RUN_TEST(test_q2_full_should_NotGetFull);
    RUN_TEST(test_q2_length_should_NotGetLength);
    RUN_TEST(test_q2_should_FillAndEmptyCustomStruct);
    RUN_TEST(test_q2_should_FillAndEmptyUint32);
    RUN_TEST(test_q2_should_FillAndEmptyUint8);
    return UNITY_END();
}
