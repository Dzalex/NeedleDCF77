/*
 * vfd.c
 *
 *  Created on: Nov 25, 2023
 *      Author: Dzale
 */


void VFD_Command(uint8_t command)
{
	HAL_SPI_Transmit(&hspi1, &command, 1, 100);
	__HAL_SPI_DISABLE(&hspi1);
}

