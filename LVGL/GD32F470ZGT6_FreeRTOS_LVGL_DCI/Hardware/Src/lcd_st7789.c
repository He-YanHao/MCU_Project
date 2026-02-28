#include "lcd_st7789.h"
#include "gd32f4xx.h"
#include "Delay.h"
#include <string.h>

// 拉高
#define	LCD_CS_SET   GPIO_BOP(GPIOF)=GPIO_PIN_7;
#define	LCD_RS_SET	 GPIO_BOP(GPIOF)=GPIO_PIN_10;
#define	LCD_WR_SET	 GPIO_BOP(GPIOG)=GPIO_PIN_12;
#define	LCD_RD_SET	 GPIO_BOP(GPIOG)=GPIO_PIN_13;
#define	LCD_RST_SET  GPIO_BOP(GPIOG)=GPIO_PIN_14;
#define	LCD_BL_SET   GPIO_BOP(GPIOD)=GPIO_PIN_6;
// 拉低
#define	LCD_CS_CLR   GPIO_BC(GPIOF)=GPIO_PIN_7;
#define	LCD_RS_CLR	 GPIO_BC(GPIOF)=GPIO_PIN_10;
#define	LCD_WR_CLR	 GPIO_BC(GPIOG)=GPIO_PIN_12;
#define	LCD_RD_CLR	 GPIO_BC(GPIOG)=GPIO_PIN_13;
#define	LCD_RST_CLR  GPIO_BC(GPIOG)=GPIO_PIN_14;
#define	LCD_BL_CLR   GPIO_BC(GPIOD)=GPIO_PIN_6;
// 数据输出
#define	DATAOUT(data) GPIO_OCTL(GPIOB)=(uint32_t)data;

// 初始化GPIO
static void lcd_gpio_config(void);
// 发送初始化寄存器指令
static void lcd_send_init_commands(void);
// 写寄存器函数
static void LCD_WR_REG(uint16_t data);

// 管理LCD重要参数
// 默认为竖屏
_lcd_dev lcddev;

__attribute__((section(".sdram.lcd_fb"),aligned(2)))
uint16_t lcd_fb[LCD_WIDTH * LCD_HEIGHT];

void lcd_init(void)
{
    memset(lcd_fb, 0, sizeof(lcd_fb));
    lcd_gpio_config();
    LCD_BL_SET;  // 点亮背光
	LCD_RST_SET; // 保证处于运行状态
	Delay_ms(1); // 等待
	LCD_RST_CLR; // 关机重启
	Delay_ms(10);// 等待稳定
	LCD_RST_SET; // 开机 清除所以配置
	Delay_ms(120); // 等待稳定
    // 发送大量配置命令进行配置
    lcd_send_init_commands();
    // 
    LCD_DisplayOn();
    // 
    LCD_Display_Dir(2);//屏幕方向
}

// LCD开启显示
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	//开启显示
}

// LCD关闭显示
void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0X28);	//关闭显示
}

// PF7  --> LCD_CS
// PF10 --> LCD_RS
// PG12 --> LCD_WR
// PG13 --> LCD_RD
// PG14 --> LCD_RST
// PD6  --> LCD_BL
static void lcd_gpio_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    // IOB 全体输出模式 上拉
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_ALL);
    //  IOB 全体推挽模式 最大速度
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_ALL);

    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    rcu_periph_clock_enable(RCU_GPIOG);
    gpio_mode_set(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);

    rcu_periph_clock_enable(RCU_GPIOF);
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_7 | GPIO_PIN_10);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7 | GPIO_PIN_10);
}

// GPIO_OSPEED_MAX

// 写寄存器函数
// data:寄存器值
static void LCD_WR_REG(uint16_t data)
{
	LCD_RS_CLR;   //写地址  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}

// 写数据函数
// data:寄存器值
static void LCD_WR_DATA(uint16_t data)
{
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}

// 开始写GRAM
static void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}

// LCD写GRAM
// RGB_Code:颜色值
static void LCD_WriteRAM(uint16_t RGB_Code)
{
	LCD_WR_DATA(RGB_Code);//写十六位GRAM
}

//写寄存器
//LCD_Reg:寄存器编号
//LCD_RegValue:要写入的值
static void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WriteRAM(LCD_RegValue);	    		 
}   

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	 	    
    LCD_WR_REG(lcddev.setxcmd); 
    LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 			 
    LCD_WR_REG(lcddev.setycmd); 
    LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 		 
} 		 

//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WriteRAM(color); 
}

//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{	   
    //设置光标位置
    LCD_SetCursor(x,y); 	 
    //写入颜色
    LCD_WriteReg(lcddev.wramcmd, color);
}

