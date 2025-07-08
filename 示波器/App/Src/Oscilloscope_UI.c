//画一个点 Gui_DrawPoint(x,y,颜色);
//画一条线 Gui_DrawLine(起点x,起点y,终点x,终点y,颜色);

//RED     红色  黑色
//GREEN   绿色  很暗的绿色
//BLUE    蓝色  深蓝色
//WHITE   白色  浅蓝色
//BLACK   黑色  黑色
//YELLOW  黄色  很暗的绿色
//最浅色是WHITE 最深色是BLACK

#include "Oscilloscope_UI.h"

void Oscilloscope_UI_Init(void)
{
    Gui_DrawLine(21,24,21,159,BLACK);
    Gui_DrawLine(21,24,127,24,BLACK);
    for(uint8_t i = 0; i < 8; i++)
    {
        Gui_DrawLine(22+15*i,12,22+15*i,24,BLACK);
    }
}    

void Oscilloscope_OUT(void)
{
    for(uint8_t i = 0; i < 135; i++)
    {
        Gui_DrawPoint(ADC_Num1[i]+22, i+25, BLACK);
    }
}

void Oscilloscope_Clear(void)
{
    for(uint8_t i = 0; i < 135; i++)
    {
        Gui_DrawPoint(ADC_Num2[i]+22, i+25, WHITE);
    }
}
