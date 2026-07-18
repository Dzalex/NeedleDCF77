/*
 * powerManagement.c
 *
 *  Created on: Nov 7, 2023
 *      Author: Dzale
 */
#include "powerManagement.h"
#include "adc.h"
#include "cmsis_os.h"

/* Forward declaration of local functions */
static void BAT_EnableBatteryCheck();
static void BAT_DisableBatteryCheck();


void LTC_Enable4VoltRail(void)
{
	HAL_GPIO_WritePin(LTC_EN2_GPIO_Port, LTC_EN2_Pin, GPIO_PIN_SET);
}

void LTC_Disable4VoltRail(void)
{
	HAL_GPIO_WritePin(LTC_EN2_GPIO_Port, LTC_EN2_Pin, GPIO_PIN_RESET);
}

void LTC_SetBurstMode(void)
{
	HAL_GPIO_WritePin(LTC_MODE_GPIO_Port, LTC_MODE_Pin, GPIO_PIN_SET);
}

void LTC_SetPulseSkipMode(void)
{
	HAL_GPIO_WritePin(LTC_MODE_GPIO_Port, LTC_MODE_Pin, GPIO_PIN_RESET);
}

void LTC_EnableHighPwrCharge(void)
{
	HAL_GPIO_WritePin(LTC_HPWR_GPIO_Port, LTC_HPWR_Pin, GPIO_PIN_SET);
}

void LTC_DisableHighPwrCharge(void)
{
	HAL_GPIO_WritePin(LTC_HPWR_GPIO_Port, LTC_HPWR_Pin, GPIO_PIN_RESET);
}

void LTC_SuspendCharging(void)
{
	HAL_GPIO_WritePin(LTC_SUSP_GPIO_Port, LTC_SUSP_Pin, GPIO_PIN_SET);
}

void LTC_ReactivateCharging(void)
{
	HAL_GPIO_WritePin(LTC_SUSP_GPIO_Port, LTC_SUSP_Pin, GPIO_PIN_RESET);
}

chargeStatus LTC_GetChargeStatus(void)
{
	// TODO: Implement Charging status --> for NTC_FAULT, BAD_BATTERY
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(LTC_CHRG_GPIO_Port, LTC_CHRG_Pin))
	{
		return CHARGING;
	}
	return NOT_CHARGING;
}

void LT1617_EnableNeg24VRail(void)
{
	HAL_GPIO_WritePin(SHDN_24V_GPIO_Port, SHDN_24V_Pin, GPIO_PIN_SET);
}

void LT1617_DisableNeg24VRail(void)
{
	HAL_GPIO_WritePin(SHDN_24V_GPIO_Port, SHDN_24V_Pin, GPIO_PIN_RESET);
}

uint16_t BAT_GetBatteryVoltage_mV(void)
{
	uint16_t adcBattValue = 0;
	uint32_t battValue_mV = 0;
	const uint32_t ADC_TIMEOUT = 3;
	const uint16_t FULL_SCALE_12B = 4095U;
	const uint16_t VDD_VOLTAGE_mV = 3000U;
	const uint8_t INPUT_DIVIDER_FACTOR = 2;

	BAT_EnableBatteryCheck();

	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, ADC_TIMEOUT);
	adcBattValue = HAL_ADC_GetValue(&hadc);		// For channel 7

	HAL_ADC_Stop(&hadc);

	BAT_DisableBatteryCheck();

	battValue_mV = ((VDD_VOLTAGE_mV * adcBattValue) / FULL_SCALE_12B) * INPUT_DIVIDER_FACTOR;
	return battValue_mV;
}

static void BAT_EnableBatteryCheck(void)
{
	HAL_GPIO_WritePin(BATT_CHECK_GPIO_Port, BATT_CHECK_Pin, GPIO_PIN_SET);
	osDelay(10);
}

static void BAT_DisableBatteryCheck(void)
{
	HAL_GPIO_WritePin(BATT_CHECK_GPIO_Port, BATT_CHECK_Pin, GPIO_PIN_RESET);
}

