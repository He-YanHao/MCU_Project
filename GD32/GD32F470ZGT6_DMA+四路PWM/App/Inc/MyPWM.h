#ifndef __MYPWM_H
#define __MYPWM_H

#include "gd32f4xx.h"

void TIMER0_GPIO_PWM_Init(void);
void MyPWM_Init(void);
void PWM_(uint32_t PWM);

#endif
