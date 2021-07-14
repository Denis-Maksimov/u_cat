#include <stdio.h>
#include <u_application/u_csv_v2.h>
#include <u_types/u_cstring.h>




//-- ASCII строку в вектор
static void 
_line_to_vec(csv_grid* csv, char* line,size_t l)
{
   size_t col=0;
   // size_t len=strlen(line);
   
   //-- (1) инициализация Строки
   u_string* str;
   str=u_str_init_a(&str, line);

   // Набор символов, которые должны входить в искомый сегмент
   char sep [10]=",;\n";
   size_t n_sep=strlen(sep);

   // число ячеек в строке
   size_t n_elem=0;

   //-- (2) разбиваем строку по токкенам-разделителям
   for (size_t i = 0; i < n_sep; i++)
   {
      n_elem+=str_split(str, sep[i]);
   }
   

   //-- (3) регистрируем максимальное число столбцов в csv-таблице
   if( n_elem > csv->cols)
   {
       csv->cols = n_elem;
   }

   //-- (4) пушим
   u_vector_push_back(&csv->line, str);


}


int read_csv(csv_grid* csv, char* name)
{
   u_vector_init(&csv->line);
   csv->cols=0;
   csv->rows=0;
    size_t i=0;
   // Переменная, в которую поочередно будут помещаться считываемые строки
   char str[1024];
   
   //Указатель, в который будет помещен адрес массива, в который считана 
   // строка, или NULL если достигнут коней файла или произошла ошибка
   char *estr;

   FILE *mf = fopen (name,"r");
   if (!mf) {fprintf (stderr,"ошибка\n"); return -1;}

   //Чтение (построчно) данных из файла в бесконечном цикле
   while (1)
   {
      // Чтение одной строки  из файла
      estr = fgets (str,sizeof(str),mf);

      //Проверка на конец файла или ошибку чтения
      if (estr == NULL)
      {
         if ( feof (mf) != 0)
         {  
            printf ("\nЧтение файла закончено\n");
            break;
         }
         else
         {
            fprintf (stderr,"Ошибка чтения из файла\n");
            break;
         }
      }
      //Если файл не закончился, и не было ошибки чтения 
      //выводим считанную строку  на экран
      
      printf ("     %s",str);
      _line_to_vec(csv, str, i);
      i++;
   }
    csv->rows=i;

   // Закрываем файл
   printf ("%d)Закрытие файла: ",i);
   if ( fclose (mf) == EOF) {
      printf ("ошибка\n");
      return -2;
   }
   else printf ("выполнено\n");
   return 0;


}

static char  _void_string[]="__NULL__";

char* 
csv_get_ceil(csv_grid* csv,size_t line, size_t column, char* buff, size_t n)
{
   // if (line > csv->rows) return &_void_string[0];
   // if (column > csv->line[line]->n_elem) return &_void_string[0];
   u_string* str=u_vector_at(&csv->line, line);
   if(str=get_i_substring(str,column))
      return get_cstr(str, buff, (n<str->len? n : str->len) );
   return 0;
}

void 
csv_get_line_vector(csv_grid* csv,size_t line, u_vector* buff)
{
   u_string* str=u_vector_at(&csv->line, line);
   for (size_t i = 0; i < csv->rows; i++)
   {
      if(str=get_i_substring(str,i))
         u_vector_push_back(buff,get_cstr_a(str));
   }

}

void
csv_get_line_vector_float(csv_grid* csv,size_t line, u_vector* vec)
{
   double* ptr;
   csv_get_line_vector(csv,line, vec);
   for (size_t i = 0; i < vec->n_elem; i++)
   {
      char* c= u_vector_at(vec, i);
      ptr=malloc(sizeof(double));
      *ptr=atof(c);
      free(vec->mem[i]);
      vec->mem[i]=ptr;
   }
   
}


void csv_print_ceil(csv_grid* csv,size_t line, size_t column)
{
   char buff[15];
   printf("%d:%d [%s] ",line,column, csv_get_ceil(csv,line,column,buff,sizeof(buff)) );
}

void csv_print(csv_grid* csv)
{

   char buff[256];
   size_t j=0;
   printf("------------------------------\n");
   for (size_t i = 0; i < csv->rows; i++)
   {
     for (size_t j = 0; j < csv->cols; j++)
     {
       csv_print_ceil(csv,i,j);
     }
      printf("\n------------------------------\n");
     
      // j= str_to_char_array_a(csv->line[i], char*** arr);
      
   }
   
}

void free_csv(csv_grid* csv)
{
   // for (size_t i = 0; i < csv->rows; i++)
   // {
   //    u_vector_deinit(csv->line[i], free);
   //    free(csv->line[i]);
   // }
   // free(csv->line);
   u_vector_deinit(&csv->line, (ufree_func)str_free);
   
}

// int main(int argc, char const *argv[])
// {
//     csv_grid csv;
//     read_csv(&csv,"./tst.csv");
//    //  csv_print_ceil(&csv, 2, 5);
//     csv_print(&csv);
//     free_csv(&csv);

// }






