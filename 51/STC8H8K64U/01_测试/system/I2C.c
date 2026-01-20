#include <AI8H.H>

void I2C_Init()
{
    P3M0 = 0x00; P3M1 = 0x00; //
    P_SW1 &= 0x3f;//确定串口1为P3.0和P3.1

    SCON = 0x50;//可变波特率8位数据方式 允许串口接收数据
    
    T2L = 0xe8;                                 //65536-11059200/115200/4=0FFE8H
    T2H = 0xff;
    AUXR = 0x15;                                //启动定时器
    ES = 1;                                     //使能串口中断
    EA = 1;
    SBUF = 0x5a;                                //发送测试数据
}

void UART1_Isr()// interrupt 4
{
    if (TI)
    {
        TI = 0;                                 //清中断标志
        P30 = !P30;                             //测试端口
    }
    if (RI)
    {
        RI = 0;                                 //清中断标志
        P31 = !P31;                             //测试端口
    }
}
