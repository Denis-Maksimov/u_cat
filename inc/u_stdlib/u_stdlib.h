/** **************************************************************
 * @autor: dn.maksimow@gmail.com
 * 
 * При использовании данного кода помните, что тут может 
 * храниться много жучков-паучков.
 * Пожалуйста, не используйте в серьёзных проектах
 * -----------------------------------------------------------
 * @brief
 * Велосипед из стандартных функций.
 * Самые ходовые функции libc
 * 
 * 
 * 
 * 
 *  
 * -----------------------------------------------------------
 * u_mallocat.o:
 *   00000000000000a0 T u_free
 *   0000000000000000 T u_get_heap
 *   0000000000000000 d u_heap
 *   0000000000000010 T u_malloc
 *   0000000000000160 T u_memcpy
 *   0000000000000140 T u_memset
 *   0000000000000190 T u_realloc
 *
 * u_string.o:
 *                   U _GLOBAL_OFFSET_TABLE_
 *                   U u_free
 *   0000000000000410 T u_intToHex
 *   0000000000000490 T u_intToStr
 *                   U u_malloc
 *   0000000000000350 T u_strchr
 *   0000000000000050 T u_strlen
 *   00000000000003b0 T u_strncmp
 *   00000000000003a0 T u_strrchr
 *   0000000000000000 t u_strrchr.part.0
 *   0000000000000300 T u_strrevchr
 *   0000000000000080 T u_strstr
 * ************************************************************** */


#ifndef U_STDLIB_H
#define U_STDLIB_H

#ifndef STDLIB_API
#define STDLIB_API extern
#endif

#include "u_stddef.h"


// #ifndef _STDLIB_H

// #include "u_mallocat.h"
// #include "u_string.h"



// #define free    u_free
// #define malloc  u_malloc
// #define realloc u_realloc
// #define memcpy  u_memcpy
// #define memset  u_memset
// #define strchr  u_strchr
// #define strlen  u_strlen
// #define strncmp u_strncmp
// #define strrchr u_strrchr
// #define strstr  u_strstr

// #define _STDLIB_H
// #else


// #define u_free       free     
// #define u_malloc     malloc  
// #define u_realloc    realloc 
// #define u_memcpy     memcpy  
// #define u_memset     memset  
// #define u_strchr     strchr  
// #define u_strlen     strlen  
// #define u_strncmp    strncmp 
// #define u_strrchr    strrchr 
// #define u_strstr     strstr  

#include <stdlib.h>
#include <string.h>
C_START
void u_free(void *__ptr);    
void *u_malloc(size_t __size); 
void *u_realloc(void *__ptr, size_t __size); 
void *u_memcpy(void * __dest, const void * __src, size_t __n);  
void *u_memset(void *__s, int __c, size_t __n); 
char *u_strchr(const char *__s, int __c); 
size_t u_strlen(const char *__s);
int u_strncmp(const char *__s1, const char *__s2, size_t __n);
char *u_strrchr(const char *__s, int __c);
char *u_strstr(const char *__haystack, const char *__needle); 

typedef void(*ufree_func)(void*);
// #endif
C_END
#endif // !U_STDLIB_H
