/*
 * interface.h
 *
 *  Created on: Nov 19, 2023
 *      Author: Dzale
 */

#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_

#include "cmsis_os.h"

extern osEventFlagsId_t interfaceEventHandle;

enum InterfaceEventFlags {
	INTERFACE_SECOND_FLAG 			= 1 << 0,
	INTERFACE_2AM_FLAG 				= 1 << 1,
	INTERFACE_BUTTON_PRESS_FLAG 	= 1 << 2,
	INTERFACE_BUTTON_HOLD_FLAG 		= 1 << 3
};

void IF_ShowDateOnVFD(RTC_DateTypeDef date);

#endif /* INC_INTERFACE_H_ */
