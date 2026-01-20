#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "W25Q16.h"

int main()
{
	uint8_t MID;
	uint16_t DID;
	
	OLED_Init();
	W25Q16_Init();
	
	OLED_ShowString(1, 1, "MID:   DID:");
	W25Q16_ReadID(&MID,&DID);
	OLED_ShowHexNum(1, 5, MID, 2);
	OLED_ShowHexNum(1, 12, DID, 4);
	OLED_ShowString(2, 1, "W:");
	OLED_ShowString(3, 1, "R:");

	uint16_t i = 0;	
	uint8_t W[4] = {12,34,56,78};
	uint8_t R[4];
	W25Q16_SectorErase(0x000000);//0x000000~0x7FFFFF

	W25Q16_PageProgram(0x000000, W, 4);	//将写入数据的测试数组写入到W25Q64中
	W25Q16_ReadData(0x000000, R, 4);		//读取刚写入的测试数据到读取数据的测试数组中
	
	OLED_ShowNum(3,3,R[i+0],2);
	OLED_ShowNum(3,6,R[i+1],2);
	OLED_ShowNum(3,9,R[i+2],2);
	OLED_ShowNum(3,12,R[i+3],2);

	while(1)
	{

	}
}
