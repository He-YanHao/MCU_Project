#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "DHT11.h"

int main()
{
	extern unsigned int rec_data[4];
	OLED_Init();
	OLED_ShowString(1,1,"SD:XX.XX");
	OLED_ShowString(3,1,"WD:XX.XX");
	while(1)
	{
		DHT11_REC_Data();
		OLED_ShowNum(1,4,rec_data[0],2);
		OLED_ShowNum(1,7,rec_data[1],2);
		OLED_ShowNum(3,4,rec_data[2],2);
		OLED_ShowNum(3,7,rec_data[3],2);	
	}
}
