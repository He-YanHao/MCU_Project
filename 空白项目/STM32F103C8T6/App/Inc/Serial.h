#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h"                  // Device header
#include "BSP_LED.h"                  // Device header
#include "OLED.h"

void Serial_Init(void);

void Serial_SendByte(uint8_t Byte);//���ڷ���һ���ֽ�
void Serial_SendArray(uint16_t Length, uint8_t *Array);//���ڷ���һ������
void Serial_SendString(char *String);//���ڷ���һ���ַ���
void Serial_SendPack(uint16_t Length, uint8_t *Array);//�������ݰ�


#endif
