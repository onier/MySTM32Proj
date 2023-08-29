/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    iwdg.c
  * @brief   This file provides code for the configuration
  *          of the IWDG instances.
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
#include "iwdg.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

IWDG_HandleTypeDef hiwdg;

//固定的计算公式
static int getReload(int ms, int prescaler) {
    switch (prescaler) {
        case IWDG_PRESCALER_4:
            prescaler = 1;
            break;
        case IWDG_PRESCALER_8:
            prescaler = 2;
            break;
        case IWDG_PRESCALER_16:
            prescaler = 4;
            break;
        case IWDG_PRESCALER_32:
            prescaler = 8;
            break;
        case IWDG_PRESCALER_64:
            prescaler = 16;
            break;
        case IWDG_PRESCALER_128:
            prescaler = 32;
            break;
        case IWDG_PRESCALER_256:
            prescaler = 64;
            break;
    }
    return ms * 32000 / (4 * prescaler * 1000) - 1;
}

void Task1(void *pvParameters) {
    while (1) {
        /* USER CODE END WHILE */
        vTaskDelay(pdMS_TO_TICKS(2000));
        HAL_IWDG_Refresh(&hiwdg);
        uint8_t rxBuffer[] = "Task1 Running \n";
        HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
    }
}

void Task2(void *pvParameters) {
    while (1) {
        /* USER CODE END WHILE */
        vTaskDelay(pdMS_TO_TICKS(2000));
        HAL_IWDG_Refresh(&hiwdg);
        uint8_t rxBuffer[] = "Task2 Running \n";
        HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
    }
}

/* IWDG init function */
void MX_IWDG_Init(void) {
    uint8_t rxBuffer[] = "MX_IWDG_Init starting \n";
    HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
    /* USER CODE BEGIN IWDG_Init 0 */

    /* USER CODE END IWDG_Init 0 */

    /* USER CODE BEGIN IWDG_Init 1 */

    /* USER CODE END IWDG_Init 1 */
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
    hiwdg.Init.Reload = getReload(1000,IWDG_PRESCALER_64);//1s的超时时间，两个任务可以共享看门狗，刷新周期会累加。下面两个task分别2秒刷新看门狗会导致超时出现。2/2>=1
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK) {
        Error_Handler();
    }
    xTaskCreate(Task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Task2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    // 启动调度器
    vTaskStartScheduler();

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
