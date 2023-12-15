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
extern const uint8_t PT6312_BYTES_PER_GRID;
extern const uint8_t VFD_GRIDS;
extern const uint8_t VFD_SEGMENTS;

/* COMMAND 1: DISPLAY MODE SETTING COMMANDS */
extern const uint8_t VFD_DISPLAY_MODE_9DIG_13SEG;
extern const uint8_t VFD_DISPLAY_MODE_11DIG_11SEG;


/* COMMAND 2: DATA SETTING COMMANDS */
extern const uint8_t VFD_DATA_SETTING_WRITE_TO_DISPLAY_MODE0;	/* Increment address after data has been written */

/* Address increment mode settings (Display mode) - use as options | */
extern const uint8_t VFD_INCREMENT_ADDRESS;
extern const uint8_t VFD_FIXED_ADDRESS;

/* Mode settings */
extern const uint8_t VFD_TEST_MODE;


/* COMMAND 3: ADDRESS SETTING COMMANDS 8 */
extern const uint8_t VFD_ADDRESS_COMAND_BASE;	/* Address: 0x00 to 15 */


/* COMMAND 4: DISPLAY CONTROL COMMANDS */
extern const uint8_t VFD_BRIGHTNESS_BASE;	/* Brightness 0 to 7 */
extern const uint8_t VFD_OFF;


void VFD_PowerOnAndInitialize();
void VFD_PowerOffAndDeinitialize();

void VFD_Test();





#endif /* INC_VFD_H_ */
