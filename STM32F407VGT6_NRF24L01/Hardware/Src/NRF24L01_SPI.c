#include "NRF24L01_SPI.h"

/** 硬件SPI */
#define SPI_WAIT_TIMEOUT       /*超时等待时间*/     ((uint16_t)0xFFFF)

/**
  * @brief :SPI初始化(硬件)
  * @param :无
  * @note  :无
  * @retval:无
  */
void NRF24L01_SPI_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* 使能 GPIO 时钟 */
    RCC_AHB1PeriphClockCmd (SPI_CLK_GPIO_CLK, ENABLE);
    /* 设置引脚复用 */
    GPIO_PinAFConfig(SPI_CLK_GPIO_PORT, GPIO_PinSource13, LINE_AF_SPI);
    GPIO_PinAFConfig(SPI_MISO_GPIO_PORT, GPIO_PinSource14, LINE_AF_SPI);
    GPIO_PinAFConfig(SPI_MOSI_GPIO_PORT, GPIO_PinSource15, LINE_AF_SPI);
    /* 配置SPI引脚引脚SCK */
    GPIO_InitStructure.GPIO_Pin = SPI_CLK_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(SPI_CLK_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits( SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN );
    /* 配置SPI引脚MISO */
    GPIO_InitStructure.GPIO_Pin = SPI_MISO_GPIO_PIN;
    GPIO_Init(SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits( SPI_MISO_GPIO_PORT, SPI_MISO_GPIO_PIN );
    /* 配置SPI引脚MOSI */
    GPIO_InitStructure.GPIO_Pin = SPI_MOSI_GPIO_PIN;
    GPIO_Init(SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits( SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PIN );
    /* 配置SPI引脚CS */
    GPIO_InitStructure.GPIO_Pin = SPI_NSS_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(SPI_NSS_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN );

    //硬件SPI配置结构体
    SPI_InitTypeDef  SPI_InitStructure;
    /* SPI时钟使能 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    /* FLASH_SPI 模式配置 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// 传输模式全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       // 配置为主机
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;   // 8位数据
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;        // 极性相位
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;           // 软件cs
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;// SPI时钟预调因数为2
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //高位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(PORT_SPI, &SPI_InitStructure);

    /* 使能 FLASH_SPI  */
    SPI_Cmd(PORT_SPI, ENABLE);
}

/**
  * @brief :SPI收发一个字节
  * @param :
  *                        @TxByte: 发送的数据字节
  * @note  :非堵塞式，一旦等待超时，函数会自动退出
  * @retval:接收到的字节
  */
uint8_t drv_spi_read_write_byte( uint8_t TxByte )
{
    uint8_t l_Data = 0;
    uint16_t l_WaitTime = 0;

    while(RESET == SPI_I2S_GetFlagStatus(PORT_SPI, SPI_I2S_FLAG_TXE))//等待发送缓冲区为空
    {
        if( SPI_WAIT_TIMEOUT == ++l_WaitTime )
        {
            break;                        //如果等待超时则退出
        }
    }
    l_WaitTime = SPI_WAIT_TIMEOUT / 2;                //重新设置接收等待时间(因为SPI的速度很快，正常情况下在发送完成之后会立即收到数据，等待时间不需要过长)
    SPI_I2S_SendData(PORT_SPI, TxByte);//发送数据

    while(RESET == SPI_I2S_GetFlagStatus(PORT_SPI, SPI_I2S_FLAG_RXNE))//等待接收缓冲区非空
    {
        if( SPI_WAIT_TIMEOUT == ++l_WaitTime )
        {
            break;                        //如果等待超时则退出
        }
    }
    l_Data = SPI_I2S_ReceiveData(PORT_SPI);//读取接收数据
    return l_Data;                //返回
}

/**
  * @brief :SPI收发字符串
  * @param :
  * @ReadBuffer: 接收数据缓冲区地址
  * @WriteBuffer:发送字节缓冲区地址
  * @Length:字节长度
  * @note  :非堵塞式，一旦等待超时，函数会自动退出
  * @retval:无
  */
void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length )
{
    spi_set_nss_low( );//拉低片选
    while( Length-- )
    {
        *ReadBuffer = drv_spi_read_write_byte( *WriteBuffer );                //收发数据
        ReadBuffer++;
        WriteBuffer++;                                //读写地址加1
    }
    spi_set_nss_high( );//拉高片选
}
