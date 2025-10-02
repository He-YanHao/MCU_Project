#include "main.h"

extern struct FOC_NUM MyFOC_NUM;

extern volatile uint32_t CountSensorCount;//全局变量，用于计数

PID_NUM MyPID = {0};

int main()
{
    Delay_Init();
    CountSensor_Init();
    board_BSP_USART_Init();
    MyPWM_Init();
    //TIMER_Init();
    MyPID.T = 1000;
    
    MyFOC_NUM.U_q = 1000;     //坐标系q
    MyFOC_NUM.theta = 5;   //角度
    while(1)
    {
        for(uint16_t i = 4095; i >= 0; i--)
        {
            Change_PWM1(i);
            Delay_ms(10);
        }
//        //MyFOC_NUM.U_q += 0.5;
//        //MyFOC_NUM.U_q = U_q_Change(MyFOC_NUM.U_q);
//        MyFOC_NUM.theta += 0.01;
//        Theta_Change(MyFOC_NUM.theta);
//        NOT_Ud_Change(MyFOC_NUM.U_q, MyFOC_NUM.theta);

//        Change_PWM1(PWM_BIT*MyFOC_NUM.DC_a);
//        Change_PWM2(PWM_BIT*MyFOC_NUM.DC_b);
//        Change_PWM3(PWM_BIT*MyFOC_NUM.DC_c);

//        Delay_us(50);
//        if(add_or())
//        {
//            //printf("CountSensorCount:%d\n",CountSensorCount);   
//        }
//        //printf("U_q:%.2f theta:%.2f U_a:%.2f U_b:%.2f U_c:%.2f\n",MyFOC_NUM.U_q,MyFOC_NUM.theta,MyFOC_NUM.U_a,MyFOC_NUM.U_b,MyFOC_NUM.U_c);
//        //printf("U_alpha:%.2f U_beta:%.2f DC_a:%.2f DC_b:%.2f DC_c:%.2f\n",MyFOC_NUM.U_alpha,MyFOC_NUM.U_beta,MyFOC_NUM.DC_a,MyFOC_NUM.DC_b,MyFOC_NUM.DC_c);
    }
}
