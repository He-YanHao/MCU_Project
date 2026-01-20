#ifndef __TM1637_H
#define __TM1637_H

void MyI2C_Init(void);//GPIO初始化
void I2C2_W_SCL(uint8_t BitValue);//SCL写
void I2C2_W_SDA(uint8_t BitValue);//SDA写
uint8_t I2C2_R_SDA(void);//SDA读
void MyI2C_Start(void);//I2C的开始
void MyI2C_Stop(void);//I2C的结束
void TM1637_ACK(void);//TM1637的应答
void MyI2C_SendByte(uint8_t Byte);//I2C发送一个字节
uint8_t ReadKey(void);//读按键值
void TM1637(void);

#endif
