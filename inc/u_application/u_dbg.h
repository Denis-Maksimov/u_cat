
#ifndef U_DBG_H
#define U_DBG_H

#include <stdio.h>
#include <time.h>

#define LOG_NAME "./logfile.txt"
#define U_LOG(format, args...) u_log (LOG_NAME, format , ## args)


/*********************************************
@brief: вывод и запись логов и дампов
@note: логи выводятся в следующем формате:

13.05.2021 Четверг 02:16:25 :
Hello, world!
............................. 
13.05.2021 Четверг 02:16:25 :
Bye, world!
............................. 
 

 *********************************************/


//запись сообщения в лог файл
void u_log(const char*  file_name,const char* msg, ...) __attribute__ ((format (printf, 2, 3)));

//вывод сообщения на экран
void u_msg(const char* msg, ...) __attribute__ ((format (printf, 1, 2)));

//вывод на экран 16-ричный дамп
void u_print_hexdump(void *mem, unsigned int len);

//запись в файл дамп памяти
void u_hexdump_file(const char*  file_name,void *mem, unsigned int len);

#endif
