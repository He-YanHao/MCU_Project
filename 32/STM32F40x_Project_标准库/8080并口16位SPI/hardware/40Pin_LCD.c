#include "40Pin_LCD.h"

_lcd_dev lcddev;
//_lcd_dev lcddev = {240, 320, 55, 0, 0x00, 0, 0};

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 

/**
 *@ GPIO初始化
 *@ 说明：使用GPIOA的00到07，GPIOE的08到15作为16位数据线。
 */
void LCD_GPIO_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE);
	//GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure;
	//GPIOA初始化
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//无所谓
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//最快速
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//无上拉无下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIOE初始化
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//无所谓
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//最快速
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//无上拉无下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//GPIOB初始化
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//无所谓
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//最快速
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//无上拉无下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**
 *@ GPIO输出数据
 *@ 参数：data:输出的数据
 */
void DATAOUT(uint16_t data)
{
	GPIO_Write(GPIOA, data);
	GPIO_Write(GPIOE, data>>8);
}

/**
 *@ 写寄存器函
 *@ 参数：data:寄存器值
 */
void LCD_WR_REG(u16 data)
{ 
	LCD_RS_CLR;//写地址
 	LCD_CS_CLR;//SPI片选选中
	DATAOUT(data);
	LCD_WR_CLR;//写使能
	LCD_WR_SET;//写失能
 	LCD_CS_SET;//SPI片选取消选中
}

/**
 *@ 写数据函数
 *@ 参数：data:寄存器值
 */
void LCD_WR_DATA(u16 data)
{
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}

/**
 *@ LCD写入RAM颜色值
 *@ RGB_Code：LCD写入RAM的颜色值
 */
void LCD_WriteRAM(uint16_t RGB_Code)
{
	LCD_WR_DATA(RGB_Code);
}

/**
 *@ LCD写入GRAM值
 */
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}

/**
 *@ 写入寄存器数值
 *@ LCD_Reg：寄存器
 *@ LCD_RegValue：寄存器的值
 */
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);
	LCD_WriteRAM(LCD_RegValue);
}

//延时i
void opt_delay(u8 i)
{
	while(i--);
}

//初始化lcd
void LCD_Init(void)
{	
	LCD_GPIO_Init();

	LCD_CS_SET;
	LCD_RS_SET;
	LCD_WR_SET;
	LCD_RD_SET;

	LCD_BL_SET;

	LCD_RST_SET;//复位置高
	delay_ms(1);
	LCD_RST_CLR;//复位置低开始复位
	delay_ms(10);
	LCD_RST_SET;//复位取消
	delay_ms(120);  

//************* Start Initial Sequence **********//
delay_ms(120);                //ms            

LCD_WR_REG(0x11);   
  
delay_ms(120);                //ms            

LCD_WR_REG(0x36);     
LCD_WR_DATA(0x00);   

LCD_WR_REG(0x3A);     
LCD_WR_DATA(0x55);   

LCD_WR_REG(0xB2);     
LCD_WR_DATA(0x0C);   
LCD_WR_DATA(0x0C);   
LCD_WR_DATA(0x00);   
LCD_WR_DATA(0x33);   
LCD_WR_DATA(0x33);   

LCD_WR_REG(0xB7);     
LCD_WR_DATA(0x46);   //VGH=13.65V,VGL=-11.38V

LCD_WR_REG(0xBB);     
LCD_WR_DATA(0x1B);   

LCD_WR_REG(0xC0);     
LCD_WR_DATA(0x2C);   

LCD_WR_REG(0xC2);     
LCD_WR_DATA(0x01);   

LCD_WR_REG(0xC3);     
LCD_WR_DATA(0x0F);   

LCD_WR_REG(0xC4);     
LCD_WR_DATA(0x20);   

LCD_WR_REG(0xC6);     
LCD_WR_DATA(0x0F);   

LCD_WR_REG(0xD0);     
LCD_WR_DATA(0xA4);   
LCD_WR_DATA(0xA1);   

LCD_WR_REG(0xD6);     
LCD_WR_DATA(0xA1); 

LCD_WR_REG(0xE0);
LCD_WR_DATA(0xF0);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x06);
LCD_WR_DATA(0x04);
LCD_WR_DATA(0x05);
LCD_WR_DATA(0x05);
LCD_WR_DATA(0x31);
LCD_WR_DATA(0x44);
LCD_WR_DATA(0x48);
LCD_WR_DATA(0x36);
LCD_WR_DATA(0x12);
LCD_WR_DATA(0x12);
LCD_WR_DATA(0x2B);
LCD_WR_DATA(0x34);

