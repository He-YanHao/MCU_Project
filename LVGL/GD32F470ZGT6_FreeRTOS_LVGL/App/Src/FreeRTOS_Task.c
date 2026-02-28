#include "FreeRTOS_Task.h"
#include "BSP_LED.h"
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"
// GUI
#include "gif_animation.h"
#include "cube_surround.h"

// 任务句柄
TaskHandle_t Task_Start_TaskHandle;
TaskHandle_t LED_Toggle_TaskHandle;
TaskHandle_t LVGL_Task_Handler;

void FreeRTOS_Start(void)
{
    xTaskCreate((TaskFunction_t)Task_Start,               // 任务函数的地址
                (char *)"Task_Start",                     // 任务名字符串
                (configSTACK_DEPTH_TYPE)Task_Start_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)Task_Start_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&Task_Start_TaskHandle);  // 任务句柄的地址
    /* 启动调度器:会自动创建空闲任务 */
    vTaskStartScheduler();
}

void Task_Start(void *pvParameters)
{
    /* 进入临界区 */
    taskENTER_CRITICAL();
    /* 创建任务 */
    xTaskCreate((TaskFunction_t)LED_Toggle,      // 任务函数的地址
                (char *)"LED_Toggle",            // 任务名字符串
                (configSTACK_DEPTH_TYPE)LED_Toggle_STACK, // 任务栈大小，默认最小128，单位4字节
                (void *)NULL,                             // 传递给任务的参数
                (UBaseType_t)LED_Toggle_PRIORITY,         // 任务的优先级
                (TaskHandle_t *)&LED_Toggle_TaskHandle);      // 任务句柄的地址

    /* 创建LVGL任务 */ 
    xTaskCreate((TaskFunction_t )LVGL_Runing_Task,
                (const char*    )"LVGL_Runing_Task",
                (uint16_t       )LV_RUNING_STACK,
                (void*          )NULL,
                (UBaseType_t    )LV_RUNING_PRIORITY,
                (TaskHandle_t*  )&LVGL_Task_Handler);
    /* 退出临界区 */
    taskEXIT_CRITICAL();
    vTaskDelete(NULL);
}

/** 
 * @brief       LVGL运行例程 
 * @param       pvParameters : 传入参数(未用到) 
 * @retval      无 
 */ 
void LVGL_Runing_Task(void *pvParameters) 
{
    while(1) 
    {
        /* LVGL计时器 */
        lv_timer_handler();
        vTaskDelay(5); 
    }
}

void LED_Toggle(void *pvParameters)
{
    while(1)
    {
        BSP_LED_ALL_TOGGLE();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
