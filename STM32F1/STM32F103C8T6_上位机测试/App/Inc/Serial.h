#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h"                  // Device header
#include "BSP_LED.h"                  // Device header
#include "OLED.h"

void Serial_Init(void);

void Serial_SendByte(uint8_t Byte);//串口发送一个字节
void Serial_SendArray(uint16_t Length, uint8_t *Array);//串口发送一个数组
void Serial_SendString(char *String);//串口发送一个字符串
void Serial_SendPack(uint16_t Length, uint8_t *Array);//发送数据包


#endif
