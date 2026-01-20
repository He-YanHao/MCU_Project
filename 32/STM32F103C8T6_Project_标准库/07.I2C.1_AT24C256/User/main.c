#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "AT24C256.h"

#define D1 0X0000
#define D2 0X7FFF
#define D3 0X8000

int main()
{
	OLED_Init();
	MyI2C_Init();

	uint8_t W1=0x11,W2=0x22,W3=0x33;

	AT24C256_WriteByte(D3,W3);
	//AT24C256_WriteByte(D2,W1);
	//AT24C256_WriteByte(D3,W1);

	OLED_ShowHexNum(1,1,AT24C256_ReadByte(D1),4);
	OLED_ShowHexNum(2,1,AT24C256_ReadByte(D2),4);
	OLED_ShowHexNum(3,1,AT24C256_ReadByte(D3),4);

//	uint8_t R1[]={"qwertyuiopasdfghjklzxcvbnkkk"};
//
//	for(uint16_t i = 0;i<=26;i++)
//	{
//	AT24C256_WriteByte(i,R1[i]);
//	}
//	
//	for(uint16_t i = 0;i<=26;i++)
//	{
//	OLED_ShowChar(1,1,AT24C256_ReadByte(i));
//	Delay_ms(500);
//	}

	while(1)
	{
	
	}
}
