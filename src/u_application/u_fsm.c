#include <u_application/u_fsm.h>

void 
change_state(fsm* f, fsm_func func, void* data)
{
    u_vector_push_back(&(f->stack), func);
    u_vector_push_back(&(f->stack_data), data);
}

void 
end_state(fsm* f)
{
    u_vector_pop_back(&(f->stack));
    u_vector_pop_back(&(f->stack_data));
}

int 
update(fsm* f)
{
    fsm_func ff=u_vector_at(&(f->stack),f->stack.n_elem-1);
    void* data=u_vector_at(&(f->stack_data),f->stack.n_elem-1);
    if(ff) {ff(f,data);return 1;}
    return 0;
}


fsm* fsm_new(fsm_func start_state_func)
{
    fsm* fv= malloc(sizeof(fsm));
    fsm_init(fv, start_state_func);
    return fv;
}

void fsm_init(fsm* fv,fsm_func start_state_func)
{
    u_vector_init(&(fv->stack));
    u_vector_init(&(fv->stack_data));
    u_vector_push_back(&(fv->stack), start_state_func);
    u_vector_push_back(&(fv->stack_data), NULL);
}

void fsm_deinit(fsm* fv)
{
    u_vector_deinit(&(fv->stack),0);
    u_vector_deinit(&(fv->stack_data),0);
}

void fsm_free(fsm* fv)
{
    fsm_deinit(fv);
    free(fv);
}
