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

enum BufferErrors {INTEGRITY_OK = 0, FIRST_BIT_NOT_ZERO,  START_OF_ENC_NOT_ONE, WRONG_MIN_PARITY, WRONG_HOUR_PARITY, WRONG_DATE_PARITY};

enum BufferErrors DCF77_CheckBufferIntegrity(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsFirstBitZero(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsStartOfEncodingOne(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsMinuteParityOk(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsHourParityOk(DCF77Buffer_t* DCF77Buffer);
bool DCF77_IsDateParityOk(DCF77Buffer_t* DCF77Buffer);

#endif /* INC_DCF77DECODING_H_ */
