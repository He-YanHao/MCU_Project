#include "main.h"

int main(void)
{
    board_Init();//开发板上所有外设初始化
    LCD_Init();
    MyADC_Init();
    LCD_Clear(WHITE);
    Oscilloscope_UI_Init();
    while(1)
    {
        Oscilloscope();
        //Delay_Leve_Change();
        //Oscilloscope_Num_OUT();
    }
}
