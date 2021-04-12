#include <u_types/u_vector.h>


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
void u_ema_free(u_ema* ma);
void u_ema_solve(u_ema* ema, size_t N);
void u_ema_attach_vector(u_ema* ema, u_vector_float* value);
void u_ema_set_N(u_ema* ema,size_t N);
void ema_print(u_ema* ema);

