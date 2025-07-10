#ifndef __BOARD_KEY_H
#define __BOARD_KEY_H

#include "stm32f4xx.h"
#include "LCD.h"

typedef enum{
    KEY0,KEY1
}KEY_NUM;

#define  KEY_TIM    RCU_TIMER5  // ¶¨Ê±Æ÷Ê±ÖÓ

void Key_Init(void);
void KeyEXTI_Init(void);
KEY_NUM Key_scan(void);

extern uint16_t Delay_Num[];
extern uint8_t Delay_Leve;
extern uint8_t KeyEXTI;

#endif
