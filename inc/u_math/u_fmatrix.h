#ifndef __U_FMATRIX_H__
#define __U_FMATRIX_H__

#include <u_stdlib/u_stddef.h>
// #include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
C_START
typedef struct
{
    union 
    {
        size_t lenght;
        size_t columns;
    };
    
    union
    {
        size_t height;
        size_t rows;
    };
    
    size_t n;
    float* data;    
} u_fmatrix;

u_fmatrix* u_fmatrix_new(size_t i,size_t j);
void u_fmatrix_free(u_fmatrix* m);

/**
 * @brief  
 * @note   с 0
 * @param  m: 
 * @param  i: строка
 * @param  j: столбец
 * @retval 
 */
float u_fmatrix_at(u_fmatrix* m, size_t i,size_t j);

/**
 * @brief  
 * @note   
 * @param  m: 
 * @param  i: строка
 * @param  j: столбец
 * @param  el: 
 * @retval None
 */
void u_fmatrix_insert(u_fmatrix* m, size_t i,size_t j, float el);



u_fmatrix* u_fmatrix_new_submatrix(u_fmatrix* m,size_t i1,size_t j1, size_t i2,size_t j2);
void u_fmatrix_fprint(FILE* stream, u_fmatrix* m);
u_fmatrix* u_fmatrix_new_T(u_fmatrix* m);

void u_fmatrix_push_line(u_fmatrix* m);
void u_fmatrix_push_column(u_fmatrix** m);
u_fmatrix* u_fmatrix_new_get_line(u_fmatrix* m,size_t line);
u_fmatrix* u_fmatrix_new_get_column(u_fmatrix* m,size_t column);

u_fmatrix* u_fmatrix_mull(u_fmatrix* A,u_fmatrix* B);


/**
 * @brief  решить систему линейных уравнений
 * @note   
 * @param  A: матрица коэффициентов уравнений 
 * @param  V: вектор столбец значений уравнений
 * @retval вектор-столбец значений 
 */
u_fmatrix* u_fmatrix_solve_equation(u_fmatrix* A,u_fmatrix* V);

//скопировать одну матрицу в другую
void u_fmatrix_copy(u_fmatrix* dest,u_fmatrix* src,size_t i_from,size_t j_from);

//определитель матрицы
float u_fmatrix_det(u_fmatrix* a);

//тензорное произведение векторов
u_fmatrix* u_fmatrix_tensor_mull_vectors(u_fmatrix* V1,u_fmatrix* V2);

float u_fmatrix_modul_of_vector(u_fmatrix* V);

void fmatrix_test();
void fmatrix_vector_test();
C_END
#endif // __U_MATRIX_H__
