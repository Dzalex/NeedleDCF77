/*
 * dcf77Decoding.c
 *
 *  Created on: Sep 9, 2024
 *      Author: Dzale
 */

#include "dcf77Decoding.h"

enum BufferErrors DCF77_CheckBufferIntegrity(DCF77Buffer_t* DCF77Buffer)
{
	if(false == DCF77_IsFirstBitOne(DCF77Buffer))
	{
		return FIRST_BIT_NOT_ONE;
	}
	//if(false == DCF77_77....)
	return INTEGRITY_OK;
}

void DCF77_FillBufferOnPosition(DCF77Buffer_t* DCF77Buffer, uint8_t currentBit, uint32_t valueOnPosition)
{
	uint64_t mask = 1 << currentBit;
	DCF77Buffer->DCF77bits = (DCF77Buffer->DCF77bits & ~mask) | (valueOnPosition << currentBit);
}



bool DCF77_IsFirstBitOne(DCF77Buffer_t* DCF77Buffer)
{
	return DCF77Buffer->DCF77Buffer_s.startOfMinute;
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


