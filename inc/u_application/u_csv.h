#ifndef __CSV_H__
#define __CSV_H__

#include <stdlib.h>
#include <u_stdlib/u_stdlib.h>
#include <u_types/u_vector.h>

typedef struct 
{
    size_t rows;
    size_t cols;
    u_vector line;
}csv_grid;

int read_csv(csv_grid* csv, char* name);
char* csv_get_ceil(csv_grid* csv,size_t line, size_t column, char *buff, size_t n);
void csv_print_ceil(csv_grid* csv,size_t line, size_t column);
void free_csv(csv_grid* csv);

#endif // __CSV_H__
