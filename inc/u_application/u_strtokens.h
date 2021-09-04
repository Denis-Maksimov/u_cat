#ifndef U_STRTOKENS_H
#define U_STRTOKENS_H

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct u_strtokens
{
    char* str;
    size_t len;
}u_strtokens;

u_strtokens* str_new();
void ustr_append(u_strtokens* s, const char* ap, size_t len);
int str_get_i_delim(u_strtokens* s,char* delim,size_t n);
u_strtokens* get_substr_delim(u_strtokens* s,const char* delim,size_t n);


#ifdef __cplusplus
};
#endif
#endif

