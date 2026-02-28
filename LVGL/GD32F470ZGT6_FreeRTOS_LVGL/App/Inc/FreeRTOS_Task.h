#ifndef __FREERTOS_TASK_H__
#define __FREERTOS_TASK_H__

void FreeRTOS_Start(void);

#define Task_Start_STACK    4096
#define Task_Start_PRIORITY 5
void Task_Start(void *pvParameters);

#define LV_RUNING_STACK       4096
#define LV_RUNING_PRIORITY    3
void LVGL_Runing_Task(void *pvParameters);

#define LED_Toggle_STACK      1024
#define LED_Toggle_PRIORITY   3
void LED_Toggle(void *pvParameters);

#endif
