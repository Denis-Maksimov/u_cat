#ifndef __FSM_H__
#define __FSM_H__

#include <u_types/u_vector.h>
#include <stdio.h>

typedef struct 
{
    u_vector stack; //стэк функций
    u_vector stack_data; //стэк данных
}fsm;

typedef void(*fsm_func)(fsm*,void*);

void change_state(fsm* f, fsm_func func, void* data);
void end_state(fsm* f);
int update(fsm* f);

fsm* fsm_new(fsm_func start_state_func);
void fsm_free(fsm* fv);

void fsm_init(fsm* fv,fsm_func start_state_func);
void fsm_deinit(fsm* fv);



#endif // __FSM_H__