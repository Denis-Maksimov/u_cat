#include <u_math/u_fmatrix.h>







u_fmatrix* u_fmatrix_new(size_t i,size_t j)
{
    u_fmatrix* rv=malloc(sizeof(u_fmatrix));
    rv->height=i;
    rv->lenght=j;
    rv->n=i*j;
    rv->data=calloc(rv->n,sizeof(float));
    memset(rv->data,0.,sizeof(float)*rv->n);
    return rv;
}

void u_fmatrix_free(u_fmatrix* m)
{
    free(m->data);
    free(m);
}


float u_fmatrix_at(u_fmatrix* m, size_t i,size_t j)
{
   return m->data[i*(m->lenght)+j];    
}


void u_fmatrix_insert(u_fmatrix* m, size_t i,size_t j, float el)
{
    
   m->data[i*m->lenght+j]=el;    
}

void u_fmatrix_push_line(u_fmatrix* m)
{
    m->height+=1;
    m->n += m->lenght ;
    
   m->data=realloc(m->data, (m->n)*sizeof(float));
   
   for (size_t i = 0; i < m->lenght; i++)
   {
       u_fmatrix_insert(m, m->height-1, i, 0.);
   }
   
}

void u_fmatrix_push_column(u_fmatrix** m)
{
    u_fmatrix* t= u_fmatrix_new_T(*m);
    free(*m);
    u_fmatrix_push_line(t);
    *m = u_fmatrix_new_T(t);

    free(t);
}



u_fmatrix* 
u_fmatrix_new_submatrix(u_fmatrix* m,size_t i1,size_t j1, size_t i2,size_t j2)
{  
    u_fmatrix* rv = u_fmatrix_new(i2-i1, j2-j1);
    // size_t n=0;
    for (size_t i = i1; i < i2; i++)
    {
        for (size_t j = j1; j < j2; j++)
        {
            // rv->data[n]=u_matrix_at(m,i,j);
            u_fmatrix_insert(rv,i-i1,j-j1,u_fmatrix_at(m,i,j));
            // n++;
        }
        
    }
    return rv;

}

u_fmatrix* 
u_fmatrix_new_get_line(u_fmatrix* m,size_t line)
{  
    return u_fmatrix_new_submatrix(m, line, 0, line+1, m->lenght);
}

u_fmatrix* 
u_fmatrix_new_get_column(u_fmatrix* m,size_t column)
{
    return u_fmatrix_new_submatrix(m, 0, column, m->height, column+1 );
}

u_fmatrix* 
u_fmatrix_new_T(u_fmatrix* m)
{
    u_fmatrix* rv=u_fmatrix_new(m->lenght,m->height);

   for (size_t i = 0; i < m->height; i++)
    {
        for (size_t j = 0; j < m->lenght; j++)
        {
            u_fmatrix_insert(rv,j,i,u_fmatrix_at(m,i,j));
        }
        
    }
    return rv;
    
}
 
void 
u_fmatrix_fprint(FILE* stream, u_fmatrix* m)
{

   for (size_t i = 0; i < m->height; i++)
    {
        for (size_t j = 0; j < m->lenght; j++)
        {
            fprintf(stream,"%*f ", 12, (float)u_fmatrix_at(m,i,j) );
        }
        fprintf(stream,"\n");
    }
    
}


static float 
_mull_el(u_fmatrix* A, u_fmatrix* B, size_t i,size_t j)
{
    float a=0.;
    for (size_t k = 0; k < A->columns; k++)
    {
        a+=u_fmatrix_at(A,i,k)*u_fmatrix_at(B,k,j);
    }
    return a;
    
}

u_fmatrix* 
u_fmatrix_mull(u_fmatrix* A,u_fmatrix* B)
{
    // printf("%d=%d",A->columns,B->rows);
    if(A->columns!=B->rows) return 0;

    u_fmatrix* C = u_fmatrix_new(A->height,B->lenght);

    for (size_t i = 0; i < A->rows; i++)
    {
        for (size_t j = 0; j < B->columns; j++)
        {

            u_fmatrix_insert(C,i,j, _mull_el(A, B,  i, j));
        }
        
    }
    return C;
    

}

void 
u_fmatrix_swap_columns(u_fmatrix* m,size_t col1,size_t col2)
{
    float tmp,tmp1;
    for (size_t i = 0; i < m->height; i++)
    {
        tmp=u_fmatrix_at(m,i,col1);
        u_fmatrix_insert(m,i,col1,u_fmatrix_at(m,i,col2));
        u_fmatrix_insert(m,i,col2,tmp);
    }
}

