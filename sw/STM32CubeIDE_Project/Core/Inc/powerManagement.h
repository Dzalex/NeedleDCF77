/*
 * powerManagement.h
 *
 *  Created on: Nov 7, 2023
 *      Author: Dzale
 */

#ifndef INC_POWERMANAGEMENT_H_
#define INC_POWERMANAGEMENT_H_
#include "main.h"


typedef enum chargeStatus { CHARGING, NOT_CHARGING, NTC_FAULT, BAD_BATTERY } chargeStatus;

void LTC_Enable4VoltRail(void);
void LTC_Disable4VoltRail(void);
void LTC_SetBurstMode(void);
void LTC_SetPulseSkipMode(void);
void LTC_EnableHighPwrCharge(void);
void LTC_DisableHighPwrCharge(void);
void LTC_SuspendCharging(void);
void LTC_ReactivateCharging(void);
chargeStatus LTC_GetChargeStatus(void);

void LT1617_EnableNeg24VRail(void);
void LT1617_DisableNeg24VRail(void);

uint16_t BAT_GetBatteryVoltage_mV(void);

#endif /* INC_POWERMANAGEMENT_H_ */
