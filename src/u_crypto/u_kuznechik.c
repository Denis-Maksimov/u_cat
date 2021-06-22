
#include <u_crypto/u_kuznechik.h>

// This is the basic non-optimized 8-bit "readble" version of the cipher.
// Conforms to included doc/GOSTR-bsh.pdf file, which has an internal
// date of September 2, 2014.


// The S-Box from section 5.1.1

//Таблица нелинейного преобразования
const uint8_t kuz_pi[0x100] = {
	0xFC, 0xEE, 0xDD, 0x11, 0xCF, 0x6E, 0x31, 0x16, 	// 00..07
	0xFB, 0xC4, 0xFA, 0xDA, 0x23, 0xC5, 0x04, 0x4D, 	// 08..0F
	0xE9, 0x77, 0xF0, 0xDB, 0x93, 0x2E, 0x99, 0xBA, 	// 10..17
	0x17, 0x36, 0xF1, 0xBB, 0x14, 0xCD, 0x5F, 0xC1, 	// 18..1F
	0xF9, 0x18, 0x65, 0x5A, 0xE2, 0x5C, 0xEF, 0x21, 	// 20..27
	0x81, 0x1C, 0x3C, 0x42, 0x8B, 0x01, 0x8E, 0x4F, 	// 28..2F
	0x05, 0x84, 0x02, 0xAE, 0xE3, 0x6A, 0x8F, 0xA0, 	// 30..37
	0x06, 0x0B, 0xED, 0x98, 0x7F, 0xD4, 0xD3, 0x1F, 	// 38..3F
	0xEB, 0x34, 0x2C, 0x51, 0xEA, 0xC8, 0x48, 0xAB, 	// 40..47
	0xF2, 0x2A, 0x68, 0xA2, 0xFD, 0x3A, 0xCE, 0xCC, 	// 48..4F
	0xB5, 0x70, 0x0E, 0x56, 0x08, 0x0C, 0x76, 0x12, 	// 50..57
	0xBF, 0x72, 0x13, 0x47, 0x9C, 0xB7, 0x5D, 0x87, 	// 58..5F
	0x15, 0xA1, 0x96, 0x29, 0x10, 0x7B, 0x9A, 0xC7, 	// 60..67
	0xF3, 0x91, 0x78, 0x6F, 0x9D, 0x9E, 0xB2, 0xB1, 	// 68..6F
	0x32, 0x75, 0x19, 0x3D, 0xFF, 0x35, 0x8A, 0x7E, 	// 70..77
	0x6D, 0x54, 0xC6, 0x80, 0xC3, 0xBD, 0x0D, 0x57, 	// 78..7F
	0xDF, 0xF5, 0x24, 0xA9, 0x3E, 0xA8, 0x43, 0xC9, 	// 80..87
	0xD7, 0x79, 0xD6, 0xF6, 0x7C, 0x22, 0xB9, 0x03, 	// 88..8F
	0xE0, 0x0F, 0xEC, 0xDE, 0x7A, 0x94, 0xB0, 0xBC, 	// 90..97
	0xDC, 0xE8, 0x28, 0x50, 0x4E, 0x33, 0x0A, 0x4A, 	// 98..9F
	0xA7, 0x97, 0x60, 0x73, 0x1E, 0x00, 0x62, 0x44, 	// A0..A7
	0x1A, 0xB8, 0x38, 0x82, 0x64, 0x9F, 0x26, 0x41, 	// A8..AF
	0xAD, 0x45, 0x46, 0x92, 0x27, 0x5E, 0x55, 0x2F, 	// B0..B7
	0x8C, 0xA3, 0xA5, 0x7D, 0x69, 0xD5, 0x95, 0x3B, 	// B8..BF
	0x07, 0x58, 0xB3, 0x40, 0x86, 0xAC, 0x1D, 0xF7, 	// C0..C7
	0x30, 0x37, 0x6B, 0xE4, 0x88, 0xD9, 0xE7, 0x89, 	// C8..CF
	0xE1, 0x1B, 0x83, 0x49, 0x4C, 0x3F, 0xF8, 0xFE, 	// D0..D7
	0x8D, 0x53, 0xAA, 0x90, 0xCA, 0xD8, 0x85, 0x61, 	// D8..DF
	0x20, 0x71, 0x67, 0xA4, 0x2D, 0x2B, 0x09, 0x5B, 	// E0..E7
	0xCB, 0x9B, 0x25, 0xD0, 0xBE, 0xE5, 0x6C, 0x52, 	// E8..EF
	0x59, 0xA6, 0x74, 0xD2, 0xE6, 0xF4, 0xB4, 0xC0, 	// F0..F7
	0xD1, 0x66, 0xAF, 0xC2, 0x39, 0x4B, 0x63, 0xB6, 	// F8..FF
};

