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

void EnableAllNeedles(void);
void DisableAllNeedles(void);

void SetSecond(RTC_TimeTypeDef time);
void SetMinute(RTC_TimeTypeDef time);
void SetHour(RTC_TimeTypeDef time);

#endif /* INC_NEEDLE_H_ */
