#ifndef __OSCILLOSCOPE_H
#define __OSCILLOSCOPE_H

#include "stm32f4xx.h"
#include "OLED.h"
#include "MyDMA.h"
#include "stdio.h"

// 定义两个缓冲区
extern volatile uint8_t adc_buffer0[ADC_BUFFER_SIZE];
extern volatile uint8_t adc_buffer1[ADC_BUFFER_SIZE];

// 标志变量
extern volatile uint8_t dma_complete;       // 传输完成标志
extern volatile uint8_t *current_buffer;       // 指向当前可用的缓冲区

void Oscilloscope_Grid(void);
void Oscilloscope(void);

#endif
