#include "unity.h"
#include "../Core/Inc/dcf77Decoding.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_IsFirstBitOne(void)
{
    DCF77Buffer_t FirstBitIsZero = {.DCF77bits = 0xFFFFFFFFFFFFFFFF};
    DCF77Buffer_t FirstBitIsOne = {0};

    FirstBitIsZero.DCF77bits = 0xFFFFFFFFFFFFFFFE;
    FirstBitIsOne.DCF77bits = 0x0000000000000001;

    TEST_ASSERT_FALSE( DCF77_IsFirstBitZero(&FirstBitIsOne) );
    TEST_ASSERT_TRUE( DCF77_IsFirstBitZero(&FirstBitIsZero) );
}

