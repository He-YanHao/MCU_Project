#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "gd32f4xx.h"
#include "systick.h"


#define CT_MAX_TOUCH 5    // 电容屏支持的点数,固定为5点

typedef struct touch
{
    uint8_t tp_pressed_count; // 按下的触摸点的数量
    uint16_t x[CT_MAX_TOUCH]; // 
    uint16_t y[CT_MAX_TOUCH]; // 
}touch_t;

// 触摸屏操作函数
// 初始化
void FT5206_Init(void);
// 获取坐标
uint8_t FT5206_Scan(void);
// 获得触摸状态结构体的指针
const touch_t *FT5206_GetState(void);

#endif
