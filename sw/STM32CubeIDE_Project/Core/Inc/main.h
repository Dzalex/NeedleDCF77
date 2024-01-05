/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
int __io_putchar(int ch);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWM_Min_Pin GPIO_PIN_1
#define PWM_Min_GPIO_Port GPIOA
#define PWM_Hour_Pin GPIO_PIN_2
#define PWM_Hour_GPIO_Port GPIOA
#define PWM_Sec_Pin GPIO_PIN_3
#define PWM_Sec_GPIO_Port GPIOA
#define VFD_STB_Pin GPIO_PIN_4
#define VFD_STB_GPIO_Port GPIOA
#define PWM_FOSC_Pin GPIO_PIN_5
#define PWM_FOSC_GPIO_Port GPIOA
#define BATT_LEVEL_Pin GPIO_PIN_7
#define BATT_LEVEL_GPIO_Port GPIOA
#define DCF77_PDN_Pin GPIO_PIN_10
#define DCF77_PDN_GPIO_Port GPIOB
#define SHDN_24V_Pin GPIO_PIN_11
#define SHDN_24V_GPIO_Port GPIOB
#define LTC_SUSP_Pin GPIO_PIN_12
#define LTC_SUSP_GPIO_Port GPIOB
#define LTC_HPWR_Pin GPIO_PIN_13
#define LTC_HPWR_GPIO_Port GPIOB
#define LTC_MODE_Pin GPIO_PIN_14
#define LTC_MODE_GPIO_Port GPIOB
#define LTC_CHRG_Pin GPIO_PIN_15
#define LTC_CHRG_GPIO_Port GPIOB
#define BATT_CHECK_Pin GPIO_PIN_8
#define BATT_CHECK_GPIO_Port GPIOA
#define LTC_EN2_Pin GPIO_PIN_11
#define LTC_EN2_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
