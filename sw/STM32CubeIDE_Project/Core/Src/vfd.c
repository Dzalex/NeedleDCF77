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
	VFD_ActivateStrobe();
	HAL_SPI_Transmit(&hspi1, &command, 1, 100);
	VFD_DeactivateStrobe();
}

void VFD_AddressSettingCommand(uint8_t address)
{
	VFD_ActivateStrobe();
	uint8_t addressSettingCommand = VFD_ADDRESS_COMAND_BASE + address;
	HAL_SPI_Transmit(&hspi1, &addressSettingCommand, 1, 100);
}

void VFD_Clear()
{
	VFD_AddressSettingCommand(0);
	uint8_t no_data = 0x00;

	for(uint8_t i = 0; i < 0x16; i++)
	{
		HAL_SPI_Transmit(&hspi1, &no_data, 1, 100);
	}
}

void VFD_ActivateStrobe()
{
	HAL_GPIO_WritePin(VFD_STB_GPIO_Port, VFD_STB_Pin, GPIO_PIN_RESET);
}

void VFD_DeactivateStrobe()
{
	HAL_GPIO_WritePin(VFD_STB_GPIO_Port, VFD_STB_Pin, GPIO_PIN_SET);
}

void VFD_Test()
{
	VFD_Command(VFD_DATA_SETTING_WRITE_TO_DISPLAY_MODE);

	VFD_Command(VFD_ADDRESS_COMAND_BASE | 0x00);

	HAL_SPI_Transmit(&hspi1, fontForVFD, 10, 100);

	__HAL_SPI_DISABLE(&hspi1);

	VFD_Command(VFD_DISPLAY_MODE_9DIG_13SEG);

	VFD_Command(VFD_BRIGHTNESS_BASE | 0x03);
}

