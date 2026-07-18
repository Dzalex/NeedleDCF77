/*
 * vfd.h
 *
 *  Created on: Nov 25, 2023
 *      Author: Dzale
 */

#ifndef INC_VFD_H_
#define INC_VFD_H_

#include "main.h"


void VFD_PowerOnAndInitialize();
void VFD_PowerOffAndDeinitialize();

void VFD_Clear();

void VFD_PrintDigitAtPosition(uint8_t digit, uint8_t position);
void VFD_PrintCharacterAtPosition(char digit, uint8_t position);
void VFD_PrintPositiveNumber(uint32_t number);
void VFD_WriteDataToDsiplayAtPosition(uint8_t *data, uint8_t size, uint8_t position);

void VFD_Test();


#endif /* INC_VFD_H_ */
