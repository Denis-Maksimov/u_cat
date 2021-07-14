#include <u_types/u_matrix.h>






u_matrix* u_matrix_new(size_t i,size_t j)
{
    u_matrix* rv=malloc(sizeof(u_matrix));
    rv->height=i;
    rv->lenght=j;
    rv->n=i*j;
    rv->data=calloc(rv->n,sizeof(void*));
    memset(rv->data,0,sizeof(void*)*rv->n);
    return rv;
}

void u_matrix_free(u_matrix* m)
{
    free(m->data);
    free(m);
}


size_t* u_matrix_at(u_matrix* m, size_t i,size_t j)
{
   return m->data[i*(m->lenght)+j];    
}


void u_matrix_insert(u_matrix* m, size_t i,size_t j, void* el)
{
    
   m->data[i*m->lenght+j]=el;    
}
void u_matrix_push_line(u_matrix* m)
{
    m->height+=1;
    m->n += m->lenght ;
    
   m->data=realloc(m->data, (m->n)*sizeof(size_t*));
   
   for (size_t i = 0; i < m->lenght; i++)
   {
       u_matrix_insert(m, m->height-1, i, 0);
   }
   
//    memset(&m->data[(m->lenght)*(m->height)],  0,  (m->lenght)*sizeof(size_t*));

}

void u_matrix_push_column(u_matrix** m)
{
    u_matrix* t= u_matrix_new_T(*m);
    free(*m);
    u_matrix_push_line(t);
    *m = u_matrix_new_T(t);

    free(t);
}



u_matrix* u_matrix_new_submatrix(u_matrix* m,size_t i1,size_t j1, size_t i2,size_t j2)
{  
    u_matrix* rv = u_matrix_new(i2-i1, j2-j1);
    // size_t n=0;
    for (size_t i = i1; i < i2; i++)
    {
        for (size_t j = j1; j < j2; j++)
        {
            // rv->data[n]=u_matrix_at(m,i,j);
            u_matrix_insert(rv,i-i1,j-j1,u_matrix_at(m,i,j));
            // n++;
        }
        
    }
    return rv;

}

u_matrix* u_matrix_new_get_line(u_matrix* m,size_t line)
{  
    return u_matrix_new_submatrix(m, line, 0, line+1, m->lenght);
}

u_matrix* u_matrix_new_get_column(u_matrix* m,size_t column)
{
    return u_matrix_new_submatrix(m, 0, column, m->height, column+1 );
}
u_matrix* u_matrix_new_T(u_matrix* m)
{
    u_matrix* rv=u_matrix_new(m->lenght,m->height);

   for (size_t i = 0; i < m->height; i++)
    {
        for (size_t j = 0; j < m->lenght; j++)
        {
            u_matrix_insert(rv,j,i,u_matrix_at(m,i,j));
        }
        
    }
    return rv;
    
}
 
void u_matrix_fprint(FILE* stream, u_matrix* m)
{

   for (size_t i = 0; i < m->height; i++)
    {
        for (size_t j = 0; j < m->lenght; j++)
        {
            fprintf(stream,"%d\t",(size_t)u_matrix_at(m,i,j));
        }
        fprintf(stream,"\n");
    }
    
}



