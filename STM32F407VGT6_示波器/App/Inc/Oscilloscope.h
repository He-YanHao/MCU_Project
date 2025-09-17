#ifndef __OSCILLOSCOPE_H
#define __OSCILLOSCOPE_H

#include "stm32f4xx.h"
#include "OLED.h"
#include "MyDMA.h"
#include "stdio.h"

// ��������������
extern volatile uint8_t adc_buffer0[ADC_BUFFER_SIZE];
extern volatile uint8_t adc_buffer1[ADC_BUFFER_SIZE];

// ��־����
extern volatile uint8_t dma_complete;       // ������ɱ�־
extern volatile uint8_t *current_buffer;       // ָ��ǰ���õĻ�����

void Oscilloscope_Grid(void);
void Oscilloscope(void);

#endif
