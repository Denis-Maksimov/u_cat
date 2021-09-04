#ifndef U_TIME_H
#define U_TIME_H
//---------------------------------------------------------------------------
#include <u_stdlib/u_stddef.h>
//---------------------------------------------------------------------------
#ifdef OS_OSX
# define CLOCK_MONOTONIC 0
#endif

C_START
longword SysGetTick();
void SysSleep(longword Delay_ms);
C_END
longword DeltaTime(longword* Elapsed);

#endif // U_TIME_H
