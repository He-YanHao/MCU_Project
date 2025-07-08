#include "Oscilloscope.h"

//ADC_Num1接收数据
void Oscilloscope_Num(void)
{
    for(uint8_t i = 0; i < 135; i++)
    {
        ADC_Num1[i] = Get_ADC_Value()/39;
    }
}
void Num1_to_Num2(void)
{
    for(uint8_t i = 0; i < 135; i++)
    {
        ADC_Num2[i] = ADC_Num1[i]; 
    }
}

void Oscilloscope(void)
{
    Oscilloscope_Num();//接收数据
    Oscilloscope_Clear();//清除上一次图像
    Oscilloscope_OUT();//制作这一次图像
    Num1_to_Num2();//搬运数据
//    Gui_DrawFont_GBK16(0, 0, BLACK, WHITE, "    ");
//    Gui_DrawFont_Number(0, 0, BLACK, WHITE, Get_ADC_Value());
    //DMA_135();//搬运数据
}
