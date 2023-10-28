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

void SetSecond(uint8_t seconds);
void SetMinute(uint8_t minute);
void SetHour(uint8_t hour);

#endif /* INC_NEEDLE_H_ */
