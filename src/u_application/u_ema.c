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