// #include "stm_cmsis.h"
#include <u_stdlib/u_time.h>
#include <u_application/s5_timers.h>


// #include <sys/time.h>
#include <stdio.h>


unsigned long long time_RDTSC()
{ 
    union ticks{ 
        unsigned long long tx;
        struct dblword 
        {
            long tl,th; 
        } dw; // little endian
    } t;

    asm("rdtsc\n": "=a"(t.dw.tl),"=d"(t.dw.th));
    return t.tx;

} // for x86 only!




// #include <sys/time.h> 
// #include <sys/times.h>
// #include <bits/types/clockid_t.h>
// #include <time.h>


static inline u_clock_t get_ticks(void){

    return SysGetTick();
    // struct timespec ts;
    // clock_gettime(CLOCK_MONOTONIC, &ts);
    // return (u_clock_t) (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);

}



//--------------------------------------------------

u32 s_pulse(u_timer* Tx, u32 tv, u16 set, u16 rst)
{
    if ((!set)||(rst)) {Tx->state=0; return 0;}
    if(!(Tx->state)){
        Tx->state=1;
        // Tx->value=tv;
        Tx->stv=get_ticks();
    }

        if (get_ticks() - Tx->stv < tv) return 1;

    return 0;
}

//--------------------------------------------------

u32 s_pext(u_timer* Tx, u32 tv, u16 set, u16 rst)
{
    if ((rst)) {Tx->state=0; return 0;}
    if (set && (!(Tx->state)))
    {
        Tx->state=1;
        // Tx->value=tv;
        Tx->stv=get_ticks();
    }

    if (get_ticks() - Tx->stv < tv) return 1;
    if (!set) Tx->state=0;
    return 0;
}

//--------------------------------------------------

u32 s_odt(u_timer* Tx, u32 tv, u16 set, u16 rst)
{
    if ((rst)) {Tx->state=0; return 0;}
    if (set && (!(Tx->state)))
    {
        Tx->state=1;
        // Tx->value=tv;
        Tx->stv=get_ticks();
    }

    if (get_ticks() - Tx->stv < tv) return 0;
    if (!set) Tx->state=0;
    return Tx->state;
}

//--------------------------------------------------

u32 s_odts(u_timer* Tx, u32 tv, u16 set, u16 rst)
{
    if ((rst)) {Tx->state=0; return 0;}
    if (set && (!(Tx->state)))
    {
        Tx->state=1;
        // Tx->value=tv;
        Tx->stv=get_ticks();
    }

    if (get_ticks() - Tx->stv < tv) return 0;
    return Tx->state;
}

//--------------------------------------------------

u32 s_offdt(u_timer* Tx, u32 tv, u16 set, u16 rst)
{
    if ((rst)) {Tx->state=0; return 0;}
    if (set)
    {
        Tx->state=1;
        // Tx->value=tv;
        Tx->stv=get_ticks();
    }

    if (get_ticks() - Tx->stv < tv) return Tx->state;
    Tx->state=0;
    return Tx->state;
}

//--------------------------------------------------

#ifndef __MY_BARE_ARM__
extern int main() __attribute__((weak,alias("test_processing")));


#include <stdio.h>

#include <unistd.h>


int test_processing()
{
    u_timer T1={0};
    u16 set=1;
    u16 to=10;

    
    float sensor=0.0;
    float heater=0.0;

    while (1)
    {
        // add_ticks(1);//-- ???? ????????????
        // usleep(100); //-- ???? ????????????

        set=!s_pext(&T1, 1000000, set, 0);
        
        if(set){to--;
            // heater=proc_pid(&PID, sensor, 1000.0);
            printf("BA!\n");
            // printf("heater = %f\t",heater);
            // sensor+=0.01*heater-0.01;
            // printf("sensor = %f\n", sensor);
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



