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
