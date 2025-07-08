#ifndef __MYDMA_H
#define __MYDMA_H

#include "stm32f4xx.h"

extern uint8_t ADC_Num1[135];
extern uint8_t ADC_Num2[135];

void SPI_DMA(uint32_t txBuffer, uint8_t BUFFER_SIZE);
void SPI_DMA_NVIC_Configuration(void);
void DMA2_Stream3_IRQHandler(void);

void DMA_135(void);
void DMA_135_NVIC_Configuration(void);
void DMA2_Stream1_IRQHandler(void);


#endif
