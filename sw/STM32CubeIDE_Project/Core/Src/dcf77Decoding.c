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
		// Special case on 58th pulse is that is always 0 and that minute mark is determined by signal Length
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
	DCF77Buffer->DCF77bits = DCF77Buffer->DCF77bits | (1UL << currentBit);
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
	int popCount = __builtin_popcount(DCF77Buffer->DCF77Buffer_s.Min);
	return !((popCount + DCF77Buffer->DCF77Buffer_s.P1) % 2);
}
bool DCF77_IsHourParityOk(DCF77Buffer_t* DCF77Buffer)
{
	int popCount = __builtin_popcount(DCF77Buffer->DCF77Buffer_s.Hour);
	return !((popCount + DCF77Buffer->DCF77Buffer_s.P2) % 2);
}
bool DCF77_IsDateParityOk(DCF77Buffer_t* DCF77Buffer)
{
	int popCount = 	__builtin_popcount(DCF77Buffer->DCF77Buffer_s.Day);
	popCount += 	__builtin_popcount(DCF77Buffer->DCF77Buffer_s.Weekday);
	popCount +=		__builtin_popcount(DCF77Buffer->DCF77Buffer_s.Month);
	popCount +=		__builtin_popcount(DCF77Buffer->DCF77Buffer_s.Year);
	return !((popCount + DCF77Buffer->DCF77Buffer_s.P3) % 2);
}