// Inverse S-Box

static const uint8_t kuz_pi_inv[0x100] = {
	0xA5, 0x2D, 0x32, 0x8F, 0x0E, 0x30, 0x38, 0xC0, 	// 00..07
	0x54, 0xE6, 0x9E, 0x39, 0x55, 0x7E, 0x52, 0x91, 	// 08..0F
	0x64, 0x03, 0x57, 0x5A, 0x1C, 0x60, 0x07, 0x18, 	// 10..17
	0x21, 0x72, 0xA8, 0xD1, 0x29, 0xC6, 0xA4, 0x3F, 	// 18..1F
	0xE0, 0x27, 0x8D, 0x0C, 0x82, 0xEA, 0xAE, 0xB4, 	// 20..27
	0x9A, 0x63, 0x49, 0xE5, 0x42, 0xE4, 0x15, 0xB7, 	// 28..2F
	0xC8, 0x06, 0x70, 0x9D, 0x41, 0x75, 0x19, 0xC9, 	// 30..37
	0xAA, 0xFC, 0x4D, 0xBF, 0x2A, 0x73, 0x84, 0xD5, 	// 38..3F
	0xC3, 0xAF, 0x2B, 0x86, 0xA7, 0xB1, 0xB2, 0x5B, 	// 40..47
	0x46, 0xD3, 0x9F, 0xFD, 0xD4, 0x0F, 0x9C, 0x2F, 	// 48..4F
	0x9B, 0x43, 0xEF, 0xD9, 0x79, 0xB6, 0x53, 0x7F, 	// 50..57
	0xC1, 0xF0, 0x23, 0xE7, 0x25, 0x5E, 0xB5, 0x1E, 	// 58..5F
	0xA2, 0xDF, 0xA6, 0xFE, 0xAC, 0x22, 0xF9, 0xE2, 	// 60..67
	0x4A, 0xBC, 0x35, 0xCA, 0xEE, 0x78, 0x05, 0x6B, 	// 68..6F
	0x51, 0xE1, 0x59, 0xA3, 0xF2, 0x71, 0x56, 0x11, 	// 70..77
	0x6A, 0x89, 0x94, 0x65, 0x8C, 0xBB, 0x77, 0x3C, 	// 78..7F
	0x7B, 0x28, 0xAB, 0xD2, 0x31, 0xDE, 0xC4, 0x5F, 	// 80..87
	0xCC, 0xCF, 0x76, 0x2C, 0xB8, 0xD8, 0x2E, 0x36, 	// 88..8F
	0xDB, 0x69, 0xB3, 0x14, 0x95, 0xBE, 0x62, 0xA1, 	// 90..97
	0x3B, 0x16, 0x66, 0xE9, 0x5C, 0x6C, 0x6D, 0xAD, 	// 98..9F
	0x37, 0x61, 0x4B, 0xB9, 0xE3, 0xBA, 0xF1, 0xA0, 	// A0..A7
	0x85, 0x83, 0xDA, 0x47, 0xC5, 0xB0, 0x33, 0xFA, 	// A8..AF
	0x96, 0x6F, 0x6E, 0xC2, 0xF6, 0x50, 0xFF, 0x5D, 	// B0..B7
	0xA9, 0x8E, 0x17, 0x1B, 0x97, 0x7D, 0xEC, 0x58, 	// B8..BF
	0xF7, 0x1F, 0xFB, 0x7C, 0x09, 0x0D, 0x7A, 0x67, 	// C0..C7
	0x45, 0x87, 0xDC, 0xE8, 0x4F, 0x1D, 0x4E, 0x04, 	// C8..CF
	0xEB, 0xF8, 0xF3, 0x3E, 0x3D, 0xBD, 0x8A, 0x88, 	// D0..D7
	0xDD, 0xCD, 0x0B, 0x13, 0x98, 0x02, 0x93, 0x80, 	// D8..DF
	0x90, 0xD0, 0x24, 0x34, 0xCB, 0xED, 0xF4, 0xCE, 	// E0..E7
	0x99, 0x10, 0x44, 0x40, 0x92, 0x3A, 0x01, 0x26, 	// E8..EF
	0x12, 0x1A, 0x48, 0x68, 0xF5, 0x81, 0x8B, 0xC7, 	// F0..F7
	0xD6, 0x20, 0x0A, 0x08, 0x00, 0x4C, 0xD7, 0x74	 	// F8..FF
};

