/*
 * dcf77.h
 *
 *  Created on: Jan 7, 2024
 *      Author: Dzale
 */

#ifndef INC_DCF77_H_
#define INC_DCF77_H_

#include "main.h"
#include "cmsis_os2.h"
#include "stdbool.h"
#include "dcf77Decoding.h"

extern osMessageQueueId_t DCF77_TimeSamplesQueueHandle;

void DCF77_Initialize(void);
void DCF77_DeInitialize(void);
ErrorStatus DCF77_GetTimeAndDate(RTC_TimeTypeDef* timeBuffer, RTC_DateTypeDef* dateBuffer);

#endif /* INC_DCF77_H_ */
