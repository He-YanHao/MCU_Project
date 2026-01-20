#ifndef __DHT11_H
#define __DHT11_H

void DHT11_OUT_Init();
void DHT11_IN_Init(void);
void DHT11_Start(void);//主机发送开始信号
uint8_t DHT11_Rec_Byte(void);//获取一个字节
void DHT11_REC_Data(void);//获取数据

#endif
