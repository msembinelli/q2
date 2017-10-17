/**********************************************************
 * Name:
 *     q2_tests.c
 *
 * Description:
 *     Unity tests for power of two queue.
 *
 *********************************************************/
/**********************************************************
 * Includes
 *********************************************************/
#include "unity.h"
#include "q2.h"
#include <stdio.h>

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
Q2(q2_ctx3, uint8_t, 256);

// Invalid size initializer (not power of two)
Q2(q2_ctx4, uint32_t, 3);

/**********************************************************
 * Procedures
 *********************************************************/
void test_q2_init_should_InitializeContext(void)
{
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx1), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx2), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx3), Q2_SUCCESS);
}

void test_q2_init_should_NotInitializeContext(void)
{
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx4), Q2_ERROR_LENGTH_NOT_POWER_OF_TWO);
    TEST_ASSERT_NOT_EQUAL(q2_init(NULL), Q2_SUCCESS);
}

void test_q2_should_FillAndEmptyCustomStruct(void)
{
    custom_struct_t input = {
        .var1 = 0xF1234567,
        .arr = { 0, 1, 2, 3 },
        .var2 = 0xBEEF,
        .var3 = 0xFF
    };
    custom_struct_t output;
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx1), Q2_SUCCESS);

    /* Fill up the queue and get length */
    uint32_t length;
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx1, &input), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_length(&q2_ctx1, &length), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(1, length);

    /* Test that queue is not full after adding 1 entry */
    bool full;
    TEST_ASSERT_EQUAL(q2_full(&q2_ctx1, &full), Q2_SUCCESS);
    TEST_ASSERT_FALSE(full);

    /* Get length when the queue is full */
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx1, &input), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_length(&q2_ctx1, &length), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(2, length);

    /* Should now be full */
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx1, &input), Q2_ERROR_FULL);
    TEST_ASSERT_EQUAL(q2_full(&q2_ctx1, &full), Q2_SUCCESS);
    TEST_ASSERT_TRUE(full);

    /* Check outputs */
    bool empty;
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx1, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL_MEMORY(&input, &output, sizeof(custom_struct_t));

    /* Test that it is not empty yet */
    TEST_ASSERT_EQUAL(q2_empty(&q2_ctx1, &empty), Q2_SUCCESS);
    TEST_ASSERT_FALSE(empty);

    /* Get last item */
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx1, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL_MEMORY(&input, &output, sizeof(custom_struct_t));

    /* Should now be empty */
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx1, &output), Q2_ERROR_EMPTY);
    TEST_ASSERT_EQUAL(q2_empty(&q2_ctx1, &empty), Q2_SUCCESS);
    TEST_ASSERT_TRUE(empty);
}

void test_q2_should_FillAndEmptyUint32(void)
{
    uint32_t input = 0x12345678;
    uint32_t output;

    TEST_ASSERT_EQUAL(q2_init(&q2_ctx2), Q2_SUCCESS);
    /* Fill up the queue */
    uint32_t length;
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input), Q2_SUCCESS);

    /* Get length */
    TEST_ASSERT_EQUAL(q2_length(&q2_ctx2, &length), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(3, length);

    /* Test that queue is not full after adding 3 entries */
    bool full;
    TEST_ASSERT_EQUAL(q2_full(&q2_ctx2, &full), Q2_SUCCESS);
    TEST_ASSERT_FALSE(full);

    /* Put last item */
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input), Q2_SUCCESS);

    /* Should now be full */
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx2, &input), Q2_ERROR_FULL);
    TEST_ASSERT_EQUAL(q2_full(&q2_ctx2, &full), Q2_SUCCESS);
    TEST_ASSERT_TRUE(full);

    /* Check output */
    bool empty;
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx2, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(input, output);
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx2, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(input, output);

    /* Test that it is not empty yet */
    TEST_ASSERT_EQUAL(q2_empty(&q2_ctx2, &empty), Q2_SUCCESS);
    TEST_ASSERT_FALSE(empty);

    /* Get the rest of the items */
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx2, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(input, output);
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx2, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(input, output);

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
    TEST_ASSERT_EQUAL(q2_init(&q2_ctx3), Q2_SUCCESS);

    for(i = 0; i < 256; i++)
    {
        TEST_ASSERT_EQUAL(q2_put(&q2_ctx3, &input), Q2_SUCCESS);
        input++;
    }

    /* Should now be full */
    TEST_ASSERT_EQUAL(q2_put(&q2_ctx3, &input), Q2_ERROR_FULL);
    bool full;
    TEST_ASSERT_EQUAL(q2_full(&q2_ctx3, &full), Q2_SUCCESS);
    TEST_ASSERT_TRUE(full);

    /* Check output */
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx3, &output), Q2_SUCCESS);
    TEST_ASSERT_EQUAL(input, output);

    /* Reset */
    TEST_ASSERT_EQUAL(q2_reset(&q2_ctx3), Q2_SUCCESS);

    /* Should now be empty */
    TEST_ASSERT_EQUAL(q2_get(&q2_ctx3, &output), Q2_ERROR_EMPTY);
    bool empty;
    TEST_ASSERT_EQUAL(q2_empty(&q2_ctx3, &empty), Q2_SUCCESS);
    TEST_ASSERT_TRUE(empty);
}

void test_q2_init_should_NotPut(void)
{
    uint32_t input = 0x12345678;

    TEST_ASSERT_EQUAL(q2_init(&q2_ctx4), Q2_ERROR_LENGTH_NOT_POWER_OF_TWO);
    TEST_ASSERT_NOT_EQUAL(q2_put(&q2_ctx4, &input), Q2_SUCCESS);
    TEST_ASSERT_NOT_EQUAL(q2_put(NULL, &input), Q2_SUCCESS);
    TEST_ASSERT_NOT_EQUAL(q2_put(&q2_ctx2, NULL), Q2_SUCCESS);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_q2_init_should_InitializeContext);
    RUN_TEST(test_q2_init_should_NotInitializeContext);
    RUN_TEST(test_q2_should_FillAndEmptyCustomStruct);
    RUN_TEST(test_q2_should_FillAndEmptyUint32);
    RUN_TEST(test_q2_should_FillAndEmptyUint8);
    RUN_TEST(test_q2_init_should_NotPut);
    return UNITY_END();
}
