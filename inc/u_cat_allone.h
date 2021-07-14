#ifndef __U_CAT_H__
#define __U_CAT_H__
/**
 * @brief Сборка всех модулей моих велосипедов
 * @note библиотека монолитна, некоторые модули зависят друг от друга
 * пытался сначала сделать независимым от стандартной библиотеки, но
 * со временем бросил это неблагодарное дело
 */



// #include "u_algorithm/u_sort.h"
#ifndef __U_SORT_H__
#define __U_SORT_H__
#include <stddef.h>


 void insert_sort_const(int* dest, const int* src, int n);
 void insert_sort(int* src, int n);
 int find_min(int* src, int n);
 void choise_sort(int* src, int n);
 void bubble_sort(int* src, int n);
 void quick_sort(int *src, int n);
 void Gnome_sort( int *mas,int n);
 void bubble_sortf(float* src, size_t n);
 void quickSort(int *numbers, int left, int right);

#endif // __U_SORT_H__

//---------------------------
//-- модули для приложения --
//---------------------------


// #include "u_application/s5_timers.h"
#ifndef __S5_TIMERS_H__
#define __S5_TIMERS_H__

#include <u_stdlib/u_stddef.h>
// #ifndef U_STDDEF_H
// #include <stdint.h>
// typedef uint32_t u32;
// typedef uint16_t u16;
// typedef uint8_t u8;

// typedef int32_t s32;
// typedef int16_t s16;
// typedef int8_t s8;
// #endif
#ifdef __MY_BARE_ARM__
extern uint32_t SysTick_VAL;
typedef u32 u_clock_t;
#else
#include <time.h>
// uint32_t SysTick_VAL;
typedef clock_t u_clock_t;
#endif // !1




typedef struct
{
    u_clock_t stv;
    // u32 value;
    u8 state:1;
    u8 state_pext:1;
}u_timer;

/*************************************************
* Формирователь импульса
* 
* ___|▔▔▔▔▔|____________      set 1
*
* ___|▔▔▔▔▔|____________      return 1
* 
* ___|▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔|__      set 2
*
* ___|▔▔▔▔▔▔▔▔▔▔▔|______      return 2
*
* ___|▔▔▔▔▔▔▔▔▔▔▔|______      tv
*
**************************************************/
u32 s_pulse(u_timer* Tx, u32 tv, u16 set, u16 rst);




/*************************************************
* Формирователь продлённого импульса
* 
* ___|▔▔▔▔▔|____________      set
*
* ___|▔▔▔▔▔▔▔▔▔▔▔|______      return
*
* ___|▔▔▔▔▔▔▔▔▔▔▔|______      tv
*
**************************************************/
u32 s_pext(u_timer* Tx, u32 tv, u16 set, u16 rst);


/*************************************************
* Формирователь задержки включения
*  
* ___|▔▔▔▔▔▔▔|__________      set
*
* ______|▔▔▔▔|__________      return 
*
* ___|▔▔|_______________      tv
*
**************************************************/
u32 s_odt(u_timer* Tx, u32 tv, u16 set, u16 rst);


/*************************************************
* Формирователь задержки включения с запоминанием
*  
* ___|▔▔▔|____________      set
*
* _____________|▔▔▔▔▔▔      return
*
* ___|▔▔▔▔▔▔▔▔|_______      tv
*
**************************************************/
u32 s_odts(u_timer* Tx, u32 tv, u16 set, u16 rst);


/*************************************************
*  Формирователь задержки выключения
*  
* ___|▔▔▔▔▔|_____________   set
*
* 
* ___|▔▔▔▔▔▔▔▔▔▔▔▔|______   return
*
* _________|▔▔▔▔▔▔|______   tv
*
**************************************************/
u32 s_offdt(u_timer* Tx, u32 tv, u16 set, u16 rst);
//------------------------------------------------------------


int test_processing();



#endif // __S5_TIMERS_H__

// #include "u_application/u_dbg.h"

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


