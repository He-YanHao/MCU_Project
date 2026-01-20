#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"

uint8_t ID;								//定义用于存放ID号的变量
int16_t AX, AY, AZ, GX, GY, GZ;			//定义用于存放各个数据的变量

int main(void)
{
//	/*模块初始化*/
//	OLED_Init();		//OLED初始化
//	MPU6050_Init();		//MPU6050初始化
//	
//	/*显示ID号*/
//	OLED_ShowString(1, 1, "ID:");		//显示静态字符串
//	ID = MPU6050_GetID();				//获取MPU6050的ID号
//	OLED_ShowHexNum(1, 4, ID, 2);		//OLED显示ID号
    OLED_I2C_Init();
	uint8_t Command[]={0x00,0x8d,0x14,0xAF,0xA5};
    OLED_I2C_SendByte(0x00);
    OLED_I2C_SendByte(0x8d);
    OLED_I2C_SendByte(0x14);
    OLED_I2C_SendByte(0xAF);
    OLED_I2C_SendByte(0xA5);
    
    while (1)
	{

	}
}
