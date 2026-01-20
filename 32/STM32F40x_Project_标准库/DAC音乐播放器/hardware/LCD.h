//颜色的宏定义
#define RED  	0xf800  //红色
#define GREEN	0x07e0  //绿色
#define BLUE 	0x001f  //蓝色
#define WHITE	0xffff  //白色
#define BLACK	0x0000  //黑色
#define YELLOW  0xFFE0  //黄色
#define GRAY0   0xEF7D  //灰色0
#define GRAY1   0x8410  //灰色1
#define GRAY2   0x4208  //灰色2

#ifndef __LCD_H
#define __LCD_H

/*常用函数*/
void LCD_Init(void);                          //LCD初始化
void LCD_Clear(uint16_t Color);               //全屏清屏函数，后面加上颜色。
void LCD_Reset(void);                         //LCD复位，全屏变白。
void PicQuan(const unsigned char *p);         //显示覆盖整个LCD的图片
void PicQuanBQ(const unsigned char *p);       //显示覆盖整个LCD的图片，但不清屏
void Pic(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end,const unsigned char *p);
//显示特定大小的图片；x_start y_start：图片开始的坐标；x_end y_end：图片结束的坐标；

/*文字显示函数*/
void Gui_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s);
//X Y：文字左上角的坐标；FC：字的颜色；BC：背景的颜色；S：要显示的文字；
void Gui_DrawFont_GBK24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s);
//X Y：文字左上角的坐标；FC：字的颜色；BC：背景的颜色；S：要显示的文字；
void Gui_DrawFont_Num32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t num);
//X Y：文字左上角的坐标；FC：字的颜色；BC：背景的颜色；S：要显示的文字；

/*画图函数*/
void Gui_DrawPoint(u16 x,u16 y,u16 Color);     //画一个点
void Gui_Circle(uint16_t X,uint16_t Y,uint16_t R,uint16_t fc);
//画圆函数；XY：圆心坐标；R：半径；fc：圆的线条颜色；
void Gui_DrawLine(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,uint16_t Color);
//X0Y0：线的起点坐标；X1Y1：线的终点坐标；Color：颜色；画线函数，使用Bresenham 画线算法
void DisplayButtonDown(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
//在屏幕显示一凸起的按钮框 按钮框左上角和右下角坐标
void DisplayButtonUp(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
//在屏幕显示一凹下的按钮框 按钮框左上角和右下角坐标
void Gui_box(uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t bc);
//画一个被斜线划掉的盒子；XY：起点坐标；W：宽；H：高；bc：斜线颜色；
void Gui_box2(uint16_t x,uint16_t y,uint16_t w,uint16_t h, uint8_t mode);
//画一个被斜线划掉的盒子；XY：起点坐标；W：宽；H：高；mode：盒子的三种风格；

/*内部调用函数*/
uint32_t LCD_ReadPoint(u16 x,u16 y);          //读TFT某一点的颜色
void LCD_GPIO_Init(void);                     //LCD的GPIO初始化
void LCD_WriteIndex(uint8_t Index);           //向液晶屏写一个8位指令
void LCD_WriteData(uint8_t Data);             //向液晶屏写一个8位数据
void LCD_WriteData_16Bit(uint16_t Data);      //向液晶屏写一个16位数据
void LCD_WriteReg(uint8_t Index,uint8_t Data);//向液晶屏写一个8位指令并向液晶屏写一个8位数据
void LCD_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end);
//设置LCD显示区域，在此区域写点数据自动换行
void LCD_SetXY(u16 x,u16 y);//设置LCD显示起始点
uint16_t LCD_BGR2RGB(uint16_t c);
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式通过该函数转换。

#endif

//各种线的宏定义
#define LCD_SCL_SET GPIO_SetBits(GPIOD,GPIO_Pin_15)//PA5时钟线置为高电平。
#define LCD_SCL_CLR GPIO_ResetBits(GPIOD,GPIO_Pin_15)//PA5时钟线置为低电平。
#define LCD_SDA_SET GPIO_SetBits(GPIOD,GPIO_Pin_14)//PA7数据线置为高电平。
#define LCD_SDA_CLR GPIO_ResetBits(GPIOD,GPIO_Pin_14)//PA7数据线置为低电平。
#define LCD_RST_SET GPIO_SetBits(GPIOD,GPIO_Pin_13);//PB0复位线置为高电平。
#define LCD_RST_CLR GPIO_ResetBits(GPIOD,GPIO_Pin_13)//PB0复位线置为低电平。
#define LCD_DC_SET GPIO_SetBits(GPIOD,GPIO_Pin_12)//PB1选择线置为高电平。
#define LCD_DC_CLR GPIO_ResetBits(GPIOD,GPIO_Pin_12)//PB1选择线置为低电平。
#define LCD_CS_SET GPIO_SetBits(GPIOD,GPIO_Pin_11)//PA4片选线置为高电平。
#define LCD_CS_CLR GPIO_ResetBits(GPIOD,GPIO_Pin_11)//PA4片选线置为低电平。
#define LCD_BLK_SET GPIO_SetBits(GPIOD,GPIO_Pin_10)//PB10背光线置为高电平。
#define LCD_BLK_CLR GPIO_ResetBits(GPIOD,GPIO_Pin_10)//PB10背光线置为低电平。
//#define LCD_SCL_SET GPIOA->BSRR=GPIO_Pin_5//PA5时钟线置为高电平。
//#define LCD_SCL_CLR GPIOA->BRR=GPIO_Pin_5//PA5时钟线置为低电平。
//#define LCD_SDA_SET GPIOA->BSRR=GPIO_Pin_7//PA7数据线置为高电平。
//#define LCD_SDA_CLR GPIOA->BRR=GPIO_Pin_7//PA7数据线置为低电平。
//#define LCD_RST_SET GPIOB->BSRR=GPIO_Pin_0//PB0复位线置为高电平。
//#define LCD_RST_CLR GPIOB->BRR=GPIO_Pin_0//PB0复位线置为低电平。
//#define LCD_DC_SET GPIOB->BSRR=GPIO_Pin_1//PB1选择线置为高电平。
//#define LCD_DC_CLR GPIOB->BRR=GPIO_Pin_1//PB1选择线置为低电平。
//#define LCD_CS_SET GPIOA->BSRR=GPIO_Pin_4//PA4片选线置为高电平。
//#define LCD_CS_CLR GPIOA->BRR=GPIO_Pin_4//PA4片选线置为低电平。
//#define LCD_BLK_SET GPIOB->BSRR=GPIO_Pin_10//PB10背光线置为高电平。
//#define LCD_BLK_CLR GPIOB->BRR=GPIO_Pin_10//PB10背光线置为低电平。
