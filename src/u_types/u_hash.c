
#include <u_types/u_hash.h>




static unsigned char T[256];
static char table_yes=0;



static void init_pearson_table(unsigned char* tbl,unsigned int __seed)
{
    srand(__seed);
    for (size_t i = 0; i < 256; i++)
    {
        tbl[i]=rand();
    } 
    table_yes=1;
}



//----------------------------------------------
// h := 0
// for each c in C loop
//     h := T[h xor c]
// end loop
// return h
//----------------------------------------------
size_t pearson_hash(char* key,size_t clamp)
{
    if(!table_yes)init_pearson_table(T,123);
    
    size_t len =strlen(key);
    size_t h;
    size_t _hh;
    unsigned char* hh=(unsigned char*)&(_hh);
    

    for (size_t j = 0; j < 8; ++j)
    {
         h = T[(key[0] + j) % 256];
         for (size_t i = 1; i < len; ++i)
         {
            h = T[h^key[i]];
         }
         hh[j] = h;
    }
    return _hh%clamp;
}



void hashprint(hash_table *tree)
{
    int a=tree->n_elem;
    for (size_t i = 0; i < tree->size; i++)
    {
       if(tree->nodes[i].flags){
           printf("i=%ld; id=%d;\n",i,tree->nodes[i].key);
           a--;
           if(!a)return;
       }
    }
    
}

//---------------------------------------------------------

void freemem(hash_table *tree, void(*data_free_f)(void*))
{
    if(data_free_f)
    {
        int a=tree->n_elem;
        for (size_t i = 0; i < tree->size; i++)
        {
            if(tree->nodes[i].flags){
                // printf("i=%d; id=%d;\n",i,tree->nodes[i].key);
                data_free_f(tree->nodes[i].data);
                a--;
                if(!a)break;
            }
        }
    }

    free(tree->nodes);
    free(tree);

}



int hash_func(int a,int size){
    return a%size;
}
#define HASH_S  1024



hash_table* addnode( hash_table *table, int id, void* data)
{
    if (!table)     // Если таблицы нет - создаём
    {
        table = c_new(hash_table);
        table->n_elem=0;
        table->size=HASH_S;
        table->nodes=calloc(HASH_S,sizeof(hnode));
        memset(table->nodes,0,HASH_S*sizeof(hnode));
    }
    hnode* el= &(table->nodes[hash_func(id,table->size)]);
    
    while (el->flags==1)
    {
        el++;
        if(&table->nodes[table->size-1]<el)return 0;
    }
    if(el->flags==0){
        el->data=data;
        el->key=id;
        el->flags=1;
    }
    table->n_elem++;
    return table;

}
hnode* get_elem_by_key(hash_table* table,int key)
{
    hnode* el= &(table->nodes[hash_func(key, table->size)]);

    while ((el->key!=key)&& (size_t)el < table->size-1)
    {
        el++;
        if(&table->nodes[table->size-1]<el)return 0;
    }
    return el;

}
void* u_get_data_by_key(hash_table* tab,int key)
{
    return get_elem_by_key(tab,key)->data;

}

//================================================================

hnode_v2* get_elem_by_key_v2(hash_table_v2* table,char* key)
{
    size_t hash=pearson_hash(key,table->size);
    hnode_v2* el= &(table->nodes[hash]);

    while (strcmp(el->key,key))
    {
        el++;
        if(&table->nodes[table->size-1]<el)return 0;
    }
    return el;

}
void* get_data_by_key_v2(hash_table_v2* tab,char* key)
{
    return get_elem_by_key_v2(tab,key)->data;

}


void hashprint_v2(hash_table_v2 *tree)
{
    int a=tree->n_elem;
    for (size_t i = 0; i < tree->size; i++)
    {
       if(tree->nodes[i].flags){
           printf("i=%ld; id=%s;\n",i, tree->nodes[i].key);
           a--;
           if(!a)return;
       }
    }
    
}

void freemem_v2(hash_table_v2 *tree, void(*data_free_f)(void*))
{
    
    
        int a=tree->n_elem;
        for (size_t i = 0; i < tree->size; i++)
        {
            if(tree->nodes[i].flags){
                // printf("i=%d; id=%d;\n",i,tree->nodes[i].key);
                free(tree->nodes[i].key);
                if(data_free_f)
                data_free_f(tree->nodes[i].data);
                a--;
                if(!a)break;
            }
        }
    

    free(tree->nodes);
    free(tree);

}

hash_table_v2* addnode_v2( hash_table_v2 *table, char* key, void* data)
{

    if (!table)     // Если таблицы нет - создаём
    {
        table = c_new(hash_table_v2);
        table->n_elem=0;
        table->size=HASH_S;
        table->nodes=calloc(HASH_S,sizeof(hnode_v2));
        memset(table->nodes,0,HASH_S*sizeof(hnode_v2));
    }
    size_t hash=pearson_hash(key,table->size);
    hnode_v2* el= &(table->nodes[hash]);

    while (el->flags==1)
    {
        el++;
        if(&table->nodes[table->size-1]<el)return 0;
    }
    if(el->flags==0){
        el->data=data;
        el->key=malloc(strlen(key)+1);
        strcpy(el->key,key);
        el->flags=1;
    }
    table->n_elem++;
    return table;
}






static int keys[]=  {1024,1,1546512,25465613,1565651,3356302,6521651};
static char* vals[]= {"hello","my", "beautifull",  "world",     "puma",  "best",   "regards"};
static char* key2[]= {"org.a","o2", "i love you",  "beam",      "johan",  "bear",   "redgard"};
// Тестирование работы
static int main_v1(int argc, char* argv[] ) 
{
  hash_table *root = 0;    // Объявляем структуру дерева
  // В цикле вводим 8 узлов дерева
  for (int i = 0; i<7; i++) 
  {
    root = addnode(root, keys[i] ,  &vals[i]); // размещаем введенный узел на дереве
  }
//   for (int i = 0; i<7; i++) 
//   {
//     printf("ID=%d data=%s\n",keys[i], ((char**)get_data_by_key(root,keys[i]))[0]); // размещаем введенный узел на дереве
//   }
  hashprint(root);
  
  freemem(root, 0);      // удаляем выделенную память
  return 0;
}


// int main(int argc, char* argv[] ) 
// {
//   hash_table_v2 *root = 0;    // Объявляем структуру дерева
//   // В цикле вводим 8 узлов дерева
//   for (int i = 0; i<7; i++) 
//   {
//     root = addnode_v2(root, key2[i] ,  &vals[i]); // размещаем введенный узел на дереве
//   }
// //   for (int i = 0; i<7; i++) 
// //   {
// //     printf("ID=%d data=%s\n",keys[i], ((char**)get_data_by_key(root,keys[i]))[0]); // размещаем введенный узел на дереве
// //   }
//   hashprint_v2(root);
  
//   freemem_v2(root, 0);      // удаляем выделенную память
//   return 0;
// }
