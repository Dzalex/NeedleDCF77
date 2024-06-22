/*
 * dcf77.h
 *
 *  Created on: Jan 7, 2024
 *      Author: Dzale
 */

#ifndef INC_DCF77_H_
#define INC_DCF77_H_

#include "main.h"
#include "cmsis_os2.h"
#include "stdbool.h"

extern osMessageQueueId_t DCF77_TimeSamplesQueueHandle;

enum BufferErrors {INTEGRITY_OK = 0, FIRST_BIT_NOT_ONE,  START_OF_ENC_NOT_ONE, WRONG_MIN_PARITY, WRONG_HOUR_PARITY, WRONG_DATE_PARITY};

typedef struct DCF77_TimeSample
{
	uint16_t signalLength;
	uint16_t pulseLength;
} DCF77_TimeSample_t;

typedef union {
	struct {
		 uint64_t startOfMinute		:1;
		 uint64_t prefix     		:16;
		 uint64_t CEST       		:1;
		 uint64_t CET        		:1;
		 uint64_t leapSecond 		:1;
		 uint64_t startEncoding		:1;
		 uint64_t Min        		:7;
		 uint64_t P1         		:1;
		 uint64_t Hour       		:6;
		 uint64_t P2         		:1;
		 uint64_t Day        		:6;
		 uint64_t Weekday    		:3;
		 uint64_t Month      		:5;
		 uint64_t Year       		:8;
		 uint64_t P3         		:1;
    } DCF77Buffer_s;
	uint64_t DCF77bits;
} DCF77Buffer_t;

void DCF77_Initialize(void);
void DCF77_DeInitialize(void);
ErrorStatus DCF77_GetTimeAndDate(RTC_TimeTypeDef* timeBuffer, RTC_DateTypeDef* dateBuffer);

enum BufferErrors DCF77_CheckBufferIntegrity(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsFirstBitOne(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsStartOfEncodingOne(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsMinuteParityOk(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsHourParityOk(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsDateParityOk(DCF77Buffer_t* DCF77Buffer);

#endif /* INC_DCF77_H_ */
