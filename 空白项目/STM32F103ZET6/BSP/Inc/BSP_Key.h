#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

enum KEY_NUM{
    NO_KEY,
    KEY0,
    KEY1,
};

void Key_Init(void);
uint8_t Key_GetNum(void);

#endif
