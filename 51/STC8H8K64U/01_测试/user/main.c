#include <AI8051U.H>                //包含AI8051U的头文件
#include "delay.h"
#include "I2C.h"

//void MCU_Init();

//void MCU_Init()
//{
////    HIRCCR |= 80;//使能内部高精度IRC
////    while(~(HIRCCR&0x01));//查询振荡器稳定标志位HIRCST
//    IRCBAND |= 0x02;//
//}

void main(void)
{
    //MCU_Init();
    P40 = 0;//打开LED部分的供电
    P0M0 = 0xff; P0M1 = 0x00;  //P0为推挽输出

//    sbit BULE = P0^3;
//    sbit RED = P0^4;
    delay_s(1);
    I2C_Init();
    while(1)
    {
        P0 |= 0x08; //置位P0.3
        P0 |= 0x10; //置位P0.4
        delay_s(1);
        P0 |= 0x08; //置位P0.3
        P0 &= 0xef; //清除P0.4
        delay_s(1);
        P0 &= 0xf7; //清除P0.3
        P0 |= 0x10; //置位P0.4
        delay_s(1);
        P0 &= 0xf7; //清除P0.3
        P0 &= 0xef; //清除P0.4
        delay_s(1);
    }
}
