#include "Oscilloscope.h"

void Oscilloscope_Grid(void)
{
    for(int16_t i = 0; i <= 104; i+=8)
    {
        for(int16_t j = 0; j < 64; j+=8)
        {
            OLED_DrawPoint(i, j, 1);
        }
    }
}

uint16_t ExtremeData[2] = {0};
//ExtremeData[0] == MAX
//ExtremeData[1] == MIN

uint16_t *GetData()
{
    ExtremeData[0] = 0;//MAX
    ExtremeData[1] = 0xFFFF;//MIN
    for(uint16_t i = 0; i < ADC_BUFFER_SIZE; i++)
    {
        if(current_buffer[i] > ExtremeData[0])
        {
            ExtremeData[0] = current_buffer[i];
        } else if(current_buffer[i] < ExtremeData[1])
        {
            ExtremeData[1] = current_buffer[i];
        }
    }
    return &ExtremeData[0];
}

void Oscilloscope(void)
{
    if(1 == dma_complete)
    {
        dma_complete = 0;//清除标志位
        Oscilloscope_Grid();//示波器网格
        for(int16_t i = 0; i < ADC_BUFFER_SIZE; i++)//写数据
        {
            OLED_DrawPoint(i, 63-current_buffer[i]/4, 1);
        }
        OLED_ShowString(110,0,"MAX",12,1);
        OLED_ShowNum(110,12,*GetData(),3,12,1);//MAX
        OLED_ShowString(110,30,"MIN",12,1);
        OLED_ShowNum(110,42,*(GetData()+1),3,12,1);//MIN
        OLED_Refresh();//写显存
        OLED_ClearFresh();//清除显存
    }
}
