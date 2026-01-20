#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>

void MyUSART_Init(void);//串口初始化
void MyUSART_SendByte(uint8_t Byte);//串口发送一个字节
void MyUSART_SendArray(uint8_t *Array, uint16_t Length);//串口发送一个数组
void MyUSART_SendString(char *String);//串口发送一个字符串
//void Serial_Printf(char *format, ...);

#endif
