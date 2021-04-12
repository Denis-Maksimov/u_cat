#ifndef __U_PID_H__
#define __U_PID_H__

#include <u_application/s5_timers.h>

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

// s32 clamp(s32 x, s32 low, s32 high);
// float fclamp(float x, float low, float high);


#endif // __U_PID_H__