// Linear vector from sect 5.1.2
//коэффициенты для выполнения линейного преобразования
static const uint8_t kuz_lvec[16] = {
	0x94, 0x20, 0x85, 0x10, 0xC2, 0xC0, 0x01, 0xFB, 
	0x01, 0xC0, 0xC2, 0x10, 0x85, 0x20, 0x94, 0x01
};




#ifndef __SSE4_1__
// #error "This version requires __SSE4_1__"
// kuznechik.c
// 04-Jan-15  Markku-Juhani O. Saarinen <mjos@iki.fi>

// poly multiplication mod p(x) = x^8 + x^7 + x^6 + x + 1
// totally not constant time
// умножение на поле Галуа
static uint8_t kuz_mul_gf256(uint8_t x, uint8_t y)
{
	uint8_t z=0;
	
	while (y) {		
		if (y & 1)
			z ^= x;
		x = (x << 1) ^ (x & 0x80 ? 0xC3 : 0x00);
		y >>= 1;
	}
		
	return z;
}


/*********************************************************************
** Линейное преобразование. :: linear operation l
* ********************************************************************
* Каждый байт из блока умножается в поле Галуа 
* на один из коэффициентов ряда 
* (148, 32, 133, 16, 194, 192, 1, 251, 1, 192, 194, 16, 133, 32, 148, 1) 
* в зависимости от порядкового номера байта 
* (ряд представлен для порядковых номеров от 15-ого до 0-ого, 
* как представлено на рисунке). Байты складываются между 
* собой по модулю 2, и все 16 байт блока сдвигаются в сторону 
* младшего разряда, а полученное число записывается на место 
* считанного байта. 
* 
* ********************************************************************/
static void kuz_l(w128_t *w)
{
	int i, j;
	uint8_t x;
	
	// 16 rounds
	for (j = 0; j < 16; j++) {

		// An LFSR with 16 elements from GF(2^8)
		x = w->b[15];	// since lvec[15] = 1

		for (i = 14; i >= 0; i--) {
			w->b[i + 1] = w->b[i];
			x ^= kuz_mul_gf256(w->b[i], kuz_lvec[i]);
		}
		w->b[0] = x;
	}
}

// inverse of linear operation l

static void kuz_l_inv(w128_t *w)
{
	int i, j;
	uint8_t x;
	
	// 16 rounds
	for (j = 0; j < 16; j++) {

		x = w->b[0];
		for (i = 0; i < 15; i++) {
			w->b[i] = w->b[i + 1];	
			x ^= kuz_mul_gf256(w->b[i], kuz_lvec[i]);
		}
		w->b[15] = x;
	}
}

// a no-op :: инициализация таблиц поиска

void kuz_init()
{
	;
}


//
/************************************************************
**      key setup :: Формирование раундовых ключей         **
 ************************************************************
 * Итерационные (или раундовые) ключи получаются путем определенных 
 * преобразований на основе мастер-ключа, длина которого, 
 * как мы уже знаем, составляет 256 бит. Этот процесс начинается 
 * с разбиения мастер-ключа пополам, так получается первая пара 
 * раундовых ключей. Для генерации каждой последующей пары раундовых 
 * ключей применяется восемь итераций сети Фейстеля, в каждой итерации 
 * используется константа, которая вычисляется путем применения 
 * линейного преобразования алгоритма к значению номера итерации.
 * 
 *************************************************************/
