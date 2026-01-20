/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：club.szlcsc.com
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * 
 
 Change Logs:
 * Date           Author       Notes
 * 2024-03-07     LCKFB-LP    first version
 */
#include "board.h"
#include "bsp_uart.h"
#include <stdio.h>


// 位带操作的宏定义，直接使用您提供的BIT_ADDR宏
#define BIT_ADDR(byte_offset,bitnum)  (volatile unsigned long*)(0x42000000 + (byte_offset * 32) + (bitnum * 4))

// 计算GPIOB寄存器的位带别名区地址
#define GPIOB_OCTL_OFFSET ((GPIOB_BASE + 0x14) - 0x40000000)
#define GPIOB_ISTAT_OFFSET ((GPIOB_BASE + 0x10) - 0x40000000)

// 定义PBout和PBin宏
#define PBout(n)    *(BIT_ADDR(GPIOB_OCTL_OFFSET,n))
#define PBin(n)     *(BIT_ADDR(GPIOB_ISTAT_OFFSET,n))

int main(void)
{
	
	board_init();
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	while(1)
	{
//		GPIO_SetBits(GPIOB, GPIO_Pin_2);
		PBout(2) = 1;
		delay_ms(500);
		
//		GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		PBout(2) = 0;
		delay_ms(500);
	}
	

}
