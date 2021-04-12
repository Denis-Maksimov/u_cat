#ifndef U_STRING_H
#define U_STRING_H

#include "u_stdlib.h"



//если не поддерживается деление
// #define NO_DIVISION
STDLIB_API void u_intToStr(unsigned int n, char* s);
// extern void u_intToStr(unsigned int n, char* s);


STDLIB_API void u_intToHex(unsigned int integer,char* baf);
//

// conversion integer --> to string 
// void intToStr(int integer,char* baf); 

/* Return the length of S.  */
STDLIB_API size_t u_strlen (const char* __s);


/* Find the first occurrence of NEEDLE in HAYSTACK.  */
STDLIB_API char* u_strstr (const char* __haystack, const char* __needle);


/* Find the first occurrence of C in S.  */
STDLIB_API char* u_strchr (const char* __s, int __c);

//обратный поиск массива от адреса, длинной len
STDLIB_API char* u_strrevchr(const char* line, int len , const char chr);

STDLIB_API int u_strncmp(const char* str1, const char* str2, size_t len);

/* Find the last occurrence of C in S.  */
STDLIB_API char* u_strrchr(const char* __s, int __c);

STDLIB_API int u_isdigit(char ch);
STDLIB_API int u_isxdigit(char ch);
STDLIB_API int u_isupper(char ch);
STDLIB_API char u_tolower(char ch);
STDLIB_API int u_isspace(char ch);

#endif // !STRING_H
