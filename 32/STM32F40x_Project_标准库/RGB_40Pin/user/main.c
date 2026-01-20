#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "LCD.h"

int main()
{
    board_init();
    LCD_GPIO_Init();
    //ceshi();
    while(1)
    {
        ceshi2();
    }
}
