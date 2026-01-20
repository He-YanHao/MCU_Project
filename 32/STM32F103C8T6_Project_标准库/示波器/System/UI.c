#include "stm32f10x.h"                  // Device header
#include "LCD.h"
#include "LCD_font.h"

void UI_Init(void)
{
    uint8_t i,j;
    Gui_DrawLine(22,24,127,24,BLACK);//
    Gui_DrawLine(22,24,22,159,BLACK);//
    for(i = 22; i < 128; i = i + 20)//Y轴坐标
    {
        Gui_DrawLine(i, 20, i, 24, BLACK);//X轴变为Y轴，Y轴变成X轴
    }
    for(i = 24; i < 160; i = i + 5)//X轴坐标
    {
        for(j = 22; j < 128; j = j + 5)//Y轴坐标
        {
            if( ((i-24)%5 == 0) && ((j-22)%5 == 0))//
            {
                Gui_DrawPoint(j,i,BLACK);//X轴变为Y轴，Y轴变成X轴
            }
        }
    }
}

void UI_buxian(void)
{
    uint8_t i,j;
    Gui_DrawLine(22,24,127,24,BLACK);//
    Gui_DrawLine(22,24,22,159,BLACK);//
    for(i = 24; i < 160; i = i + 5)//X轴坐标
    {
        for(j = 22; j < 128; j = j + 5)//Y轴坐标
        {
            if( ((i-24)%5 == 0) && ((j-22)%5 == 0))//
            {
                Gui_DrawPoint(j,i,BLACK);//X轴变为Y轴，Y轴变成X轴
            }
        }
    }
}
