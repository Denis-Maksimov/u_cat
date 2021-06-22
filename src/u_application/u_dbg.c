
#ifndef HEXDUMP_COLS
#define HEXDUMP_COLS 16
#endif
#include <u_application/u_dbg.h>
#include <ctype.h>
static void 
_hexdump(FILE* stream, void *mem, unsigned int len);
#include <stdarg.h>

static void 
_u_msg(FILE* stream,const char* msg, va_list args);


void u_log(const char*  file_name,const char* msg, ...)
{
        
    FILE * f=fopen(file_name,"a");
    va_list args;
    va_start(args, msg);
    _u_msg(f, msg, args);
    va_end(args);
    fclose(f);
}

void u_msg(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    _u_msg(stdout, msg, args);
    va_end(args);    
}

static void 
_u_msg(FILE* stream,const char* msg, va_list args)
{
 
    struct tm * timeinfo;
    char buffer [128];                                // строка, в которой будет храниться текущее время
    time_t rawtime=time ( 0 );                               // текущая дата в секундах
    timeinfo = localtime ( &rawtime );               // текущее локальное время, представленное в структуре
    strftime (buffer,128,"%x %A %X ",timeinfo);
    fprintf(stream,"%s:\n",buffer);
    vfprintf(stream, msg,  args);
    fprintf(stream,"\n.............................\n");
    fflush(stream);
}


void u_print_hexdump(void *mem, unsigned int len)
{
        _hexdump(stdout, mem, len);
}

void u_hexdump_file(const char*  file_name,void *mem, unsigned int len)
{
    FILE * f=fopen(file_name,"a");
    fprintf(f,"==IT IS A HEXDUMP OF VALUE AT ADDRESS 0x%X==\n",mem);
    _hexdump(f, mem, len);
    fclose(f);
}

//------------------------------------------------------------------------------
// hexdump, a very nice function, it's not mine.
// I found it on the net somewhere some time ago... thanks to the author ;-)
//------------------------------------------------------------------------------

void _hexdump(FILE* stream, void *mem, unsigned int len)
{
        unsigned int i, j;
        
        for(i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
        {
                /* print offset */
                if(i % HEXDUMP_COLS == 0)
                {
                        fprintf(stream,"0x%04x: ", i);
                }

                /* print hex data */
                if(i < len)
                {
                        fprintf(stream,"%02x ", 0xFF & ((char*)mem)[i]);
                }
                else /* end of block, just aligning for ASCII dump */
                {
                        fprintf(stream,"   ");
                }

                /* print ASCII dump */
                if(i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
                {
                        for(j = i - (HEXDUMP_COLS - 1); j <= i; j++)
                        {
                                if(j >= len) /* end of block, not really printing */
                                {
                                        fprintf(stream," ");
                                }
                                else if(isprint((((char*)mem)[j] & 0x7F))) /* printable char */
                                {
                                        fprintf(stream,"%c",(0xFF & ((char*)mem)[j]));
                                }
                                else /* other char */
                                {
                                        fprintf(stream,".");
                                }
                        }
                        fprintf(stream,"\n");
                }
        }
}

