#include "main.h"

#define MAX 128 

int main()
{
    uint32_t ret;
    Delay_ms(50);

    BSP_USART_init();
    printf("\r\n\r\n\r\n\r\nSTM32F103C8T6\r\n");
    LED_Init();
    BSP_RTC_Init();
    ret = BSP_SD_card_init();
    if (ret != SD_OK) {
        printf("BSP_SD_card_init re is :%d\r\n", ret);
    }

    static uint32_t W[MAX] = {0};
    static uint32_t R[MAX] = {0};
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

    ret = BSP_SD_Write_Block(&W[0], 100);
    if (ret != SD_OK) {
        printf("BSP_SD_Write_Block ERROR:%d\r\n", ret);
    }

    ret = BSP_SD_Read_Block(&R[0], 100);
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

    while(1)
    {
        LED0_Turn();
        Delay_ms(200);
        LED1_Turn();
        Delay_ms(200);
    }
}
