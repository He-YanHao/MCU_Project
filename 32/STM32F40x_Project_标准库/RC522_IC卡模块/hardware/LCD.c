//地线               GND
//电源线             VDD
//时钟线             SCL
//数据               SDA
//复位               RST
//数据命令选择       DC
//片选输入           CS
//背光控制线         BLK

//Image2LCD取模选项设置
//不包含图像头数据  16位BMP 水平扫描 从左到右 自顶至底 低位在前
#include "stm32f4xx.h"                  // Device header
#include "board.h"
#include "LCD.h"
#include "LCD_font.h"

#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        160

void LCD_GPIO_Init(void)//连续的
{
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//无所谓
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//最快速
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//推挽
	GPIO_Init(GPIOD, &GPIO_InitStructure);

    LCD_BLK_SET;//通过IO控制背光亮
}

void SPI_WriteData(uint8_t Data)//向SPI总线传输一个8位数据
{
	uint8_t i=0;//计数
	for(i=8;i>0;i--)//循环八次
	{
		if(Data&0x80)//与0x80，也就是0b 1000 0000 取和，则可清除Data当前位外所有，进行判断。
		{
		LCD_SDA_SET;//设数据位为高电平
		}
        else
		{
		LCD_SDA_CLR;//设数据位为低电平
		}
	LCD_SCL_CLR;//时钟线设为低电平
    LCD_SCL_SET;//时钟线设为高电平  
    //也就是说时钟线平时是高电平
    Data<<=1;//数据为左移一位，也就是说数据位先发高位
	}
}

void LCD_WriteIndex(uint8_t Index)//向液晶屏写一个8位指令
{
   LCD_CS_CLR;//片选线为低电平，选中LCD。
   LCD_DC_CLR;//数据命令选择线为低电平，表示命令。
   SPI_WriteData(Index);//发送一个字节。
   LCD_CS_SET;//片选线为高电平，选中结束，发送结束。
}

void LCD_WriteData(uint8_t Data)//向液晶屏写一个8位数据
{
   LCD_CS_CLR;//片选线为低电平，选中LCD。
   LCD_DC_SET;//数据命令选择线为高电平，表示数据。
   SPI_WriteData(Data);//发送一个字节。
   LCD_CS_SET;//片选线为高电平，选中结束，发送结束。
}

void LCD_WriteData_16Bit(uint16_t Data)//向液晶屏写一个16位数据
{
    LCD_CS_CLR;//片选线为低电平，选中LCD。
    LCD_DC_SET;//数据命令选择线为高电平，表示数据。
	SPI_WriteData(Data>>8); 	//写入高8位数据。
	SPI_WriteData(Data); 		//写入低8位数据。
    LCD_CS_SET;//片选线为高电平，选中结束，发送结束。
}

void LCD_WriteReg(uint8_t Index,uint8_t Data)//SPI写命令时序开始并向液晶屏写一个8位数据
{
    LCD_WriteIndex(Index);//先写入八位命令。
    LCD_WriteData(Data);//再写入八位数据。
}

void LCD_Reset(void)//LCD复位
{
	LCD_RST_CLR;//复位线为低电平，表示复位。
	delay_ms(100);//等待。
	LCD_RST_SET;//复位线为高电平，表示复位结束，进入常态。
	delay_ms(50);//等待。
}


