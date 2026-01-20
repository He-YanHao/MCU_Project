#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "stm32f4xx.h"

//外部可调用函数的声明
void uart1_init(uint32_t __Baud);
void USART1_IRQHandler(void);
void usart_send_data(uint8_t ucch);
void usart_send_String(uint8_t *ucstr);
int fputc(int ch, FILE *f);

#endif
