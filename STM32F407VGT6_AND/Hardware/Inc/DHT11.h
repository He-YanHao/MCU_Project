#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f4xx.h"
#include "Delay.h"

#define DHT11_UP     GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define DHT11_LOW    GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define DATA_GPIO_IN GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)

extern uint8_t rec_data[4];

void DHT11_Init(void);
void DHT11_Mode_OUT(void);
void DHT11_Mode_IN(void);
uint8_t DHT11_Read_Data(void);  // ·µ»Ø: 0=Ê§°Ü, 1=³É¹¦
uint8_t DHT11_Read_Data_Diagnostic(void);

#endif