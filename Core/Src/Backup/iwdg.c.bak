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

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

IWDG_HandleTypeDef hiwdg;

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
    hiwdg.Init.Reload = getReload(1000,IWDG_PRESCALER_64);
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN IWDG_Init 2 */
    while (1) {
        /* USER CODE END WHILE */
        HAL_Delay(900);
        HAL_IWDG_Refresh(&hiwdg);
        uint8_t rxBuffer[] = "IWDG Running \n";
        HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END IWDG_Init 2 */

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
