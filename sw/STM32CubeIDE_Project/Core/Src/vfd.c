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

const uint8_t PT6312_BYTES_PER_GRID = 2;
const uint8_t PT6312_MEMORY_SIZE = 0x16;
const uint8_t VFD_GRIDS = 9;
const uint8_t VFD_SEGMENTS = 8;
const uint8_t VFD_SELECTED_BRIGHTNESS = 0x03;

const uint8_t VFD_DISPLAY_MODE_9DIG_13SEG = 0x05;
const uint8_t VFD_DISPLAY_MODE_11DIG_11SEG = 0x07;

const uint8_t VFD_DATA_SETTING_WRITE_TO_DISPLAY_MODE = 0x40;

const uint8_t VFD_INCREMENT_ADDRESS = 0x00;
const uint8_t VFD_FIXED_ADDRESS = 0x02;

const uint8_t VFD_TEST_MODE = 0x04;

const uint8_t VFD_ADDRESS_COMAND_BASE = 0xC0;

const uint8_t VFD_BRIGHTNESS_BASE = 0x88;
const uint8_t VFD_OFF = 0x80;

const uint32_t SPI_TIMEOUT = 100;


/* Forward declaration of local functions */
void VFD_Command(uint8_t command);
void VFD_AddressSettingCommand(uint8_t address);
void VFD_WriteDataToDsiplayAtPosition(uint8_t *data, uint8_t size, uint8_t position);
void VFD_ActivateStrobe();
void VFD_DeactivateStrobe();


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

	__HAL_SPI_ENABLE(&hspi1);

	//Set Command 2 to write data
	VFD_Command(VFD_DATA_SETTING_WRITE_TO_DISPLAY_MODE | VFD_INCREMENT_ADDRESS);

	//Set Command 3 to clear Display ram
	VFD_Clear();

	//Set Command 1 Mode 9dig 13seg
	VFD_Command(VFD_DISPLAY_MODE_9DIG_13SEG);

	//Set Command 4 to turn on display with brightness
	VFD_Command(VFD_BRIGHTNESS_BASE | VFD_SELECTED_BRIGHTNESS);
}

void VFD_PowerOffAndDeinitialize()
{
	LT1617_DisableNeg24VRail();
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	LTC_Disable4VoltRail();
	__HAL_SPI_DISABLE(&hspi1);
}

void VFD_ShowDate(RTC_DateTypeDef date)
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
	dateDataBuffer[5] = VFD_FONT_NUMBERS[0]; // Year 20xx in next decade use 1
	dateDataBuffer[6] = VFD_FONT_NUMBERS[date.Year / 10];
	dateDataBuffer[7] = VFD_FONT_NUMBERS[date.Year % 10] | VFD_ACTIVATE_DOT;

	VFD_WriteDataToDsiplayAtPosition(dateDataBuffer, sizeof(dateDataBuffer), 0);
}

void VFD_Command(uint8_t command)
{
	VFD_ActivateStrobe();
	HAL_SPI_Transmit(&hspi1, &command, 1, SPI_TIMEOUT);
	VFD_DeactivateStrobe();
}

void VFD_AddressSettingCommand(uint8_t address)
{
	VFD_ActivateStrobe();
	uint8_t addressSettingCommand = VFD_ADDRESS_COMAND_BASE + address;
	HAL_SPI_Transmit(&hspi1, &addressSettingCommand, 1, SPI_TIMEOUT);
}

void VFD_Clear()
{
	VFD_AddressSettingCommand(0);
	uint8_t no_data = 0x00;

	for(uint8_t i = 0; i < PT6312_MEMORY_SIZE; i++)
	{
		HAL_SPI_Transmit(&hspi1, &no_data, 1, SPI_TIMEOUT);
	}

	VFD_DeactivateStrobe();
}

void VFD_PrintDigitAtPosition(uint8_t digit, uint8_t possition)
{
	if(possition > VFD_GRIDS)
		return;

	if(digit > 9)
		return;

	VFD_WriteDataToDsiplayAtPosition((uint8_t *)&VFD_FONT_NUMBERS[digit], 1, possition);
}

void VFD_PrintCharacterAtPosition(char digit, uint8_t position)
{
	if(position > VFD_GRIDS)
		return;

	if(digit < 'a' || digit > 'z')
		return;

	uint8_t characterToWrite = digit - 'a';

	VFD_WriteDataToDsiplayAtPosition((uint8_t *)&VFD_FONT_LETTERS[characterToWrite], 1, position);
}

void VFD_WriteDataToDsiplayAtPosition(uint8_t *data, uint8_t size, uint8_t position)
{
	if(position > VFD_GRIDS){
		return;
	}

	uint8_t addressForPossition = position * PT6312_BYTES_PER_GRID;
	uint8_t allOff = 0x00;

	VFD_Command(VFD_DATA_SETTING_WRITE_TO_DISPLAY_MODE);
	VFD_AddressSettingCommand(addressForPossition);

	for(uint8_t i = 0; i < size; i++)
	{
		HAL_SPI_Transmit(&hspi1, ( (uint8_t *)data + i ), 1, SPI_TIMEOUT);
		HAL_SPI_Transmit(&hspi1, &allOff, 1, SPI_TIMEOUT);
	}

	VFD_DeactivateStrobe();

	VFD_Command(VFD_DISPLAY_MODE_9DIG_13SEG);
	VFD_Command(VFD_BRIGHTNESS_BASE | VFD_SELECTED_BRIGHTNESS);
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
	for(int i = 0; i < 26; i++)
	{
		VFD_PrintCharacterAtPosition('a' + i, i % 9);
		osDelay(200);
	}

	VFD_Clear();
	VFD_PrintCharacterAtPosition('s', 0);
	VFD_PrintCharacterAtPosition('i', 1);
	VFD_PrintCharacterAtPosition('s', 2);
	VFD_PrintCharacterAtPosition('e', 3);

	osDelay(1000);
	VFD_Clear();
}

