/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    wwdg.c
  * @brief   This file provides code for the configuration
  *          of the WWDG instances.
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
#include "wwdg.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

WWDG_HandleTypeDef hwwdg;
#define XAPBI_CLK 42000000   //xapbi始终频率

/* WWDG init function */\
/* 最大值127
 * 1 / 1 00 68.27 μs 4.37 ms
 * 1 / 2 01 136.54 μs 8.74 ms
 * 1 / 4 10 273.08 μs 17.48 ms
 * 1 / 8 11 546.16 μs 34.96 ms
 */
static int get_counter(int max_ms, int prescaler) {
    switch (prescaler) {
        case WWDG_PRESCALER_1:
            prescaler = 1;
            break;
        case WWDG_PRESCALER_2:
            prescaler = 2;
            break;
        case WWDG_PRESCALER_4:
            prescaler = 4;
            break;
        case WWDG_PRESCALER_8:
            prescaler = 8;
            break;
    }
    return max_ms * XAPBI_CLK / (4096 * prescaler * 1000) + 64;
}

static int get_window(int max_counter, int min_ms, int prescaler) {
    switch (prescaler) {
        case WWDG_PRESCALER_1:
            prescaler = 1;
            break;
        case WWDG_PRESCALER_2:
            prescaler = 2;
            break;
        case WWDG_PRESCALER_4:
            prescaler = 4;
            break;
        case WWDG_PRESCALER_8:
            prescaler = 8;
            break;
    }
    return max_counter - ((min_ms * XAPBI_CLK) / (4096 * prescaler * 1000));
}

void WWDG_Task1(void *pvParameters) {
    while (1) {
        /* USER CODE END WHILE */
        vTaskDelay(pdMS_TO_TICKS(20));
        HAL_WWDG_Refresh(&hwwdg);
        uint8_t rxBuffer[] = "WWDG Task1 Running \n";
        HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
    }
}

void WWDG_Task2(void *pvParameters) {
    while (1) {
        /* USER CODE END WHILE */
        vTaskDelay(pdMS_TO_TICKS(28));
        HAL_WWDG_Refresh(&hwwdg);
        uint8_t rxBuffer[] = "WWDG Task2 Running \n";
        HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
    }
}

void MX_WWDG_Init(void) {
    uint8_t rxBuffer[] = "MX_WWDG_Init  \n";
    HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
    /* USER CODE BEGIN WWDG_Init 0 */

    /* USER CODE END WWDG_Init 0 */

    /* USER CODE BEGIN WWDG_Init 1 */

    /* USER CODE END WWDG_Init 1 */
    hwwdg.Instance = WWDG;
    hwwdg.Init.Prescaler = WWDG_PRESCALER_8;

    hwwdg.Init.Counter = get_counter(30, hwwdg.Init.Prescaler);
    hwwdg.Init.Window = get_window(hwwdg.Init.Counter, 10, hwwdg.Init.Prescaler);
    hwwdg.Init.EWIMode = WWDG_EWI_DISABLE;
    if (HAL_WWDG_Init(&hwwdg) != HAL_OK) {
        Error_Handler();
    }

    /* USER CODE BEGIN WWDG_Init 2 */
//    xTaskCreate(WWDG_Task1, "WWDG_Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(WWDG_Task2, "WWDG_Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    // 启动调度器
    vTaskStartScheduler();
    /* USER CODE END WWDG_Init 2 */

}

void HAL_WWDG_MspInit(WWDG_HandleTypeDef *wwdgHandle) {

    if (wwdgHandle->Instance == WWDG) {
        /* USER CODE BEGIN WWDG_MspInit 0 */

        /* USER CODE END WWDG_MspInit 0 */
        /* WWDG clock enable */
        __HAL_RCC_WWDG_CLK_ENABLE();
        /* USER CODE BEGIN WWDG_MspInit 1 */

        /* USER CODE END WWDG_MspInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
