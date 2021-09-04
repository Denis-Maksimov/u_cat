#include <u_application/u_strtokens.h>

u_strtokens* str_new(){
    u_strtokens* rv=malloc(sizeof(u_strtokens));
    rv->str=0;
    rv->len=0;
    return rv;
}

void ustr_append(u_strtokens* s, const char* ap, size_t len)
{
    if(!s)return;
    size_t lens=s->len+len;
    if(!s->str)
    {
        s->str=malloc(len*sizeof(char));
    }else
    {
        if(ap[len]){lens+=1;}
        s->str=realloc(s->str,(s->len+len)*sizeof(char));
    }
    size_t j=0;
    for (size_t i = s->len; i < (s->len+len); i++)
    {
        s->str[i]=ap[j++];
    }
    s->str[lens]=0;
    s->len=s->len+len;
}

int str_get_i_delim(u_strtokens* s,char* delim,size_t n)
{
    if(!delim)return -3;
    if(!s)return -2;
    if(!delim[n]){
        n--;
    }

    for (int i = 0; i < s->len; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            if(s->str[i]==delim[j])
            {
                return i;
            }
            
        }
    }
    return -1;
}
//[фыва]
//[0123]
//
u_strtokens* get_substr_delim(u_strtokens* s,const char* delim,size_t n)
{
    
    int i=str_get_i_delim(s, delim, n);
    if (i<0)return 0;
    u_strtokens* rv = str_new();
    ustr_append(rv,s->str,(size_t)i);
    return rv;
}

