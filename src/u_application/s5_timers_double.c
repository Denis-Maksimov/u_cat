
#include <u_application/s5_timers_double.h>

// #include <sys/time.h> 
// #include <sys/times.h>
// #include <bits/types/clockid_t.h>
// #include <time.h>
#include <u_stdlib/u_time.h>


static double get_timeval(void){

    // struct timespec ts;
    // clock_gettime(CLOCK_MONOTONIC, &ts);
    // long ms = (ts.tv_nsec / 1000000);
    // double rv= (double)ts.tv_sec;
    // rv+=((double)ms)/1000;
    // return rv;
    
    return ((double)SysGetTick())/1000.;


}


//--------------------------------------------------

u32 u_pulse(u_timer_double* Tx, u_timeval_t tv, u16 set, u16 rst)
{
    if ((!set)||(rst)) {Tx->state=0; return 0;}
    double tmp=get_timeval();
    if(!(Tx->state)){
        Tx->state=1;
        // Tx->value=tv;
        Tx->stv=tmp;
    }

        if ((tmp - Tx->stv) < tv) return 1;

    return 0;
}

//--------------------------------------------------

u32 u_pext(u_timer_double* Tx, u_timeval_t tv, u16 set, u16 rst)
{
    if ((rst)) {Tx->state=0; return 0;}
    double tmp=get_timeval();
    if (set && (!(Tx->state)))
    {
        Tx->state=1;
        // Tx->value=tv;
        Tx->stv=tmp;
    }

    if ((tmp - Tx->stv) < tv) return 1;
    if (!set) Tx->state=0;
    return 0;
}

//--------------------------------------------------

u32 u_odt(u_timer_double* Tx, u_timeval_t tv, u16 set, u16 rst)
{
    if ((rst)) {Tx->state=0; return 0;}
    double tmp=get_timeval();
    if (set && (!(Tx->state)))
    {
        Tx->state=1;
        // Tx->value=tv;
        Tx->stv=tmp;
    }

    if ((tmp - Tx->stv) < tv) return 0;
    if (!set) Tx->state=0;
    return Tx->state;
}

//--------------------------------------------------

u32 u_odts(u_timer_double* Tx, u_timeval_t tv, u16 set, u16 rst)
{
    if ((rst)) {Tx->state=0; return 0;}
    double tmp=get_timeval();
    if (set && (!(Tx->state)))
    {
        Tx->state=1;
        // Tx->value=tv;
        Tx->stv=tmp;
    }

    if ((tmp - Tx->stv) < tv) return 0;
    return Tx->state;
}

//--------------------------------------------------

u32 u_offdt(u_timer_double* Tx, u_timeval_t tv, u16 set, u16 rst)
{
    if ((rst)) {Tx->state=0; return 0;}
    double tmp=get_timeval();
    if (set)
    {
        Tx->state=1;
        // Tx->value=tv;
        Tx->stv=tmp;
    }

    if ((tmp - Tx->stv) < tv) return Tx->state;
    Tx->state=0;
    return Tx->state;
}

//--------------------------------------------------

#ifndef __MY_BARE_ARM__
// extern int main() __attribute__((weak,alias("test_processing")));


#include <stdio.h>
#include <unistd.h>


int test_processing_v2()
{
    u_timer_double T1={0};
    u16 set=1;
    u16 to=10;


    while (1)
    {


        set=!u_pext(&T1, 0.2, set, 0);
        
        if(set){to--;
   
            printf("BA!\n");
            sleep(0);

            if(!to)return 0;
            
        }else
        {
            // printf("AH!\n");
        }
        
        
    }
    return 0;
    
}

#endif //__MY_BARE_ARM__



