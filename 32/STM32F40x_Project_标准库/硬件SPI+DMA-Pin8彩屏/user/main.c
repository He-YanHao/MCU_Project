#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "LCD.h"
#include "MyDMA.h"

uint32_t a[4] = {1,2,3,4};
uint32_t b[4] = {0,0,0,0};

int main()
{
    board_init();
    LCD_Init();
    LCD_Clear(RED);
    MyDMA_Init((uint32_t)a, (uint32_t)b, 4);
    //while (DMA_GetFlagStatus(DMA2_Stream0,DMA_FLAG_TCIF0)==DISABLE)
    {
    }
    for(uint8_t i = 0; i < 4; i++)
    {
        Gui_DrawFont_Number(20*i,0,BLACK,WHITE,a[i]);
        Gui_DrawFont_Number(20*i,20,BLACK,WHITE,b[i]);
    }
    while(1)
    {
        //LCD_Clear(RGB16);
        //RGB16++;
    }
}
