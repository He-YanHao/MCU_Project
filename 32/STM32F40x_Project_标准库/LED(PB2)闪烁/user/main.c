#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "LCD.h"
#include "LED.h"

int main()
{
    board_init();
    LCD_Init();
    LED_GPIO_Init();
    while(1)
    {
        LED_GPIO_Liang();
        delay_ms(500);
        LED_GPIO_Mie();
        delay_ms(500);
    }
}
