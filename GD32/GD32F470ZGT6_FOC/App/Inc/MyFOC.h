#ifndef __MYFOC_H
#define __MYFOC_H

#include <math.h>

#define DRIVE_MAX 4900 

struct FOC_NUM{     //定义FOC所需数据的结构体
    double U_a;     //三相电压A
    double U_b;     //三相电压B
    double U_c;     //三相电压C
    double U_alpha; //坐标系α
    double U_beta;  //坐标系β
    double U_d;     //坐标系d
    double U_q;     //坐标系q
    double theta;   //角度
    double DC_a;    //A相PWM占空比
    double DC_b;    //B相PWM占空比
    double DC_c;    //C相PWM占空比
};

void Clark_Change(double U_a, double U_b);                      //克拉克变化
void Opposite_Clark_Change(double U_alpha, double U_beta);      //反克拉克变化
void Park_Change(double U_alpha, double U_beta, double theta);  //帕克变化
void Opposite_Park_Change(double U_d, double U_q, double theta);//反帕克变化
void FOC_Change(double U_q, double theta);                      //反变化

#endif
