#ifndef __BOARD_KEY_H
#define __BOARD_KEY_H

#include "stm32f4xx.h"
#include "Delay.h"

typedef enum{
    KEY0,//按键1
    KEY1 //按键2
}KEY_NUM;

typedef enum {
    BUTTON_EVENT_NONE,      // 无事件
    BUTTON_EVENT_PRESS,     // 按下
    BUTTON_EVENT_RELEASE,   // 释放
    BUTTON_EVENT_CLICK,     // 单击
    BUTTON_EVENT_DOUBLE_CLICK, // 双击
    BUTTON_EVENT_LONG_PRESS,   // 长按
    BUTTON_EVENT_HOLD       // 保持按下
} ButtonEvent;

#define  KEY_TIM    RCU_TIMER5  // 定时器时钟

void Key_Init(void);
void KeyEXTI_Init(void);
KEY_NUM Key_scan(void);

//extern uint16_t Delay_Num[];
//extern uint8_t Delay_Leve;
//extern uint8_t KeyEXTI;

#endif
