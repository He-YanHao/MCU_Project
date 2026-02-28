#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"                  // Device header

void BSP_LED_Init(void);
void BSP_LED_ON(void);
void BSP_LED_OFF(void);
void BSP_LED_Turn(void);

#endif
