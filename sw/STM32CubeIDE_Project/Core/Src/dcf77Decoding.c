/*
 * dcf77Decoding.c
 *
 *  Created on: Sep 9, 2024
 *      Author: Dzale
 */

#include "dcf77Decoding.h"

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


