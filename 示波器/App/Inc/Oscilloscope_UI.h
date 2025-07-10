#ifndef __OSCILLOSCOPE_UI_H
#define __OSCILLOSCOPE_UI_H

#include "stm32f4xx.h"
#include "LCD.h"
#include "MyDMA.h"

extern uint8_t ADC_Num1[135];
extern uint8_t ADC_Num2[135];
extern uint16_t Delay_Num[];
extern uint8_t Delay_Leve;

void Oscilloscope_UI_Init(void);

void Oscilloscope_OUT(void);
void Oscilloscope_Clear(void);
void Oscilloscope_Change(void);//�ı�ͼ�� �������������һ��ͼ���������������������һ��ͼ��
void Oscilloscope_Num_OUT(void);

#endif
