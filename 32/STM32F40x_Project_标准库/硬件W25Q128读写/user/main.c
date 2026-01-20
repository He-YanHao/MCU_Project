#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "W25Q128.h"
#include "LCD.h"
#include "shui.h"

int main(void)
{
	board_init();
	W25Q128_init();
	LCD_Init();
	LCD_Clear(RED);
//	uint8_t numW[10] = {0};//Ð´
//	uint8_t numR[10] = {0};//¶Á
//	uint16_t i = 0;
//	for(i = 0; i < 10; i++)
//	{
//		numW[i] = i;//Ð´
//	}
//	W25Q128_erase_sector(0);//²Á³ýiÇøÓò
//	W25Q128_write_enable();//Ð´Ê¹ÄÜ
//	W25Q128_wait_busy();//Ã¦¼ì²â
//	W25Q128_write(numW, 0, 10);
//	W25Q128_read(numR, 0, 10);
//	for(i = 0; i < 10; i++)
//	{
//		Gui_DrawFont_Number(i*8+2, 0, BLACK, WHITE, numW[i]);
//	}	
//	for(i = 0; i < 10; i++)
//	{
//		Gui_DrawFont_Number(i*8+2, 16, BLACK, WHITE, numR[i]);
//	}
	Pic(0, 0, 127, 159, gImage_shui);
    while(1) 
    {

    }
}
