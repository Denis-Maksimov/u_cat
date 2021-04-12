#ifndef __U_CSTRING_H__
#define __U_CSTRING_H__

#include <stdio.h>
#include <stdlib.h>
#include <u_stdlib/u_stdlib.h>


typedef struct u_string_t
{
    char* str;
    size_t len; //-- длинна куска
    struct u_string_t* next; //-- следующая подстрока
    struct u_string_t* head; //-- голова (but not using)
}u_string;


//-- длинна части
size_t u_str_len(u_string* Str);

//-- иннициализировать с выделением памяти в str
u_string* u_str_init_a(u_string** str, char* cstring);

//-- дописать в конец
void str_append(u_string* str, char* cstring);

//-- длинна всей строки
size_t string_len(u_string* str);

//-- извлечь си строку с выделением памяти
char* get_cstr_a(u_string* str);

//-- извлечь си строку в буффер
char* get_cstr(u_string* str, char* buff, size_t n);

//-- извлечь си строку в буффер с n-го символа (начиная с 0)
char* get_cstr_at(u_string* str, size_t at, char* buff, size_t n);

//-- дописать текст в at-ю позицию
void str_insert(u_string* str, size_t at, char* cstring);

//-- освободить память
void str_free(u_string* str);

//-- сшить куски вместе
u_string* str_refresh(u_string** _str);

//-- разбить на токкены
size_t str_split(u_string* str, char tokken);

//-- извлечь кусочки Строки в массив строк
size_t str_to_char_array_a(u_string* str, char*** arr);

//-- взять i-ю подстроку
u_string* get_i_substring(u_string* str,size_t i);

#endif // __U_CSTRING_H__