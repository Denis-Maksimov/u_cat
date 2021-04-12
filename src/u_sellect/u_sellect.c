/*************************************************
 * За неимением дескрипторов будем изворачиваться
**************************************************/


#include <u_sellect/u_sellect.h>
// typedef long int __fd_mask;




// #define __FD_SETSIZE		1024
// #define __NFDBITS	(8 * (int) sizeof (__fd_mask))



/* Access macros for `fd_set'.  */
// #define	FD_SET(fd, fdsetp)	__FD_SET (fd, fdsetp)
// #define	FD_CLR(fd, fdsetp)	__FD_CLR (fd, fdsetp)
// #define	FD_ISSET(fd, fdsetp)	__FD_ISSET (fd, fdsetp)
// #define	FD_ZERO(fdsetp)		__FD_ZERO (fdsetp)


int u_select (int n, fd_set * __fds,  u_clock_t  __timeout)
{
    u_clock_t to=__timeout+u_clock();
    while (to>u_clock())
    {
        for (unsigned int i = 0; i < n; i++)
        {
            if(
                (__fds->__fds_bits[i].addr[0])!=
                (__fds->__fds_bits[i].old_val)
            ){
                __fds->__fds_bits[i].isset=1;
                __fds->__fds_bits[i].old_val=__fds->__fds_bits[i].addr[0];
                return 1;
            }
                __fds->__fds_bits[i].isset=0;
        }
    // __systime++;
    }
    return 0;

}

//--
void u_fd_set(unsigned int* reg, fd_set * fds)
{
    fds->__fds_bits[fds->n].addr=reg;
    fds->__fds_bits[fds->n].old_val=(*reg);
    fds->n++;
}

int u_fd_isset(unsigned int* reg, fd_set * fds)
{
    for (unsigned int i = 0; i < fds->n; i++)
    {
        if(fds->__fds_bits[i].addr==reg){
            return fds->__fds_bits[i].isset;
        }
    }
    return -1;
}





// #include <stdio.h>
// int sys_tick=0;
// extern int main() __attribute__((weak,alias("sellect_test")));
// int sellect_test()
// {
//     fd_set fdset={0};
//     u_fd_set(&sys_tick, &fdset);
//     while (1)
//     {
//         int ret = u_select( 1, &fdset, 1000000 );
//         // проверяем успешность вызова
//         if ( ret == -1 )
//         {
//             return -1;  
//         }else if( ret == 0 )
//         {
//             // таймаут, событий не произошло 
//             puts("ba");
//             sys_tick++;
//             if(sys_tick==10)return 0;
        
//         }else{
//             // обнаружили событие
//             if (u_fd_isset(&sys_tick,  &fdset) )
//             {
//                 puts("boo");
//             }
//         }   
//     }
//     return 0;
// }
















