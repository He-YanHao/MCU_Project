#ifndef __MYPWM_H
#define __MYPWM_H

#include "stm32f4xx.h"

void MyPWM_Init(void);

void Change_PWM1(uint16_t PWM);
void Change_PWM2(uint16_t PWM);
void Change_PWM3(uint16_t PWM);


#endif