void kuz_set_round_key(kuz_key_t *round_key, const uint8_t master_key[32])
{
	int i, j;
	w128_t 
        c,       //итерационная константа
        x, y, z; //половинки Ki
	
    x.q[1]=0;
    y.q[0]=0;

	for (i = 0; i < 16; i++) {
	//TODO Здесь не учтёны системы с прямым порядком байтов (little-endian)
		x.b[i] = master_key[i];
		y.b[i] = master_key[i + 16];
	}

	round_key->k[0].q[0] = x.q[0];
	round_key->k[0].q[1] = x.q[1];

	round_key->k[1].q[0] = y.q[0];
	round_key->k[1].q[1] = y.q[1];

	for (i = 1; i <= 32; i++) {

		//1. рассчёт итерационной константы Ci
        //Итерационные константы Ci получаются с помощью 
        //L-преобразования порядкового номера итерации.
		c.q[0] = 0;
		c.q[1] = 0;
		c.b[15] = i;		// load round in lsb :: раунд в последний байт
		kuz_l(&c);
		
        
        // 2.функция шифрования z=F(x,Ci) 

        //Операция наложения или побитовый XOR
        z.q[0] = x.q[0] ^ c.q[0];
        z.q[1] = x.q[1] ^ c.q[1];
        //S-преобразование
        for (j = 0; j < 16; j++)
            z.b[j] = kuz_pi[z.b[j]];
        //L-преобразование
        kuz_l(&z);

        //3.Сеть Фейстеля
        // K1 -> F(K1,C1) -> K2 -> F(K2,C2) ... и т.д.
		z.q[0] ^= y.q[0];
		z.q[1] ^= y.q[1];
		
		y.q[0] = x.q[0];
		y.q[1] = x.q[1];

		x.q[0] = z.q[0];
		x.q[1] = z.q[1];
        
        //постепенно записываем 10 ключей
		if ((i & 7) == 0) {
			round_key->k[(i >> 2)].q[0] = x.q[0];
			round_key->k[(i >> 2)].q[1] = x.q[1];
			round_key->k[(i >> 2) + 1].q[0] = y.q[0];
			round_key->k[(i >> 2) + 1].q[1] = y.q[1];
		}
	}
}


void kuz_set_encrypt_key(kuz_key_t *round_key, const uint8_t master_key[32])
{
    kuz_set_round_key(round_key, master_key);
}

void kuz_set_decrypt_key(kuz_key_t *round_key, const uint8_t master_key[32])
{
	kuz_set_round_key(round_key, master_key);
}


// encrypt a block - 8 bit way

void kuz_encrypt_block(kuz_key_t *round_key, void *blk)
{
	int i, j;
	w128_t x;

	x.q[0] = ((uint64_t *) blk)[0]; //левая часть блока
	x.q[1] = ((uint64_t *) blk)[1]; //правая часть блока

    //9 раундов К1...К9
	for (i = 0; i < 9; i++) {

        //Операция наложения раундового ключа или побитовый XOR ключа и входного блока данных
		x.q[0] ^= round_key->k[i].q[0];   //левая половина
		x.q[1] ^= round_key->k[i].q[1];   //правая половина
        

        //Нелинейное преобразование, которое представляет собой 
        //простую замену одного байта на другой в соответствии с таблицей;
		for (j = 0; j < 16; j++){
			x.b[j] = kuz_pi[x.b[j]];
        }
        //линейное преобразование
		kuz_l(&x);
	}

    //К10
    //Операция наложения раундового ключа или побитовый XOR ключа и входного блока данных
	((uint64_t *) blk)[0] = x.q[0] ^ round_key->k[9].q[0];
	((uint64_t *) blk)[1] = x.q[1] ^ round_key->k[9].q[1];
}

// decrypt a block - 8 bit way

void kuz_decrypt_block(kuz_key_t *key, void *blk)
{
	int i, j;
	w128_t x;

	x.q[0] = ((uint64_t *) blk)[0] ^ key->k[9].q[0];
	x.q[1] = ((uint64_t *) blk)[1] ^ key->k[9].q[1];

	for (i = 8; i >= 0; i--) {
	
		kuz_l_inv(&x);
		for (j = 0; j < 16; j++)
			x.b[j] = kuz_pi_inv[x.b[j]];
	
		x.q[0] ^= key->k[i].q[0];	
		x.q[1] ^= key->k[i].q[1];
	}
	((uint64_t *) blk)[0] = x.q[0];
	((uint64_t *) blk)[1] = x.q[1];
}

