#ifndef __U_FUZZY_H__
#define __U_FUZZY_H__

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <u_types/u_vector.h>


#define MIN(x,y)    (((x)<(y))?(x):(y))
#define MAX(x,y)    (((x)>(y))?(x):(y))

typedef 
enum {
    OR,
    OR_NOT,
    OR_HIGHLY,
    OR_MORE_OR_LESS,

    AND,
    AND_NOT,
    AND_HIGHLY,
    AND_MORE_OR_LESS
}op_type;



typedef struct{

    u_vector* rulez;
    size_t n_params;
    
    float sigma; //дисперсия для гауссианы

}u_fuzzy;



u_fuzzy* u_fuzzy_new(size_t n_params);
void u_fuzzy_free(u_fuzzy* fuzzy);




void u_fuzzy_add_rule(   u_fuzzy* fuzzy,    //хэндл
                    op_type operand,        //как рассматривать параметры
                    float* params,          //массив параметров
                    size_t n,               //число параметров
                    float output);           //реакция на правило


/***************************************************
 * @brief  цикл работы регулятора
 * @note   
 * @param  fuzzy: хэндл
 * @param  _x: показания с датчика
 * @param  _n: размер массива показаний
 * @retval воздействие на исполнительный механизм
 ***************************************************/
float u_fuzzy_update(u_fuzzy* fuzzy, float* _x, size_t _n);


void u_fuzzy_set_sigma(u_fuzzy* fuzzy, float sigma);



void u_fuzzy_test(void);


#endif // __U_FUZZY_H__