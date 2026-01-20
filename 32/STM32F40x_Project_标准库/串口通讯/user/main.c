#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "LCD.h"
#include "uart.h" 

int main()
{
    board_init();
    //uart1_init();
    LCD_Init();
    LCD_Clear(WHITE);
	Gui_DrawFont_GBK16(10,10,RED,BLUE,(uint8_t *)"电若色");
	//Gui_DrawFont_GBK16(10,10,RED,BLUE,"CSAGIYAGEVWG");
    
    while(1)
    {
    
    }
}
