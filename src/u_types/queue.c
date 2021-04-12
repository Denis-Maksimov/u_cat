
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>    
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

#define c_new(t)     ((t*)malloc(sizeof(t)))
#define c_new_n(t,n)     ((t*)malloc(sizeof(t)*n))

struct queue 
{
  size_t size_queue; //max размер очереди
  void* qu;          //элементы
  size_t count; //текущее число элементов
};


struct queue* init(size_t lenght_queue) 
{
  struct queue* q = c_new(struct queue);
  q->size_queue = lenght_queue;
  q->qu=c_new_n(u8,lenght_queue);
  q->count = 0;
  return q;
}

void free_queue(struct queue* q)
{
    free(q->qu);
    free(q);
}
//---------------------------------
int isempty(struct queue *q) 
{
  if(!q->count)
    return 1;
  return 0;
}

int isfull(struct queue *q) 
{
  if(q->count >= q->size_queue)
    return 1;
  return 0;
}

int insert(struct queue *q, void* x) {
  if(!isfull(q)) 
  {
    ((u8*)q->qu)[q->count]=x;
    q->count++;
    return 0;
  }
  return -1;
}

int pop_queue(struct queue *q) {
  int retval;

  if(isempty(q)) {
    return 0;
  }

  retval = ((u8*)q->qu)[0];

  //сдвинуть по байтикам
  for(int i = 0; i < q->count; i++) {
    ((size_t*)q->qu)[i] = ((size_t*)q->qu)[i+1];
  }

  q->count--;
  return retval;
}
//---------------------------------
//---------------------------------
// void print_queue(struct queue *q) {
    
//   if(isempty(q)) 
//   {
//     printf("Очередь пуста!\n");
//     return;
//   }
//   for(int i=0; i < q->count; i++)
//     printf("%d ",((u8*)q->qu)[i]);
//   return;
// }

//---------------------------------
// #define QMAX 100


// int main(int argc, char const *argv[])
// {
//   struct queue* q= init(QMAX);
//   int a;
  
//   print_queue(q);
//   int i=0;

//   //--полностью заполняем от нуля до конца
//   while(!isfull(q)) {
//     insert(q, i);
//     i++;
//   }
//   printf("\n");
//   print_queue(q);

//   //--читаем с очереди пока там есть значения
//   while(!isempty(q)) 
//   {
//     a = pop_queue(q);
//     printf("\nУдален элемент %d\n", a);
//     print_queue(q);
//   }

//   //--утечку памяти никто не отменял - попользовался прибери за собой
//   free_queue(q);

//   return 0;
// }
