#include "stm32f10x.h"
//#include "bsp_led.h"
//#include "bsp_SysTick.h"
#include "TM1637.h"
 
int main( void )
{
    TM1637_Init();
    
    
    while ( 1 )
    {
        TM1637_Display_INC();
        TM1637_SetBrightness( 3 );					//设置亮度等级 0---7
    }
 
}
