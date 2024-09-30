/*
 * dcf77Decoding.c
 *
 *  Created on: Sep 9, 2024
 *      Author: Dzale
 */

#include "dcf77Decoding.h"

/* Constants related to DCF77 protocol */
/* Min and max values from da6180B.pdf */
const uint16_t ZERO_PULS_DURATION_MAX = 130;
const uint16_t ZERO_PULS_DURATION_MIN = 40;
const uint16_t ONE_PULS_DURATION_MAX = 250;
const uint16_t ONE_PULS_DURATION_MIN = 140;

const uint16_t MINUTE_MARK_PULS_DURATION_MAX = 2100;
const uint16_t MINUTE_MARK_PULS_DURATION_MIN = 1600;

enum PulseType DCF77_CheckPulseType(DCF77_TimeSample_t* sampleToCheck)
{
	if(		sampleToCheck->pulseLength > ZERO_PULS_DURATION_MIN && \
			sampleToCheck->pulseLength < ZERO_PULS_DURATION_MAX)
	{
		/*
		 * This case is for Minute mark - it is always 0
		 *  ͞ |______| ͞|___| ͞ ͞|_....
		 *  	 |   |
		 *  	 |  First bit with value 0
		 *  Minute pause
		 */
		if( sampleToCheck->signalLength > MINUTE_MARK_PULS_DURATION_MIN && \
			sampleToCheck->signalLength < MINUTE_MARK_PULS_DURATION_MAX)
		{
			return MINUTE_PULSE;
		}
		return ZERO_PULSE;
	}
	else if(sampleToCheck->pulseLength > ONE_PULS_DURATION_MIN && \
			sampleToCheck->pulseLength < ONE_PULS_DURATION_MAX)
	{
		return ONE_PULSE;
	}

	return UNKNOWN_PULSE;
}

enum BufferErrors DCF77_CheckBufferIntegrity(DCF77Buffer_t* DCF77Buffer)
{
	if(false == DCF77_IsFirstBitZero(DCF77Buffer))
	{
		return FIRST_BIT_NOT_ZERO;
	}
	if(false == DCF77_IsStartOfEncodingOne(DCF77Buffer))
	{
		return START_OF_ENC_NOT_ONE;
	}
	if(false == DCF77_IsMinuteParityOk(DCF77Buffer))
	{
		return WRONG_MIN_PARITY;
	}
	if(false == DCF77_IsHourParityOk(DCF77Buffer))
	{
		return WRONG_HOUR_PARITY;
	}
	if(false == DCF77_IsDateParityOk(DCF77Buffer))
	{
		return WRONG_DATE_PARITY;
	}

	return INTEGRITY_OK;
}

void DCF77_SetBufferBitOnPosition(DCF77Buffer_t* DCF77Buffer, uint8_t currentBit)
{
	DCF77Buffer->DCF77bits = DCF77Buffer->DCF77bits | (1ULL << currentBit);
}

bool DCF77_IsFirstBitZero(DCF77Buffer_t* DCF77Buffer)
{
	return 0 == DCF77Buffer->DCF77Buffer_s.startOfMinute;
}
bool DCF77_IsStartOfEncodingOne(DCF77Buffer_t* DCF77Buffer)
{
	return DCF77Buffer->DCF77Buffer_s.startEncoding;
}
bool DCF77_IsMinuteParityOk(DCF77Buffer_t* DCF77Buffer)
{
	return DCF77Buffer->DCF77Buffer_s.P1 == __builtin_parity(DCF77Buffer->DCF77Buffer_s.Min);
}
bool DCF77_IsHourParityOk(DCF77Buffer_t* DCF77Buffer)
{
	return DCF77Buffer->DCF77Buffer_s.P2 == __builtin_parity(DCF77Buffer->DCF77Buffer_s.Hour);
}
bool DCF77_IsDateParityOk(DCF77Buffer_t* DCF77Buffer)
{
	int paritySum = __builtin_parity(DCF77Buffer->DCF77Buffer_s.Day);
	paritySum ^= 	__builtin_parity(DCF77Buffer->DCF77Buffer_s.Weekday);
	paritySum ^=	__builtin_parity(DCF77Buffer->DCF77Buffer_s.Month);
	paritySum ^=	__builtin_parity(DCF77Buffer->DCF77Buffer_s.Year);
	return DCF77Buffer->DCF77Buffer_s.P3 == paritySum;
}

void DCF77_DecodeTimeToRTCTimeBuffer(DCF77Buffer_t* DCF77Buffer, CopyOf_RTC_TimeTypeDef* timeBuffer)
{
	timeBuffer -> Minutes = ( DCF77Buffer -> DCF77Buffer_s.Min & 0xF ) + \
							( ( (DCF77Buffer -> DCF77Buffer_s.Min) & (1ULL << 4) ) >> 4 ) * 10 +\
							( ( (DCF77Buffer -> DCF77Buffer_s.Min) & (1ULL << 5) ) >> 5 ) * 20 +\
							( ( (DCF77Buffer -> DCF77Buffer_s.Min) & (1ULL << 6) ) >> 6 ) * 40;

	timeBuffer -> Hours =	( DCF77Buffer -> DCF77Buffer_s.Hour & 0xF ) + \
							( ( (DCF77Buffer -> DCF77Buffer_s.Hour) & (1ULL << 4) )  >> 4 ) * 10 + \
							( ( (DCF77Buffer -> DCF77Buffer_s.Hour) & (1ULL << 5) )  >> 5 ) * 20;

	if(12 < timeBuffer -> Hours)
	{
		timeBuffer -> Hours -= 12;
		timeBuffer -> TimeFormat = RTC_HOURFORMAT12_PM;
	}
	else
	{
		timeBuffer -> TimeFormat = RTC_HOURFORMAT12_AM;
	}

	timeBuffer -> Seconds = 0;
}

void DCF77_DecodeDateToRTCDateBuffer(DCF77Buffer_t* DCF77Buffer, CopyOf_RTC_DateTypeDef* dateBuffer)
{
	dateBuffer -> Date = 	( DCF77Buffer -> DCF77Buffer_s.Day & 0xF ) + \
							( ( (DCF77Buffer -> DCF77Buffer_s.Day) & (1ULL << 4) ) >> 4 ) * 10 +\
							( ( (DCF77Buffer -> DCF77Buffer_s.Day) & (1ULL << 5) ) >> 5 ) * 20;

	dateBuffer -> WeekDay = DCF77Buffer -> DCF77Buffer_s.Weekday & 0x7;

	dateBuffer -> Month =	( DCF77Buffer -> DCF77Buffer_s.Month & 0xF ) + \
							( ( (DCF77Buffer -> DCF77Buffer_s.Month) & (1ULL << 4) ) >> 4 ) * 10;

	dateBuffer -> Year =	( DCF77Buffer -> DCF77Buffer_s.Year & 0XF ) + \
							( ( (DCF77Buffer -> DCF77Buffer_s.Year) & (1ULL << 4) ) >> 4 ) * 10 +\
							( ( (DCF77Buffer -> DCF77Buffer_s.Year) & (1ULL << 5) ) >> 5 ) * 20 +\
							( ( (DCF77Buffer -> DCF77Buffer_s.Year) & (1ULL << 6) ) >> 6 ) * 40 +\
							( ( (DCF77Buffer -> DCF77Buffer_s.Year) & (1ULL << 7) ) >> 7 ) * 80;
}
