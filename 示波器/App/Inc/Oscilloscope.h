#ifndef __OSCILLOSCOPE_H
#define __OSCILLOSCOPE_H

#include "stm32f4xx.h"
#include "MyADC.h"
#include "MyDMA.h"
#include "Oscilloscope_UI.h"

extern uint8_t ADC_Num1[135];
extern uint8_t ADC_Num2[135];

void Oscilloscope_Num(void);
void Oscilloscope(void);

#endif
