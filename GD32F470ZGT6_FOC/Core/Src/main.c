#include "main.h"
#include <stdio.h>

#define XIU 1

extern struct FOC_NUM MyFOC_NUM;

char str[20];
char* pstr = &str[0];

int main(void)
{
    MyPWM_Init();
    MyUSART_Init();
    MyFOC_NUM.U_q = 3500;
    MyFOC_NUM.theta = 1.17;
    double NUM = 0;
    while(1)
    {
        MyFOC_NUM.theta += 0.01;
        if(MyFOC_NUM.theta >= 6.28)
        {
            MyFOC_NUM.theta -= 6.28;
        }

        //MyFOC_NUM.U_q += 1;
        if(MyFOC_NUM.U_q > 4095)
        {
            MyFOC_NUM.U_q = 0;
        }

        FOC_Change(MyFOC_NUM.U_q, MyFOC_NUM.theta);

        Change_PWM0(XIU * fabs(MyFOC_NUM.U_a));
        Change_PWM1(XIU * fabs(MyFOC_NUM.U_b));
        Change_PWM2(XIU * fabs(MyFOC_NUM.U_c));

        sprintf(str, "%.2f", MyFOC_NUM.U_q);
        USART0_STR("U_q:");
        USART0_STR(pstr);        
        sprintf(str, "%.2f", MyFOC_NUM.theta);
        USART0_STR(" theta:");
        USART0_STR(pstr);  
//        sprintf(str, "%.2f", MyFOC_NUM.U_alpha);
//        USART0_STR(" U_alpha:");
//        USART0_STR(pstr);  
//        sprintf(str, "%.2f", MyFOC_NUM.U_beta);
//        USART0_STR(" U_beta:");
//        USART0_STR(pstr);  
        sprintf(str, "%.2f", MyFOC_NUM.U_a);
        USART0_STR(" U_a:");
        USART0_STR(pstr);
        sprintf(str, "%.2f", MyFOC_NUM.U_b);
        USART0_STR(" U_b:");
        USART0_STR(pstr);
        sprintf(str, "%.2f", MyFOC_NUM.U_c);
        USART0_STR(" U_c:");
        USART0_STR(pstr);
        
        NUM = MyFOC_NUM.U_a + MyFOC_NUM.U_b + MyFOC_NUM.U_c;
        sprintf(str, "%.2f", NUM);
        USART0_STR(" NUM:");
        USART0_STR(pstr);
        
        USART0_STR("\n ");
    }
}
