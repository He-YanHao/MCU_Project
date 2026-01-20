#ifndef __MYCAN_H
#define __MYCAN_H

#include "gd32f4xx.h"
//#include "BSP_LED.h"
#include "Delay.h"
#include "usart.h"

void MyCAN_Init(void);
uint8_t can_send_message(uint32_t id, uint8_t *data, uint8_t length);
uint8_t can_receive_message(uint32_t *id, uint8_t *data, uint8_t *length);

#endif
