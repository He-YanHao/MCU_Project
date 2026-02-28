#include "MyFOC.h"

struct FOC_NUM MyFOC_NUM = {0};

void Clark_Change(double U_a, double U_b) //克拉克变化
{
    MyFOC_NUM.U_alpha = U_a;
    MyFOC_NUM.U_beta = (U_a + 2*U_b)/(sqrt(3));
}

void Opposite_Clark_Change(double U_alpha, double U_beta)//反克拉克变化
{
    MyFOC_NUM.U_a = U_alpha;
    MyFOC_NUM.U_b = (sqrt(3)*U_beta - U_alpha)/2;
    MyFOC_NUM.U_c = (- U_alpha - (sqrt(3))*U_beta)/2;
}

void Park_Change(double U_alpha, double U_beta, double theta)//帕克变化
{
    MyFOC_NUM.U_d = U_alpha * cos(theta) + U_beta * sin(theta);
    MyFOC_NUM.U_q = -U_alpha * sin(theta) + U_beta * cos(theta);
}

void Opposite_Park_Change(double U_d, double U_q, double theta)//反帕克变化
{
    MyFOC_NUM.U_alpha = U_d * cos(theta) - U_q * sin(theta);
    MyFOC_NUM.U_beta = U_d * sin(theta) + U_q * cos(theta);   
}

double U_q_Change(double U_q)
{
    if(U_q < DRIVE_MAX)
    {
    } else
    {
        MyFOC_NUM.U_q -= DRIVE_MAX;
        U_q -= DRIVE_MAX;
        U_q_Change(MyFOC_NUM.U_q);
    }
    
    if(U_q < 0)
    {
        MyFOC_NUM.U_q = 0;
        return 0;
    }
    return U_q;
}

void Theta_Change(double theta)
{
    if(theta < 2*PI)  //假设小于2PI
    {                 //就什么也不做
    }else             //不然就是大于2PI
    {
        theta = theta - 2*PI;//就减
        MyFOC_NUM.theta = MyFOC_NUM.theta - 2*PI;//就减
        Theta_Change(theta);//再判断一次
    }
    if(theta > 0)
    {
    }else
    {
        theta = theta + 2*PI;//就加
        MyFOC_NUM.theta = MyFOC_NUM.theta + 2*PI;
        Theta_Change(theta);//再判断一次
    }
}

double constrain(double NUM, double num_min, double num_max)
{
    if(NUM < num_min)
    {
        return num_min;
    } else if(NUM > num_max)
    {
        return num_max;
    } 
    return NUM;
}

void NOT_Ud_Change(double U_q, double theta)//反变化
{
    MyFOC_NUM.U_alpha = - U_q * sin(theta);
    MyFOC_NUM.U_beta = U_q * cos(theta);
    MyFOC_NUM.U_a = MyFOC_NUM.U_alpha;
    MyFOC_NUM.U_b = (sqrt(3)*MyFOC_NUM.U_beta - MyFOC_NUM.U_alpha)/2;
    MyFOC_NUM.U_c = (- MyFOC_NUM.U_alpha - (sqrt(3))*MyFOC_NUM.U_beta)/2;
    MyFOC_NUM.DC_a = constrain((MyFOC_NUM.U_a+MyFOC_NUM.U_q)/DRIVE_MAX/2, 0, 1);
    MyFOC_NUM.DC_b = constrain((MyFOC_NUM.U_b+MyFOC_NUM.U_q)/DRIVE_MAX/2, 0, 1);
    MyFOC_NUM.DC_c = constrain((MyFOC_NUM.U_c+MyFOC_NUM.U_q)/DRIVE_MAX/2, 0, 1);
}

void FOC_Change(double U_d, double U_q, double theta)//反变化
{


}
