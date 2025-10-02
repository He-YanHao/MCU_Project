#include "main.h"
#include "DHT11.h"
#include <stdio.h>

extern uint8_t rec_data[4];

int main(void)
{
    Delay_Init();
    board_BSP_USART_Init();
    Delay_ms(1000);  // 等待系统稳定
    DHT11_Init();
    
    printf("DHT11 Test Start...\n");
    
    while(1)
    {
        uint8_t result = DHT11_Read_Data_Diagnostic();
        
        if(result == 1)
        {
            float humidity = rec_data[0] + rec_data[1] * 0.1f;
            float temperature = rec_data[2] + rec_data[3] * 0.1f;
            
            printf("湿度: %.1f%%, 温度: %.1f℃\n", humidity, temperature);
            printf("原始数据: %d, %d, %d, %d\n", 
                   rec_data[0], rec_data[1], rec_data[2], rec_data[3]);
        }
        else
        {
            printf("读取失败，错误代码: %d\n", result);
        }
        
        Delay_ms(3000);  // 每次读取间隔3秒
    }
}