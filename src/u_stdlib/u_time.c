#include <u_stdlib/u_time.h>

#ifdef OS_OSX
int clock_gettime(int clk_id, struct timespec* t) 
{
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}
#endif

//---------------------------------------------------------------------------
longword SysGetTick()
{
#ifdef OS_WINDOWS
    return timeGetTime();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (longword) (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
#endif
}
//---------------------------------------------------------------------------
void SysSleep(longword Delay_ms)
{
#ifdef OS_WINDOWS
	Sleep(Delay_ms);
#else
    struct timespec ts;
    ts.tv_sec = (time_t)(Delay_ms / 1000);
    ts.tv_nsec =(long)((Delay_ms - ts.tv_sec) * 1000000);
    nanosleep(&ts, (struct timespec *)0);
#endif
}
//---------------------------------------------------------------------------
longword DeltaTime(longword* Elapsed)
{
    longword TheTime;
    TheTime=SysGetTick();
    // Checks for rollover
    if (TheTime<(*Elapsed))
        *Elapsed=0;
    return TheTime-(*Elapsed);
}
