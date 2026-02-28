#ifndef __NRF24L01_SPI_H__
#define __NRF24L01_SPI_H__

#include "stm32f4xx.h"

//SPI引脚定义
#define SPI_CLK_GPIO_PORT   /*配置时钟端口*/       GPIOB
#define SPI_CLK_GPIO_CLK    /*配置时钟端口RCC*/    RCC_AHB1Periph_GPIOB
#define SPI_CLK_GPIO_PIN    /*配置时钟引脚*/       GPIO_Pin_13

#define SPI_MISO_GPIO_PORT  /*配置MISO端口*/      GPIOB
#define SPI_MISO_GPIO_CLK   /*配置MISO端口RCC*/   RCC_AHB1Periph_GPIOB
#define SPI_MISO_GPIO_PIN   /*配置MISO引脚*/      GPIO_Pin_14

#define SPI_MOSI_GPIO_PORT  /*配置MOSI端口*/      GPIOB
#define SPI_MOSI_GPIO_CLK   /*配置MOSI端口RCC*/   RCC_AHB1Periph_GPIOB
#define SPI_MOSI_GPIO_PIN   /*配置MOSI引脚*/      GPIO_Pin_15

#define SPI_NSS_GPIO_PORT   /*配置片选端口*/        GPIOB
#define SPI_NSS_GPIO_CLK    /*配置片选端口RCC*/     RCC_AHB1Periph_GPIOB
#define SPI_NSS_GPIO_PIN    /*配置片选端口引脚*/     GPIO_Pin_9

#define spi_set_nss_high( ) /*片选置高*/          GPIO_WriteBit(SPI_NSS_GPIO_PORT,SPI_NSS_GPIO_PIN,Bit_SET)  //片选置高
#define spi_set_nss_low( )  /*片选置低*/          GPIO_WriteBit(SPI_NSS_GPIO_PORT,SPI_NSS_GPIO_PIN,Bit_RESET)//片选置低

/******** 硬件SPI修改此次 ********/
#define RCU_SPI_HARDWARE RCC_APB1Periph_SPI2
#define PORT_SPI         SPI2
#define LINE_AF_SPI      GPIO_AF_SPI2

//初始化NRF24L01的SPI代码
void NRF24L01_SPI_Init(void);
//SPI读写一个字节
uint8_t drv_spi_read_write_byte( uint8_t TxByte );
//SPI读写一个字符串 读取字符串指针 写字符串指针 长度
void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length );

#endif
