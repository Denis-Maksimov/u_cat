#ifndef __U_PID_H__
#define __U_PID_H__

#include <u_application/s5_timers.h>
#include <u_application/s5_timers_double.h>
C_START
typedef struct 
{
    float value;        //--задание на выход
                        
    float Kp;           //-- пропорциональная составляющая
    float Ki;           //-- интегральная составляющая
    float Kd;           //-- дифференциальная составляющая
                        
    float prev_delta;    //-- предыдущая разность между уставкой и значением
    float integral;     //-- интеграл расхождений 

    u_timer timer;	//-- таймер 
    u_clock_t period;   //-- период дискретизации
    u16 set;		//-- *private*

}u_pid;

typedef struct 
{
    double value;        //--задание на выход
                        
    double Kp;           //-- пропорциональная составляющая
    double Ki;           //-- интегральная составляющая
    double Kd;           //-- дифференциальная составляющая
                        
    double prev_delta;    //-- предыдущая разность между уставкой и значением
    double integral;     //-- интеграл расхождений 

    u_timer_double timer;	//-- таймер 
    u_timeval_t period;   //-- период дискретизации
    u16 set;		//-- *private*

}u_pid_d;

/**
 * @brief  инициализация ПИД
 * @note   
 * @param  Pid: хэндл ПИД
 * @param  Kp: пропорциональная составляющая 
 * @param  Ki: интегральная составляющая
 * @param  Kd: дифференциальная составляющая
 * @param  period: период дискретизации
 * @retval None
 */
void init_pid(u_pid* Pid,float Kp,float Ki,float Kd,u_clock_t period);


/**
 * @brief  создаёт ПИД с аллокацией памяти
 * @note   
 * @param  Kp: пропорциональная составляющая 
 * @param  Ki: интегральная составляющая
 * @param  Kd: дифференциальная составляющая
 * @param  period: период дискретизации
 * @retval аллоцированный указатель на ПИД
 */
u_pid*
new_pid(float Kp,float Ki,float Kd, u_clock_t period);

/**
 * @brief  цикл работы ПИД-регулятора
 * @note   
 * @param  Pid: ПИД-регулятор
 * @param  x: текущее значение
 * @param  setting: уставка
 * @retval Y
 */
float proc_pid(u_pid* Pid,float x,float setting);

/**
 * @brief реаллоцирует ПИД 
 * @note   
 * @param  Pid: ПИД-регулятор
 * @retval none
 */
void free_pid(u_pid* Pid);


//=======================================================


/**
 * @brief  инициализация ПИД
 * @note   
 * @param  Pid: хэндл ПИД
 * @param  Kp: пропорциональная составляющая 
 * @param  Ki: интегральная составляющая
 * @param  Kd: дифференциальная составляющая
 * @param  period: период дискретизации
 * @retval None
 */
void 
init_pid_d(u_pid_d* Pid,double Kp,double Ki,double Kd, u_timeval_t period);


/**
 * @brief  создаёт ПИД с аллокацией памяти
 * @note   
 * @param  Kp: пропорциональная составляющая 
 * @param  Ki: интегральная составляющая
 * @param  Kd: дифференциальная составляющая
 * @param  period: период дискретизации
 * @retval аллоцированный указатель на ПИД
 */
u_pid_d*
new_pid_d(double Kp,double Ki,double Kd, u_timeval_t period);

/**
 * @brief  цикл работы ПИД-регулятора
 * @note   
 * @param  Pid: ПИД-регулятор
 * @param  x: текущее значение
 * @param  setting: уставка
 * @retval Y
 */
double 
proc_pid_d(u_pid_d* Pid, double x, double setting);

/**
 * @brief реаллоцирует ПИД 
 * @note   
 * @param  Pid: ПИД-регулятор
 * @retval none
 */
void 
free_pid_d(u_pid_d* Pid);

// s32 clamp(s32 x, s32 low, s32 high);
// float fclamp(float x, float low, float high);

void test_PID();

C_END

#endif // __U_PID_H__

