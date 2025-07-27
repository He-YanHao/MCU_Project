#ifndef __MYCAN_H
#define __MYCAN_H

#include "stm32f4xx.h"
#include "MyUSART.h"

void MyCAN_Init(void);
uint8_t CAN_SendMessage(uint32_t id, uint8_t* data, uint8_t length);


#endif
