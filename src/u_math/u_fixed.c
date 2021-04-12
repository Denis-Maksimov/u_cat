
typedef union
{
    int raw;
    struct{
        unsigned int n:16;  //дробное
        int m:16; //целое
        
    };
    
}u_fixed16;

u_fixed16 fix_add(u_fixed16 a, u_fixed16 b)
{
    u_fixed16 c={.raw=a.raw+b.raw};
    return c;
}
u_fixed16 fix_sub(u_fixed16 a, u_fixed16 b)
{
    u_fixed16 c={.raw=a.raw-b.raw};
    return c;
}

u_fixed16 fix_mull(u_fixed16 a, u_fixed16 b)
{
    int sign=a.raw & 0x80000000;
    a.raw &=~ 0x80000000;

    long aa=(long)a.raw;
    long bb=(long)b.raw;
    long r=(aa*bb)>>16;
    u_fixed16 rv={.raw=r|sign};
    return rv;
}
u_fixed16 fix_div(u_fixed16 a, u_fixed16 b)
{
    int sign=a.raw & 0x80000000;
    a.raw &=~ 0x80000000;

    long aa=(long)a.raw<<16;
    long bb=(long)b.raw;
    long r=(aa/bb);
    u_fixed16 rv={.raw=r|sign};
    return rv;
}

#ifdef _STDIO_H

#include <stdio.h>
void print_drob(u_fixed16 a){

    printf("%d.",a.m);
    u_fixed16 b={.m=10000,.n=0};
    a.m=0;
    a=fix_mull(a,b);
    printf("%d\n",a.m);
}
#endif // DEBUG

u_fixed16 u_int_to_fixed16(short m,unsigned short n){
    u_fixed16 a={.m=n,.n=0};
    u_fixed16 b={.m=10000,.n=0};
    b=fix_div(a,b);
    b.m=m;
    b.n+=1;
    // print_drob(b);
    return b;
}
u_fixed16 u_sqrt(u_fixed16 A);
u_fixed16 u_ln(u_fixed16 A);


__attribute__((weak)) int main(int argc, char const *argv[]) 
{
    u_fixed16 a=u_int_to_fixed16(3,0000);
    a=u_ln(a);

    // print_drob(a);
  
    return 0;
}


u_fixed16 u_sqrt(u_fixed16 A)
{
    u_fixed16 x=fix_div(A, (u_fixed16)0x00020000);
    for (int k=0;k<20;k++){
        x=fix_div(fix_add(x, fix_div(A,x)),(u_fixed16)0x00020000);
    }
    return x;
}


/*
*    A^n
* ------------------------
* A^x'= A^x * ln(A)
* A^x''= A^x * ln(A) * ln(A)
* 
*       f(x0)'              f(x0)''
* C =   ------ * (x-x0) +  ------ * (x-x0)^2 + ... + остаточное выражение
*         1!                 2!
* 
* x0 = 0  <= Превратим Тейлора в Маклорена
* 
* 
*  ln(A) * x + 0.5 * (ln(A)* x)^2 + 0.1667 * (ln(A)* x)^3  + 0.0417 * (ln(A)* x)^4
* 
* */

/*
*    ln(a)
* ------------------------
* ln(x)'    = 1/x
* ln(x)''   =-1/x^2
* ln(x)'''  = 2/x^3
* ln(x)'''  =-6/x^4
* ln(x)'''' = 24/x^5
* 
*          f(x0)'              f(x0)''
* f(x) =   ------ * (x-x0) +  ------ * (x-x0)^2 + ... + остаточное выражение
*            1!                 2!
* 
* x0 = 1
* 
* 
* f(x) = (x-1)  - 1/((x-1)^2)*2 + 3/(x-1)^3 - 4/(x-1)^4
* 
* */
u_fixed16 u_pow(u_fixed16 A, u_fixed16 X){

}
u_fixed16 _step(u_fixed16 A, int X){
    if(!X){return u_int_to_fixed16(1,0);}
    u_fixed16 x=u_int_to_fixed16(X,0);
    for(int i=1;i<X;i++){
        A=fix_mull(A, x);
    }
    return A;
}
u_fixed16 u_ln(u_fixed16 A){
    u_fixed16 a=fix_div( u_int_to_fixed16(1,0),_step(fix_sub(A,u_int_to_fixed16(1,0)),1));
    u_fixed16 b= _step(fix_sub(A,u_int_to_fixed16(1,0)),2);
    b=fix_div(u_int_to_fixed16(1,0),b);
    b=fix_div(b,u_int_to_fixed16(2,0));
    a.raw-=b.raw;

   

    for(int i=3;i<500;i){

        b= _step(fix_sub(A,u_int_to_fixed16(1,0)),i);
        b=fix_div(u_int_to_fixed16(1,0),b);
        b=fix_div(b,u_int_to_fixed16(i,0));
        a.raw+=b.raw;
        i++;
        // print_drob(a);
        b= _step(fix_sub(A,u_int_to_fixed16(1,0)),i);
        b=fix_div(u_int_to_fixed16(1,0),b);
        b=fix_div(b,u_int_to_fixed16(i,0));
        a.raw-=b.raw;
        i++;
        // print_drob(a);
    }


    return a;
}