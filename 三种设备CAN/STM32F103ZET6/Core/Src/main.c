#include "main.h"

int main()
{
    LED_Init();
    MyCAN_Init();
    Serial_Init();
    Serial_SendString("MEISHNEM");
    while(1)
    {
        
    }
}
