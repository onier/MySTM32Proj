//
// Created by ubuntu on 2023/8/30.
//
#include <string.h>
#include <stdio.h>
#include "soft_watchdog.h"
#include "usart.h"

List_t allTaskParameterDataList;
List_t timeoutTaskParameterDataList;
int interval;
SemaphoreHandle_t taskDataMutex; // 声明互斥量

CreateTaskParametersData *
Register_Soft_WatchDog(TaskFunction_t taskFun, void *taskParameter, configSTACK_DEPTH_TYPE usStackDepth,
                       int timeout_ms) {
    {
        CreateTaskParametersData *taskParametersData = pvPortMalloc(sizeof(CreateTaskParametersData));
        taskParametersData->taskFun = taskFun;
        taskParametersData->taskCreateParameters = taskParameter;
        taskParametersData->taskHandle = xTaskGetCurrentTaskHandle();
        taskParametersData->timeout_ms = timeout_ms;
//        taskParametersData->refeshTick = HAL_GetTick();//使用真实时间会导致误杀。
        taskParametersData->refeshTick = 0;//注册时为0，不会启用这个任务的软件狗功能，初始化卡主无法判断。
        strcpy(taskParametersData->name, pcTaskGetName(taskParametersData->taskHandle));
        taskParametersData->uxPriority = uxTaskPriorityGet(taskParametersData->taskHandle);
        taskParametersData->usStackDepth = usStackDepth;
        vListInitialiseItem(&taskParametersData->listItem);
        for (;;) {
            vTaskDelay(pdMS_TO_TICKS(300));
            if (xSemaphoreTake(taskDataMutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
                uint8_t rxBuffer[] = "Register_Soft_WatchDog xSemaphoreTake sucess \n";
                HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
                vListInsert(&allTaskParameterDataList, &taskParametersData->listItem);
                xSemaphoreGive(taskDataMutex);
                {
                    uint8_t rxBuffer[] = "Register_Soft_WatchDog xSemaphoreGive sucess \n";
                    HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
                }
                break;
            } else {
                {
                    uint8_t rxBuffer[] = "Register_Soft_WatchDog xSemaphoreTake  fail \n";
                    HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
                }
            }
        }
        return taskParametersData;
    }
}

void Reflesh_Soft_WatchDog(CreateTaskParametersData *data) {
    data->refeshTick = HAL_GetTick();
}

void Restart_Timeout_Task() {
    if (timeoutTaskParameterDataList.uxNumberOfItems > 0) {
        ListItem_t *currentItem = listGET_HEAD_ENTRY(&timeoutTaskParameterDataList);
        size_t liOffset = offsetof(CreateTaskParametersData, listItem);
        while (currentItem != listGET_END_MARKER(&timeoutTaskParameterDataList) && currentItem != NULL) {
            CreateTaskParametersData *data = (CreateTaskParametersData *) ((char *) currentItem - liOffset);
            vTaskDelete(data->taskHandle);
            xTaskCreate(data->taskFun, data->name, data->usStackDepth, data->taskCreateParameters, data->uxPriority,
                        NULL);
            listGET_OWNER_OF_NEXT_ENTRY(currentItem, &timeoutTaskParameterDataList);
            uxListRemove(&data->listItem);
            vPortFree(data);
        }
    }
}

void Soft_Watchdog_Task() {
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(interval));
        uint8_t rxBuffer[] = "Soft_Watchdog_Task Running \n";
        HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
        for (;;) {
            if (xSemaphoreTake(taskDataMutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
                {
                    uint8_t rxBuffer[] = "Soft_Watchdog_Task xSemaphoreTake  sucesss \n";
                    HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
                }
                uint32_t currentTime = HAL_GetTick();
                ListItem_t *currentItem = listGET_HEAD_ENTRY(&allTaskParameterDataList);
                size_t liOffset = offsetof(CreateTaskParametersData, listItem);
                while (currentItem != listGET_END_MARKER(&allTaskParameterDataList) && currentItem != NULL) {
                    CreateTaskParametersData *data = (CreateTaskParametersData *) ((char *) currentItem - liOffset);
                    uint32_t time = currentTime - data->refeshTick;
                    if (time > data->timeout_ms && data->refeshTick != 0) {
                        {
//                            {
//                                uint8_t rxBuffer[256];
//                                sprintf(rxBuffer, "%s task timeout timeout time=%d timeout_ms=%d \n", data->name, time,
//                                        data->timeout_ms);
//                                HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
//                            }
                        }
                        listGET_OWNER_OF_NEXT_ENTRY(currentItem, &allTaskParameterDataList);//删除之前将当前指针指向下一个数据
                        uxListRemove(&data->listItem);
                        vListInsert(&timeoutTaskParameterDataList, &data->listItem);
                    } else {
                        listGET_OWNER_OF_NEXT_ENTRY(currentItem, &allTaskParameterDataList);
                    }
                }
                xSemaphoreGive(taskDataMutex);
                {
                    uint8_t rxBuffer[] = "Soft_Watchdog_Task xSemaphoreGive  sucesss \n";
                    HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
                }
                Restart_Timeout_Task();
                break;
            } else {
                {
                    uint8_t rxBuffer[] = "Soft_Watchdog_Task xSemaphoreTake  fail \n";
                    HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
                }
            }
        }
    }
}

void Test_Soft_Watchdog_Task() {
    uint8_t rxBuffer[] = "Test_Soft_Watchdog_Task Starting \n";
    HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
    CreateTaskParametersData *createTaskParametersData = Register_Soft_WatchDog(&Test_Soft_Watchdog_Task,
                                                                                "Test_Soft_Watchdog_Task",
                                                                                configMINIMAL_STACK_SIZE, 1000);
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1500));
        Reflesh_Soft_WatchDog(createTaskParametersData);
        uint8_t rxBuffer[] = "Test_Soft_Watchdog_Task reflesh \n";
        HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
    }
}

void Test_Soft_Watchdog_Task1() {
    uint8_t rxBuffer[] = "Test_Soft_Watchdog_Task1 Starting \n";
    HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
    CreateTaskParametersData *createTaskParametersData = Register_Soft_WatchDog(&Test_Soft_Watchdog_Task1,
                                                                                "Test_Soft_Watchdog_Task1",
                                                                                configMINIMAL_STACK_SIZE, 1000);
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1500));
        Reflesh_Soft_WatchDog(createTaskParametersData);
        uint8_t rxBuffer[] = "Test_Soft_Watchdog_Task1 reflesh \n";
        HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
    }
}

void Enable_System_Soft_Watchdog(int interval_ms) {
    taskDataMutex = xSemaphoreCreateMutex();
    uint8_t rxBuffer[] = "Enable_System_Soft_Watchdog starting \n";
    HAL_UART_Transmit(&huart1, rxBuffer, sizeof(rxBuffer), 1000);
    interval = interval_ms;
    vListInitialise(&allTaskParameterDataList);
    vListInitialise(&timeoutTaskParameterDataList);
    xTaskCreate(Soft_Watchdog_Task, "Soft_Watchdog_Task", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1,
                NULL);
    xTaskCreate(Test_Soft_Watchdog_Task, "Test_Soft_Watchdog_Task", configMINIMAL_STACK_SIZE, NULL,
                tskIDLE_PRIORITY + 1,
                NULL);
    xTaskCreate(Test_Soft_Watchdog_Task1, "Test_Soft_Watchdog_Task1", configMINIMAL_STACK_SIZE, NULL,
                tskIDLE_PRIORITY + 1,
                NULL);
}