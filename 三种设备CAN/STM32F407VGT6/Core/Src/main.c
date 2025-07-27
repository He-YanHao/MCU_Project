#include "main.h"

int main(void)
{
    board_Init();//开发板上所有外设初始化
    MyCAN_Init();
    USART1_Init();
    // 示例：发送测试消息
    //uint8_t F_Data[8] = {0x88, 0x99, 0xcf, 0x44, 0x00};
    //uint8_t S_Data[8] = {};

    while(1)
    {

    }
}
