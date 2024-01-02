/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdbool.h"
#include "rtc.h"
#include "interface.h"
#include "needle.h"
#include "vfd.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for interfaceTask */
osThreadId_t interfaceTaskHandle;
const osThreadAttr_t interfaceTask_attributes = {
  .name = "interfaceTask",
  .stack_size = 96 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for DCF77Task */
osThreadId_t DCF77TaskHandle;
const osThreadAttr_t DCF77Task_attributes = {
  .name = "DCF77Task",
  .stack_size = 96 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for ButtonPeriodicCheck100ms */
osTimerId_t ButtonPeriodicCheck100msHandle;
const osTimerAttr_t ButtonPeriodicCheck100ms_attributes = {
  .name = "ButtonPeriodicCheck100ms"
};
/* Definitions for interfaceEvent */
osEventFlagsId_t interfaceEventHandle;
const osEventFlagsAttr_t interfaceEvent_attributes = {
  .name = "interfaceEvent"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartInterfaceTask(void *argument);
void StartDCF77Task(void *argument);
void ButtonPeriodicCheck(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of ButtonPeriodicCheck100ms */
  ButtonPeriodicCheck100msHandle = osTimerNew(ButtonPeriodicCheck, osTimerPeriodic, NULL, &ButtonPeriodicCheck100ms_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of interfaceTask */
  interfaceTaskHandle = osThreadNew(StartInterfaceTask, NULL, &interfaceTask_attributes);

  /* creation of DCF77Task */
  DCF77TaskHandle = osThreadNew(StartDCF77Task, NULL, &DCF77Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of interfaceEvent */
  interfaceEventHandle = osEventFlagsNew(&interfaceEvent_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartInterfaceTask */
/**
  * @brief  Function implementing the interfaceTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartInterfaceTask */
void StartInterfaceTask(void *argument)
{
  /* USER CODE BEGIN StartInterfaceTask */
	uint32_t receivedInterfaceFlag = 0;
	bool showingDate = false;
	RTC_TimeTypeDef currentTime = {0};
	RTC_DateTypeDef currentDate = {0};

	NDL_EnableAllNeedles();

  /* Infinite loop */
	for(;;)
	{
		receivedInterfaceFlag = osEventFlagsWait(interfaceEventHandle, 1, osFlagsWaitAny, osWaitForever);
		switch(receivedInterfaceFlag)
		{
		case INTERFACE_SECOND_FLAG:
			HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
			NDL_SetAllNeedles(currentTime);

			if(showingDate == true)
			{
				IF_ShowDateOnVFD(currentDate);
			}
			break;
		case INTERFACE_2AM_FLAG:

			break;
		case INTERFACE_BUTTON_PRESS_FLAG:
			if(showingDate == false)
			{
				VFD_PowerOnAndInitialize();
			}
			showingDate = true;
			break;
		case INTERFACE_BUTTON_HOLD_FLAG:

		break;
		}

		osDelay(1);
	}
  /* USER CODE END StartInterfaceTask */
}

/* USER CODE BEGIN Header_StartDCF77Task */
/**
* @brief Function implementing the DCF77Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDCF77Task */
void StartDCF77Task(void *argument)
{
  /* USER CODE BEGIN StartDCF77Task */
  /* Infinite loop */
  for(;;)
  {
	  osThreadSuspend(DCF77TaskHandle);
	  osDelay(1);
  }
  /* USER CODE END StartDCF77Task */
}

/* ButtonPeriodicCheck function */
void ButtonPeriodicCheck(void *argument)
{
  /* USER CODE BEGIN ButtonPeriodicCheck */

  /* USER CODE END ButtonPeriodicCheck */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

