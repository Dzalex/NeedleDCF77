/*
 * dcf77Decoding.h
 *
 *  Created on: Sep 9, 2024
 *      Author: Dzale
 */

#ifndef INC_DCF77DECODING_H_
#define INC_DCF77DECODING_H_

#include <stdint.h>
#include "stdbool.h"

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

#define RTC_HOURFORMAT12_AM            ((uint8_t)0x00)
#define RTC_HOURFORMAT12_PM            ((uint8_t)0x01)

/**
  * @brief  This is ugly, but this is only way to not include HAL file
  * which will make tests impossible to build
  */
typedef struct
{
  uint8_t Hours;
  uint8_t Minutes;
  uint8_t Seconds;
  uint8_t TimeFormat;
  uint32_t SubSeconds;
  uint32_t SecondFraction;
  uint32_t DayLightSaving;
  uint32_t StoreOperation;
} CopyOf_RTC_TimeTypeDef;

/**
  * @brief  RTC Date structure definition, ugly as above
  */
typedef struct
{
  uint8_t WeekDay;
  uint8_t Month;
  uint8_t Date;
  uint8_t Year;
} CopyOf_RTC_DateTypeDef;

enum PulseType {ZERO_PULSE = 0, ONE_PULSE = 1, MINUTE_PULSE, UNKNOWN_PULSE};
enum BufferErrors {INTEGRITY_OK = 0, FIRST_BIT_NOT_ZERO,  START_OF_ENC_NOT_ONE, WRONG_MIN_PARITY, WRONG_HOUR_PARITY, WRONG_DATE_PARITY};

enum PulseType DCF77_CheckPulseType(DCF77_TimeSample_t* sampleToCheck);
enum BufferErrors DCF77_CheckBufferIntegrity(DCF77Buffer_t* DCF77Buffer);
void DCF77_SetBufferBitOnPosition(DCF77Buffer_t* DCF77Buffer, uint8_t currentBit);
bool DCF77_IsFirstBitZero(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsStartOfEncodingOne(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsMinuteParityOk(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsHourParityOk(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsDateParityOk(DCF77Buffer_t* DCF77Buffer);
void DCF77_DecodeTimeToRTCTimeBuffer(DCF77Buffer_t* DCF77Buffer, CopyOf_RTC_TimeTypeDef* timeBuffer);
void DCF77_DecodeDateToRTCDateBuffer(DCF77Buffer_t* DCF77Buffer, CopyOf_RTC_DateTypeDef* dateBuffer);

#endif /* INC_DCF77DECODING_H_ */
