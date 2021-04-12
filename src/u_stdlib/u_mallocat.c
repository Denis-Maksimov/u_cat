// #include <stdlib.h>
// #include <string.h>
#include <u_stdlib/u_mallocat.h>
// typedef unsigned char u8;
// typedef unsigned short u16;
// typedef unsigned int u32;
// typedef signed char s8;
// typedef signed short s16;
// typedef signed int s32;

// #define uHEAP_SIZE  MY_HEAP_SIZE

struct memory_block{
    u16 is_available:1;
	u16 size:15;
};//_head_mcb={1,uHEAP_SIZE-sizeof(struct memory_block)};

static u8 u_heap[uHEAP_SIZE]={1|(((uHEAP_SIZE-sizeof(struct memory_block))<<1)&0xff),(((uHEAP_SIZE-sizeof(struct memory_block))<<1)&0xff00)>>8};
// static u8 init_flag=0;
// static inline void first_init(){
//     ((struct memory_block*)((void*)heap))->is_available=1;
//     ((struct memory_block*)((void*)heap))->size=uHEAP_SIZE-sizeof(struct memory_block);
// }

u8* u_get_heap(void)
{
    return &u_heap[0];
}

static inline void* process_alloc(struct memory_block* heap_mb,u16 size){
    heap_mb->is_available=0;
    u16 old = heap_mb->size;
    heap_mb->size=size;

    if(old-size>0){
        

        struct memory_block* new = ((struct memory_block*)(((u8*)(void*)heap_mb)+size+sizeof(struct memory_block)));
        new->is_available=1;
        new->size=old-size-sizeof(struct memory_block);
        // if(old-size>2){}
    }
    return &heap_mb[1];
}

void* u_malloc(size_t size){
    // if(!init_flag){
    //     init_flag=1;
    //     first_init();
    // }
    s16 aval_size=uHEAP_SIZE;
    struct memory_block* heap_ptr=((struct memory_block*)((void*)u_heap));
    while (aval_size>sizeof(struct memory_block))
    {
        aval_size=aval_size-sizeof(struct memory_block);
        if(heap_ptr->is_available && ((s32)(heap_ptr->size)-(s32)size)>=0)
        {
            return process_alloc(heap_ptr,(u16)size);
        }

        //если в блоке нет памяти или он занят, прыгаем в следующий блок
        // if(!heap_ptr->is_available||heap_ptr->size-size<0){
            aval_size=aval_size-heap_ptr->size;
            heap_ptr=( struct memory_block*)((u8*)heap_ptr+heap_ptr->size+sizeof(struct memory_block));
            // continue;
        // }

    }
    return 0;
    
}
// #include <stdio.h>
static inline void merge_heap(void)
{
    //TODO
    //указатель на начало
    struct memory_block* heap_mb=((struct memory_block*)((void*)u_heap));
    //указатель на следующий для шагания
    struct memory_block* heap_mb_next;

    //когда это значение == 0, значит память кончилась
    s16 aval_size=uHEAP_SIZE-sizeof(struct memory_block)-heap_mb->size;

    while(aval_size>0){
        //ищем где следующий блок
        heap_mb_next=(struct memory_block*)((u8*)heap_mb+heap_mb->size+sizeof(struct memory_block));

        if(heap_mb_next-heap_mb>aval_size)return;
        //если текущий и следующий свободные, то мержим их
        if(heap_mb->is_available&&heap_mb_next->is_available)
        {
            //увеличиваем размер 1-го на размер 2-го+размер заголовка
            heap_mb->size+=heap_mb_next->size;
            

        }else{//иначе шагаем дальше
            heap_mb=heap_mb_next;
            aval_size-=sizeof(struct memory_block);
            /* code */
        }
        //и показываем, что проверили
            // printf("~%d~",aval_size);
            // fflush(stdout);
            aval_size-=heap_mb_next->size;
    }
}

void u_free(void* ptr)
{
    if(!ptr)return;
    struct memory_block* heap_mb=(struct memory_block*)ptr-1;
    heap_mb->is_available=1;
    merge_heap();
    
    //Шаг назад, говорим, что свободны, и мержим смежные блоки в хипе
}


void* u_memset(void *__s, int __c, size_t __n)
{
    for(size_t i=0; i<__n; i++)
    {
        ((u8*)__s)[i]=(u8)__c;
    }
    return __s;
}

void* u_memcpy(void *__dest, const void *__src, size_t __n)
{
    for(size_t i=0;i<__n;i++)
    {
        ((u8*)__dest)[i]=((u8*)__src)[i];
    }
    return __dest;
}


/* Re-allocate the previously allocated block
   in PTR, making the new block SIZE bytes long.  */
void* u_realloc (void *__ptr, size_t __size)
{
    if(!__ptr) return u_malloc(__size);
    struct memory_block* heap_mb=(struct memory_block*)__ptr-1;

    size_t siZa = heap_mb->size;
    if(siZa == __size){
        u8 tmp[siZa];
        u_memcpy(tmp,__ptr,siZa);
        u_free(__ptr);
        __ptr=u_malloc(__size);
        u_memcpy(__ptr, tmp, siZa);
        return __ptr;
    }
    if(siZa > __size){
        siZa=__size;
    }
    u8 tmp[siZa];
    u_memcpy(tmp,__ptr,siZa);
    u_free(__ptr);
    __ptr=u_malloc(__size);
    u_memcpy(__ptr, tmp, siZa);
    return __ptr;
    // TODO XXX: может грохнуть ранее выделенную память, если не хватает места
    
}
