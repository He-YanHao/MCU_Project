#ifndef __BOARD_LED_H
#define __BOARD_LED_H

#include "stm32f4xx.h"
#include "stdio.h"

void BSP_LED_Init(void);
void BSP_LED_ON(void);
void BSP_LED_OFF(void);
void BSP_LED_Toggle(void);

#endif
