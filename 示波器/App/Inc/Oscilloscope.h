#ifndef __OSCILLOSCOPE_H
#define __OSCILLOSCOPE_H

#include "stm32f4xx.h"
#include "Delay.h"
#include "MyADC.h"
#include "MyDMA.h"
#include "Oscilloscope_UI.h"

extern uint8_t ADC_Num1[135];
extern uint8_t ADC_Num2[135];
extern uint16_t Delay_Num[];
extern uint8_t Delay_Leve;
extern uint8_t KeyEXTI;

void Oscilloscope_Num(void);
void Oscilloscope(void);
void Delay_Leve_Change(void);

#endif