void 
u_fmatrix_swap_rows(u_fmatrix* m,size_t row1,size_t row2)
{
    float tmp,tmp1;
    for (size_t i = 0; i < m->lenght; i++)
    {
        tmp=u_fmatrix_at(m,row1,i);
        u_fmatrix_insert(m,row1,i,u_fmatrix_at(m,row2,i));
        u_fmatrix_insert(m,row2,i,tmp);
    }
}



static int 
search (u_fmatrix* a, double what, 
 int match, size_t *uI, size_t *uJ, size_t starti, size_t startj) {
 // Поиск в матрице a[m][n] элемента с указанным значением what
 // Возвращаеются его номер строки и столбца uI, uJ, если элемент найден.
 // match - искать равный элемент или отличный от указанного.
 // Вернёт 0 - не найдено, не 0 - найдено
 if ((!a->height) || (!a->lenght)) return 0;
 if ((starti >= a->height) || (startj >= a->lenght)) return 0;
 for (size_t i = starti; i < a->height; i++)
 for (size_t j = startj; j < a->lenght; j++) {
  if (match == 1) {
   if (u_fmatrix_at(a,i,i)== what) {
    *uI = i; *uJ = j; return 1;
   }
  }
  else if (u_fmatrix_at(a,i,j) != what) {
   *uI = i; *uJ = j; return 1;
  }
 }
 return 0;
}

float 
u_fmatrix_det(u_fmatrix* a)
{
    
 //Вычисление определителя квадратной матрицы a[n][n]
 size_t m = a->lenght;
 if (m == 0) return 0;
 if (m == 1) return a->data[0];
 if (m == 2) return (a->data[0] * u_fmatrix_at(a,1,1) - u_fmatrix_at(a,1,0)  * u_fmatrix_at(a,0,1));

 int sign = 0; // смена знака определителя. по умолчанию - нет
 float det = 1.; // определитель
 float tmp;
 size_t x, y;

 for (size_t i = 0; i < a->lenght; i++) 
 { // цикл по всей главной диагонали
  if (u_fmatrix_at(a,i,i) == 0.) { // если элемент на диагонали равен 0, то ищем ненулевой элемент в матрице
   if (!search(a, 0., 0, &y, &x, i, i)) return 0.; // если все элементы нулевые, то опр. = 0
   if (i != y) { // меняем i-ую строку с y-ой
    u_fmatrix_swap_rows(a, i, y);
    sign = !sign;
   }
   if (i != x) { // меняем i-ый столбец с x-ым
    u_fmatrix_swap_columns(a,i, x);
    sign = !sign;
   }
   // таким образом, в a[i][i], теперь ненулевой элемент.
  }

  // выносим элемент a[i][i] за определитель
  det *= u_fmatrix_at(a,i,i);
  tmp = u_fmatrix_at(a,i,i);
  for (x = i; x < m; x++) 
  {
      u_fmatrix_insert(a,i,x,u_fmatrix_at(a,i,x)/tmp);
  }
  // таким образом a[i][i] теперь равен 1
  // зануляем все элементы стоящие под (i, i)-ым,
  // при помощи вычитания с опр. коеффициентом
    for (y = i + 1; y < a->lenght; y++) 
    {
        tmp = u_fmatrix_at(a,y,i);
        for (x = i; x < a->lenght; x++)
            u_fmatrix_insert(a,y,x,u_fmatrix_at(a,y,x)-u_fmatrix_at(a,i,x)*tmp);
        // a[y][x] -= (a[i][x]*tmp);
    }
  }
  if (sign) return det*(-1);
  return det;
};


void 
u_fmatrix_copy(u_fmatrix* dest,u_fmatrix* src,size_t i_from,size_t j_from)
{
    for (size_t i = 0; i < src->rows; i++)
    {
        for (size_t j = 0; j < src->columns; j++)
        {
            u_fmatrix_insert(dest,i+i_from,j+j_from,
                u_fmatrix_at(src,i,j)
            );
        }
        
    }
    
}