// #include "u_application/u_pid.h"
#ifndef __U_PID_H__
#define __U_PID_H__

// #include <u_application/s5_timers.h>

typedef struct 
{
    float value;        //--задание на выход
                        
    float Kp;           //-- пропорциональная составляющая
    float Ki;           //-- интегральная составляющая
    float Kd;           //-- дифференциальная составляющая
                        
    float prev_delta;    //-- предыдущая разность между уставкой и значением
    float integral;     //-- интеграл расхождений 

    u_timer timer;	//-- таймер 
    u_clock_t period;   //-- период дискретизации
    u16 set;		//-- *private*

}u_pid;

/**
 * @brief  инициализация ПИД
 * @note   
 * @param  Pid: хэндл ПИД
 * @param  Kp: пропорциональная составляющая 
 * @param  Ki: интегральная составляющая
 * @param  Kd: дифференциальная составляющая
 * @param  period: период дискретизации
 * @retval None
 */
void init_pid(u_pid* Pid,float Kp,float Ki,float Kd,u_clock_t period);


/**
 * @brief  создаёт ПИД с аллокацией памяти
 * @note   
 * @param  Kp: пропорциональная составляющая 
 * @param  Ki: интегральная составляющая
 * @param  Kd: дифференциальная составляющая
 * @param  period: период дискретизации
 * @retval аллоцированный указатель на ПИД
 */
u_pid*
new_pid(float Kp,float Ki,float Kd, u_clock_t period);

/**
 * @brief  цикл работы ПИД-регулятора
 * @note   
 * @param  Pid: ПИД-регулятор
 * @param  x: текущее значение
 * @param  setting: уставка
 * @retval Y
 */
float proc_pid(u_pid* Pid,float x,float setting);

/**
 * @brief реаллоцирует ПИД 
 * @note   
 * @param  Pid: ПИД-регулятор
 * @retval none
 */
void free_pid(u_pid* Pid);

// s32 clamp(s32 x, s32 low, s32 high);
// float fclamp(float x, float low, float high);


#endif // __U_PID_H__


//---------------------------
//-- математические модули --
//---------------------------
// #include "u_math/u_math.h"
#ifndef __U_MATH_H__
#define __U_MATH_H__

typedef  unsigned int u32;
typedef  int s32;

extern u32 u_sqrt(u32 A);

#endif // __U_MATH_H__
//--------------------------------
//-- пока нахрен этот велосипед --
//--------------------------------
// #include "u_sellect/u_sellect.h"

//--------------------------

//-----------------------------------
//-- модули стандартной библиотеки --
//-----------------------------------
// #include "u_stdlib/u_mallocat.h"




#ifndef U_STDDEF_H
#define U_STDDEF_H

#include <stddef.h>
#include <stdint.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;


#endif // !U_STDDEF_H



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

// #include "u_stddef.h"


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

#endif // !U_STDLIB_H


// #include "u_stdlib/u_string.h"
#ifndef U_STRING_H
#define U_STRING_H

// #include "u_stdlib.h"



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


//--------------------------

//-------------------------
//-- модули типов данных --
//-------------------------
// #include "u_types/u_hash.h"
#ifndef __U_HASH_H__
#define __U_HASH_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef c_new
#define c_new(t)     ((t*)malloc(sizeof(t)))
#endif

// Структура - хэш таблица 
typedef struct hnode_tag
{
  int key;           // уникальный ключ
  void* data;             //данные
  int flags;
} hnode;

typedef struct 
{
  hnode* nodes;
  int n_elem;
  size_t size;
} hash_table;


void hashprint(hash_table*); // Вывод узлов
size_t pearson_hash(char* key,size_t clamp);//хэш функция Пирсона
hash_table* addnode( hash_table *tree, int id, void* data);// Добавление узлов
void freemem(hash_table *tree, void(*data_free_f)(void*));//Освобождение памяти таблицы
hnode* get_elem_by_key(hash_table*,int key);//--взять ноду по id
void* u_get_data_by_key(hash_table* ,int key);//данные по id



