#include "stdio.h"
/*test*/
//test
#define def 3.0;
int a;
int a, b, c;
char a;
char a, b, c;
float a;
float a, b, c;
double a;
double a, b, c;
long a;
long a, b, c;
short a;
short a, b, c;
int f1();
int f2(char a);
int f3(int a, float b);
int f4(int a, char b, float c);
//test
int f5(int c, char a, float x)
{
    int a;
    int a, b, c;
    char a;
    char a, b, c;
    float a;
    float a, b, c;
    double a;
    double a, b, c;
    long a;
    long a, b, c;
    short a;
    short a, b, c;
    a = 0x123;
    a = 0123;
    a = 123;
    a = 123L;
    a = 123S;
    a = 123.1;
    a = 'c';
}
int main()
{
    f5(f5(c, a, x), a, x);
    //test
    a = (((a > c) + (a >= c) - (a < c)) + (a <= c)) + (a && b);
    if (a > c && ((a < c || a >= c) || a <= c)) //test
    {
        if (a != c && (a == c || a))
            return a;
        else //test
        {
            if (a == b)
            {
                while (a == 1) //test
                {
                    while (b == 1)
                        return a;
                    break;
                    continue;
                    return 3.0;
                    //test
                }
                for (;;) //test
                {
                    for (a = 0; a < 3; a = a + 1)
                        a = a + 2;
                    a = a + 2;
                }
                for (a = 0; a < 3; a = a + 1)
                    a = a + 2;
                return;
            }
        }
    }
}