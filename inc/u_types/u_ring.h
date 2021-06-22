#ifndef U_RING_H
#define U_RING_H
#include <stdint.h>

typedef struct 
{
    void** data;
    size_t len; //вместимость буфера
    size_t end_pos;
    size_t start_pos;
    void (*free_func)(void*);
}u_ring_buf;



u_ring_buf* u_ring_new(size_t n_el,void (*free_f)(void*));
void u_ring_free(u_ring_buf* rb);
void u_ring_fifo_push(u_ring_buf* rb, void* data);
void* u_ring_fifo_pop(u_ring_buf* rb);
size_t u_ring_calc_len(u_ring_buf* rb);



typedef struct 
{
    float* data;
    size_t len; //вместимость буфера
    size_t end_pos;
    size_t start_pos;
}u_ring_float;

size_t u_ring_float_calc_len(u_ring_float *rb);
u_ring_float* u_ring_float_new(size_t n_el);
void u_ring_float_free(u_ring_float* rb);
void u_ring_float_fifo_push(u_ring_float* rb, float data);
float u_ring_float_fifo_pop(u_ring_float* rb);
float u_ring_float_max(u_ring_float *rb);
float u_ring_float_min(u_ring_float *rb);


#endif // !U_RING_H
