#ifndef U_VECTOR_H
#define U_VECTOR_H

#include <u_stdlib/u_stdlib.h>
// #define STDLIB_API extern

typedef struct 
{
    void** mem;        //участок памяти, в котором находятся данные
    size_t n_elem;      //число элементов

}u_vector;

typedef struct 
{
    float* mem;        //участок памяти, в котором находятся данные
    size_t n_elem;      //число элементов   

}u_vector_float;




STDLIB_API u_vector* u_vector_new(void);
STDLIB_API void u_vector_free(u_vector* vec,void(*free_func)(void*));

STDLIB_API void u_vector_init(u_vector* vec);
STDLIB_API void u_vector_deinit(u_vector* vec,void(*free_func)(void*));
STDLIB_API void u_vector_push_back(u_vector* vec, void* el);
STDLIB_API void* u_vector_pop_back(u_vector* vec);
STDLIB_API void* u_vector_at(u_vector* vec,size_t at);
STDLIB_API int u_vector_cmp(u_vector* vec,size_t ind,void* el);
STDLIB_API int u_vector_is_exist(u_vector* vec,void* el);
STDLIB_API int u_vector_get_indx(u_vector* vec,void* el);
STDLIB_API int u_vector_find_indx(u_vector* vec,int(*cmp)(void* el, void* comparation),void* comparation );
//---------------------------------------------------------------------------------
STDLIB_API u_vector_float* u_vector_float_new(void);
STDLIB_API void u_vector_float_free(u_vector_float* vec);

STDLIB_API void u_vector_float_init(u_vector_float* vec);
STDLIB_API void u_vector_float_deinit(u_vector_float* vec);
STDLIB_API void u_vector_float_push_back(u_vector_float* vec, float el);
STDLIB_API float u_vector_float_pop_back(u_vector_float* vec);
STDLIB_API float u_vector_float_at(u_vector_float* vec,size_t at);
STDLIB_API int u_vector_float_cmp(u_vector_float* vec,size_t ind,float el);
STDLIB_API int u_vector_float_is_exist(u_vector_float* vec,float el);
STDLIB_API size_t u_vector_float_get_indx(u_vector_float* vec,float el);
STDLIB_API float u_vector_float_get_last(u_vector_float* vec);
STDLIB_API float u_vector_float_max(u_vector_float* vec);
STDLIB_API float u_vector_float_min(u_vector_float* vec);
STDLIB_API float u_vector_float_pop_front(u_vector_float* vec);
STDLIB_API float u_vector_fifo_forward(u_vector_float* vec, float el); // эквивалентно push_back+pop_front

// STDLIB_API int u_vector_float_find_indx(u_vector_float* vec,int(*cmp)(void* el, void* comparation),void* comparation );
u_vector_float*
u_vector_float_copy_new(u_vector_float* vec);
#endif //!U_VECTOR_H
