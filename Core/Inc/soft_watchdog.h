//
// Created by ubuntu on 2023/8/30.
//

#ifndef MYSTM32PROJ_SOFT_WATCHDOG_H
#define MYSTM32PROJ_SOFT_WATCHDOG_H

#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
#include "semphr.h"

typedef struct {
    //task函数
    TaskFunction_t taskFun;
    //list链表指针
    ListItem_t listItem;
    //task参数
    void *taskCreateParameters;
    //task handle
    TaskHandle_t *taskHandle;
    //优先级
    UBaseType_t uxPriority;
    //超时时间
    int timeout_ms;
    //任务名称
    const char name[configMAX_TASK_NAME_LEN];
    //刷新的时间
    uint32_t refeshTick;
    configSTACK_DEPTH_TYPE usStackDepth;
} CreateTaskParametersData;

/*
 * 启用软件狗。
 * interval_ms 为软件狗的扫描周期，请根据需要设定
 */
void Enable_System_Soft_Watchdog(int interval_ms);

CreateTaskParametersData *
Register_Soft_WatchDog(TaskFunction_t taskFun, void *taskParameter, const configSTACK_DEPTH_TYPE usStackDepth,
                       int timeout_ms);

void Reflesh_Soft_WatchDog(CreateTaskParametersData * data);
#endif //MYSTM32PROJ_SOFT_WATCHDOG_H
