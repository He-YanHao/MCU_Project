#include "main.h"

int main()
{
    uint32_t ID = 0xAA;
    uint8_t Length = 8;
    uint8_t Data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x0};

    uint32_t SHOU_ID = 0;
    uint8_t SHOU_Length = 0;
    uint8_t SHOU_Data[8] = {};
      
    LED_Init();
    MyCAN_Init();
    Serial_Init();
    MyCAN_Transmit(ID, Length, &Data[0]);
    Delay_ms(500);
    MyCAN_Receive(&SHOU_ID, &SHOU_Length, &SHOU_Data[0]);

    Serial_SendArray(SHOU_Data,SHOU_Length);
    LED0_Turn();
    while(1)
    {
        
    }
}
