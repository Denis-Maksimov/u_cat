// #include "stm_cmsis.h"
#include <u_application/s5_timers.h>


#ifdef __MY_BARE_ARM__


static inline uint32_t get_ticks(void){
    return SysTick_VAL;
}
static inline void set_ticks(uint32_t a){
    SysTick_VAL=a;
}
static inline void add_ticks(uint32_t a){
    SysTick_VAL+=a;
}
#else


static inline clock_t get_ticks(void){
    return clock();
}
static inline void set_ticks(clock_t a){
    // void;
}
static inline void add_ticks(clock_t a){
    // void;
}


#endif // DEBUG


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
    u16 to=40;

    
    float sensor=0.0;
    float heater=0.0;

    while (1)
    {
        // add_ticks(1);//-- за кадром
        // usleep(100); //-- за кадром

        set=!s_pext(&T1, 1000000, set, 0);
        
        if(set){to--;
            // heater=proc_pid(&PID, sensor, 1000.0);
            printf("BA!\n");
            // printf("heater = %f\t",heater);
            // sensor+=0.01*heater-0.01;
            // printf("sensor = %f\n", sensor);
            if(!to)return 0;
            
        }else
        {
            // printf("AH!\n");
        }
        
        
    }
    return 0;
    
}

#endif //__MY_BARE_ARM__



