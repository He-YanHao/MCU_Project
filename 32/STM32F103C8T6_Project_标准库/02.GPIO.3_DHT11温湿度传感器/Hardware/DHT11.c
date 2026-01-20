#include "stm32f10x.h"                  // Device header
#include "Delay.h"

unsigned int rec_data[4];

void DHT11_OUT_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void DHT11_IN_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DHT11_Start(void)//主机发送开始信号
{
	DHT11_OUT_Init(); //输出模式
	GPIO_SetBits(GPIOA, GPIO_Pin_1); //先拉高
	Delay_us(30);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1); //拉低电平至少18us
	Delay_ms(20);
	GPIO_SetBits(GPIOA, GPIO_Pin_1); //拉高电平20~40us
	Delay_us(30);
	DHT11_IN_Init(); //输入模式
}

uint8_t DHT11_Rec_Byte(void)//获取一个字节
{
	uint8_t i = 0;
	uint8_t data;
	for(i=0;i<8;i++) //1个数据就是1个字节byte，1个字节byte有8位bit
	{
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0); //从1bit开始，低电平变高电平，等待低电平结束
		Delay_us(30); //延迟30us是为了区别数据0和数据1，0只有26~28us
		data <<= 1; //左移
		if( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 1 ) //如果过了30us还是高电平的话就是数据1
		{
			data |= 1; //数据+1
		}
		while( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 1 ); //高电平变低电平，等待高电平结束
	}
	return data;
}

void DHT11_REC_Data(void)//获取数据
{
	unsigned int R_H,R_L,T_H,T_L;
	unsigned char RH,RL,TH,TL,CHECK;
	Delay_s(2);
	DHT11_Start(); //主机发送信号
	GPIO_SetBits(GPIOA, GPIO_Pin_1); //拉高电平

	if( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0 ) //判断DHT11是否响应
	{
		while( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0); //低电平变高电平，等待低电平结束
		while( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 1); //高电平变低电平，等待高电平结束

		R_H = DHT11_Rec_Byte();
		R_L = DHT11_Rec_Byte();
		T_H = DHT11_Rec_Byte();
		T_L = DHT11_Rec_Byte();
		CHECK = DHT11_Rec_Byte(); //接收5个数据

		GPIO_ResetBits(GPIOA, GPIO_Pin_1); //当最后一bit数据传送完毕后，DHT11拉低总线 50us
		Delay_us(55); //这里延时55us
		GPIO_SetBits(GPIOA, GPIO_Pin_1); //随后总线由上拉电阻拉高进入空闲状态。

		if(R_H + R_L + T_H + T_L == CHECK) //和检验位对比，判断校验接收到的数据是否正确
		{
			RH = R_H;
			RL = R_L;
			TH = T_H;
			TL = T_L;
		}
	}
	rec_data[0] = RH;
	rec_data[1] = RL;
	rec_data[2] = TH;
	rec_data[3] = TL;
}