void LCD_Init(void)//LCD初始化	LCD Init For 1.44Inch LCD Panel with ST7735R.
{	
	LCD_GPIO_Init();//GPIO初始化。
	LCD_Reset();//LCD复位。

	LCD_WriteIndex(0x11);//LCD写入命令  Sleep exit
	delay_ms(120);//等待

	//ST7735R Frame Rate
	LCD_WriteIndex(0xB1); 
	LCD_WriteData(0x01); 
	LCD_WriteData(0x2C); 
	LCD_WriteData(0x2D); 

	LCD_WriteIndex(0xB2); 
	LCD_WriteData(0x01); 
	LCD_WriteData(0x2C); 
	LCD_WriteData(0x2D); 

	LCD_WriteIndex(0xB3); 
	LCD_WriteData(0x01); 
	LCD_WriteData(0x2C); 
	LCD_WriteData(0x2D); 
	LCD_WriteData(0x01); 
	LCD_WriteData(0x2C); 
	LCD_WriteData(0x2D); 
	
	LCD_WriteIndex(0xB4); //Column inversion 
	LCD_WriteData(0x07); 
	
	//ST7735R Power Sequence
	LCD_WriteIndex(0xC0); 
	LCD_WriteData(0xA2); 
	LCD_WriteData(0x02); 
	LCD_WriteData(0x84); 
	LCD_WriteIndex(0xC1); 
	LCD_WriteData(0xC5); 

	LCD_WriteIndex(0xC2); 
	LCD_WriteData(0x0A); 
	LCD_WriteData(0x00); 

	LCD_WriteIndex(0xC3); 
	LCD_WriteData(0x8A); 
	LCD_WriteData(0x2A); 
	LCD_WriteIndex(0xC4); 
	LCD_WriteData(0x8A); 
	LCD_WriteData(0xEE); 
	
	LCD_WriteIndex(0xC5); //VCOM 
	LCD_WriteData(0x0E); 
	
	LCD_WriteIndex(0x36); //MX, MY, RGB mode 
	LCD_WriteData(0xC0); 
	
	//ST7735R Gamma Sequence
	LCD_WriteIndex(0xe0); 
	LCD_WriteData(0x0f); 
	LCD_WriteData(0x1a); 
	LCD_WriteData(0x0f); 
	LCD_WriteData(0x18); 
	LCD_WriteData(0x2f); 
	LCD_WriteData(0x28); 
	LCD_WriteData(0x20); 
	LCD_WriteData(0x22); 
	LCD_WriteData(0x1f); 
	LCD_WriteData(0x1b); 
	LCD_WriteData(0x23); 
	LCD_WriteData(0x37); 
	LCD_WriteData(0x00); 	
	LCD_WriteData(0x07); 
	LCD_WriteData(0x02); 
	LCD_WriteData(0x10); 

	LCD_WriteIndex(0xe1); 
	LCD_WriteData(0x0f); 
	LCD_WriteData(0x1b); 
	LCD_WriteData(0x0f); 
	LCD_WriteData(0x17); 
	LCD_WriteData(0x33); 
	LCD_WriteData(0x2c); 
	LCD_WriteData(0x29); 
	LCD_WriteData(0x2e); 
	LCD_WriteData(0x30); 
	LCD_WriteData(0x30); 
	LCD_WriteData(0x39); 
	LCD_WriteData(0x3f); 
	LCD_WriteData(0x00); 
	LCD_WriteData(0x07); 
	LCD_WriteData(0x03); 
	LCD_WriteData(0x10);  
	
	LCD_WriteIndex(0x2a);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x7f);

	LCD_WriteIndex(0x2b);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x9f);
	
	LCD_WriteIndex(0xF0); //Enable test command  
	LCD_WriteData(0x01); 
	LCD_WriteIndex(0xF6); //Disable ram power save mode 
	LCD_WriteData(0x00); 

	LCD_WriteIndex(0x3A); //65k mode 
	LCD_WriteData(0x05); 

	LCD_WriteIndex(0x29);//Display on	 

}

/*************************************************
函数名：LCD_Set_Region
功能：设置LCD显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void LCD_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end)
{
	LCD_WriteIndex(0x2a);
	LCD_WriteData(0x00);
	LCD_WriteData(x_start);
    //LCD_WriteData(x_start+2);
	LCD_WriteData(0x00);
	LCD_WriteData(x_end+2);

	LCD_WriteIndex(0x2b);
	LCD_WriteData(0x00);
	LCD_WriteData(y_start+0);
	LCD_WriteData(0x00);
	LCD_WriteData(y_end+1);
	
	LCD_WriteIndex(0x2c);
}

/*************************************************
函数名：LCD_Set_XY
功能：设置LCD显示起始点
入口参数：xy坐标
*************************************************/
void LCD_SetXY(u16 x,u16 y)
{
  	LCD_SetRegion(x,y,x,y);//设置LCD显示区域
}

