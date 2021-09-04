#ifndef U_CHAIN_H
#define U_CHAIN_H
#include <u_stdlib/u_stdlib.h>
C_START
//-------------
struct Array{
    void* data;
    size_t n;
};
//-------------

struct chain
{
  struct chain* prew_chain;
  struct chain* next_chain;
  struct Array array;

};

// extern struct chain* init_chain();
/**
 * @brief  создаёт двусвязный список и помещает данные в первый элемент
 * @note   Не теряйте указатель на первый элемент!
 * @param  _data: данные
 * @param  n: размер данных в байтах
 * @retval указатель
 */
struct chain* u_new_chain(void* _data, size_t n);

struct chain* u_find_last(struct chain* ch);

struct chain* u_find_first(struct chain* ch);

/**
 * @brief  объединяет 2 цепи в одну
 * @note   
 * @param  first: 
 * @param  second: 
 * @retval 
 */
 int u_attach(struct chain* first, struct chain* second);

 struct chain* u_insert_after(struct chain* dest, void* data, size_t n);
 struct chain* u_insert_after_ch(struct chain* dest, struct chain* ch);
 struct chain* u_insert_before(struct chain* dest, void* data, size_t n);
 void u_free_chain(struct chain* ch);
 struct chain* u_cut(struct chain* ch);
 struct chain* u_copy(struct chain* ch);
 void u_rewrite_chain(struct chain* ch,void* data, size_t n);

C_END
#endif // !U_CHAIN_H
