#include <u_types/u_vector.h>
#include <u_application/u_ema.h>



u_ema* 
u_ema_new()
{
    u_ema* ma=malloc(sizeof(u_ema));
    ma->value=u_vector_float_new();
    ma->ema=u_vector_float_new();
    ma->N=9;
    // ma->flags=0x00;
    return ma;
}


void
u_ema_push(u_ema* ma, float val)
{
    float alfa= 2./(1.+ (float)ma->N);
    u_vector_float_push_back(ma->value, val);
    u_vector_float_push_back(   
                    ma->ema, 
                    u_vector_float_get_last(ma->value)*(alfa)+
                    u_vector_float_get_last(ma->ema)*(1-alfa)
        );
}

float
u_ema_pop(u_ema* ma)
{
    return u_vector_float_pop_front(ma->value);
}

void
u_ema_fwd(u_ema* ma, float val)
{
    float alfa= 2./(1.+ (float)ma->N);
    u_vector_fifo_forward(ma->value, val);
    u_vector_fifo_forward(   
                    ma->ema, 
                    u_vector_float_get_last(ma->value)*(alfa)+
                    u_vector_float_get_last(ma->ema)*(1-alfa)
        );
// u_vector_fifo_forward
}

void
u_ema_free(u_ema* ma)
{
    // u_ema* ma=malloc(sizeof(u_ema));
    // ma->value=u_vector_float_new();
    // ma->ema=u_vector_float_new();
    u_vector_float_free(ma->value);
    u_vector_float_free(ma->ema);
    free(ma);
    // // ma->flags=0x00;
}

void 
u_ema_solve(u_ema* ema, size_t N)
{
    u_vector_float_deinit(ema->ema);
    float alfa= 2./(1.+ (float)N);
    u_vector_float_push_back(ema->ema, 0.);
    if(!ema->value->n_elem)return;
    for (size_t i = 0; i < ema->value->n_elem; i++)
    {
        
        u_vector_float_push_back(   
                    ema->ema, 
                    u_vector_float_at(ema->value,i)*(alfa)+
                    u_vector_float_at(ema->ema,i)*(1-alfa)
        );
        // ema->ema
    }
    
}

void 
u_ema_attach_vector(u_ema* ema, u_vector_float* value)
{
    u_vector_float_free(ema->value);
    u_vector_float_free(ema->ema);
    ema->value = u_vector_float_copy_new(value);
    u_ema_solve(ema, ema->N);
}

void 
u_ema_set_N(u_ema* ema,size_t N)
{
    if(N)
    {
        ema->N=N;
        u_ema_solve(ema, ema->N);
    }
}

#include <stdio.h>
void
ema_print(u_ema* ema)
{
    for (size_t i = 0; i < ema->value->n_elem; i++)
    {
        printf("val:%f <---> ema:%f\n",
            u_vector_float_at(ema->value,i),
            u_vector_float_at(ema->ema,1+i)
        );
    }
}


//------------------------------------------

float u_avg(u_vector_float* x)
{
    if(!x)return __builtin_inff32();
    float avg=0.;
    for (size_t i = 0; i < x->n_elem; i++)
    {
        avg+=u_vector_float_at(x,i);
    }

    return avg/(float)x->n_elem;

}

//коэффициент ковариации
float u_cov(u_vector_float* x,u_vector_float* y)
{
    float avg_x=u_avg(x);
    float avg_y=u_avg(y);
    size_t n = x->n_elem>y->n_elem ? y->n_elem : x->n_elem;
    float summ=0.;
    for (size_t i = 0; i < n; i++)
    {
        summ+=(u_vector_float_at(x,i)*u_vector_float_at(y,i)-avg_x*avg_y);
    }
    return summ/(float)n;

}

#include <math.h>
//дисперсия
float u_D(u_vector_float* x)
{
    return u_cov(x, x);
}

//среднеквадратичное отклонение
float u_sigma(u_vector_float* x)
{
    return sqrtf(u_D(x));
}

