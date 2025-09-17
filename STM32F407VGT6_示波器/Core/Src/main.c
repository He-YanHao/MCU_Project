#include "main.h"

int main(void)
{
    board_Init();//开发板上所有外设初始化
    Delay_ms(50);
    OLED_Init();
    OLED_Clear();
    MyADC_Init();
    MyDMA_Init();
    //ADC开始扫描
    ADC_SoftwareStartConv(ADC1);
    while(1)
    {
        Oscilloscope();
        //printf("%d\n",dma_complete);
    }
}
