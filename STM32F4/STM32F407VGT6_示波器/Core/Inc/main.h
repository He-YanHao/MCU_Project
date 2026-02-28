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

// 标志变量
extern volatile uint8_t dma_complete;       // 传输完成标志
extern volatile uint8_t *current_buffer;       // 指向当前可用的缓冲区

// 定义两个缓冲区
extern volatile uint8_t adc_buffer0[ADC_BUFFER_SIZE];
extern volatile uint8_t adc_buffer1[ADC_BUFFER_SIZE];

#endif