u_fmatrix* 
u_fmatrix_solve_equation(u_fmatrix* A,u_fmatrix* V)
{
    u_fmatrix* rv=u_fmatrix_new(V->height,1);
    u_fmatrix* tmp=u_fmatrix_new(A->height,A->lenght+1);
    u_fmatrix_copy(tmp,A,0,0);
    u_fmatrix_copy(tmp,V,0,A->lenght);

    u_fmatrix* sub=u_fmatrix_new_submatrix(tmp,0,0,A->rows,A->columns);

    //главный определитель
    float det=u_fmatrix_det(sub);
    u_fmatrix_free(sub);

    for (size_t i = 0; i < A->lenght; i++)
    {
        u_fmatrix_swap_columns(tmp,i,tmp->lenght-1);
            sub=u_fmatrix_new_submatrix(tmp,0,0,A->rows,A->columns);
            u_fmatrix_insert(rv,i,0, u_fmatrix_det(sub)/det);
            u_fmatrix_free(sub);
        u_fmatrix_swap_columns(tmp,i,tmp->lenght-1);
    }
    u_fmatrix_free(tmp);
    return rv;
}

#include <math.h>


void fmatrix_test()
{
        u_fmatrix* m= u_fmatrix_new(4,4);
    float a=1.;
    u_fmatrix_insert(m,2,2, 1.);
    float b=u_fmatrix_at(m,2,2);
    
    // u_matrix* mm= u_matrix_new_submatrix(m,1,1, 3,4);

    // // u_msg("b=%d",*b);
    // u_msg(".");
    // u_matrix_fprint(stdout, m);
    // u_msg(".");
    // u_matrix_fprint(stdout, mm);

    for (size_t i = 0; i < m->n; i++)
    {
        m->data[i]=(float)(rand()%15);
    }
    u_fmatrix_fprint(stdout, m);
    // u_matrix* mm=u_matrix_new_T(m);
    printf("___\n");
    // u_matrix_add_column(&mm);
    // u_matrix_add_line(m);

    // u_fmatrix_push_column(&m);
    // u_fmatrix_push_line(m);
    //  u_fmatrix_swap_rows(m,1,3);
    u_fmatrix_fprint(stdout, m);
    printf("___\n");
    u_fmatrix* q=u_fmatrix_new_get_column( m,3);
    u_fmatrix_fprint(stdout, q);
    u_fmatrix* T=u_fmatrix_new_T( m);
    // u_fmatrix* C=u_fmatrix_mull(m,T);
    printf("___solve__\n");






    u_fmatrix *eq=u_fmatrix_solve_equation(m,q);
    // float d=u_fmatrix_det(m);
    // printf("%f\n",d);
    u_fmatrix_fprint(stdout, eq);

    // if(C) {
    //     u_fmatrix_fprint(stdout, C);
    //     u_fmatrix_free(C);
    // }
    u_fmatrix_free(eq);
    u_fmatrix_free(m);
    u_fmatrix_free(q);
    u_fmatrix_free(T);
    // u_matrix_free(mm);
    // u_matrix_fprint(stdout, mm);
    // u_print_hexdump(mm->data,mm->n*(sizeof(size_t*)));
}


float 
u_fmatrix_modul_of_vector(u_fmatrix* V)
{
    float rv=0.;
    if(V->lenght==1)
    {
        for (size_t i = 0; i < V->height; i++)
        {
            rv+=(u_fmatrix_at(V,i,0)*u_fmatrix_at(V,i,0));
        }
        return sqrtf(rv);
    }
    else if(V->height==1)
    {
        for (size_t i = 0; i < V->lenght; i++)
        {
            rv+=(u_fmatrix_at(V,0,i)*u_fmatrix_at(V,0,i));
        } 
        return sqrtf(rv);
    }
    else return NAN;
}


u_fmatrix* 
u_fmatrix_tensor_mull_vectors(u_fmatrix* V1,u_fmatrix* V2)
{
    u_fmatrix* tmp=u_fmatrix_new_T(V2);
    u_fmatrix* rv = u_fmatrix_mull(V1, tmp);
    u_fmatrix_free(tmp);
    return rv;
}


void fmatrix_vector_test()
{
    u_fmatrix* V1 = u_fmatrix_new(9,1);
    for (size_t i = 0; i < 9; i++)
    {
        u_fmatrix_insert(V1,i,0,(float)i+1);
    }
    
    u_fmatrix* table =u_fmatrix_tensor_mull_vectors(V1, V1);
    u_fmatrix_fprint(stdout,table);
    printf("modul=%f\n",u_fmatrix_modul_of_vector(V1));
    u_fmatrix_free(V1);
    u_fmatrix_free(table);
}