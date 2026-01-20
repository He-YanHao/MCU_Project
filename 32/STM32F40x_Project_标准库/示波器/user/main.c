#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "LCD.h"
#include "MyADC.h"
#include "UI.h"

int main(void)
{
    board_init();
    LCD_Init();
    LCD_Clear(WHITE);
    MyADC_Init();
    UI_Init();
    uint16_t WD[140] = {0};
    uint16_t CC[140] = {0};
    uint8_t i;
    //Gui_DrawFont_heng(127,0,BLACK,WHITE,"s0d");
    while(1)
    {
        
        for(i =0; i < 140; i ++)//获取ADC值
        {
            WD[i] = Get_ADC_Value(ADC_Channel_5);
            //delay_ms();
        }

        for(i =0; i < 136; i ++)//清除图像
        {
            Gui_DrawPoint(CC[i+4]/39+22, i + 25, WHITE);
            Gui_DrawLine(CC[i+3]/39+22, i + 24, CC[i+4]/39+22, i + 25, WHITE);
        }

        for(i = 3; i < 140; i ++)//保存ADC值
        {
        CC[i] = WD[i];
        }
        
        for(i = 0; i < 136; i ++)//写入图像
        {
            Gui_DrawPoint(WD[i+4]/39+22, i + 25, BLACK);
            Gui_DrawLine(WD[i+3]/39+22, i + 24, WD[i+4]/39+22, i + 25, BLACK);
        }
        
        UI_buxian();
    }
}
