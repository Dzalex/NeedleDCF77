/*
 * dcf77.c
 *
 *  Created on: Jan 23, 2024
 *      Author: Dzale
 */

#include "dcf77.h"
#include "main.h"
#include "tim.h"

/* Constants related to DCF77 protocol */
/* Min and max values from da6180B.pdf */
const uint16_t ZERO_PULS_DURATION_MAX = 130;
const uint16_t ZERO_PULS_DURATION_MIN = 40;
const uint16_t ONE_PULS_DURATION_MAX = 250;
const uint16_t ONE_PULS_DURATION_MIN = 140;

const uint16_t MINUTE_MARK_PULS_DURATION_MAX = 2100;
const uint16_t MINUTE_MARK_PULS_DURATION_MIN = 1600;

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

enum PulseType {ZERO_PULSE = 0, ONE_PULSE = 1, MINUTE_PULSE, UNKNOWN_PULSE};

/* Forward declaration of local functions */
static void DCF77_EnableReceiver(void);
static void DCF77_DisableReceiver(void);
static void DCF77_StartICTimers(void);
static void DCF77_StopICTimers(void);
enum PulseType DCF77_CheckPulseType(DCF77_TimeSample_t* sampleToCheck);
void DCF77_FillBufferOnPosition(DCF77Buffer_t* DCF77Buffer, uint8_t currentBit, uint32_t valueOnPosition);

void DCF77_Initialize(void)
{
	DCF77_EnableReceiver();
	DCF77_StartICTimers();
}

void DCF77_DeInitialize(void)
{
	DCF77_DisableReceiver();
	DCF77_StopICTimers();
}

	struct DCF77Buffer_t DCF77Buffer;

enum PulseType DCF77_CheckPulseType(DCF77_TimeSample_t* sampleToCheck)
{
	if(		sampleToCheck->pulseLength > ZERO_PULS_DURATION_MIN && \
			sampleToCheck->pulseLength < ZERO_PULS_DURATION_MAX)
	{
		return ZERO_PULSE;
	}
	else if(sampleToCheck->pulseLength > ONE_PULS_DURATION_MIN && \
			sampleToCheck->pulseLength < ONE_PULS_DURATION_MAX)
	{
		return ONE_PULSE;
	}
	else if(sampleToCheck->pulseLength > MINUTE_MARK_PULS_DURATION_MIN && \
			sampleToCheck->pulseLength < MINUTE_MARK_PULS_DURATION_MAX)
	{
		return MINUTE_PULSE;
	}

	return UNKNOWN_PULSE;
}

void DCF77_FillBufferOnPosition(DCF77Buffer_t* DCF77Buffer, uint8_t currentBit, uint32_t valueOnPosition)
{
	uint64_t mask = 1 << currentBit;
	DCF77Buffer->DCF77bits = (DCF77Buffer->DCF77bits & ~mask) | (valueOnPosition << currentBit);
}

static void DCF77_EnableReceiver(void)
{
	HAL_GPIO_WritePin(DCF77_PDN_GPIO_Port, DCF77_PDN_Pin, GPIO_PIN_RESET);
}

static void DCF77_DisableReceiver(void)
{
	HAL_GPIO_WritePin(DCF77_PDN_GPIO_Port, DCF77_PDN_Pin, GPIO_PIN_SET);
}

static void DCF77_StartICTimers(void)
{
	HAL_TIM_IC_Start_IT(&htim22, TIM_CHANNEL_1);   	// main channel
	HAL_TIM_IC_Start(&htim22, TIM_CHANNEL_2);   	// indirect channel
}

static void DCF77_StopICTimers(void)
{
	HAL_TIM_IC_Stop_IT(&htim22, TIM_CHANNEL_1);   	// main channel
	HAL_TIM_IC_Stop(&htim22, TIM_CHANNEL_2);   		// indirect channel
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	static DCF77_TimeSample_t currentSample;

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // If the interrupt is triggered by channel 1
		{
			// Read the IC values
			currentSample.signalLength = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			currentSample.pulseLength = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

			osMessageQueuePut(DCF77_TimeSamplesQueueHandle, &currentSample, 0U, 0U);
		}
}
