#include <u_application/u_pid.h>
#include <u_stdlib/u_mallocat.h>

// struct pid PID={.Kp=0.8,.Ki=0.5,.Kd=0.2};

void
init_pid(u_pid* Pid,float Kp,float Ki,float Kd,u_clock_t period)
{
    if(!period)period=150;
    u_memset(Pid, 0, sizeof(u_pid));
    Pid->Kd=Kd;
    Pid->Kp=Kp;
    Pid->Ki=Ki;
    Pid->period=period;
    Pid->set=1;
}

u_pid*
new_pid(float Kp,float Ki,float Kd, u_clock_t period)
{
    u_pid* Pid=malloc(sizeof(u_pid));
    init_pid(Pid,Kp, Ki, Kd, period);
    return Pid;
}

void
free_pid(u_pid* Pid)
{
    free(Pid);
}
static float 
solve_pid(u_pid* Pid,float x,float setting)
{
    float delta=(setting-x);
    Pid->integral+=delta;
    float rv= 
        Pid->Kp*delta
        -
        Pid->Kd*(delta-Pid->prev_delta)
        +
        Pid->Ki*Pid->integral;
    
    Pid->prev_delta=delta;
    return rv;
}
// #include <stdio.h>
float 
proc_pid(u_pid* Pid,float x,float setting)
{
    Pid->set=!s_pext(&(Pid->timer), Pid->period, Pid->set, 0);
    if(Pid->set)
    {
        // printf ("!");
        Pid->value= solve_pid(Pid, x, setting);
    }
    return Pid->value;
}


