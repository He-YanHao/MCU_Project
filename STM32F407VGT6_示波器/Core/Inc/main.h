#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "Delay.h"
#include "board.h"
#include "board_LED.h"
#include "OLED.h"
#include "MyADC.h"
#include "Oscilloscope.h"
#include "MyDMA.h"

// ��־����
extern volatile uint8_t dma_complete;       // ������ɱ�־
extern volatile uint8_t *current_buffer;       // ָ��ǰ���õĻ�����

// ��������������
extern volatile uint8_t adc_buffer0[ADC_BUFFER_SIZE];
extern volatile uint8_t adc_buffer1[ADC_BUFFER_SIZE];

#endif
