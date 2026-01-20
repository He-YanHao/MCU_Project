#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "delay.h"
#include "Lcd_Driver.h"
#include "LCD_Config.h"
#include "GUI.h"
#include "QDTFT_demo.h"

//        GND   电源地
//        VCC   接5V或3.3v电源
//        SCL   接PA5（SCL）
//        SDA   接PA7（SDA）
//        RES   接PB0
//        DC    接PB1
//        CS    接PA4 
//				BL	  接PB10

int main()
{
	OLED_Init();
	SystemInit();	//System init.
	delay_init(72);//Delay init.
	
	while(1)
	{
	QDTFT_Test_Demo();//See the test details in QDTFT_Demo.c
	}
}
