#ifndef __U_SELLECT_H__
#define __U_SELLECT_H__

/*************************************
* Всё это хорошо, вот только нахуя?
* ***********************************/

// #include "Sys_mgr.h"
// typedef unsigned int  u_clock_t;
#include <time.h>
typedef clock_t u_clock_t;

typedef struct{
    unsigned int* addr;
    unsigned int  old_val;
    int isset;
}__fd_mask;

typedef struct
  {
    __fd_mask __fds_bits[32];
    unsigned int n;
  } fd_set;


int u_select (int n, fd_set * __fds, u_clock_t  __timeout);

void u_fd_set(unsigned int* reg, fd_set * fds);
int u_fd_isset(unsigned int* reg, fd_set * fds);



#endif // __U_SELLECT_H__