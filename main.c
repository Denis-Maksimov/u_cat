#include <u_cat.h>
#include <u_types/u_matrix.h>


int main(int argc, char *argv[])
{
    u_matrix* m= u_matrix_new(4,4);
    int a=1;
    u_matrix_insert(m,2,2,a);
    size_t* b=u_matrix_at(m,2,2);
    
    // u_matrix* mm= u_matrix_new_submatrix(m,1,1, 3,4);

    // // u_msg("b=%d",*b);
    // u_msg(".");
    // u_matrix_fprint(stdout, m);
    // u_msg(".");
    // u_matrix_fprint(stdout, mm);

    for (size_t i = 0; i < m->n; i++)
    {
        m->data[i]=i;
    }
    u_matrix_fprint(stdout, m);
    // u_matrix* mm=u_matrix_new_T(m);
    printf("___\n");
    // u_matrix_add_column(&mm);
    // u_matrix_add_line(m);

    u_matrix_push_column(&m);
    // u_matrix_add_line(m);
    
    u_matrix* q=u_matrix_new_get_column( m,2);
    u_matrix_fprint(stdout, q);
    u_matrix_free(m);
    // u_matrix_free(mm);
    // u_matrix_fprint(stdout, mm);
    // u_print_hexdump(mm->data,mm->n*(sizeof(size_t*)));
    return 0;
}









