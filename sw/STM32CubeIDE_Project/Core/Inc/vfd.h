/*
 * vfd.h
 *
 *  Created on: Nov 25, 2023
 *      Author: Dzale
 */

#ifndef INC_VFD_H_
#define INC_VFD_H_

#include "main.h"

/* General constants for PT6312 */
const uint8_t PT6312_BYTES_PER_GRID = 2;
const uint8_t VFD_GRIDS = 9;
const uint8_t VFD_SEGMENTS = 8;

/* COMMAND 1: DISPLAY MODE SETTING COMMANDS */
const uint8_t VFD_DISPLAY_MODE_9DIG_13SEG = 0x05;


/* COMMAND 2: DATA SETTING COMMANDS */
const uint8_t VFD_DATA_SETTING_WRITE_TO_DISPLAY_MODE = 0x40;	/* Increment address after data has been written */

/* Address increment mode settings (Display mode) - use as options | */
const uint8_t VFD_INCREMENT_ADDRESS = 0x00;
const uint8_t VFD_FIXED_ADDRESS = 0x04;

/* Mode settings */
const uint8_t VFD_TEST_MODE = 0x08;


/* COMMAND 3: ADDRESS SETTING COMMANDS 8 */
const uint8_t VFD_ADDRESS_COMAND_BASE = 0xC0;	/* Address: 0x00 to 15 */


/* COMMAND 4: DISPLAY CONTROL COMMANDS */
const uint8_t VFD_BRIGHTNESS_BASE = 0x88;	/* Brightness 0 to 7 */
const uint8_t VFD_OFF = 0x80;


void VFD_PowerOnAndInitialize();
void VFD_PowerOffAndDeinitialize();





#endif /* INC_VFD_H_ */