//===================================================================================


// Структура - хэш таблица 
typedef struct h_node_tag
{
  char* key;           // уникальный ключ
  void* data;             //данные
  int flags;
} hnode_v2;

typedef struct 
{
  hnode_v2* nodes;
  size_t n_elem;
  size_t size;
} hash_table_v2;


void hashprint_v2(hash_table_v2 *tree);
void freemem_v2(hash_table_v2 *tree, void(*data_free_f)(void*));
hnode_v2* get_elem_by_key_v2(hash_table_v2* table,char* key);
void* get_data_by_key_v2(hash_table_v2* tab,char* key);
hash_table_v2* addnode_v2( hash_table_v2 *table, char* key, void* data);


#endif // __U_HASH_H__
// #include "u_types/u_vector.h"
#ifndef U_VECTOR_H
#define U_VECTOR_H

// #include <u_stdlib/u_stdlib.h>
// #define STDLIB_API extern

typedef struct 
{
    void** mem;        //участок памяти, в котором находятся данные
    size_t n_elem;      //число элементов

}u_vector;

typedef struct 
{
    float* mem;        //участок памяти, в котором находятся данные
    size_t n_elem;      //число элементов   

}u_vector_float;




STDLIB_API u_vector* u_vector_new(void);
STDLIB_API void u_vector_free(u_vector* vec,void(*free_func)(void*));

STDLIB_API void u_vector_init(u_vector* vec);
STDLIB_API void u_vector_deinit(u_vector* vec,void(*free_func)(void*));
STDLIB_API void u_vector_push_back(u_vector* vec, void* el);
STDLIB_API void* u_vector_pop_back(u_vector* vec);
STDLIB_API void* u_vector_at(u_vector* vec,size_t at);
STDLIB_API int u_vector_cmp(u_vector* vec,size_t ind,void* el);
STDLIB_API int u_vector_is_exist(u_vector* vec,void* el);
STDLIB_API int u_vector_get_indx(u_vector* vec,void* el);
STDLIB_API int u_vector_find_indx(u_vector* vec,int(*cmp)(void* el, void* comparation),void* comparation );
//---------------------------------------------------------------------------------
STDLIB_API u_vector_float* u_vector_float_new(void);
STDLIB_API void u_vector_float_free(u_vector_float* vec);

STDLIB_API void u_vector_float_init(u_vector_float* vec);
STDLIB_API void u_vector_float_deinit(u_vector_float* vec);
STDLIB_API void u_vector_float_push_back(u_vector_float* vec, float el);
STDLIB_API float u_vector_float_pop_back(u_vector_float* vec);
STDLIB_API float u_vector_float_at(u_vector_float* vec,size_t at);
STDLIB_API int u_vector_float_cmp(u_vector_float* vec,size_t ind,float el);
STDLIB_API int u_vector_float_is_exist(u_vector_float* vec,float el);
STDLIB_API size_t u_vector_float_get_indx(u_vector_float* vec,float el);
STDLIB_API float u_vector_float_get_last(u_vector_float* vec);
STDLIB_API float u_vector_float_max(u_vector_float* vec);
STDLIB_API float u_vector_float_min(u_vector_float* vec);
STDLIB_API float u_vector_float_pop_front(u_vector_float* vec);
STDLIB_API float u_vector_fifo_forward(u_vector_float* vec, float el); // эквивалентно push_back+pop_front

// STDLIB_API int u_vector_float_find_indx(u_vector_float* vec,int(*cmp)(void* el, void* comparation),void* comparation );
u_vector_float*
u_vector_float_copy_new(u_vector_float* vec);

#endif //!U_VECTOR_H

// #include "u_application/u_csv.h"
#ifndef __CSV_H__
#define __CSV_H__

#include <stdlib.h>
#include <u_stdlib/u_stdlib.h>
// #include <u_types/u_vector.h>

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



// #include "u_application/u_fuzzy.h"
#ifndef __U_FUZZY_H__
#define __U_FUZZY_H__

