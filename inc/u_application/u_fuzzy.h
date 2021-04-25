#ifndef __U_FUZZY_H__
#define __U_FUZZY_H__

#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define FUZZY_VERY_LOW_NEGATIVE -125
#define FUZZY_LOW_NEGATIVE  -90
#define FUZZY_MEDIUM_NEGATIVE  -55
#define FUZZY_SMALL_NEGATIVE  -20
#define fNO   0
#define FUZZY_SMALL_POSITIVE   20
#define FUZZY_MEDIUM_POSITIVE   55
#define FUZZY_LOW_POSITIVE   90
#define FUZZY_VERY_LOW_POSITIVE  125

#define MIN(x,y)    (((x)<(y))?(x):(y))
#define MAX(x,y)    (((x)>(y))?(x):(y))

typedef 
enum {
    AND,
    OR
}op_type;

struct rule
{
    int8_t fe;  //fuzzy error
    op_type op; // and/or
    int8_t fde; //fuzzy delta e
    int8_t z;   //выходной сигнал
};










#endif // __U_FUZZY_H__