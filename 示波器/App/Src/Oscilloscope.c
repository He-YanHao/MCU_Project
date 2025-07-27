#include "Oscilloscope.h"

//ADC_Num1接收数据
void Oscilloscope_Num(void)
{
    for(uint8_t i = 0; i < 135; i++)
    {
        ADC_Num1[i] = Get_ADC_Value()/39;
        Delay_us(Delay_Num[Delay_Leve]);
    }
}

void Num1_to_Num2(void)
{
    for(uint8_t i = 0; i < 135; i++)
    {
        ADC_Num2[i] = ADC_Num1[i];
    }
}

void Delay_Leve_Change(void)
{
    if(KeyEXTI == 1)
    {
        if(Delay_Leve >= 11)
        {
            Delay_Leve = 0;
        }
        else
        {
            Delay_Leve++;
        }
        while(KeyEXTI != 0)// 等待定时器复位标志
        { 
            
        }
    }
}

void Oscilloscope(void)
{
    Oscilloscope_Num();//接收数据
    Oscilloscope_Change();//改变图像 清除单个像素上一次图像后立即制作单个像素这一次图像
    Num1_to_Num2();//搬运数据
}
