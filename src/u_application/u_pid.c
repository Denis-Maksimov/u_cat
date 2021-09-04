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


#include <stdio.h>
#include <unistd.h>

float 
proc_pid(u_pid* Pid,float x,float setting)
{
    Pid->set=!s_pext(&(Pid->timer), Pid->period, Pid->set, 0);
    if(Pid->set)
    {
        
        Pid->value= solve_pid(Pid, x, setting);
        printf ("%f!\n",Pid->value);
        fflush(stdout);
    }


    sleep(0);

    return Pid->value;
}



//===============================================================


void
init_pid_d(u_pid_d* Pid,double Kp,double Ki,double Kd, u_timeval_t period)
{
    if(period <= 0.0) 
    {
        period = 1.;
    }
    u_memset(Pid, 0, sizeof(u_pid_d));

    Pid->Kd=Kd;
    Pid->Kp=Kp;
    Pid->Ki=Ki;
    Pid->period=period;
    Pid->set=1;
}

u_pid_d*
new_pid_d(double Kp,double Ki,double Kd, u_timeval_t period)
{
    u_pid_d* Pid=malloc(sizeof(u_pid_d));
    init_pid_d(Pid,Kp, Ki, Kd, period);
    return Pid;
}

void 
free_pid_d(u_pid_d* Pid)
{
    free(Pid);
}
static double 
solve_pid_d(u_pid_d* Pid,double x, double setting)
{
    double delta=(setting-x);
    Pid->integral+=delta;
    double rv= 
        Pid->Kp*delta
        -
        Pid->Kd*(delta-Pid->prev_delta)
        +
        Pid->Ki*Pid->integral;
    
    Pid->prev_delta=delta;
    return rv;
}


double 
proc_pid_d(u_pid_d* Pid, double x, double setting)
{
    // Pid->set=(!u_pext(&Pid->timer, Pid->period, Pid->set, 0));

    
    if(!u_offdt(&Pid->timer,Pid->period,0,0))
    {
        u_offdt(&Pid->timer,Pid->period,1,0);

        Pid->value= solve_pid_d(Pid, x, setting);
        
        printf ("%f!\n",Pid->value);
        fflush(stdout);
    }
    sleep(0);
    return Pid->value;
}


double machine(double x, double job)
{
    return (job*0.01)+x-0.0001;
}



void 
test_PID()
{

    double setting = 36.6;
    double temp= 0.;
    double heater_job;
    u_pid_d* Pid=
    new_pid_d( 3.8, 1. ,1.,1.);

    u_timer_double T1={0};
    u_timer_double T2={0};
    u16 sets=1;
    u_offdt(&T1,25.,1,0);

    while(u_offdt(&T1,25.,0,0))
    {
       heater_job=proc_pid_d(Pid, temp, setting);
       

       sets=!u_pext(&T2, 0.1, sets, 0);
       if(sets){
           temp = machine(temp,heater_job);
           printf("temperature= %f\n",temp);
       }
       
       sleep(0);
    }
    

}







