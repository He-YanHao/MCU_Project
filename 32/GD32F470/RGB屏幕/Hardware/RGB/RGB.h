#ifndef __RGB_H
#define __RGB_H

#include "gd32f4xx.h"
#include "systick.h"
#include <stdint.h>
//#include "exmc_sdram.h"

void RGB_BL_Init(void);//BL背光初始化
void RGB_BL_OFF(void);//BL背光关闭
void RGB_BL_ON(void);//BL背光开启
void RGB_TLT_Init(void);//TFT外设开启
static void ipa_config(void);
void RGB_data_Init(void);//数据位初始化

#define HORIZONTAL_SYNCHRONOUS_PULSE 10
#define HORIZONTAL_BACK_PORCH 150
#define ACTIVE_WIDTH 800
#define HORIZONTAL_FRONT_PORCH 15

#define VERTICAL_SYNCHRONOUS_PULSE 10
#define VERTICAL_BACK_PORCH 140
#define ACTIVE_HEIGHT 480
#define VERTICAL_FRONT_PORCH 40

#define LCD_FRAME_BUF_ADDR 0XC0000000


#endif