#else

// kuznechik_128bit.c
// 04-Jan-15  Markku-Juhani O. Saarinen <mjos@iki.fi>

// Basic 128-bit version without platform - specific optimizations

// Conforms to included doc/GOSTR-bsh.pdf file, which has an internal
// date of September 2, 2014.




#include <mmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>


// lookup tables initialized by kuz_init()

static int kuz_initialized = 0;
static __m128i kuz_pil_enc128[16][256];
static __m128i kuz_l_dec128[16][256];
static __m128i kuz_pil_dec128[16][256];

// poly multiplication mod p(x) = x^8 + x^7 + x^6 + x + 1

static uint8_t kuz_mul_gf256(uint8_t x, uint8_t y)
{
	uint8_t z;
	
	z = 0;
	while (y) {		
		if (y & 1)
			z ^= x;
		x = (x << 1) ^ (x & 0x80 ? 0xC3 : 0x00);
		y >>= 1;
	}
		
	return z;
}

// (slow) linear operation l

static void kuz_l(w128_t *w)
{
	int i, j;
	uint8_t x;
	
	// 16 rounds
	for (j = 0; j < 16; j++) {

		// An LFSR with 16 elements from GF(2^8)
		x = w->b[15];	// since lvec[15] = 1

		for (i = 14; i >= 0; i--) {
			w->b[i + 1] = w->b[i];
			x ^= kuz_mul_gf256(w->b[i], kuz_lvec[i]);
		}
		w->b[0] = x;
	}
}

// inverse of linear operation l

static void kuz_l_inv(w128_t *w)
{
	int i, j;
	uint8_t x;
	
	// 16 rounds
	for (j = 0; j < 16; j++) {

		x = w->b[0];
		for (i = 0; i < 15; i++) {
			w->b[i] = w->b[i + 1];	
			x ^= kuz_mul_gf256(w->b[i], kuz_lvec[i]);
		}
		w->b[15] = x;
	}
}

// key setup

void kuz_set_encrypt_key(kuz_key_t *kuz, const uint8_t key[32])
{
	int i, j;
	w128_t c, x, y, z;
		
	if (!kuz_initialized)
		kuz_init();
		
	for (i = 0; i < 16; i++) {
		// this will be have to changed for little-endian systems
		x.b[i] = key[i];
		y.b[i] = key[i + 16];
	}

	kuz->k[0].q[0] = x.q[0];
	kuz->k[0].q[1] = x.q[1];
	kuz->k[1].q[0] = y.q[0];
	kuz->k[1].q[1] = y.q[1];

	for (i = 1; i <= 32; i++) {

		// C Value
		c.q[0] = 0;
		c.q[1] = 0;
		c.b[15] = i;		// load round in lsb
		kuz_l(&c);
		
		z.q[0] = x.q[0] ^ c.q[0];
		z.q[1] = x.q[1] ^ c.q[1];
		for (j = 0; j < 16; j++)
			z.b[j] = kuz_pi[z.b[j]];
		kuz_l(&z);
		z.q[0] ^= y.q[0];
		z.q[1] ^= y.q[1];
		
		y.q[0] = x.q[0];
		y.q[1] = x.q[1];

		x.q[0] = z.q[0];
		x.q[1] = z.q[1];

		if ((i & 7) == 0) {
			kuz->k[(i >> 2)].q[0] = x.q[0];
			kuz->k[(i >> 2)].q[1] = x.q[1];
			kuz->k[(i >> 2) + 1].q[0] = y.q[0];
			kuz->k[(i >> 2) + 1].q[1] = y.q[1];
		}
	}
}

// these two funcs are identical in the simple implementation

void kuz_set_decrypt_key(kuz_key_t *kuz, const uint8_t key[32])
{
	int i;

	kuz_set_encrypt_key(kuz, key);

	for (i = 1; i < 10; i++)
		kuz_l_inv(&kuz->k[i]);
}

// encrypt a block - 128 bit way

