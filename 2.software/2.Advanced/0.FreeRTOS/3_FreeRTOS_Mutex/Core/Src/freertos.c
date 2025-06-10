/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include <stdio.h>
#include "key.h"
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
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for task_H */
osThreadId_t task_HHandle;
const osThreadAttr_t task_H_attributes = {
  .name = "task_H",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow2,
};
/* Definitions for task_M */
osThreadId_t task_MHandle;
const osThreadAttr_t task_M_attributes = {
  .name = "task_M",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for task_L */
osThreadId_t task_LHandle;
const osThreadAttr_t task_L_attributes = {
  .name = "task_L",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Mutex01 */
osMutexId_t Mutex01Handle;
const osMutexAttr_t Mutex01_attributes = {
  .name = "Mutex01"
};
/* Definitions for BinarySem01 */
osSemaphoreId_t BinarySem01Handle;
const osSemaphoreAttr_t BinarySem01_attributes = {
  .name = "BinarySem01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Task_H_Func(void *argument);
void Task_M_Func(void *argument);
void Task_L_Func(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of Mutex01 */
  Mutex01Handle = osMutexNew(&Mutex01_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of BinarySem01 */
  BinarySem01Handle = osSemaphoreNew(1, 1, &BinarySem01_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of task_H */
  task_HHandle = osThreadNew(Task_H_Func, NULL, &task_H_attributes);

  /* creation of task_M */
  task_MHandle = osThreadNew(Task_M_Func, NULL, &task_M_attributes);

  /* creation of task_L */
  task_LHandle = osThreadNew(Task_L_Func, NULL, &task_L_attributes);

  /* USER CODE BEGIN RTOS_THREADS */

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
		HAL_GPIO_TogglePin(LED_T_GPIO_Port, LED_T_Pin);
    osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Task_H_Func */
/**
* @brief Function implementing the task_H thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_H_Func */
void Task_H_Func(void *argument)
{
  /* USER CODE BEGIN Task_H_Func */
  /* Infinite loop */
  for(;;)
  {
		// osSemaphoreAcquire(BinarySem01Handle, osWaitForever);
		osMutexAcquire(Mutex01Handle, osWaitForever);
    printf("High Task get mutex, start\r\n");
    HAL_Delay(1000);
    printf("High Task give mutex, end\r\n");
    // osSemaphoreRelease(BinarySem01Handle);
		osMutexRelease(Mutex01Handle);
    osDelay(1000);
  }
  /* USER CODE END Task_H_Func */
}

/* USER CODE BEGIN Header_Task_M_Func */
/**
* @brief Function implementing the task_M thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_M_Func */
void Task_M_Func(void *argument)
{
  /* USER CODE BEGIN Task_M_Func */
  /* Infinite loop */
  for(;;)
  {
		printf("Middle Task use cpu, but do nothing\r\n");
    osDelay(1000);
  }
  /* USER CODE END Task_M_Func */
}

/* USER CODE BEGIN Header_Task_L_Func */
/**
* @brief Function implementing the task_L thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_L_Func */
void Task_L_Func(void *argument)
{
  /* USER CODE BEGIN Task_L_Func */
  /* Infinite loop */
  for(;;)
  {
		// osSemaphoreAcquire(BinarySem01Handle, osWaitForever);
		osMutexAcquire(Mutex01Handle, osWaitForever);
    printf("Low Task get mutex, start\r\n");
    HAL_Delay(3000);
    printf("Low Task give mutex, end\r\n");
    // osSemaphoreRelease(BinarySem01Handle);
		osMutexRelease(Mutex01Handle);
    osDelay(1000);
  }
  /* USER CODE END Task_L_Func */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

