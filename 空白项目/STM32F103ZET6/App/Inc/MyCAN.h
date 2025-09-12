#ifndef __MYCAN_H
#define __MYCAN_H

void MyCAN_Init(void);
void MyCAN_Transmit(uint32_t ID, uint8_t Length, uint8_t *Data);//发送
uint8_t MyCAN_ReceiveFlag(void);
void MyCAN_Receive(uint32_t *ID, uint8_t *Length, uint8_t *Data);//接收

#endif
