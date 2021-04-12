
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>    
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

#define QMAX  16 //байт

enum bytes{
  _byte=1,
  _hword=2,
  _word=4,
};

struct queue 
{
  size_t size_queue; //max размер очереди
  enum bytes data_size; //размер элемента данных
  u8* qu[QMAX];          //элементы
  size_t count; //текущее число элементов
  u8 busy_flag; //происходит ли чтение-запись в данный момент
};


void init(struct queue* q, enum bytes b) 
{
  q->size_queue = QMAX/b;
  q->data_size=b;
  q->count = 0;
  q->busy_flag=0;
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

int insert(struct queue *q, int x) {
  if(!isfull(q)) 
  {
    if(q->data_size==_byte)
      ((s8*)q->qu)[q->count] = (s8)x;
    if(q->data_size==_hword)
      ((s16*)q->qu)[q->count] = (s16)x;
    if(q->data_size==_word)
      ((s32*)q->qu)[q->count] = (s32)x;

    q->count++;
    return 0;
  }
  return -1;
}



u32 pop_queue(struct queue *q) {

  if(isempty(q)) {
    return 0;
  }
  u32 retval;

  if(q->data_size==_byte)
    retval = (u32)(((u8*)q->qu)[0]);
  if(q->data_size==_hword)
    retval = (u32)(((u16*)q->qu)[0]);
  if(q->data_size==_word)
    retval = (u32)(((u32*)q->qu)[0]);

  //сдвинуть по байтикам
  for(int i = 0; i < q->count; i+=q->data_size) {
    ((u8*)q->qu)[i] = ((u8*)q->qu)[i+q->data_size];
  }

  q->count--;
  return retval;
}
//---------------------------------
//---------------------------------
void print_queue(struct queue *q) {
    
  if(isempty(q)) 
  {
    printf("Очередь пуста!\n");
    return;
  }
  switch (q->data_size)
  {
  case _word:
    for(int i=0; i < q->count; i++)
    printf("%d ",((s32*)q->qu)[i]);
    return;
  case _hword:
    for(int i=0; i < q->count; i++)
    printf("%d ",((s16*)q->qu)[i]);
    return;
  case _byte:
    for(int i=0; i < q->count; i++)
    printf("%d ",((s8*)q->qu)[i]);
    return;
  
  default:
    break;
  }
  
}

//---------------------------------
// #define QMAX 100
struct queue q;

int main(int argc, char const *argv[])
{
  init(&q, 1);
  
//   print_queue(&q);
  for(int i=0;i<10000000;i++){

      //--полностью заполняем от нуля до конца
      while(!isfull(&q)) {
        insert(&q, i);
        i++;
      }
    //   printf("\n");
    //   print_queue(&q);

      //--читаем с очереди пока там есть значения
      while(!isempty(&q)) 
      {
          pop_queue(&q);
        // printf("\nУдален элемент %d\n", );
        // print_queue(&q);
      }
  }
  //--утечку памяти никто не отменял - попользовался прибери за собой
  // free_queue(q);
    print_queue(&q);
  return 0;
}
