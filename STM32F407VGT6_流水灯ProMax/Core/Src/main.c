#include "main.h"

int main()
{
    Delay_Init();
    MyPWM_Init();
    PWM_ALL_MAX();//ȫ��
    while(1)
    {
        for(uint8_t i = 1; i <= 8; i++)
        {
            PWM_ALL_Change(i);
        }
        PWM_ALL_MAX();//ȫ��
        Delay_ms(80);
    }
}