#include <stdint.h>
#include <math.h>
#include <stdio.h>
// #include <u_types/u_vector.h>


#define U_MIN(x,y)    (((x)<(y))?(x):(y))
#define U_MAX(x,y)    (((x)>(y))?(x):(y))

typedef 
enum {
    OR,
    OR_NOT,
    OR_HIGHLY,
    OR_MORE_OR_LESS,

    AND,
    AND_NOT,
    AND_HIGHLY,
    AND_MORE_OR_LESS
}op_type;



typedef struct{

    u_vector* rulez;
    size_t n_params;
    
    float sigma; //дисперсия для гауссианы

}u_fuzzy;



u_fuzzy* u_fuzzy_new(size_t n_params);
void u_fuzzy_free(u_fuzzy* fuzzy);




void u_fuzzy_add_rule(   u_fuzzy* fuzzy,    //хэндл
                    op_type operand,        //как рассматривать параметры
                    float* params,          //массив параметров
                    size_t n,               //число параметров
                    float output);           //реакция на правило


/***************************************************
 * @brief  цикл работы регулятора
 * @note   
 * @param  fuzzy: хэндл
 * @param  _x: показания с датчика
 * @param  _n: размер массива показаний
 * @retval воздействие на исполнительный механизм
 ***************************************************/
float u_fuzzy_update(u_fuzzy* fuzzy, float* _x, size_t _n);


void u_fuzzy_set_sigma(u_fuzzy* fuzzy, float sigma);



void u_fuzzy_test(void);


#endif // __U_FUZZY_H__


// #include "u_application/u_ema.h"
#ifndef __U_EMA_H__
#define __U_EMA_H__

// #include <u_types/u_vector.h>


typedef struct u_ema_t
{
    // size_t flags;
    u_vector_float* value;
    u_vector_float* ema; //--экспоненциальная скользящая средняя
    // u_vector_float* sma; //--скользящая средняя
    size_t N;
}u_ema;


u_ema* u_ema_new(void);
void u_ema_push(u_ema* ma, float val);
float u_ema_pop(u_ema* ma);
void u_ema_fwd(u_ema* ma, float val);
void u_ema_free(u_ema* ma);
void u_ema_solve(u_ema* ema, size_t N);
void u_ema_attach_vector(u_ema* ema, u_vector_float* value);
void u_ema_set_N(u_ema* ema,size_t N);
void ema_print(u_ema* ema);

//--------------------------------------

// среднее
float u_avg(u_vector_float* x);

// коэффициент ковариации
float u_cov(u_vector_float* x,u_vector_float* y);

//дисперсия
float u_D(u_vector_float* x);

//среднеквадратичное отклонение
float u_sigma(u_vector_float* x);

//коэффициент корреляции Пирсона или линейный коэффициент корреляции 
float u_r(u_vector_float* x,u_vector_float* y);

//--------------------------------------

typedef struct
{
    u_vector_float* source; //--отсортированый исходник
    u_vector_float* IVR;    //--интервальный вариационный ряд
    u_vector_float* F;      //--функция распределения случайной величины
    float delta;            //--ширина интервала разбиения (по Стёрджесу)
    float max;              //--максимальное значение
    float min;              //--минимальное значение
    size_t n;               //--число интервалов разбивки
}u_ivr;

u_ivr* u_new_IVR(u_vector_float* x);
float u_median_IVR(u_ivr* ivr);
float u_moda_IVR(u_ivr* ivr);
void u_free_IVR(u_ivr* ivr);
#endif // __U_EMA_H__


// #include "u_application/u_fsm.h"
#ifndef __FSM_H__
#define __FSM_H__

// #include <u_types/u_vector.h>
#include <stdio.h>

typedef struct 
{
    u_vector stack; //стэк функций
    u_vector stack_data; //стэк данных
}fsm;

typedef void(*fsm_func)(fsm*,void*);

