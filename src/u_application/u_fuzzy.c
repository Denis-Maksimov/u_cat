#include <u_application/u_fuzzy.h>


uint8_t num_of_rulez =0;

struct rule
{
    op_type op; // как проверять совокупность параметров
    float G;   //выходной сигнал
    float* params;    //набор контролируемых параметров
};

#ifndef M_PI
# define M_PI		3.14159265358979323846
#endif // !M_PI

#define U_COEFF(sigma) (1./sigma*sqrtf(2.*M_PI))


//            2
//    _ _(x-A)_________
//                  2
//        2*  sigma  
//      
// e
//
//x-элемент А-нечёткое множество
static float 
u_gauss(float x, float Mo,float sigma){
    return U_COEFF(sigma)*expf(-(powf(x-Mo,2.)/(2.*powf(sigma,2.))));
}

void 
u_fuzzy_set_sigma(u_fuzzy* fuzzy, float sigma)
{
    fuzzy->sigma=sigma;
}

// void 
// u_fuzzy_set_job(u_fuzzy* fuzzy, float setting)
// {
//     fuzzy->setting=setting;
// }


u_fuzzy* 
u_fuzzy_new(size_t n_params)
{
    u_fuzzy* rv = malloc(sizeof(u_fuzzy));
    rv->rulez=u_vector_new();
    
    rv->sigma=30.; //default val
    rv->n_params=n_params;
    // rv->_setting=calloc(n_params,sizeof(float));
    return rv;
}

static
void
_free_rule(void* v){
    free(((struct rule*)v)->params);
    free(v);
}
void
u_fuzzy_free(u_fuzzy* fuzzy)
{
    u_vector_free(fuzzy->rulez,_free_rule);
    free(fuzzy);
}

void
u_fuzzy_add_rule(   u_fuzzy* fuzzy,
                    op_type operand, 
                    float* params,
                    size_t n,
                    float output)
{
    struct rule* rulezzz= malloc(sizeof(struct rule));
    // rulezzz->fe=fuzzy_error;
    rulezzz->op=operand;
    // rulezzz->fde=fuzzy_delta_E;
    rulezzz->params=  calloc(n,sizeof(float));
    for (size_t i = 0; i < n; i++)
    {
      rulezzz->params[i]=params[i];      
    }
    rulezzz->G=output;
    u_vector_push_back(fuzzy->rulez,rulezzz);
}



float
u_fuzzy_update(u_fuzzy* fuzzy, float* _x, size_t _n)
{
    float summ_alpha_c = 0., summ_alpha = 0.;
    float* _mu = calloc(_n,sizeof(float));


    float min,max;
    //пробегаем по правилам и смотрим в какой степени соответствуетт каждому правилу
    for (size_t i = 0; i < fuzzy->rulez->n_elem; i++)
    {
        struct rule* tmp;
        float alpha=0.,mue=0.,mude=0.;
        
        tmp=u_vector_at(fuzzy->rulez,i);
        
        for (size_t i = 0; i < fuzzy->n_params; i++)
        {
            
            _mu[i]=u_gauss(_x[i], tmp->params[i], fuzzy->sigma);
            if(!i)min=max=_mu[0];
            else{
                min=U_MIN(min,_mu[i]);
                max=U_MAX(max,_mu[i]);
            }
        }

        switch (tmp->op)
        {
        
        case OR:
            alpha=max;
            break;
        case OR_NOT:
            alpha=1.-max;
            break;
        case OR_HIGHLY:
            alpha=powf(max,2.);
            break;
        case OR_MORE_OR_LESS:
            alpha=sqrtf(max);
            break;
        //======================================
        case AND:
            alpha=min;
            break;
        case AND_NOT:
            alpha=1.-min;
            break;
        case AND_MORE_OR_LESS:
            alpha=sqrtf(min);
            break;
        case AND_HIGHLY:
            alpha=powf(min,2.);
            break;
        default:
            printf("invalid fuzzy operand!\n");
            break;
        }
        summ_alpha_c += (alpha*tmp->G);
        printf("альфа:%f\n",alpha);
        summ_alpha += alpha;
    }
    // fuzzy->prev_delta=(e);
    free(_mu);
    // free(_e);
    return summ_alpha_c/summ_alpha;
}




void 
u_fuzzy_test(void)
{

    u_fuzzy* fuzzy=u_fuzzy_new(3);
    u_fuzzy_set_sigma(fuzzy, 12.);
    float par0[]={0.,0.,0.};
    float par1[]={0.,0.,40.};
    float par2[]={0.,0.,80.};

    u_fuzzy_add_rule(fuzzy,AND,par0,3, 40.);
    u_fuzzy_add_rule(fuzzy,AND,par1,3, 80.);
    u_fuzzy_add_rule(fuzzy,AND,par2,3, 0.);


    // fuzzy->_setting[1]=2.5;
    float x[]={0.,0.,0.};
    float y;
    for (size_t i = 0; i < 100; i++)
    {
        // if(i==50)fuzzy->setting=125.;
        y=u_fuzzy_update(fuzzy, x,3);
        x[2]=(y);
        printf("%f;%f;%f\n", x[0],x[1],x[2]);
        
    }




    u_fuzzy_free(fuzzy);

}