void kuz_encrypt_block(kuz_key_t *key, void *blk)
{
	int i;
	__m128i x;

	x = *((__m128i *) blk);

	for (i = 0; i < 9; i++) {
		x ^= *((__m128i *) &key->k[i]);
		x = kuz_pil_enc128[ 0][((uint8_t *) &x)[ 0]] ^
			kuz_pil_enc128[ 1][((uint8_t *) &x)[ 1]] ^
			kuz_pil_enc128[ 2][((uint8_t *) &x)[ 2]] ^
			kuz_pil_enc128[ 3][((uint8_t *) &x)[ 3]] ^
			kuz_pil_enc128[ 4][((uint8_t *) &x)[ 4]] ^
			kuz_pil_enc128[ 5][((uint8_t *) &x)[ 5]] ^
			kuz_pil_enc128[ 6][((uint8_t *) &x)[ 6]] ^
			kuz_pil_enc128[ 7][((uint8_t *) &x)[ 7]] ^
			kuz_pil_enc128[ 8][((uint8_t *) &x)[ 8]] ^
			kuz_pil_enc128[ 9][((uint8_t *) &x)[ 9]] ^
			kuz_pil_enc128[10][((uint8_t *) &x)[10]] ^
			kuz_pil_enc128[11][((uint8_t *) &x)[11]] ^
			kuz_pil_enc128[12][((uint8_t *) &x)[12]] ^
			kuz_pil_enc128[13][((uint8_t *) &x)[13]] ^
			kuz_pil_enc128[14][((uint8_t *) &x)[14]] ^
			kuz_pil_enc128[15][((uint8_t *) &x)[15]];
			
	}
	x ^= *((__m128i *) &key->k[9]);

	*((__m128i *) blk) = x;
}

// decrypt a block - 128 bit way

void kuz_decrypt_block(kuz_key_t *key, void *blk)
{
	int i, j;
	__m128i x;

	x = *((__m128i *) blk);
	
	x = kuz_l_dec128[ 0][((uint8_t *) &x)[ 0]] ^
		kuz_l_dec128[ 1][((uint8_t *) &x)[ 1]] ^
		kuz_l_dec128[ 2][((uint8_t *) &x)[ 2]] ^
		kuz_l_dec128[ 3][((uint8_t *) &x)[ 3]] ^
		kuz_l_dec128[ 4][((uint8_t *) &x)[ 4]] ^
		kuz_l_dec128[ 5][((uint8_t *) &x)[ 5]] ^
		kuz_l_dec128[ 6][((uint8_t *) &x)[ 6]] ^
		kuz_l_dec128[ 7][((uint8_t *) &x)[ 7]] ^
		kuz_l_dec128[ 8][((uint8_t *) &x)[ 8]] ^
		kuz_l_dec128[ 9][((uint8_t *) &x)[ 9]] ^
		kuz_l_dec128[10][((uint8_t *) &x)[10]] ^
		kuz_l_dec128[11][((uint8_t *) &x)[11]] ^
		kuz_l_dec128[12][((uint8_t *) &x)[12]] ^
		kuz_l_dec128[13][((uint8_t *) &x)[13]] ^
		kuz_l_dec128[14][((uint8_t *) &x)[14]] ^
		kuz_l_dec128[15][((uint8_t *) &x)[15]];

	for (i = 9; i > 1; i--) {
		x ^= *((__m128i *) &key->k[i]);
		x = kuz_pil_dec128[ 0][((uint8_t *) &x)[ 0]] ^
			kuz_pil_dec128[ 1][((uint8_t *) &x)[ 1]] ^
			kuz_pil_dec128[ 2][((uint8_t *) &x)[ 2]] ^
			kuz_pil_dec128[ 3][((uint8_t *) &x)[ 3]] ^
			kuz_pil_dec128[ 4][((uint8_t *) &x)[ 4]] ^
			kuz_pil_dec128[ 5][((uint8_t *) &x)[ 5]] ^
			kuz_pil_dec128[ 6][((uint8_t *) &x)[ 6]] ^
			kuz_pil_dec128[ 7][((uint8_t *) &x)[ 7]] ^
			kuz_pil_dec128[ 8][((uint8_t *) &x)[ 8]] ^
			kuz_pil_dec128[ 9][((uint8_t *) &x)[ 9]] ^
			kuz_pil_dec128[10][((uint8_t *) &x)[10]] ^
			kuz_pil_dec128[11][((uint8_t *) &x)[11]] ^
			kuz_pil_dec128[12][((uint8_t *) &x)[12]] ^
			kuz_pil_dec128[13][((uint8_t *) &x)[13]] ^
			kuz_pil_dec128[14][((uint8_t *) &x)[14]] ^
			kuz_pil_dec128[15][((uint8_t *) &x)[15]];
	}
	x ^= *((__m128i *) &key->k[1]);
	for (j = 0; j < 16; j++)
		((uint8_t *) &x)[j] = kuz_pi_inv[((uint8_t *) &x)[j]];	
	x ^= *((__m128i *) &key->k[0]);

	*((__m128i *) blk) = x;
}