void fsm_change_state(fsm* f, fsm_func func, void* data);
void fsm_end_state(fsm* f);
int fsm_update(fsm* f);

fsm* fsm_new(fsm_func start_state_func,void* data);
void fsm_free(fsm* fv);

void fsm_init(fsm* fv,fsm_func start_state_func,void* data);
void fsm_deinit(fsm* fv);



#endif // __FSM_H__

// #include "u_types/u_cstring.h"
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

// #include "u_types/u_ring.h"
#ifndef U_RING_H
#define U_RING_H
#include <stdint.h>

typedef struct 
{
    void** data;
    size_t len; //вместимость буфера
    size_t end_pos;
    size_t start_pos;
    void (*free_func)(void*);
}u_ring_buf;



u_ring_buf* u_ring_new(size_t n_el,void (*free_f)(void*));
void u_ring_free(u_ring_buf* rb);
void u_ring_fifo_push(u_ring_buf* rb, void* data);
void* u_ring_fifo_pop(u_ring_buf* rb);
size_t u_ring_calc_len(u_ring_buf* rb);



typedef struct 
{
    float* data;
    size_t len; //вместимость буфера
    size_t end_pos;
    size_t start_pos;
}u_ring_float;

size_t u_ring_float_calc_len(u_ring_float *rb);
u_ring_float* u_ring_float_new(size_t n_el);
void u_ring_float_free(u_ring_float* rb);
void u_ring_float_fifo_push(u_ring_float* rb, float data);
float u_ring_float_fifo_pop(u_ring_float* rb);
float u_ring_float_max(u_ring_float *rb);
float u_ring_float_min(u_ring_float *rb);


#endif // !U_RING_H

//--------------------
//-- сетевые модули --
//--------------------
// #include "u_network/u_network.h"
#ifndef U_NETWORK_H
#define U_NETWORK_H


#if defined (_WIN32)|| defined(_WIN64)|| defined(__WIN32__) || defined(__WINDOWS__)
#define OS_WINDOWS
#endif

#if defined(unix) || defined(__unix__) || defined(__unix)
#define PLATFORM_UNIX
#endif

#if !defined (PLATFORM_UNIX) && !defined (OS_WINDOWS)
#error "not support APPLE"
#endif // !1



//-------------------WINDOWS-----------------------
#ifdef OS_WINDOWS
#include <winsock2.h>


typedef  SOCKET u_socket_t;

#define U_INVALID_SOCKET    INVALID_SOCKET


#endif //OS_WINDOWS
//-------------------LINUX-----------------------
#ifdef PLATFORM_UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <sys/select.h>
#include <sys/types.h>
#include <netdb.h>
// #include <fcntl.h>


#define U_INVALID_SOCKET    0

typedef int u_socket_t;


#endif // PLATFORM_UNIX
//-------------------ALL-----------------------
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

typedef struct 
{
    u_socket_t in;  //for reading
    u_socket_t out; //for writing
}io_sockets;



int start_NW();
void stop_NW();
int u_close_sock(u_socket_t sock);

typedef  struct sockaddr_in u_sockaddr_in_t;
typedef  struct sockaddr u_sockaddr_t;
u_socket_t u_accept(u_socket_t socket,u_sockaddr_t* client_addr, int* size_client_addr);
int u_getsockname(u_socket_t socket, u_sockaddr_t* addr, int* namelen);
#endif // !U_NETWORK_H

// #include "u_network/u_server.h"
#ifndef U_SERVER
#define U_SERVER
// #include <u_network/u_network.h>
// #include <u_application/u_fsm.h>
// #include <u_types/u_vector.h>

#define U_IP(a,b,c,d)  ((a&0xff)|((b&0xff)<<8)|((c&0xff)<<16)|((d&0xff)<<24))

typedef struct u_server_t{
    io_sockets io;
    // void(*handler)(fsm*,io_sockets*);
    fsm* fsm_rd; //finitie state machine for read
    fsm* fsm_wr; //finitie state machine for write
    void(*wr)(fsm*,struct u_server_t*);
    void(*rd)(fsm*,struct u_server_t*);
    struct timeval timeout;
    
    union{
        u_vector* buffer;
        void* proto_data;
    };

}u_server;




