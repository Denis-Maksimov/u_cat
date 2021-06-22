#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <u_types/u_ring.h>

typedef void (*free_f)(void *);

size_t u_ring_calc_len(u_ring_buf *rb)
{
    if (rb->start_pos < rb->end_pos)
        return rb->end_pos - rb->start_pos;
    if (rb->start_pos > rb->end_pos)
    {
        return rb->len - rb->end_pos + rb->start_pos;
    }
    return 0;
}
size_t u_ring_float_calc_len(u_ring_float *rb)
{
    if (rb->start_pos < rb->end_pos)
        return rb->end_pos - rb->start_pos;
    if (rb->start_pos > rb->end_pos)
    {
        return rb->len - rb->end_pos + rb->start_pos;
    }
    return 0;
}

u_ring_buf *
u_ring_new(size_t n_el, void (*free_f)(void *))
{
    u_ring_buf *rv = malloc(sizeof(u_ring_buf));
    // rv->start_pos = n_el - 1;
    rv->start_pos = 0;
    rv->end_pos = 0;

    rv->data = calloc(n_el + 1, sizeof(size_t));
    memset(rv->data, 0, n_el * sizeof(size_t));
    rv->len = n_el - 1;
    rv->free_func = free_f;
    return rv;
}

void u_ring_free(u_ring_buf *rb)
{
    if (rb->free_func)
    {
        rb->free_func(rb->data);
    }
    free(rb->data);
    free(rb);
}
// void u_ring_resize(u_ring_buf* rb,size_t n_el)
// {
//     rb->data=realloc(rb->data,n_el);
//     rb->len=n_el;
// }

void u_ring_fifo_push(u_ring_buf *rb, void *data)
{
    size_t ep = (rb->end_pos == rb->len) ? (0) : (rb->end_pos + 1);

    if (ep == rb->start_pos)
    {
        if (rb->free_func)
        {
            rb->free_func(rb->data);
        }
        rb->start_pos = (rb->start_pos == rb->len) ? (0) : (rb->start_pos + 1);
    }
    ((size_t *)(rb->data))[rb->end_pos] = (size_t)data;
    rb->end_pos = ep;
}

void *
u_ring_fifo_pop(u_ring_buf *rb)
{
    if(!u_ring_calc_len(rb)) return 0;
    size_t sp = (rb->start_pos == rb->len) ? (0) : (rb->start_pos + 1);

    if (sp == rb->end_pos)
    {
        rb->end_pos = (rb->end_pos == rb->len) ? (0) : (rb->end_pos + 1);
    }
    // ((size_t*)(rb->data))[rb->end_pos]=data;
    void *rv = ((rb->data))[rb->start_pos];
    rb->start_pos = sp;
    return rv;
}

//---------------------------------------------------------------

u_ring_float*
u_ring_float_new(size_t n_el)
{
    u_ring_float *rv = malloc(sizeof(u_ring_float));
    // rv->start_pos = n_el - 1;
    rv->start_pos = 0;
    rv->end_pos = 0;

    rv->data = calloc(n_el + 1, sizeof(float));
    memset(rv->data, 0., n_el * sizeof(float));
    rv->len = n_el - 1;
    return rv;
}
void 
u_ring_float_free(u_ring_float *rb)
{
    free(rb->data);
    free(rb);
}

void 
u_ring_float_fifo_push(u_ring_float *rb, float data) 
{
    size_t ep = (rb->end_pos == rb->len) ? (0) : (rb->end_pos + 1);

    if (ep == rb->start_pos) //--если перекрылись сдвигаем старт
    {
        rb->start_pos = (rb->start_pos == rb->len) ? (0) : (rb->start_pos + 1);
    }
    rb->data[rb->end_pos] = data;
    rb->end_pos = ep;
}

float 
u_ring_float_fifo_pop(u_ring_float *rb) 
{
    size_t len=u_ring_float_calc_len(rb);
    if(!len) return 0.;
    size_t sp = (rb->start_pos == rb->len) ? (0) : (rb->start_pos + 1);

    if (sp == rb->end_pos)
    {
        rb->end_pos = (rb->end_pos == rb->len) ? (0) : (rb->end_pos + 1);
    }
    // ((size_t*)(rb->data))[rb->end_pos]=data;
    float rv = rb->data[rb->start_pos];
    rb->start_pos = sp;
    return rv;
}

float 
u_ring_float_max(u_ring_float *rb)
{
    size_t len=u_ring_float_calc_len(rb);
    if(!len) return 0.;

    size_t iter=rb->start_pos;
    float max = rb->data[iter];
    
    for (size_t i = 0; i < len; i++)
    {
       max = rb->data[iter]>max ? rb->data[iter] : max;
       iter=iter+i>=rb->len?0:iter+1;
    }
    
    return max;
    
}


float 
u_ring_float_min(u_ring_float *rb)
{
    size_t len=u_ring_float_calc_len(rb);
    if(!len) return 0.;

    size_t iter=rb->start_pos;
    float min = rb->data[iter];
    
    for (size_t i = 0; i < len; i++)
    {
       min = rb->data[iter]<min ? rb->data[iter] : min;
       iter=iter+i>=rb->len?0:iter+1;
    }
    
    return min;
    
}

void u_ring_float_resize(u_ring_float *rb, size_t n)
{
    size_t max = u_ring_float_calc_len(rb);
    if (rb->len == n)
        return;

    float* new=calloc(n + 1, sizeof(float));
    if (rb->len > n)
    {
        if (max > n)
        {
            
            for (size_t i = 0; i < n; i++)
            {
                new[i]=u_ring_float_fifo_pop(rb);
            }
            free(rb->data);
            rb->data=new;
            rb->start_pos=0;
            rb->end_pos=n;
            return;
        }
        if (max <= n)
        {
            //TODO
            /* code */
        }
    }
    if (rb->len < n)
        //TODO
        ;
}

