#include "stm32f4xx.h"

#ifndef __BOARD_H__
#define __BOARD_H__

void board_init(void);
void delay_us(uint32_t _us);
void delay_ms(uint32_t _ms);
void delay_s(uint32_t s);

#endif
