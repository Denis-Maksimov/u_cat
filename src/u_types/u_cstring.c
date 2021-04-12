#include <stdio.h>
#include <stdlib.h>
#include <u_stdlib/u_stdlib.h>
#include <u_types/u_cstring.h>

// typedef struct u_string_t
// {
//     char* str;
//     size_t len; //-- длинна куска
//     struct u_string_t* next;
//     struct u_string_t* head;
// }u_string;



size_t u_str_len(u_string* Str)
{
    return Str->len;
}

u_string* u_str_init_a(u_string** str, char* cstring)
{
    *str=malloc(sizeof(u_string));
    
    (*str)->next=0;
    (*str)->head=(*str);

    if(cstring){
        (*str)->len=strlen(cstring);
        (*str)->str=malloc((*str)->len+1);
        memcpy((*str)->str,cstring,(*str)->len+1);
    }else{
        (*str)->len=0;
        (*str)->str=0;
    }
    return *str;
}

void str_append(u_string* str, char* cstring)
{
    u_string* _str=str;
    while (_str->next){
        _str=_str->next;
    }
    u_string* estr;
    u_str_init_a(&estr,cstring);
    estr->head=_str->head;
    _str->next= estr;    
}

size_t string_len(u_string* str)
{
    u_string* _str=str;
    size_t len=str->len;
    while (_str=_str->next){
        len+=_str->len;
    }
    return len;
    
}

char* get_cstr_a(u_string* str)
{
    char* rv = malloc(string_len(str)+1);
    char* _rv=rv;
    u_string* _str=str;
    memcpy(_rv,_str->str,_str->len);
    _rv += _str->len;
    while (_str=_str->next){
        memcpy(_rv,_str->str,_str->len);
        _rv += _str->len;
    }
    _rv[0]=0;
    return rv;
}

char* get_cstr(u_string* str, char* buff, size_t n)
{
    if(!(str && buff))return 0;
    // char* rv = malloc(string_len(str)+1);
    char* _rv=buff;
    u_string* _str=str;
    size_t _n;
    
    if(n<_str->len){
        _n=n;
        memcpy(_rv,_str->str,_n);
         _rv += _n;
         _rv[0]=0;
        return buff;
    }
    _n=_str->len;
    memcpy(_rv,_str->str,_n);
    _rv += _n;
    n-=_n;

    while (_str=_str->next){
        if(n<_str->len){
            _n=n;
            memcpy(_rv,_str->str,_n);
            _rv += _n;
            _rv[0]=0;
            return buff;
        }
        _n=_str->len;
        memcpy(_rv,_str->str,_n);
        _rv += _n;
        n-=_n;
    }
    _rv[0]=0;
    return buff;
}

char* get_cstr_at(u_string* str, size_t at, char* buff, size_t n)
{
    if(str->len < at)
    {
        if(!str->next) {return 0;}
        return get_cstr_at(str->next, at-str->len, buff,n);
    }
    // char* rv = malloc(string_len(str)+1);
    char* _rv=buff;
    u_string* _str=str;
    size_t _n;
    
    if(n < _str->len-at){
        _n=n;
        memcpy(_rv,&_str->str[at],_n);
         _rv += _n;
         _rv[0]=0;
        return buff;
    }
    _n=_str->len-at;
    memcpy(_rv, &_str->str[at],_n);
    _rv += _n;
    n-=_n;

    while (_str=_str->next){
        if(n<_str->len){
            _n=n;
            memcpy(_rv,_str->str,_n);
            _rv += _n;
            _rv[0]=0;
            return buff;
        }
        _n=_str->len;
        memcpy(_rv,_str->str,_n);
        _rv += _n;
        n-=_n;
    }
    _rv[0]=0;
    return buff;
}

void str_insert(u_string* str, size_t at, char* cstring)
{
    if(str->len < at)
    {
        if(!str->next) {str_append(str, cstring); return;}
        str_insert(str->next, at-str->len, cstring);
        return;
    }
    u_string* istr;
    u_string* astr;
    u_str_init_a(&istr,cstring);
    u_str_init_a(&astr, &str->str[at]);

    astr->next=str->next;
    str->next=istr;
    istr->next=astr;
    str->len=at;
    str->str=realloc(str->str,at+1);
    str->str[at]=0;

    astr->head=str->head;
    istr->head=str->head;

}

void str_free(u_string* str)
{
    if(str->next)
        str_free(&str->next[0]);
    free(str->str);
    free(str);
}

u_string* str_refresh(u_string** _str)
{
    u_string* str=(*_str);
    char* cs=get_cstr_a(str);
    str_free(str);
    str=u_str_init_a(&str,cs);
    free(cs);
    return str;
}

size_t str_split(u_string* str, char tokken)
{
    size_t rv=1;
    char* cstring=0;
    u_string* astr=0;
    while(cstring = strchr(str->str,tokken)){

        rv++;
        u_str_init_a(&astr, cstring+1);

        astr->next=str->next;
        str->next=astr;
        str->len=cstring-str->str;
        *cstring=0;
        str->str=realloc(str->str,cstring-str->str+1);
        
        astr->head=str->head;
        str=astr;
    }
    return rv;
}

size_t str_to_char_array_a(u_string* str, char*** arr)
{
    // char** arr=(*_arr);
    size_t i=0;

    *arr=malloc(sizeof(char*));
    arr[0][i] = malloc(str->len+1);
    memcpy(arr[0][i],str->str,str->len);
    arr[0][i][str->len]=0;
    i++;
    while (str=str->next)
    {
        arr[0]=realloc(arr[0], sizeof(char*)*(i+1) );
        arr[0][i] = malloc(str->len+1);
        memcpy(arr[0][i],str->str,str->len);
        arr[0][i][str->len]=0;
        i++;
    }

    return i;
}

u_string* get_i_substring(u_string* str,size_t i)
{
    if(!str) return 0;
    for (size_t j = 0; j < i; j++)
    {
        str=str->next;
        if(!str) return 0;
    }
    return str;
}


/*


int main(int argc, char const *argv[])
{
    u_string* str;
    str=u_str_init_a(&str,"Hello,, ");
    str_append(str,"World!");
    str_insert(str,7,"my wonderwall ");

    char buff[256];
    
    // char* cs=get_cstr_a(str);
    get_cstr_at(str, 5, &buff[0], 7);
    // get_cstr(str, &buff[0], 7);
    puts(buff);
    // free(cs);

    str=str_refresh(&str);
    str_split(str, ',');
    
    char** _arr;
    size_t n=str_to_char_array_a(str, &_arr);
    for (size_t i = 0; i < n; i++)
    {
        puts(_arr[i]);
        free(_arr[i]);
    }
    free(_arr);
    
    // puts(str->str);
    str_free(str);

}

*/
