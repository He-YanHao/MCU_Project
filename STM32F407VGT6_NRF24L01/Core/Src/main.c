#include "main.h"

#define RECEIVING_MODE        0// 1:接收模式 0：发送模式

volatile uint8_t g_RF24L01RxBuffer[30];
//usart_send_String
int main(void)
{
    board_Init();//开发板上所有外设初始化
    //SPI初始化
    NRF24L01_SPI_Init( );//
    #if RECEIVING_MODE

    NRF24L01_Gpio_Init_receive( );
    //检测nRF24L01
    NRF24L01_check( );
    //NRF初始化
    RF24L01_Init( );
    usart_send_data(NRF24L01_Read_Status_Register());
    RF24L01_Set_Mode( MODE_RX );//NRF接收模式        .
    printf("MODE_RX\r\n");
    while(1)
    {
        if( 0 != g_RF24L01RxBuffer[0])
        {
            printf("Data = %s\r\n",g_RF24L01RxBuffer);
            Buff_Clear();
        }
    }

    #else

    NRF24L01_Gpio_Init_transmit( );
    //检测nRF24L01
    NRF24L01_check( );
    //NRF初始化
    RF24L01_Init( );
    RF24L01_Set_Mode( MODE_TX );//NRF发送模式        .
    printf("MODE_TX\r\n");
    uint8_t YES_NO = 0;
    while(1)
    {
        YES_NO = NRF24L01_TxPacket((uint8_t*)"hello LCKFB!\r\n",13);//NRF发送数据
        usart_send_data(YES_NO);
        printf("Send\r\n");
        Delay_ms(200);
    }
    #endif
}
