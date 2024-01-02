/*
 * interface.c
 *
 *  Created on: Nov 19, 2023
 *      Author: Dzale
 */
#include "main.h"
#include "interface.h"
#include "vfd.h"
#include "vfd_font.h"

const uint32_t flagsWithUnblockAbility = INTERFACE_SECOND_FLAG | INTERFACE_2AM_FLAG | INTERFACE_BUTTON_PRESS_FLAG | INTERFACE_BUTTON_HOLD_FLAG;

void IF_ShowDateOnVFD(RTC_DateTypeDef date)
{
	uint8_t dateDataBuffer[8];

	// Fill day positions
	dateDataBuffer[0] = VFD_FONT_NUMBERS[date.Date / 10]; 						// First position, can be 0, 1, 2, 3
	dateDataBuffer[1] = VFD_FONT_NUMBERS[date.Date % 10] | VFD_ACTIVATE_DOT; 	//Last digit of date plus Dot

	// Fill Monts positions
	dateDataBuffer[2] = VFD_FONT_NUMBERS[date.Month / 10];
	dateDataBuffer[3] = VFD_FONT_NUMBERS[date.Month % 10] | VFD_ACTIVATE_DOT;

	// Fill Year
	dateDataBuffer[4] = VFD_FONT_NUMBERS[2];
	dateDataBuffer[5] = VFD_FONT_NUMBERS[0]; 									// Year 20xx in next century use 1
	dateDataBuffer[6] = VFD_FONT_NUMBERS[date.Year / 10];
	dateDataBuffer[7] = VFD_FONT_NUMBERS[date.Year % 10] | VFD_ACTIVATE_DOT;

	VFD_WriteDataToDsiplayAtPosition(dateDataBuffer, sizeof(dateDataBuffer), 0);
}
