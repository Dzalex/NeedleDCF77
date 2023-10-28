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

void SetSecond(uint8_t seconds)
{
	static uint16_t secondsPulseWidth;
	if(seconds >= 0 && seconds < 60)
	{
		secondsPulseWidth = (INT16_MAX / 59) * seconds ;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, secondsPulseWidth);
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
	}
}

void SetMinute(uint8_t minute)
{
	static uint16_t minutePulseWidth;
	if(minute >= 0 && minute < 60)
	{
		minutePulseWidth = (INT16_MAX / 59) * minute ;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, minutePulseWidth);
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
	}
}

void SetHour(uint8_t hour)
{
	static uint16_t hourPulseWidth;
	if(hour >= 0 && hour < 60)
	{
		hourPulseWidth = (INT16_MAX / 12) * hour ;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, hourPulseWidth);
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
	}
}
