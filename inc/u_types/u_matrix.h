#ifndef __U_MATRIX_H__
#define __U_MATRIX_H__

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    size_t lenght;
    size_t height;
    size_t n;
    void** data;    
} u_matrix;

u_matrix* u_matrix_new(size_t i,size_t j);
void u_matrix_free(u_matrix* m);

/**
 * @brief  
 * @note   с 0
 * @param  m: 
 * @param  i: строка
 * @param  j: столбец
 * @retval 
 */
size_t* u_matrix_at(u_matrix* m, size_t i,size_t j);

/**
 * @brief  
 * @note   
 * @param  m: 
 * @param  i: строка
 * @param  j: столбец
 * @param  el: 
 * @retval None
 */
void u_matrix_insert(u_matrix* m, size_t i,size_t j, void* el);



u_matrix* u_matrix_new_submatrix(u_matrix* m,size_t i1,size_t j1, size_t i2,size_t j2);
void u_matrix_fprint(FILE* stream, u_matrix* m);
u_matrix* u_matrix_new_T(u_matrix* m);

void u_matrix_push_line(u_matrix* m);
void u_matrix_push_column(u_matrix** m);
u_matrix* u_matrix_new_get_line(u_matrix* m,size_t line);
u_matrix* u_matrix_new_get_column(u_matrix* m,size_t column);
#endif // __U_MATRIX_H__
