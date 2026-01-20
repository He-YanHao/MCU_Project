#include "stm32f4xx.h"                  // Device header

#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

void W25Q128_init(void);//W25Q128初始化
uint8_t spi_read_write_byte(uint8_t dat);//SPI读写一个byte
uint16_t W25Q128_readID(void);//读取芯片ID&读取设备ID
void W25Q128_write_enable(void);//发送写使能
void W25Q128_wait_busy(void);//检测线路是否繁忙
void W25Q128_erase_sector(uint32_t addr);//擦除一个扇区 范围=0~4096。
void W25Q128_write(uint8_t* buffer, uint32_t addr, uint16_t numbyte);//写数据到W25Q128进行保存
void W25Q128_read(uint8_t* buffer,uint32_t read_addr,uint16_t read_length);//读取W25Q128的数据
//W25Q128将16M的容量分为256个块（Block),每个块大小为64K（64000）个字节。
//每个块又分为16个扇区（Sector),每个扇区4K个字节。
//W25Q128的最小擦除单位为一个扇区，也就是每次必须擦除4K个字节。
#endif

//宏定义
#define BSP_GPIO_RCU           RCC_AHB1Periph_GPIOA // GPIO时钟
#define BSP_SPI_RCU            RCC_APB2Periph_SPI1  // SPI时钟
#define BSP_SPI_NSS_RCU        RCC_AHB1Periph_GPIOA // CS引脚时钟

#define BSP_GPIO_PORT          GPIOA
#define BSP_GPIO_AF            GPIO_AF_SPI1

#define BSP_SPI                SPI1

#define BSP_SPI_NSS            GPIO_Pin_4        // 软件CS
        
#define BSP_SPI_SCK            GPIO_Pin_5
#define BSP_SPI_SCK_PINSOURCE  GPIO_PinSource5

#define BSP_SPI_MISO           GPIO_Pin_6
#define BSP_SPI_MISO_PINSOURCE GPIO_PinSource6

#define BSP_SPI_MOSI           GPIO_Pin_7
#define BSP_SPI_MOSI_PINSOURCE GPIO_PinSource7

#define W25QXX_CS_ON(x)  GPIO_WriteBit(BSP_GPIO_PORT, BSP_SPI_NSS, x ? Bit_SET : Bit_RESET)
