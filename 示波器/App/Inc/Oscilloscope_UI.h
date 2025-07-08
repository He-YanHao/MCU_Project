#ifndef __OSCILLOSCOPE_UI_H
#define __OSCILLOSCOPE_UI_H

#include "stm32f4xx.h"
#include "LCD.h"
#include "MyDMA.h"

extern uint8_t ADC_Num1[135];
extern uint8_t ADC_Num2[135];

void Oscilloscope_UI_Init(void);

void Oscilloscope_OUT(void);
void Oscilloscope_Clear(void);

#endif
