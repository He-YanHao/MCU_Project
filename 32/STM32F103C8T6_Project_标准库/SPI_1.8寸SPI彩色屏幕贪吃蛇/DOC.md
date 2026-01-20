# DOC

## HAL

```
//地线					GND   接PA9			推挽输出
//电源线				VDD   接PA10		推挽输出
//时钟线 				SCL   接PA11		推挽输出
//数据 					SDA   接PA12		推挽输出
//复位   				RST   接PA15		推挽输出
//数据命令选择  DC    接PB3			推挽输出
//片选输入   		CS    接PB4			推挽输出
//背光控制线		BLK		接PB5			推挽输出
```

```
//液晶IO初始化配置
void LCD_GPIO_Init(void)//LCD的GPIO初始化
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB ,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
      
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA,GPIO_Pin_10);
	GPIO_ResetBits(GPIOA,GPIO_Pin_9);
}
```

```
#define LCD_SCL_SET GPIO_SetBits(GPIOA,GPIO_Pin_11)//PA5时钟线置为高电平。
#define LCD_SCL_CLR GPIO_ResetBits(GPIOA,GPIO_Pin_11)//PA5时钟线置为低电平。
#define LCD_SDA_SET GPIO_SetBits(GPIOA,GPIO_Pin_12)//PA7数据线置为高电平。
#define LCD_SDA_CLR GPIO_ResetBits(GPIOA,GPIO_Pin_12)//PA7数据线置为低电平。
#define LCD_RST_SET GPIO_SetBits(GPIOA,GPIO_Pin_15);//PB0复位线置为高电平。
#define LCD_RST_CLR GPIO_ResetBits(GPIOA,GPIO_Pin_15)//PB0复位线置为低电平。
#define LCD_DC_SET GPIO_SetBits(GPIOB,GPIO_Pin_3)//PB1选择线置为高电平。
#define LCD_DC_CLR GPIO_ResetBits(GPIOB,GPIO_Pin_3)//PB1选择线置为低电平。
#define LCD_CS_SET GPIO_SetBits(GPIOB,GPIO_Pin_4)//PA4片选线置为高电平。
#define LCD_CS_CLR GPIO_ResetBits(GPIOB,GPIO_Pin_4)//PA4片选线置为低电平。
#define LCD_BLK_SET GPIO_SetBits(GPIOB,GPIO_Pin_5)//PB10背光线置为高电平。
#define LCD_BLK_CLR GPIO_ResetBits(GPIOB,GPIO_Pin_5)//PB10背光线置为低电平。
```

## 寄存器

```
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PA5（SCL）
//              SDA   接PA7（SDA）
//              RES   接PB0
//              DC    接PB1
//              CS    接PA4 
//				BL	  接PB10
```

```
void LCD_GPIO_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_10| GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
      
	
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
```

```
#define LCD_SCL_SET GPIO_SetBits(GPIOA,GPIO_Pin_15)//PA5时钟线置为高电平。
#define LCD_SCL_CLR GPIO_ResetBits(GPIOA,GPIO_Pin_15)//PA5时钟线置为低电平。
#define LCD_SDA_SET GPIO_SetBits(GPIOA,GPIO_Pin_7)//PA7数据线置为高电平。
#define LCD_SDA_CLR GPIO_ResetBits(GPIOA,GPIO_Pin_7)//PA7数据线置为低电平。
#define LCD_RST_SET GPIO_SetBits(GPIOB,GPIO_Pin_0);//PB0复位线置为高电平。
#define LCD_RST_CLR GPIO_ResetBits(GPIOB,GPIO_Pin_0)//PB0复位线置为低电平。
#define LCD_DC_SET GPIO_SetBits(GPIOB,GPIO_Pin_1)//PB1选择线置为高电平。
#define LCD_DC_CLR GPIO_ResetBits(GPIOB,GPIO_Pin_1)//PB1选择线置为低电平。
#define LCD_CS_SET GPIO_SetBits(GPIOA,GPIO_Pin_4)//PA4片选线置为高电平。
#define LCD_CS_CLR GPIO_ResetBits(GPIOA,GPIO_Pin_4)//PA4片选线置为低电平。
#define LCD_BLK_SET GPIO_SetBits(GPIOB,GPIO_Pin_10)//PB10背光线置为高电平。
#define LCD_BLK_CLR GPIO_ResetBits(GPIOB,GPIO_Pin_10)//PB10背光线置为低电平。
```

