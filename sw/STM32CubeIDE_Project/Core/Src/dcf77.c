/*
 * dcf77.c
 *
 *  Created on: Jan 23, 2024
 *      Author: Dzale
 */

#include "dcf77.h"
#include "dcf77Decoding.h"
#include "main.h"
#include "tim.h"
#include "stdbool.h"

const uint32_t SYNCHRONISATION_TIMEOUT = 5 * 60 * 1000;	// Sync time of 5 minutes max

/* Forward declaration of local functions */
static void DCF77_EnableReceiver(void);
static void DCF77_DisableReceiver(void);
static void DCF77_StartICTimers(void);
static void DCF77_StopICTimers(void);

enum PulseType DCF77_CheckPulseType(DCF77_TimeSample_t* sampleToCheck);

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

ErrorStatus DCF77_GetTimeAndDate(RTC_TimeTypeDef* timeBuffer, RTC_DateTypeDef* dateBuffer)
{
	DCF77Buffer_t DCF77Buffer;
	DCF77_TimeSample_t newSample;
	uint8_t currentBit = 0;
	enum PulseType currentPulseType;
	uint32_t timeoutTickCount = SYNCHRONISATION_TIMEOUT + osKernelGetTickCount();

	while(timeoutTickCount > osKernelGetTickCount())
	{
		osStatus_t QueueStatus = osMessageQueueGet(DCF77_TimeSamplesQueueHandle, &newSample, NULL, 10U);
		if(osOK != QueueStatus)
		{
			osDelay(1);
			continue;
		}

		currentPulseType = DCF77_CheckPulseType(&newSample);
		switch(currentPulseType)
		{
		case MINUTE_PULSE:
			if(INTEGRITY_OK == DCF77_CheckBufferIntegrity(&DCF77Buffer))
			{
				// Calculate time - we have complete buffer;
				// Casting to CopyOf_ types so we know what are we doing.
				DCF77_DecodeTimeToRTCTimeBuffer(&DCF77Buffer, (CopyOf_RTC_TimeTypeDef*)timeBuffer);
				DCF77_DecodeDateToRTCDateBuffer(&DCF77Buffer, (CopyOf_RTC_DateTypeDef*)dateBuffer);
				return SUCCESS;
			}
			DCF77_ResetBuffer(&DCF77Buffer);
			currentBit = 1;
			break;
		case ZERO_PULSE:
			currentBit++;
			break;
		case ONE_PULSE:
			DCF77_SetBufferBitOnPosition(&DCF77Buffer, currentBit);
			currentBit++;
			break;
		case UNKNOWN_PULSE:
			break;
		}
	}
	return ERROR;
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
			currentSample.pulseLength = currentSample.signalLength - HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

			osMessageQueuePut(DCF77_TimeSamplesQueueHandle, &currentSample, 0U, 0U);
		}
}
