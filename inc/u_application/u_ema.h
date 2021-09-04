#ifndef __U_EMA_H__
#define __U_EMA_H__

#include <u_types/u_vector.h>

C_START
typedef struct u_ema_t
{
    // size_t flags;
    u_vector_float* value;
    u_vector_float* ema; //--экспоненциальная скользящая средняя
    // u_vector_float* sma; //--скользящая средняя
    size_t N;
}u_ema;


u_ema* u_ema_new(void);
void u_ema_push(u_ema* ma, float val);
float u_ema_pop(u_ema* ma);
void u_ema_fwd(u_ema* ma, float val);
void u_ema_free(u_ema* ma);
void u_ema_solve(u_ema* ema, size_t N);
void u_ema_attach_vector(u_ema* ema, u_vector_float* value);
void u_ema_set_N(u_ema* ema,size_t N);
void ema_print(u_ema* ema);

//--------------------------------------

// среднее
float u_avg(u_vector_float* x);

// коэффициент ковариации
float u_cov(u_vector_float* x,u_vector_float* y);

//дисперсия
float u_D(u_vector_float* x);

//среднеквадратичное отклонение
float u_sigma(u_vector_float* x);

//коэффициент корреляции Пирсона или линейный коэффициент корреляции 
float u_r(u_vector_float* x,u_vector_float* y);

//--------------------------------------

typedef struct
{
    u_vector_float* source; //--отсортированый исходник
    u_vector_float* IVR;    //--интервальный вариационный ряд
    u_vector_float* F;      //--функция распределения случайной величины
    float delta;            //--ширина интервала разбиения (по Стёрджесу)
    float max;              //--максимальное значение
    float min;              //--минимальное значение
    size_t n;               //--число интервалов разбивки
}u_ivr;

u_ivr* u_new_IVR(u_vector_float* x);
float u_median_IVR(u_ivr* ivr);
float u_moda_IVR(u_ivr* ivr);
void u_free_IVR(u_ivr* ivr);

C_END
#endif // __U_EMA_H__