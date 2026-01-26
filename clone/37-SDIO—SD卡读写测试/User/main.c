#include "stm32f10x.h"
#include "./sdio/bsp_sdio_sdcard.h"
#include "./sdio/sdio_test.h"
#include "./usart/bsp_usart.h"	
#include "./led/bsp_led.h"
#include "./key/bsp_key.h" 

void delay(void)
{
    for(uint32_t i = 0; i < 7200; i++)
    {
        for(uint32_t j = 0; j < 1000; j++)
        {
        }
    }
}

int main(void)
{				
    uint32_t num = 99;
    LED_GPIO_Config();
	LED_BLUE;	
	Key_GPIO_Config();
    /*³õÊ¼»¯USART1*/
    USART_Config();
	printf("STM32F103ZET6\r\n");	
    num = SD_Init();
    printf("SD_Init:%d\r\n", num);

    while (1)
    {
        LED1_TOGGLE;
        delay();
	}
}
