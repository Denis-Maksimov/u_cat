// kuznechik.c
// 04-Jan-15  Markku-Juhani O. Saarinen <mjos@iki.fi>

// main() for testing

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "kuznechik.h"

void print_w128(w128_t *x)
{
	int i;
	
	for (i = 0; i < 16; i++)
		printf(" %02X", x->b[i]);
	printf("\n");
}


int main(int argc, char **argv)
{	
    kuz_key_t key;
    w128_t x={0};
    w128_t y={0};
    memcpy(&x,"hello",5);
    print_w128(&x);

    const uint8_t testvec_key[32] = {
		0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
		0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10, 
		0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF	
	};

	kuz_init();
	kuz_set_encrypt_key(&key, testvec_key);
	kuz_encrypt_block(&key, &x);

    kuz_set_decrypt_key(&key, testvec_key);
    kuz_decrypt_block(&key, &x);
    // print_w128(&y);
	print_w128(&y);
	
	return 0;
}
