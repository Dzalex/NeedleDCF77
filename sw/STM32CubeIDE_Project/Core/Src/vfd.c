/*
 * vfd.c
 *
 *  Created on: Nov 25, 2023
 *      Author: Dzale
 */
#include "spi.h"
#include "tim.h"
#include "cmsis_os.h"

#include "vfd.h"
#include "vfd_font.h"
#include "powerManagement.h"

/* Forward declaration of local functions */
void VFD_Command(uint8_t command);
void VFD_Clear();


void VFD_PowerOnAndInitialize()
{
	//Turn on 4V
	LTC_Enable4VoltRail();
	//Activate SPI module from HAL - if decidet to use MSP_deInit -- not for now

	//Activate PWM Fosc
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	//Turn on 24V
	LT1617_EnableNeg24VRail();

	//Delay 200ms
	osDelay(200);

	//Set Command 2 to write data
	VFD_Command(VFD_DATA_SETTING_WRITE_TO_DISPLAY_MODE | VFD_INCREMENT_ADDRESS);

	//Set Command 3 to clear Display ram
	VFD_Clear();

	//Set Command 1 Mode 9dig 13seg
	VFD_Command(VFD_DISPLAY_MODE_9DIG_13SEG);

	//Set Command 4 to turn on display with brightness
	VFD_Command(VFD_BRIGHTNESS_BASE | 0x03);
}

void VFD_PowerOffAndDeinitialize()
{

}

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