/*************************************************
函数名：LCD_DrawPoint
功能：画一个点
入口参数：无
返回值：无
*************************************************/
void Gui_DrawPoint(u16 x,u16 y,u16 Color)
{
	LCD_SetRegion(x,y,x+1,y+1);//设置LCD显示区域，在此区域写点数据自动换行
	LCD_WriteData_16Bit(Color);//写入颜色
}

/*****************************************
 函数功能：读TFT某一点的颜色                          
 出口参数：color  点颜色值                                 
******************************************/
uint32_t LCD_ReadPoint(u16 x,u16 y)
{
  unsigned int Data;
  LCD_SetXY(x,y);
  LCD_WriteData(Data);
  return Data;
}

/*************************************************
函数名：LCD_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void LCD_Clear(uint16_t Color)               
{
   uint16_t i,m;
   LCD_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   LCD_WriteIndex(0x2C);
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {
	  	LCD_WriteData_16Bit(Color);
    }
}

//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式通过该函数转换。
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
uint16_t LCD_BGR2RGB(uint16_t c)
{
  u16  r,g,b,rgb;   
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);		 
  return(rgb);
}

void Gui_Circle(uint16_t X,uint16_t Y,uint16_t R,uint16_t fc)//画圆函数 
{//Bresenham算法
    uint8_t a,b; 
    int c; 
    a=0; 
    b=R; 
    c=3-2*R; 
    while (a<b) 
    { 
        Gui_DrawPoint(X+a,Y+b,fc);     //        7 
        Gui_DrawPoint(X-a,Y+b,fc);     //        6 
        Gui_DrawPoint(X+a,Y-b,fc);     //        2 
        Gui_DrawPoint(X-a,Y-b,fc);     //        3 
        Gui_DrawPoint(X+b,Y+a,fc);     //        8 
        Gui_DrawPoint(X-b,Y+a,fc);     //        5 
        Gui_DrawPoint(X+b,Y-a,fc);     //        1 
        Gui_DrawPoint(X-b,Y-a,fc);     //        4 

        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
    { 
        Gui_DrawPoint(X+a,Y+b,fc); 
        Gui_DrawPoint(X+a,Y+b,fc); 
        Gui_DrawPoint(X+a,Y-b,fc); 
        Gui_DrawPoint(X-a,Y-b,fc); 
        Gui_DrawPoint(X+b,Y+a,fc); 
        Gui_DrawPoint(X-b,Y+a,fc); 
        Gui_DrawPoint(X+b,Y-a,fc); 
        Gui_DrawPoint(X-b,Y-a,fc); 
    }
}



void Gui_DrawLine(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,uint16_t Color)//画线函数，使用Bresenham 画线算法
{
int dx,             // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2, 
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping	

	LCD_SetXY(x0,y0);
	dx = x1-x0;//计算x距离
	dy = y1-y0;//计算y距离

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
	{//且线的点数等于x距离，以x轴递增画点
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)//要画的点数不会超过x距离
		{
			//画点
			Gui_DrawPoint(x0,y0,Color);
			
			// test if error has overflowed
			if (error >= 0) //是否需要增加y坐标值
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//增加y坐标值
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//x坐标值每次画点后都递增1
		} // end for
	} // end if |slope| <= 1
	else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
	{//以y轴为递增画点
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			Gui_DrawPoint(x0,y0,Color);

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}


void Gui_box(uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t bc)
{
	Gui_DrawLine(x,y,x+w,y,0xEF7D);
	Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
	Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
	Gui_DrawLine(x,y,x,y+h,0xEF7D);
    Gui_DrawLine(x+1,y+1,x+1+w-2,y+1+h-2,bc);
}

void Gui_box2(uint16_t x,uint16_t y,uint16_t w,uint16_t h, uint8_t mode)
{
	if (mode==0)	{
		Gui_DrawLine(x,y,x+w,y,0xEF7D);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
		Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
		Gui_DrawLine(x,y,x,y+h,0xEF7D);
		}
	if (mode==1)	{
		Gui_DrawLine(x,y,x+w,y,0x2965);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
		Gui_DrawLine(x,y+h,x+w,y+h,0xEF7D);
		Gui_DrawLine(x,y,x,y+h,0x2965);
	}
	if (mode==2)	{
		Gui_DrawLine(x,y,x+w,y,0xffff);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xffff);
		Gui_DrawLine(x,y+h,x+w,y+h,0xffff);
		Gui_DrawLine(x,y,x,y+h,0xffff);
	}
}

/**************************************************************************************
功能描述: 在屏幕显示一凸起的按钮框
输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonDown(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	Gui_DrawLine(x1,  y1,  x2,y1, GRAY2);  //H
	Gui_DrawLine(x1+1,y1+1,x2,y1+1, GRAY1);  //H
	Gui_DrawLine(x1,  y1,  x1,y2, GRAY2);  //V
	Gui_DrawLine(x1+1,y1+1,x1+1,y2, GRAY1);  //V
	Gui_DrawLine(x1,  y2,  x2,y2, WHITE);  //H
	Gui_DrawLine(x2,  y1,  x2,y2, WHITE);  //V
}

/**************************************************************************************
功能描述: 在屏幕显示一凹下的按钮框
输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonUp(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	Gui_DrawLine(x1,  y1,  x2,y1, WHITE); //H
	Gui_DrawLine(x1,  y1,  x1,y2, WHITE); //V
	
	Gui_DrawLine(x1+1,y2-1,x2,y2-1, GRAY1);  //H
	Gui_DrawLine(x1,  y2,  x2,y2, GRAY2);  //H
	Gui_DrawLine(x2-1,y1+1,x2-1,y2, GRAY1);  //V
    Gui_DrawLine(x2  ,y1  ,x2,y2, GRAY2); //V
}

/**
  *@函数作用    显示文字
  *@参数1    X坐标
  *@参数2    Y坐标
  *@参数3    字的颜色
  *@参数4    背景的颜色
  *@参数5    文字
  */
