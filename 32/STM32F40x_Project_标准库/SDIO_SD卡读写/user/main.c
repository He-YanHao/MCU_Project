#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "SD.h"

int main()
{
    board_init();
    SD_Init();

    while(1)
    {

    }
}
