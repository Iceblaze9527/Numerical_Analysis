#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

using namespace std;

//坐标变换函数
double coor_trans(double x_input);
//利用Taylor展开式逼近
double taylor(double x);
//改进Euler法
double euler(double x);
//计算阶乘
int fact(int n);

int main( int argc, char** argv) 
{
    double x_input;
    char c;
    do
    {
        cin.clear(); 
        printf("please input x:(press esc + enter to quit)\n");
        cin >> x_input;
        double x = coor_trans(x_input);
        int sgn = copysign(1,x);
        x = fabs(x);
        double y_std = sin(x_input);
        double y_tyl = sgn * taylor(x);
        double y_eul = sgn * euler(x);
        printf("Results:\n");
        printf("Using sine function from cmath library:    y = sin(%.6f) = %.6f\n", x_input, y_std);
        printf("Using Taylor series to approximate:        y = sin(%.6f) = %.6f\n", x_input, y_tyl);
        printf("Using improved Euler method:               y = sin(%.6f) = %.6f\n\n", x_input, y_eul);
        cin.clear();
    }
    while((c = cin.get())!=27);//Unix系统无法使用<conio.h>和getch()函数，所以只能输入esc后按enter
    return 0;
}
double coor_trans(double x_input)
{
    double x;
    double res = fmod(x_input/M_PI_2, 4) + (x_input < 0 ? 4 : 0);//这一步是取余操作，但C++的%只支持整型，而取模函数fmod在负数/正数时取值为负，因此需要转换
    if (res >=0 && res <=1)
        x = res * M_PI_2;
    else if (res >=1 && res <=3)
        x = (2 - res) * M_PI_2;
    else
        x = (res - 4) * M_PI_2;
    return x;
}
double taylor(double x)
{
    return x - pow(x,3)/fact(3) + pow(x,5)/fact(5) - pow(x,7)/fact(7) + pow(x,9)/fact(9) - pow(x,11)/fact(11);
}
double euler(double x)
{
    double h = 1e-4, y = 0, z = 1;
    double y_new, z_new;
    int n = round(x/h);
    for (int i = 1 ; i <= n ; i++ )
    {
        y_new = (1-h*h/2) * y + h * z;
        z_new = (1-h*h/2) * z - h * y;
        y = y_new;
        z = z_new;
    }
    return y;
}
int fact(int n)
{
    int fact = 1;
    for(int i = 1; i <= n; i++ )
        fact = fact * i;  
    return fact;
}