void Gui_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s)
{
	uint16_t i,j;//for循环使用
	uint16_t k;//用K保存字符的值，代替进行判断和显示
	uint16_t x0;//将横坐标赋值给X0

	x0=x;//将横坐标赋值给X0

	while(*s)//只要指针对应的有数据就循环，开始显示
	{
		if((*s) < 128)//判断文字是否属于ASCII编码
		{
			k=*s;//用K保存字符的值，代替进行判断和显示
			if (k==13)//ASCII编码的13位是回车，所以换行
			{
				x = x0;//回到第一列
                y = y + 16;//行数加16（字号为16像素）
			}
			else
			if ( k > 32 )//大于32就对应ASCII编码中的文字
            {
                k = k - 32;//减去不是文字的部分
            }
            else
            {
                k = 0;//特殊情况就将K值赋值为零，显示空格。
            }
			for(i = 0; i < 16; i++)//循环16次，写列数据
            {
                for(j = 0; j < 8; j++)//循环8次，写行数据
				{
				    if(asc16[k*16+i]&(0x80>>j))//k*16+i是一个范围，包含该字符的16个数组
                    {
                        Gui_DrawPoint(x+j,y+i,fc);//如果这一位为1，就写字的颜色
                    }
					else//判断
					if (fc!=bc)//判断背景颜色与文字颜色是否一致
                    {
                        Gui_DrawPoint(x+j,y+i,bc);//如果这一位为0，就写背景的颜色
                    }
				}
            }
        x = x + 8;//横坐标+8，进入下一个单位
		s++;//指针后推
		}
		else//考虑文字不属于ASCII编码的情况
		{
        	for (k=0;k<hz16_num;k++)//hz16_num是汉字总数，这里用作循环总数。
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
              //同时比对三个编码，如果是就显示汉字。
			  {
				    for(i=0;i<16;i++)//竖着18行
				    {
						for(j=0;j<8;j++) 
						{
						    if(hz16[k].Msk[i*2]&(0x80>>j))
                            {
                                Gui_DrawPoint(x+j,y+i,fc);
                            }
							else 
							if (fc!=bc)
                            {
                                Gui_DrawPoint(x+j,y+i,bc);
                            }
						}
						for(j=0;j<8;j++) 
						{
						    if(hz16[k].Msk[i*2+1]&(0x80>>j))
                            {
                            Gui_DrawPoint(x+j+8,y+i,fc);
                            }
                            else 
							if (fc!=bc) 
                            {
                            Gui_DrawPoint(x+j+8,y+i,bc);
                            }
						}
				    }
				}
			 }
        s = s + 3;
        x = x + 16;
		}
	}
}

