#ifndef __MYUSART_H
#define __MYUSART_H

#include "stm32f4xx.h"

void USART1_Init(void);
void USART_SendByte(USART_TypeDef* USARTx, uint8_t data);
void USART_SendString(USART_TypeDef* USARTx, char* str);

#endif
