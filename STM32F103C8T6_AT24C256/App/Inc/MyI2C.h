#ifndef __MYI2C_H
#define __MYI2C_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define DELAY_NUM 10

void My_I2C_GPIO_Init(void);         //���ų�ʼ��

void Write_SCL(uint8_t bit);         //дʱ��
void Write_SDA(uint8_t bit);         //д����
uint8_t Read_SDA(void);              //������

void I2C_Start(void);                //I2C��ʼ
void I2C_Stop(void);                 //I2C����
void MyI2C_SendAck(uint8_t AckBit);  //I2C����Ӧ��λAckBit��0��ʾӦ��1��ʾ��Ӧ��
uint8_t MyI2C_ReceiveAck(void);//I2C����Ӧ��λ��0��ʾӦ��1��ʾ��Ӧ��

void I2C_SendByte(uint8_t Byte);     //��������
uint8_t I2C_SendBytePack(uint8_t ADDR, uint8_t *Data, uint8_t Size);
void I2C_ReceiveByte(uint8_t Byte);  //��������



#endif