/**
  *@函数作用    显示文字
  *@参数1    X坐标
  *@参数2    Y坐标
  *@参数3    字的颜色
  *@参数4    背景的颜色
  *@参数5    文字
  */
void Gui_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
	unsigned char i,j;
	unsigned short k;
	while(*s)
	{
		if( *s < 0x80 )//判断文字是否属于ASCII编码
		{
			k=*s;
			if (k>32)
            {
            k-=32;
            }
            else
            {
            k=0;
            }
		    for(i=0;i<16;i++)
            {
            	for(j=0;j<8;j++) 
				{
			    	if(asc16[k*16+i]&(0x80>>j))	
					{
                    Gui_DrawPoint(x+j,y+i,fc);
                    }
					else 
					if (fc!=bc) 
                    {
                    Gui_DrawPoint(x+j,y+i,bc);
                    }
				}
            }
			s++;x+=8;
		}
		else 
		{
        	for (k=0;k<hz16_num;k++)// 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))//
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
						{
						    if(hz16[k].Msk[i*2]&(0x80>>j))
                            {
                                Gui_DrawPoint(x+j,y+i,fc);
                            }
							else 
							if (fc!=bc)
                            {
                                Gui_DrawPoint(x+j,y+i,bc);
                            }
						}
						for(j=0;j<8;j++) 
						{
						    if(hz16[k].Msk[i*2+1]&(0x80>>j))
                            {
                            Gui_DrawPoint(x+j+8,y+i,fc);
                            }
                            else 
							if (fc!=bc) 
                            {
                            Gui_DrawPoint(x+j+8,y+i,bc);
                            }
						}
				    }
				}
			 }
            s = s + 2;
            x = x + 16;
        }
    }
}

void Gui_DrawFont_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num)
{
	unsigned char i,j,k,c;

    for(i=0;i<32;i++)
	{
		for(j=0;j<4;j++) 
		{
			c=*(sz32+num*32*4+i*4+j);
			for (k=0;k<8;k++)	
			{
	
		    	if(c&(0x80>>k))	Gui_DrawPoint(x+j*8+k,y+i,fc);
				else {
					if (fc!=bc) Gui_DrawPoint(x+j*8+k,y+i,bc);
				}
			}
		}
	}
}

void PicQuan(const unsigned char *p) //显示图片
{
	int i;
	unsigned char picH,picL;
	LCD_Clear(WHITE); //清屏  

	LCD_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);		//坐标设置
	for(i=0;i<128*160;i++)
	{	
	picL=*(p+i*2);//数据低位在前
	picH=*(p+i*2+1);
	LCD_WriteData_16Bit(picH<<8|picL);
	}
}

void PicQuanBQ(const unsigned char *p) //显示图片
{
	int i;
	unsigned char picH,picL;

	LCD_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);		//坐标设置
	for(i=0;i<128*160;i++)
	{	
	picL=*(p+i*2);//数据低位在前
	picH=*(p+i*2+1);
	LCD_WriteData_16Bit(picH<<8|picL);
	}
}

//取模方式 水平扫描 从左到右 低位在前
void Pic(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end,const unsigned char *p) //显示40*40 QQ图片
{
	int i; 
	unsigned char picH,picL;
	LCD_Clear(WHITE); //清屏  
	LCD_SetRegion(x_start,y_start,x_end,y_end);		//坐标设置
	for(i=0;i<(x_end-x_start+1)*(y_end-y_start+1);i++)
	{	
		picL=*(p+i*2);	//数据低位在前
		picH=*(p+i*2+1);				
		LCD_WriteData_16Bit(picH<<8|picL);  						
	}	
}