LCD_WR_REG(0xE1);
LCD_WR_DATA(0xF0);
LCD_WR_DATA(0x0B);
LCD_WR_DATA(0x0F);
LCD_WR_DATA(0x0F);
LCD_WR_DATA(0x0D);
LCD_WR_DATA(0x26);
LCD_WR_DATA(0x31);
LCD_WR_DATA(0x43);
LCD_WR_DATA(0x47);
LCD_WR_DATA(0x38);
LCD_WR_DATA(0x14);
LCD_WR_DATA(0x14);
LCD_WR_DATA(0x2C);
LCD_WR_DATA(0x32);

LCD_WR_REG(0x21);  

LCD_WR_REG(0x29);      // Display on

}

//LCD开启显示
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	//开启显示
}

//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0X28);	//关闭显示
}

//dir:方向选择 	0-0度旋转，1-180度旋转，2-270度旋转，3-90度旋转
void LCD_Display_Dir(u8 dir)
{
	if(dir==0||dir==1)			//竖屏
	{
			lcddev.dir=0;	//竖屏
			lcddev.width=240;
			lcddev.height=320;

			lcddev.wramcmd=0X2C;
			lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;
		
		if(dir==0)        //0-0度旋转
		{
			LCD_WR_REG(0x36); 
			LCD_WR_DATA((0<<3)|(0<<7)|(0<<6)|(0<<5));
		}else							//1-180度旋转
		{
			LCD_WR_REG(0x36); 
			LCD_WR_DATA((0<<3)|(1<<7)|(1<<6)|(0<<5));		
		}
		
	}else if(dir==2||dir==3)
	{
		
			lcddev.dir=1;	//横屏
			lcddev.width=320;
			lcddev.height=240;
	
			lcddev.wramcmd=0X2C;
			lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B; 
	
				if(dir==2)				//2-270度旋转
				{
					LCD_WR_REG(0x36); 
					LCD_WR_DATA((0<<3)|(1<<7)|(0<<6)|(1<<5));
	
				}else							//3-90度旋转
				{
					LCD_WR_REG(0x36); 
					LCD_WR_DATA((0<<3)|(0<<7)|(1<<6)|(1<<5));
				}		
	}	


		//设置显示区域	
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
}

/**
 *@ 设置光标位置
 *@ Xpos：X坐标
 *@ Ypos：Y坐标
 */
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 	    
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(Xpos&0XFF);
	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(Ypos&0XFF);
}

//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WriteRAM(POINT_COLOR); 
}

//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
		//设置光标位置
		LCD_SetCursor(x, y); 	 
		//写入颜色
		LCD_WriteReg(lcddev.wramcmd, color);
}

//清屏函数
//color:要清屏的填充色
void LCD_Clear(u16 color)
{
	u32 index=0;//已清除像素的数量
	u32 totalpoint = lcddev.width * lcddev.height;//得到像素总点数
	LCD_SetCursor(0x00,0x0000);	//设置光标位置 
	LCD_WriteRAM_Prepare();		//开始写入GRAM	  	  
	for(index=0; index < totalpoint; index++)
	{
		LCD_WriteRAM(color);
	}	
} 


































