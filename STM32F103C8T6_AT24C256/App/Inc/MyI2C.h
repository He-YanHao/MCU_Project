#ifndef __MYI2C_H
#define __MYI2C_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define DELAY_NUM 10

void My_I2C_GPIO_Init(void);         //引脚初始化

void Write_SCL(uint8_t bit);         //写时钟
void Write_SDA(uint8_t bit);         //写数据
uint8_t Read_SDA(void);              //读数据

void I2C_Start(void);                //I2C开始
void I2C_Stop(void);                 //I2C结束
void MyI2C_SendAck(uint8_t AckBit);  //I2C发送应答位AckBit，0表示应答，1表示非应答。
uint8_t MyI2C_ReceiveAck(void);//I2C接收应答位，0表示应答，1表示非应答

void I2C_SendByte(uint8_t Byte);     //发送数据
uint8_t I2C_SendBytePack(uint8_t ADDR, uint8_t *Data, uint8_t Size);
void I2C_ReceiveByte(uint8_t Byte);  //接收数据



#endif
