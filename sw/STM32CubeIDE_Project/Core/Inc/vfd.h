/*
 * vfd.h
 *
 *  Created on: Nov 25, 2023
 *      Author: Dzale
 */

#ifndef INC_VFD_H_
#define INC_VFD_H_

#include "main.h"


/* COMMAND 1: DISPLAY MODE SETTING COMMANDS */
const uint8_t DISPLAY_MODE_9DIG_13SEG = 0x05;

/* COMMAND 2: DATA SETTING COMMANDS */
const uint8_t DATA_SETTING_WRITE_TO_DISPLAY_MODE = 0x40;	/* Increment address after data has been written */
/* Address increment mode settings (Display mode) - use as options | */
const uint8_t INCREMENT_ADDRESS = 0x00;
const uint8_t FIXED_ADDRESS = 0x04;
/* Mode settings */
const uint8_t TEST_MODE = 0x08;


#endif /* INC_VFD_H_ */
