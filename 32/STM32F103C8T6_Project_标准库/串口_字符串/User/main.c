#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "USART.h"

int main()
{
    OLED_Init();
	MyUSART_Init();
    uint8_t F[10];
    uint16_t i;
    for(i = 0; i < 10; i++)
    {
    F[i] = i;
    }
    MyUSART_SendString("20000");
    while (1)
    {
//    MyUSART_SendByte(0x88);
//    Delay_ms(1000);
	}
}
