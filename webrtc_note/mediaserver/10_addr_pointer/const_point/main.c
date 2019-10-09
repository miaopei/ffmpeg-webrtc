#include <stdio.h>

int main(int argc, char* argv[])
{
    int p = 3;
    int a = 4;
    const int pp = 4;
    const int *point = &pp;

    int const *p2 = &pp;

    int* const p3 = &p;

    const int* const p4 = &p;
    int const * const p5 = &p;


    printf("p = %d \n", p);
    //////////////////////////

    printf("pp = %d \n", pp);
    //pp  = 5; //报错
    //////////////////////////

    printf("*point = %d \n", *point);
    //*point = 7; //报错
    point = &p;
    printf("*point = %d \n", *point);

    //////////////////////////
    
    printf("*p2 = %d \n", *p2);
    //*p2 = 8; //报错
    p2 = &p;
    printf("*p2 = %d \n", *p2);

    //////////////////////////
    printf("*p3 = %d \n", *p3);
    *p3 = 9;
    printf("*p3 = %d \n", *p3);
    //p3 = &a; //报错
    //////////////////////////

    printf("*p4 = %d \n", *p4);
    //*p4 = 9; //报错
    //p4 = &a; //报错
    //
    //////////////////////////

    printf("*p5 = %d \n", *p5);
    //*p5 = 9; //报错
    //p5 = &a; //报错

}
