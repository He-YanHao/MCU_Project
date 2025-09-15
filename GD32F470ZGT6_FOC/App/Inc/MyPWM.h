#ifndef __MYPWM_H
#define __MYPWM_H

#include "gd32f4xx.h"

void TIMER0_GPIO_PWM_Init(void);
void MyPWM_Init(void);
void Change_PWM0(uint32_t PWM);
void Change_PWM1(uint32_t PWM);
void Change_PWM2(uint32_t PWM);
void Change_PWM3(uint32_t PWM);

#endif