struct server_work_in
{
    io_sockets* io;
    void(*handler)(io_sockets* io);

};



// int server_work(struct server_work_in* in);
// void init_server(uint16_t port, io_sockets* io);
// void deinit_server(io_sockets* io);

//  int howto_test (void) ;

u_server* u_server_new(uint16_t port);
void u_server_free(u_server* srv);
int u_server_update(u_server* srv);

#endif // !U_SERVER


//-------------------------
//-- модули криптографии --
//-------------------------


//---------------------------------------------------
// Найдено на github.
// Алгоритм шифрования "Кузнечик"
// подробнее см. ГОСТ Р 34.12-2015
// КУЗНЕЧИК = КУЗнецов, НЕЧаев И Компания.
// описание на хабре https://habr.com/ru/post/459004/
// Охренительная лекция МФТИ https://www.youtube.com/watch?v=PbDOmI1iM64
//---------------------------------------------------
// kuznechik.h
// 04-Jan-15  Markku-Juhani O. Saarinen <mjos@iki.fi>
//---------------------------------------------------

#ifndef U_KUZNECHIK_H
#define U_KUZNECHIK_H

#include <stdint.h>

// my 128-bit datatype
typedef union {	
    uint64_t q[2];
    uint8_t  b[16];
} w128_t;

// cipher context :: контекст шифра
typedef struct {
	w128_t k[10];		// round keys
} kuz_key_t;

typedef union {	
    w128_t   h[2];
    uint64_t q[4];
    uint8_t  b[32];
} w256_t;

// init lookup tables :: инициализация таблиц поиска
void kuz_init();

// key setup :: Формирование раундовых ключей (де)шифрования
void kuz_set_encrypt_key(kuz_key_t *subkeys, const uint8_t key[32]);	
void kuz_set_decrypt_key(kuz_key_t *subkeys, const uint8_t key[32]);	

// single-block ecp ops
void kuz_encrypt_block(kuz_key_t *subkeys, void *x);
void kuz_decrypt_block(kuz_key_t *subkeys, void *x);


/*****************************************************************
* чтобы зашифровать текст, длина которого больше длины блока, существует 
* несколько режимов, описанных в стандарте — ГОСТ 34.13-2015:
*
** режим простой замены (Electronic Codebook, ECB);
** режим гаммирования (Counter, CTR);
** режим гаммирования с обратной связью по выходу (Output Feedback, OFB);
** режим простой замены с зацеплением (Cipher Block Chaining, CBC);
** режим гаммирования с обратной связью по шифротексту (Cipher Feedback, CFB);
** режим выработки имитовставки (Message Authentication Code, MAC).
*
* Во всех режимах длина текста всегда должна быть кратна длине блока, поэтому текст 
* всегда дополняется справа одним единичным битом и нулями до длины блока.
*****************************************************************/

#include <stddef.h>

void* kuz_encrypt_ECB(const w256_t key,void *x, size_t n);
void* kuz_decrypt_ECB(const w256_t key,void *x, size_t n);
void kuz_test();

/**
*  CBC 
* шифруем блок и прибавляем вектор. шифрованый блок является 
* вектором для следующего блока
* 
*/

//блок текст XOR вектор -> шифрованый блок, который является вектором для следующего
void* kuz_encrypt_CBC(const w256_t key,const w128_t vec,void *x, size_t n);
//шифрованый блок, который является вектором для следующего ->расшифровка блока XOR вектор = текстовый блок
void* kuz_decrypt_CBC(const w256_t key,const w128_t Vec,void *x, size_t n);



/**
 * режим гаммирования (Counter, CTR);
 * шифруем счётчик и прибавляем к нему блок
 * E(X) = Y
 * E(Y) = X
 */



#endif





#endif // __U_CAT_H__
