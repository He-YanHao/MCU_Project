#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "LCD.h"

int main()
{
    board_init();
    LCD_Init();
    LCD_Clear(WHITE);
    while(1)
    {
    LCD_Clear(RED);
    delay_ms(1000);
    LCD_Clear(GREEN);
    delay_ms(1000);
    LCD_Clear(BLUE);
    delay_ms(1000);
    }
}
