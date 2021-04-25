#include <u_types/u_vector.h>




void 
u_vector_init(u_vector* vec)
{
    if(!vec)return;
    vec->mem=0;
    vec->n_elem=0;
}

u_vector* 
u_vector_new(void)
{
    u_vector* rv=malloc(sizeof(u_vector));
    u_vector_init(rv);
    return rv;
}

void u_vector_deinit(u_vector* vec,void(*free_func)(void*))
{
    if(!vec)return;
    if(free_func){
        for (size_t i = 0; i < vec->n_elem; i++)
        {
            free_func((vec->mem[i]));
        }   
    }
    free(vec->mem);
    vec->mem=0;
    vec->n_elem=0;
}

void 
u_vector_free(u_vector* vec,void(*free_func)(void*))
{
    if(!vec)return;
    u_vector_deinit(vec, free_func);
    free(vec);
}


void u_vector_push_back(u_vector* vec, void* el)
{
    if(!vec)return;
    vec->n_elem++;
    vec->mem=realloc(vec->mem, (sizeof(size_t))*vec->n_elem);
    vec->mem[vec->n_elem-1]=el;

}

void* u_vector_pop_back(u_vector* vec)
{
    if(!vec)return 0;
    if(!vec->n_elem)return 0;

    vec->n_elem--;
    void* rv = vec->mem[vec->n_elem];
    vec->mem=realloc(vec->mem, sizeof(size_t)*vec->n_elem);
    return rv;
}


void* u_vector_at(u_vector* vec,size_t at)
{
    if(!vec)return 0;
    if(at>=vec->n_elem) return 0;
    return vec->mem[at];
}

#define IS_EQUAL    0
#define NO_EQUAL    1

int u_vector_cmp(u_vector* vec,size_t ind,void* el)
{
    if(!vec)return -1;
    if(u_vector_at(vec, ind)==el)
        return IS_EQUAL;
    return NO_EQUAL;
}

int u_vector_is_exist(u_vector* vec,void* el)
{
    if(!vec)return -1;
    for (size_t i = 0; i < vec->n_elem; i++)
    {
        if(!u_vector_cmp(vec,i,el))return 1;
    }
    return 0;
}

int u_vector_get_indx(u_vector* vec,void* el)
{
    if(!vec)return -1;
    for (size_t i = 0; i < vec->n_elem; i++)
    {
        if(!u_vector_cmp(vec,i,el))return i;
    }
    return 0;
}


int u_vector_find_indx(u_vector* vec,int(*cmp)(void* el, void* comparation),void* comparation )
{
    if(!vec)return -1;
    for (size_t i = 0; i < vec->n_elem; i++)
    {
        if(!cmp(vec->mem[i], comparation))
            return i;
    }
    return 0;
}


void* u_vector_find(u_vector* vec,int(*cmp)(void* el, void* comparation),void* comparation )
{
    if(!vec)return 0;
    for (size_t i = 0; i < vec->n_elem; i++)
    {
        if(!cmp(vec->mem[i], comparation))
            return vec->mem[i];
    }
    return 0;
}


//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------

void 
u_vector_float_init(u_vector_float* vec)
{
    if(!vec)return;
    vec->mem=0;
    vec->n_elem=0;
}

u_vector_float* 
u_vector_float_new(void)
{
    u_vector_float* rv=malloc(sizeof(u_vector_float));
    u_vector_float_init(rv);
    return rv;
}

void u_vector_float_deinit(u_vector_float* vec)
{
    if(!vec)return;

    free(vec->mem);
    vec->mem=0;
    vec->n_elem=0;
}

void 
u_vector_float_free(u_vector_float* vec)
{
    if(!vec)return;
    u_vector_float_deinit(vec);
    free(vec);
}


void u_vector_float_push_back(u_vector_float* vec, float el)
{
    if(!vec)return;
    if(!vec->n_elem){
        vec->n_elem++;
        vec->mem=malloc(sizeof(float));
    }else{
        vec->n_elem++;
        vec->mem=realloc(vec->mem, (sizeof(float))*vec->n_elem);
    }
    vec->mem[vec->n_elem-1]=el;
}

float u_vector_float_pop_back(u_vector_float* vec)
{
    if(!vec)return 0;
    if(!vec->n_elem)return 0;

    vec->n_elem--;
    float rv = vec->mem[vec->n_elem];
    vec->mem=realloc(vec->mem, sizeof(float)*vec->n_elem);
    return rv;
}


float u_vector_float_at(u_vector_float* vec,size_t at)
{
    if(!vec)return 0.;
    if(at>=vec->n_elem) return 0.;
    return vec->mem[at];
}

u_vector_float*
u_vector_float_copy_new(u_vector_float* vec)
{
    if(!vec)return 0;
    u_vector_float* rv = u_vector_float_new();
    rv->n_elem=vec->n_elem;
    rv->mem=calloc(vec->n_elem,sizeof(float));
    memcpy(rv->mem,vec->mem,sizeof(float)*(vec->n_elem));
    return rv;
}

#define IS_EQUAL    0
#define NO_EQUAL    1

int u_vector_float_cmp(u_vector_float* vec,size_t ind,float el)
{
    if(!vec)return -1;
    if(u_vector_float_at(vec, ind)==el)
        return IS_EQUAL;
    return NO_EQUAL;
}

int u_vector_float_is_exist(u_vector_float* vec,float el)
{
    if(!vec)return -1;
    for (size_t i = 0; i < vec->n_elem; i++)
    {
        if(!u_vector_float_cmp(vec,i,el))return 1;
    }
    return 0;
}

size_t u_vector_float_get_indx(u_vector_float* vec,float el)
{
    if(!vec)return -1;
    for (size_t i = 0; i < vec->n_elem; i++)
    {
        if(!u_vector_float_cmp(vec,i,el))return i;
    }
    return 0;
}

float u_vector_float_get_last(u_vector_float* vec){
    if(!vec)return 0; 
    if(!vec->n_elem) return 0;
    return vec->mem[vec->n_elem-1];
}

float 
u_vector_float_min(u_vector_float* vec)
{
    if(!vec)return 0.;
    float min=u_vector_float_at(vec,0);
    float tmp;
    for (size_t i = 1; i < vec->n_elem; i++)
    {
        tmp=u_vector_float_at(vec,i);
        min=min<tmp?min:tmp;
    }
    return min;
}
float 
u_vector_float_max(u_vector_float* vec)
{
    if(!vec)return 0.;
    float max=u_vector_float_at(vec,0);
    float tmp;
    for (size_t i = 1; i < vec->n_elem; i++)
    {
        tmp=u_vector_float_at(vec,i);
        max=max<tmp?tmp:max;
    }
    return max;
}
// int u_vector_find_indx(u_vector* vec,int(*cmp)(void* el, void* comparation),void* comparation )
// {
//     if(!vec)return -1;
//     for (size_t i = 0; i < vec->n_elem; i++)
//     {
//         if(!cmp(vec->mem[i], comparation))
//             return i;
//     }
//     return 0;
// }


// void* u_vector_find(u_vector* vec,int(*cmp)(void* el, void* comparation),void* comparation )
// {
//     if(!vec)return 0;
//     for (size_t i = 0; i < vec->n_elem; i++)
//     {
//         if(!cmp(vec->mem[i], comparation))
//             return vec->mem[i];
//     }
//     return 0;
// }






