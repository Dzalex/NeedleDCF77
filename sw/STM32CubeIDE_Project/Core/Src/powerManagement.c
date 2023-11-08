/*
 * powerManagement.c
 *
 *  Created on: Nov 7, 2023
 *      Author: Dzale
 */
#include "powerManagement.h"

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

void LT1617_EnableNeg24VRail(void)
{
	HAL_GPIO_WritePin(SHDN_24V_GPIO_Port, SHDN_24V_Pin, GPIO_PIN_SET);
}

void LT1617_DisableNeg24VRail(void)
{
	HAL_GPIO_WritePin(SHDN_24V_GPIO_Port, SHDN_24V_Pin, GPIO_PIN_RESET);
}

