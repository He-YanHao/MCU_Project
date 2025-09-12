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
    Gui_DrawLine(20,24,20,159,BLACK);//时间
    Gui_DrawLine(20,24,127,24,BLACK);//大小
    for(uint8_t i = 0; i < 8; i++)
    {
        Gui_DrawLine(21+15*i,12,21+15*i,24,BLACK);//大小
    }
    for(uint8_t ii = 0; ii < 10; ii++)
    {
        Gui_DrawLine(11,24+15*ii,20,24+15*ii,BLACK);//时间
    }
}

void Oscilloscope_OUT(void)//制作这一次图像
{
    for(uint8_t i = 0; i < 135; i++)
    {
        Gui_DrawPoint(ADC_Num1[i]+22, i+25, BLACK);
    }
}

void Oscilloscope_Clear(void)//清除上一次图像
{
    for(uint8_t i = 0; i < 135; i++)
    {
        Gui_DrawPoint(ADC_Num2[i]+22, i+25, WHITE);
    }
}

void Oscilloscope_Change(void)//改变图像 清除单个像素上一次图像后立即制作单个像素这一次图像
{
    for(uint8_t i = 0; i < 135; i++)
    {
        Gui_DrawPoint(ADC_Num2[i]+21, i+25, WHITE);
        Gui_DrawPoint(ADC_Num1[i]+21, i+25, BLACK);
    }
}

uint8_t Delay_Leve_LiShi = 0;

void Oscilloscope_Num_OUT(void)
{
    if(Delay_Leve_LiShi != Delay_Leve)
    {
        Delay_Leve_LiShi = Delay_Leve;
        Gui(0, 0, 15, 15, WHITE);//清除
        Gui_DrawFont_Number(0, 0, BLACK, WHITE, Delay_Leve);//档次
        Gui(30, 0, 62, 15, WHITE);//清除
        Gui_DrawFont_Number(30, 0, BLACK, WHITE, Delay_Num[Delay_Leve]);//时间
    }
}
