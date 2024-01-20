/*
 * dcf77.h
 *
 *  Created on: Jan 7, 2024
 *      Author: Dzale
 */

#ifndef INC_DCF77_H_
#define INC_DCF77_H_

#include "main.h"

void DCF77_EnableReceiver(void)
{
	HAL_GPIO_WritePin(DCF77_PDN_GPIO_Port, DCF77_PDN_Pin, GPIO_PIN_RESET);
}

void DCF77_DisableReceiver(void)
{
	HAL_GPIO_WritePin(DCF77_PDN_GPIO_Port, DCF77_PDN_Pin, GPIO_PIN_SET);
}

#endif /* INC_DCF77_H_ */
