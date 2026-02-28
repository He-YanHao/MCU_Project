#include "main.h"

int main()
{
    board_Init();
    while(1)
    {
        LED_Toggle();
        Delay_ms(500);
    }
}