// initalize lookup tables

void kuz_init()
{
	int i, j;
	w128_t x;

	for (i = 0; i < 16; i++) {
		for (j = 0; j < 256; j++) {
			x.q[0] = 0;
			x.q[1] = 0;
			x.b[i] = kuz_pi[j];
			kuz_l(&x);
			kuz_pil_enc128[i][j] = *((__m128i *) &x);
			
			x.q[0] = 0;
			x.q[1] = 0;
			x.b[i] = j; // kuz_pi[j];
			kuz_l_inv(&x);
			kuz_l_dec128[i][j] = *((__m128i *) &x);			

			x.q[0] = 0;
			x.q[1] = 0;
			x.b[i] = kuz_pi_inv[j];
			kuz_l_inv(&x);
			kuz_pil_dec128[i][j] = *((__m128i *) &x);			
		}
	}
	kuz_initialized = 1;
}
#endif

//=======================================================================
#include <stdlib.h>
#include <string.h>

size_t 
solve_count_blocks(size_t n)
{
    size_t n_blocks=n/16;
    n_blocks=(n%16)?n_blocks+1:n_blocks;
    return n_blocks;
}

static w128_t* 
set_memory_alloc_for_Y(void *x, size_t n_blocks, size_t n)
{
    w128_t* y=malloc(n_blocks*sizeof(w128_t));
    memset(y,0,n_blocks*sizeof(w128_t));
    memcpy(y,x,n_blocks*sizeof(w128_t));
    return y;
}

void*
kuz_encrypt_ECB(const w256_t key,void *x, size_t n)
{
    //считаем сколько нужно блоков
    size_t n_blocks=solve_count_blocks(n);

    w128_t block;
    kuz_key_t round_key;

    kuz_set_encrypt_key(&round_key, key.b);

    // w128_t* y=malloc(n_blocks*sizeof(w128_t));
    // memset(y,0,n_blocks*sizeof(w128_t));
    // memcpy(y,x,n);
    w128_t* y=set_memory_alloc_for_Y(x, n_blocks, n);

    for (size_t i = 0; i < n_blocks; i++)
    {
        kuz_encrypt_block(&round_key, &y[i]);
    }
    return y;
    
}

void* 
kuz_decrypt_ECB(const w256_t key,void *x, size_t n)
{
    //считаем сколько нужно блоков
    size_t n_blocks=solve_count_blocks(n);

    w128_t block;
    kuz_key_t round_key;
    
    //готовим память под расшифрованный текст
    // w128_t* y=malloc(n_blocks*sizeof(w128_t));
    // memset(y,0,n_blocks*sizeof(w128_t));
    // memcpy(y,x,n_blocks*sizeof(w128_t));
    w128_t* y=set_memory_alloc_for_Y(x, n_blocks, n_blocks);

    //инициализируем ключ
    kuz_set_decrypt_key(&round_key, key.b);  

    //расшифровываем последовательно блоки
    for (size_t i = 0; i < n_blocks; i++)
    {
        kuz_decrypt_block(&round_key, &y[i]);
    }
    return y;
}




