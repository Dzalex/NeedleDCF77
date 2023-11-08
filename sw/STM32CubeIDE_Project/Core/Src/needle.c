/*
 * needle.c
 *
 *  Created on: Oct 28, 2023
 *      Author: Dzale
 */

#include "needle.h"

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

void EnableAllNeedles(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4); //Start Second
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); //Start Minute
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); //Start Hour
}

void DisableAllNeedles(void)
{
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4); //Stop Second
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2); //Stop Minute
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3); //Stop Hour
}

void SetSecond(RTC_TimeTypeDef time)
{
	static uint16_t secondsPulseWidth;
	if(time.Seconds < 60)
	{
		secondsPulseWidth = (UINT16_MAX / 59) * time.Seconds ;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, secondsPulseWidth);
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
	}
}

void SetMinute(RTC_TimeTypeDef time)
{
	static uint16_t minutePulseWidth;
	if(time.Minutes < 60)
	{
		minutePulseWidth = (UINT16_MAX / 59) * time.Minutes ;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, minutePulseWidth);
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
	}
}

void SetHour(RTC_TimeTypeDef time)
{
	static uint16_t hourPulseWidth;
	if(time.Hours < 13)
	{
		hourPulseWidth = (UINT16_MAX / 12) * time.Hours ;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, hourPulseWidth);
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
	}
}
