#include "touch.h"

// 初始化FT5206触摸屏
void FT5206_Init(void)
{
}

// 
touch_t g_touch_status = {0};

// 
uint8_t FT5206_Scan(void)
{

    return 0;
}

// 获得触摸状态结构体的指针
const touch_t *FT5206_GetState(void)
{
    return &g_touch_status;
}
