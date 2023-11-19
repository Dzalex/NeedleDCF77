/*
 * needle.h
 *
 *  Created on: Oct 28, 2023
 *      Author: Dzale
 */

#ifndef INC_NEEDLE_H_
#define INC_NEEDLE_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"

void NDL_EnableAllNeedles(void);
void NDL_DisableAllNeedles(void);

void NDL_SetAllNeedles(RTC_TimeTypeDef time);
void NDL_SetSecond(RTC_TimeTypeDef time);
void NDL_SetMinute(RTC_TimeTypeDef time);
void NDL_SetHour(RTC_TimeTypeDef time);

#endif /* INC_NEEDLE_H_ */
