#include "unity.h"
#include "../App/Inc/dcf77Decoding.h"

//DCF77Buffer_t testBufferConsistent1 = {.DCF77bits = 0b0000000010010001001010010000101011000111001100100101110101000110}; //10.09.2024
DCF77Buffer_t testBufferConsistent1 = {.DCF77bits = 0b0000000010010001001010010000110001101000100100100100011001011110};
DCF77Buffer_t testBufferConsistent2 = {.DCF77bits = 0b0000000010010001001010010000101011001000010100100010110001111000}; //10.09.2024

void setUp(void)
{
}

void tearDown(void)
{
}

void test_CheckPulseType(void)
{
    DCF77_TimeSample_t zeroPulse = {.pulseLength = 120, .signalLength = 998};
    DCF77_TimeSample_t onePulse = {.pulseLength = 218, .signalLength = 1003};
    DCF77_TimeSample_t unknownPulse1 = {.pulseLength = 12, .signalLength = 22};
    DCF77_TimeSample_t unknownPulse2 = {.pulseLength = 330, .signalLength = 500};
    DCF77_TimeSample_t minutemarkPulse1 = {.pulseLength = 100, .signalLength = 1900};

    TEST_ASSERT_EQUAL(ZERO_PULSE, DCF77_CheckPulseType(&zeroPulse));
    TEST_ASSERT_EQUAL(ONE_PULSE, DCF77_CheckPulseType(&onePulse));
    TEST_ASSERT_EQUAL(UNKNOWN_PULSE, DCF77_CheckPulseType(&unknownPulse1));
    TEST_ASSERT_EQUAL(UNKNOWN_PULSE, DCF77_CheckPulseType(&unknownPulse2));
    TEST_ASSERT_EQUAL(MINUTE_PULSE, DCF77_CheckPulseType(&minutemarkPulse1));
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

void test_IsStartOfEncodingOne(void)
{
    DCF77Buffer_t startOfEncodingIsZero = {.DCF77bits = 0xFFFFFFFFFFFFFFFF};
    DCF77Buffer_t startOfEncodingIsOne = {.DCF77bits = 0};

    // Bit #20 is Encoding bit, always 1
    startOfEncodingIsZero.DCF77bits = 0 << 20;
    startOfEncodingIsOne.DCF77bits = 1 << 20;

    TEST_ASSERT_FALSE( DCF77_IsStartOfEncodingOne(&startOfEncodingIsZero) );
    TEST_ASSERT_TRUE( DCF77_IsStartOfEncodingOne(&startOfEncodingIsOne) );
}

void test_IsMinuteParityOk(void)
{
    DCF77Buffer_t minuteParityOk = testBufferConsistent1;
    DCF77Buffer_t minuteParityNotOk = { .DCF77bits = testBufferConsistent2.DCF77bits ^ (1UL << 28) }; 

    TEST_ASSERT_FALSE( DCF77_IsMinuteParityOk(&minuteParityNotOk) );
    TEST_ASSERT_TRUE( DCF77_IsMinuteParityOk(&minuteParityOk) );
}

void test_IsHourParityOk(void)
{
    DCF77Buffer_t hourParityOk = testBufferConsistent1;
    DCF77Buffer_t hourParityNotOk = { .DCF77bits = testBufferConsistent2.DCF77bits ^ (1UL << 35) };

    TEST_ASSERT_FALSE( DCF77_IsHourParityOk(&hourParityNotOk) );
    TEST_ASSERT_TRUE( DCF77_IsHourParityOk(&hourParityOk) );
}

void test_IsDateParityOk(void)
{
    DCF77Buffer_t hourParityOk1 = testBufferConsistent1;
    DCF77Buffer_t hourParityOk2 = testBufferConsistent2;
    DCF77Buffer_t hourParityNotOk = { .DCF77bits = testBufferConsistent1.DCF77bits ^ (1UL << 58) };

    TEST_ASSERT_FALSE( DCF77_IsDateParityOk(&hourParityNotOk) );
    TEST_ASSERT_TRUE( DCF77_IsDateParityOk(&hourParityOk1) );
    TEST_ASSERT_TRUE( DCF77_IsDateParityOk(&hourParityOk2) );
}

DCF77Buffer_t sampleWith23h44m10_9_24 = {.DCF77bits = 0b0000000010010001001010010000110001101000100100100100011001011110};

void test_DecodeTime(void)
{
    TEST_ASSERT_EQUAL(INTEGRITY_OK, DCF77_CheckBufferIntegrity(&sampleWith23h44m10_9_24) );
    CopyOf_RTC_TimeTypeDef time23h44m = {.Hours=11, .Minutes = 44, .TimeFormat = RTC_HOURFORMAT12_PM};

    CopyOf_RTC_TimeTypeDef returnedTime = {0};
    DCF77_DecodeTimeToRTCTimeBuffer(&sampleWith23h44m10_9_24, &returnedTime);

    TEST_ASSERT_EQUAL(time23h44m.Minutes, returnedTime.Minutes);
    TEST_ASSERT_EQUAL(time23h44m.Hours, returnedTime.Hours);
    TEST_ASSERT_EQUAL_MEMORY(&time23h44m, &returnedTime, sizeof(CopyOf_RTC_TimeTypeDef));
}

void test_DecodeDate(void)
{
    TEST_ASSERT_EQUAL(INTEGRITY_OK, DCF77_CheckBufferIntegrity(&sampleWith23h44m10_9_24) );
    CopyOf_RTC_DateTypeDef date10_09_24 = {.Date = 10, .Month = 9, .Year = 24, .WeekDay = 2};

    CopyOf_RTC_DateTypeDef returnedDate = {0};
    DCF77_DecodeDateToRTCDateBuffer(&sampleWith23h44m10_9_24, &returnedDate);
    TEST_ASSERT_EQUAL_MEMORY(&date10_09_24, &returnedDate, sizeof(CopyOf_RTC_DateTypeDef));
}

// DCF77 hour/minute fields are BCD: units in bits [3:0], tens in bits [7:4] (each *10)
DCF77Buffer_t sampleWithMidnight00h00m = {.DCF77Buffer_s = { .Hour = 0x00, .Min = 0x00 } }; // 00h00
DCF77Buffer_t sampleWithNoon12h00m     = {.DCF77Buffer_s = { .Hour = 0x12, .Min = 0x00 } }; // 12h00

void test_DecodeTime_MidnightIsTwelveAM(void)
{
    CopyOf_RTC_TimeTypeDef expectedMidnight = {.Hours = 12, .Minutes = 0, .TimeFormat = RTC_HOURFORMAT12_AM};

    CopyOf_RTC_TimeTypeDef returnedTime = {0};
    DCF77_DecodeTimeToRTCTimeBuffer(&sampleWithMidnight00h00m, &returnedTime);

    TEST_ASSERT_EQUAL(expectedMidnight.Hours, returnedTime.Hours);
    TEST_ASSERT_EQUAL(expectedMidnight.TimeFormat, returnedTime.TimeFormat);
}

void test_DecodeTime_NoonIsTwelvePM(void)
{
    CopyOf_RTC_TimeTypeDef expectedNoon = {.Hours = 12, .Minutes = 0, .TimeFormat = RTC_HOURFORMAT12_PM};

    CopyOf_RTC_TimeTypeDef returnedTime = {0};
    DCF77_DecodeTimeToRTCTimeBuffer(&sampleWithNoon12h00m, &returnedTime);

    TEST_ASSERT_EQUAL(expectedNoon.Hours, returnedTime.Hours);
    TEST_ASSERT_EQUAL(expectedNoon.TimeFormat, returnedTime.TimeFormat);
}

//00010011000111110011000101100100000011100100000000000000000 - 16h 03m 18.07.26
DCF77Buffer_t sampleWith19h63m18_7_26 = {.DCF77bits = 0b0000000010011000111110011000101100101100011100100000000000000000}; // Impossible minutes
DCF77Buffer_t sampleWith29h03m18_7_26 = {.DCF77bits = 0b0000000010011000111110011000110100100000011100100000000000000000}; // Impossibe hour
DCF77Buffer_t sampleWith16h03m32_7_26 = {.DCF77bits = 0b0000010010011000111110110010101011000000011100100000000000000000}; // Impossible day
DCF77Buffer_t sampleWith16h03m18_13_26 = {.DCF77bits = 0b0000000010011010011110011000101011000000011100100000000000000000}; // Impossible month


void test_ImpossibleData(void)
{
    TEST_ASSERT_EQUAL(IMPOSSIBLE_DATA, DCF77_CheckBufferIntegrity(&sampleWith19h63m18_7_26) );
    TEST_ASSERT_EQUAL(IMPOSSIBLE_DATA, DCF77_CheckBufferIntegrity(&sampleWith29h03m18_7_26) );
    TEST_ASSERT_EQUAL(IMPOSSIBLE_DATA, DCF77_CheckBufferIntegrity(&sampleWith16h03m32_7_26) );
    TEST_ASSERT_EQUAL(IMPOSSIBLE_DATA, DCF77_CheckBufferIntegrity(&sampleWith16h03m18_13_26) );
}