
#include <math.h>


double u4_Tj(double x,double n)
{
    return
    0.5*(
        pow((x+sqrt(x*x-1)),n)
        +
        pow((x-sqrt(x*x-1)),n)
    );
}