//блок текст XOR вектор -> шифрованый блок, который является вектором для следующего
void* 
kuz_encrypt_CBC(const w256_t key,const w128_t Vec,void *x,size_t n)
{
    w128_t vec;
    vec.q[0]=Vec.q[0];
    vec.q[1]=Vec.q[1];

    size_t n_blocks=solve_count_blocks(n);
    
    w128_t* Y=set_memory_alloc_for_Y(x, n_blocks, n);

    kuz_key_t round_key;
    kuz_set_encrypt_key(&round_key, key.b);


    //пробегаем последовательно блоки
    for (size_t i = 0; i < n_blocks; i++)
    {
        Y[i].q[0]^=vec.q[0];
        Y[i].q[1]^=vec.q[1];

        kuz_encrypt_block(&round_key, &Y[i]);

        vec.q[0]=Y[i].q[0];
        vec.q[1]=Y[i].q[1];
    }
    
    return Y;

}
//шифрованый блок, который является вектором для следующего ->расшифровка блока XOR вектор = текстовый блок
void* 
kuz_decrypt_CBC(const w256_t key,const w128_t Vec,void *x, size_t n)
{
    w128_t vec;
    w128_t vec1;
    vec.q[0]=Vec.q[0];
    vec.q[1]=Vec.q[1];

    size_t n_blocks=solve_count_blocks(n);
    
    w128_t* Y=set_memory_alloc_for_Y(x, n_blocks, n_blocks);

    kuz_key_t round_key;
    kuz_set_decrypt_key(&round_key, key.b);


    //пробегаем последовательно блоки
    for (size_t i = 0; i < n_blocks; i++)
    {
        //шифрованый блок является вектором для следующего
        vec1.q[0]=Y[i].q[0];
        vec1.q[1]=Y[i].q[1];

        //расшифровка блока
        kuz_decrypt_block(&round_key, &Y[i]);

        //XOR вектор
        Y[i].q[0]^=vec.q[0];
        Y[i].q[1]^=vec.q[1];

        vec.q[0]=vec1.q[0];
        vec.q[1]=vec1.q[1];

    }
    return Y;
}




#include <u_application/u_dbg.h>

// const char* msg="Съешь ещё этих мягких французских булочек да выпей чаю\0";


void kuz_test2()
{
    char* msg="Съешь ещё этих мягких французских булочек да выпей чаю";
    // char* msg="Asigned floor bloom";
    
    w256_t key;
    key.q[0]=0xcafebabedeadf00d;
    key.q[1]=0xcafebabedeadf00d;
    key.q[2]=0xa11ec0decafef00d;
    key.q[3]=0xcafe1984deadf00d;
	
    w128_t Vec;
    Vec.q[0]=0x0;
    Vec.q[1]=0x0;
    Vec.q[0]=0x0123456789abcdef;
    Vec.q[1]=0xfedcba9876543210;

    u_msg("test CBC");
    u_msg(msg);
    // u_print_hexdump(msg, strlen(msg));
    void * crypted=kuz_encrypt_CBC(key,Vec, msg, strlen(msg));
    u_msg("::hexdump::");
    u_print_hexdump(crypted, strlen(msg));
    // char* uncrypted=kuz_decrypt_ECB(key,crypted, strlen(msg));
    char* uncrypted=kuz_decrypt_CBC(key,Vec,crypted, strlen(msg));
    u_msg(uncrypted);
    // u_msg("::hexdump unc::");
    // u_print_hexdump(uncrypted, strlen(msg));
    free(crypted);
    free(uncrypted);
    

}

void kuz_test1()
{
    char* msg="Съешь ещё этих мягких французских булочек да выпей чаю";
    // char* msg="Asigned floor bloom";
    
    w256_t key;
    key.q[0]=0xcafebabedeadf00d;
    key.q[1]=0xcafebabedeadf00d;
    key.q[2]=0xa11ec0decafef00d;
    key.q[3]=0xcafe1984deadf00d;

    u_msg("test ECB");
    u_msg(msg);
    void * crypted=kuz_encrypt_ECB(key,msg, strlen(msg));
    u_msg("::hexdump::");
    u_print_hexdump(crypted, strlen(msg));
    char* uncrypted=kuz_decrypt_ECB(key,crypted, strlen(msg));
    u_msg(uncrypted);
    free(crypted);
    free(uncrypted);
    

}



void kuz_test()
{
    
    kuz_test1();
    kuz_test2();

}










