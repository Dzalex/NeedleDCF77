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

void VFD_Clear()
{
	VFD_Command(VFD_ADDRESS_COMAND_BASE | 0x00);

	for(uint8_t i = 0; i < 0x16; i++)
	{
		HAL_SPI_Transmit(&hspi1, 0x00, 1, 100);
	}

	__HAL_SPI_DISABLE(&hspi1);
}