//dir:方向选择 	0-0度旋转，1-180度旋转，2-270度旋转，3-90度旋转
void LCD_Display_Dir(uint8_t dir)
{
	if(dir==0||dir==1)			//竖屏
	{
        lcddev.dir=0;	//竖屏
        lcddev.width=240;
        lcddev.height=320;

        lcddev.wramcmd=0X2C;
        lcddev.setxcmd=0X2A;
        lcddev.setycmd=0X2B;

		if( dir == 0 ){ // 0-0度旋转
			LCD_WR_REG(0x36); 
			LCD_WR_DATA((0<<3)|(0<<7)|(0<<6)|(0<<5));
		} else { // 1-180度旋转
			LCD_WR_REG(0x36); 
			LCD_WR_DATA((0<<3)|(1<<7)|(1<<6)|(0<<5));		
		}

	} else if(dir==2||dir==3) {
		
        lcddev.dir=1;	//横屏
        lcddev.width=320;
        lcddev.height=240;

        lcddev.wramcmd=0X2C;
        lcddev.setxcmd=0X2A;
        lcddev.setycmd=0X2B; 

        if(dir == 2) { //2-270度旋转
            LCD_WR_REG(0x36); 
            LCD_WR_DATA((0<<3)|(1<<7)|(0<<6)|(1<<5));
        } else { //3-90度旋转
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

// 设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
// sx,sy:窗口起始坐标(左上角)
// width,height:窗口宽度和高度,必须大于0!!
// 窗体大小:width*height. 
void LCD_Set_Window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
	uint16_t twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;

    LCD_WR_REG(lcddev.setxcmd); 
    LCD_WR_DATA(sx>>8); 
    LCD_WR_DATA(sx&0XFF);	 
    LCD_WR_DATA(twidth>>8); 
    LCD_WR_DATA(twidth&0XFF);  
    LCD_WR_REG(lcddev.setycmd); 
    LCD_WR_DATA(sy>>8); 
    LCD_WR_DATA(sy&0XFF); 
    LCD_WR_DATA(theight>>8); 
    LCD_WR_DATA(theight&0XFF); 
}


// 读LCD数据
// 返回值:读到的值
uint16_t LCD_RD_DATA(void)
{
    // IOB 全体输出模式 上拉
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_ALL);
    DATAOUT(0);//全部输出0

	LCD_RS_SET;
	LCD_CS_CLR;
	//读取数据(读寄存器时,并不需要读2次)
	LCD_RD_CLR;		   

	uint16_t t = ((uint16_t)GPIO_ISTAT(GPIOB));
	LCD_RD_SET;
	LCD_CS_SET; 

    // IOB 全体输出模式 上拉
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_ALL);

    return t;
}

//读显示屏 ID
uint16_t LCD_Read_ID(uint8_t reg)
{
	LCD_WR_REG(reg);
	LCD_RD_DATA(); 				//dummy read 	
	LCD_RD_DATA();   	    	//读到0X00
	lcddev.id=LCD_RD_DATA();   	//读取85								   
	lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA();  	//读取52 	
	return lcddev.id;
}

//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
uint16_t LCD_ReadPoint(uint16_t x, uint16_t y)
{
 	uint16_t r,g,b;
	if(x >= lcddev.width || y >= lcddev.height) {
        return 0;	//超过了范围,直接返回
    }

	LCD_SetCursor(x,y);
	LCD_WR_REG(0X2E);//发送读GRAM指令
	
    // IOB 全体输出模式 上拉
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_ALL);
    DATAOUT(0xFFFF);//全部输出高

	LCD_RS_SET;
	LCD_CS_CLR;	    
	//读取数据(读GRAM时,第一次为假读)	
	LCD_RD_CLR;		
	Delay_ms(1);//延时	
 	r = ((uint16_t)GPIO_ISTAT(GPIOB));  										//实际坐标颜色 
	LCD_RD_SET;

 	//dummy READ
	LCD_RD_CLR;					   
	Delay_ms(1);//延时			   
 	r = ((uint16_t)GPIO_ISTAT(GPIOB));  	//实际坐标颜色
	LCD_RD_SET;
	 
	LCD_RD_CLR;					   
	Delay_ms(1);//延时
	b = ((uint16_t)GPIO_ISTAT(GPIOB));//读取蓝色值
	LCD_RD_SET;
	g=r&0XFF;//第一次读取的是RG的值,R在前,G在后,各占8位
	g<<=8;

	LCD_CS_SET;
    // IOB 全体输出模式 上拉
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_ALL);
	DATAOUT(0xFFFF);//全部输出高
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//公式转换一下
}		

//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint16_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//得到总点数
	LCD_SetCursor(0x00,0x0000);	//设置光标位置 
	LCD_WriteRAM_Prepare();     		//开始写入GRAM	  	  
	for(index=0;index<totalpoint;index++)LCD_WriteRAM(color);
}

//在指定区域内填充指定颜色
//区域大小:(xend-xsta+1)*(yend-ysta+1)
//xsta
//color:要填充的颜色
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{          
	uint16_t i,j;
	uint16_t xlen=0;
	uint16_t temp;
	if((lcddev.id == 0X6804) && (lcddev.dir == 1))	//6804横屏的时候特殊处理  
	{
		temp=sx;
		sx=sy;
		sy=lcddev.width-ex-1;	  
		ex=ey;
		ey=lcddev.width-temp-1;
 		lcddev.dir=0;	 
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 			
		LCD_Fill(sx,sy,ex,ey,color);  
 		lcddev.dir=1;	 
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  	 
 	} else {
		xlen=ex-sx+1;	 
		for(i=sy;i<=ey;i++) {
		 	LCD_SetCursor(sx,i);      				//设置光标位置 
			LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
			for(j=0;j<xlen;j++)LCD_WriteRAM(color);	//设置光标位置 	    
		}
	}
}

//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
 	for(i=0;i<height;i++) {
 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)LCD_WriteRAM(color[i*width+j]);//写入数据 
	}
}

void LCD_Fill_Windows(uint16_t *color)
{
    LCD_Color_Fill(0, 0, 160, 120, (uint16_t *)color);
}

//// 全屏刷新函数
//void LCD_Fill_Windows(uint16_t *color)
//{
//	uint32_t index = 0;      
//	uint32_t totalpoint = LCD_WIDTH * LCD_HEIGHT;
//	LCD_SetCursor(0x00,0x0000);	//设置光标位置 
//	LCD_WriteRAM_Prepare(); //开始写入GRAM	  	  
//	for(index=0;index<totalpoint;index++) {
//        LCD_WriteRAM(color[index]);
//    }
//}

// 发送初始化配置寄存器指令
static void lcd_send_init_commands(void)
{
    LCD_WR_REG(0x11);   

    Delay_ms(120);//ms            

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

    LCD_WR_REG(0x29);
}
