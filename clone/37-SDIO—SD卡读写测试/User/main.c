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
#define MAX 128 

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
    
    
    static uint8_t W[MAX] = {0};
    static uint8_t R[MAX] = {0};
    for(uint32_t i = 0; i < MAX; i++) {
        W[i] = i;
    }
    printf("W[128] init printf");
    for(uint32_t i = 0; i < MAX; i++) {
        if (i%0xf == 0) {
            printf("\r\n");
        }
        printf("%d  ",W[i]);
    }
    printf("\r\n");
    uint32_t ret;

    ret = SD_WriteBlock(&W[0], 100, 512);
    if (ret != SD_OK) {
        printf("BSP_SD_Write_Block ERROR:%d\r\n", ret);
    }

    ret = SD_ReadBlock(&R[0], 100, 512);
    if (ret != SD_OK) {
        printf("BSP_SD_Read_Block ERROR:%d\r\n", ret);
    }

    printf("R[128] printf");
    for(uint32_t i = 0; i < MAX; i++) {
        if (i%0xf == 0) {
            printf("\r\n");
        }
        printf("%d  ",R[i]);
    }
    printf("\r\nR[128] printf OK\r\n");
    
    while (1)
    {
        LED1_TOGGLE;
        delay();
	}
}
