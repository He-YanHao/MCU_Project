#include "main.h"

int main(void)
{
    BSP_LED_Init();
    MyCAN_Init();
    MyUSART_Init();
    uint8_t tx_data[8] = {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
//    uint8_t tx_data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x0};
    uint8_t MYCAN_SHOU[8] = {};
    uint32_t ID = 0;
    uint8_t length = 0;
    while(1)
    {
        if(key_scan() == 1)
        {
            can_send_message(0x123,tx_data,8);
            for(uint8_t i = 0; i < length; i++)
            {
                tx_data[i]++;
            }
            ID = can_receive_message(&ID,&MYCAN_SHOU[0],&length);
            //USART0_DATA(ID);
            //USART0_DATA(length);
            for(uint8_t i = 0; i < length; i++)
            {
                USART0_DATA(MYCAN_SHOU[i]);
            }            
        }
        BSP_LED_ALL_TOGGLE();
        Delay_ms(200);
    }
}
