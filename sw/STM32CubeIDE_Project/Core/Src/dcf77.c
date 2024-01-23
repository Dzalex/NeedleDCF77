/*
 * dcf77.c
 *
 *  Created on: Jan 23, 2024
 *      Author: Dzale
 */

#include "dcf77.h"
#include "main.h"
#include "tim.h"

/* Forward declaration of local functions */
static void DCF77_EnableReceiver(void);
static void DCF77_DisableReceiver(void);
static void DCF77_StartICTimers(void);
static void DCF77_StopICTimers(void);

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