//коэффициент корреляции Пирсона или линейный коэффициент корреляции 
float u_r(u_vector_float* x,u_vector_float* y)
{
    return u_cov(x,y)/(u_sigma(x)*u_sigma(y));
}

/**
 * @brief  формула Стерджеса
 * @note   определяет на сколько интервалов разбивать выборку
 * @param  n: объём выборки
 * @retval 
 */
size_t u_sturges(size_t n)
{
    return (size_t)(roundf(       
        1+3.322*logf((float)n)
    ));
}



float u_median(u_vector_float* x)
{
    if(!x)return __builtin_inff32();
    float sum=0.;
    for (size_t i = 0; i < x->n_elem; i++)
    {
        sum+=u_vector_float_at(x,i);
    }

    return sum/2;

}

//=================================================================

#include <u_algorithm/u_sort.h>

u_ivr* 
u_new_IVR(u_vector_float* x)
{
    
    u_ivr* ivr = malloc(sizeof(u_ivr));
    ivr->source = u_vector_float_copy_new(x);
    bubble_sortf(ivr->source->mem, ivr->source->n_elem);
    ivr->max = u_vector_float_max(ivr->source);
    ivr->min = u_vector_float_min(ivr->source);
    u_vector_float* IVR = u_vector_float_new();
    u_vector_float* F = u_vector_float_new();
    size_t n = u_sturges(ivr->source->n_elem);
    ivr->delta = ivr->max-ivr->min;
    ivr->delta = ivr->delta/((float)n);

    float tmp = 0;
    float tmp_sum=0;
    size_t t=0; //--счётчик границ интервала
    //--для всех отсортированных значений
    for (size_t i = 0; i < ivr->source->n_elem; i++)
    {
        //--если меньше границы интервала то накапливаем значения
        if(u_vector_float_at(ivr->source,i)<(ivr->min+ivr->delta*t))
        {
            tmp=tmp+1.0;
            tmp_sum+=1.;
        }else{
        //--иначе сохраняем
            u_vector_float_push_back(IVR,tmp/(float)ivr->source->n_elem);
            u_vector_float_push_back(F,tmp_sum/(float)ivr->source->n_elem);
            tmp=0.;
            t+=1;
            i--;
        }

    }
    tmp=ivr->source->n_elem-tmp_sum;
    tmp_sum=tmp_sum+tmp;
    u_vector_float_push_back(IVR,tmp/(float)ivr->source->n_elem);
    u_vector_float_push_back(F,tmp_sum/(float)ivr->source->n_elem);


    ivr->IVR=IVR;
    ivr->F=F;
    ivr->n=n;
    
    return ivr;
    
}


float 
u_median_IVR(u_ivr* ivr)
{
    if(ivr->source->n_elem&0x01){
        return u_vector_float_at(ivr->source,ivr->source->n_elem/2+1);
    }
    return u_vector_float_at(ivr->source,ivr->source->n_elem/2)-(ivr->delta/2.);
}

void 
u_free_IVR(u_ivr* ivr)
{
    u_vector_float_free(ivr->F);
    u_vector_float_free(ivr->IVR);
    u_vector_float_free(ivr->source);
    free(ivr);
}

float
u_moda_IVR(u_ivr* ivr)
{
    float right;
    size_t a=u_vector_float_get_indx(ivr->IVR, u_vector_float_max(ivr->IVR));
    // if(a<=ivr->n)
        right = (ivr->min+ivr->delta*a);
    // else
    //     {}
    float left;
    float m_l=ivr->IVR->mem[a-1];
    left = (ivr->min+ivr->delta*(a-1));
    if(a==1){m_l=0.;
        left=ivr->min;
    }
    
    return
    left+(
        (ivr->IVR->mem[a]-m_l)
        /
        (2.*ivr->IVR->mem[a]-m_l-ivr->IVR->mem[a+1])
        )*ivr->delta;
    // else

}

//=================================================================





