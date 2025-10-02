#ifndef __MYPWM_H
#define __MYPWM_H

#include "stm32f4xx.h"
#include "Delay.h"

#define PWM_MAX 4095
#define PWM_MIN 0
#define DELAY Delay_us(50)

void MyPWM_Init(void);

void Change_TIM2_PWM1(uint16_t PWM);
void Change_TIM2_PWM2(uint16_t PWM);
void Change_TIM2_PWM3(uint16_t PWM);
void Change_TIM2_PWM4(uint16_t PWM);

void Change_TIM3_PWM1(uint16_t PWM);
void Change_TIM3_PWM2(uint16_t PWM);
void Change_TIM3_PWM3(uint16_t PWM);
void Change_TIM3_PWM4(uint16_t PWM);

void PWM_ALL_Change(uint8_t PWM);
void PWM_ALL_MAX(void);
void PWM_ALL_MIN(void);

#endif
