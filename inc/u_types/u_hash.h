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