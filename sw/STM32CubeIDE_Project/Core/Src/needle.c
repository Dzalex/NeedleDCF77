/*
 * needle.c
 *
 *  Created on: Oct 28, 2023
 *      Author: Dzale
 */

#include "needle.h"

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* Forward declaration of local functions */
static uint16_t calculateSecondPulseWidth(RTC_TimeTypeDef time);
static uint16_t calculateMinutePulseWidth(RTC_TimeTypeDef time);
static uint16_t calculateHourPulseWidth(RTC_TimeTypeDef time);


void NDL_EnableAllNeedles(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4); //Start Second
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); //Start Minute
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); //Start Hour
}

void NDL_DisableAllNeedles(void)
{
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4); //Stop Second
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2); //Stop Minute
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3); //Stop Hour
}

void NDL_SetAllNeedles(RTC_TimeTypeDef time)
{
	NDL_SetSecond(time);
	NDL_SetMinute(time);
	NDL_SetHour(time);
}

void NDL_SetSecond(RTC_TimeTypeDef time)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, calculateSecondPulseWidth(time) );
}

void NDL_SetMinute(RTC_TimeTypeDef time)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, calculateMinutePulseWidth(time) );
}

void NDL_SetHour(RTC_TimeTypeDef time)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, calculateHourPulseWidth(time) );
}

static uint16_t calculateSecondPulseWidth(RTC_TimeTypeDef time)
{
	uint16_t secondsPulseWidth = (__HAL_TIM_GET_AUTORELOAD(&htim2) / 60) * time.Seconds;
	return secondsPulseWidth;
}

static uint16_t calculateMinutePulseWidth(RTC_TimeTypeDef time)
{
	uint16_t minutePulseWidth = (__HAL_TIM_GET_AUTORELOAD(&htim2) / 60) * time.Minutes;
	return minutePulseWidth;
}

static uint16_t calculateHourPulseWidth(RTC_TimeTypeDef time)
{
	uint16_t hourPulseWidth = (__HAL_TIM_GET_AUTORELOAD(&htim2) / 12) * time.Hours;
	return hourPulseWidth;
}
