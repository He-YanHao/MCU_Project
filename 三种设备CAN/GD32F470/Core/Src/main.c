#include "main.h"

int main(void)
{
    BSP_LED_Init();
    MyCAN_Init();
    while(1)
    {
        BSP_LED_ALL_TOGGLE();
        Delay_ms(500);
        BSP_LED1_TOGGLE();
        Delay_ms(500);
        BSP_LED2_TOGGLE();
        Delay_ms(500);
        BSP_LED3_TOGGLE();
        Delay_ms(500);
        BSP_LED4_TOGGLE();
        Delay_ms(500);
    }
}
