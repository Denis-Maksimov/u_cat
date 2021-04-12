#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#include "aes.h"
//------------------------------------------------------------------------------
// hexdump, a very nice function, it's not mine.
// I found it on the net somewhere some time ago... thanks to the author ;-)
//------------------------------------------------------------------------------
#ifndef HEXDUMP_COLS
#define HEXDUMP_COLS 16
#endif
void hexdump(void *mem, unsigned int len)
{
        unsigned int i, j;
        for(i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
        {
                /* print offset */
                if(i % HEXDUMP_COLS == 0){
                        printf("0x%04x: ", i);
                }

                /* print hex data */
                if(i < len) {
                        printf("%02x ", 0xFF & ((char*)mem)[i]);
                }
                else /* end of block, just aligning for ASCII dump */
                {
                        printf("   ");
                }

                /* print ASCII dump */
                if(i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
                {
                        for(j = i - (HEXDUMP_COLS - 1); j <= i; j++)
                        {
                                /* end of block, not really printing */
                                if(j >= len) {
                                        putchar(' ');

                                }else 
                                if(isprint((((char*)mem)[j] & 0x7F))) {
                                    /* printable char */
                                        putchar(0xFF & ((char*)mem)[j]);
                                } else {
                                    /* other char */
                                        putchar('.');
                                }
                        }
                        putchar('\n');
                }
        }
}


#include "kuznechik.h"
#include "sha256.h"
void print_w128(w128_t *x)
{
	int i;
	
	for (i = 0; i < 16; i++)
		printf(" %02X", x->b[i]);
	printf("\n");
}

void prepare_pass_phrase(BYTE* hash,const char* str)
{
    size_t n=strlen(str);
    SHA256_CTX ctx={0};
    sha256_init(&ctx);
    sha256_update(&ctx,str,n);
    sha256_final(&ctx,hash);
}
void encryptFile(const char* file,const char* pass){
    rename(file,"uncrypted.tmp");
    FILE* fd=fopen("./uncrypted.tmp","rb");
    FILE* fdc=fopen(file,"wb");
    w128_t x;
    kuz_key_t key;
    BYTE hash[32]={0};
   prepare_pass_phrase(hash, pass);
   kuz_set_encrypt_key(&key, hash);

    fseek(fd,0,SEEK_END);
    int n=ftell(fd);
    fseek(fd,0,SEEK_SET);
    
    fwrite(&n,sizeof(n),1,fdc);
     printf("size=%d\n",n);
    hexdump(hash,32);
    while(!feof(fd)){
        fread(x.b,sizeof(x.b),1,fd);
        kuz_encrypt_block(&key, &x);
        fwrite(x.b,sizeof(x.b),1,fdc);
    }
   fclose(fd);
   fclose(fdc);
   remove("./uncrypted.tmp");

}

void decryptFile(const char* file,const char* pass){
    rename(file,"crypted.tmp");
    FILE *fd = fopen("crypted.tmp", "rb");
    FILE *fdc = fopen(file, "wb");
    w128_t x;
    kuz_key_t key;
    BYTE hash[32] = {0};
    prepare_pass_phrase(hash, pass);
    kuz_set_decrypt_key(&key, hash);
    int n=0;
    fread(&n,sizeof(n),1,fd);
    printf("size=%d\n",n);
    while (!feof(fd))
    {
        fread(x.b, sizeof(x.b), 1, fd);
        
        kuz_decrypt_block(&key, &x);
        if(n<sizeof(x.b))
        {
            fwrite(x.b, n, 1, fdc); break;
        }else{
            fwrite(x.b, sizeof(x.b), 1, fdc);
        }
        n-=sizeof(x.b);
    }
    // fseek(fdc,n,SEEK_SET);
    // fwrite(x.b, sizeof(x.b), 1, fdc);
    fclose(fd);
    fclose(fdc);
    remove("./crypted.tmp");
}

// #include <sys/types.h>
// void encryptMsg(void* msg, ssize_t len, const char* pass){
    
//     w128_t x={0};
//     kuz_key_t key;
//     BYTE hash[32]={0};
//    prepare_pass_phrase(hash, pass);
//    kuz_set_encrypt_key(&key, hash);

//     while(len>sizeof(x.b)){
//         memcpy(x.b,msg,sizeof(x.b));
//         kuz_encrypt_block(&key, &x);
//         memcpy(msg,x.b,sizeof(x.b));
//         len-=sizeof(x.b);
//     }
//     if(len)fprintf(stderr,"must хэв кратно 8 байтам");


// }

int main(int argc, char const *argv[]){

    // printf("%s-\n",argv[1]);
    char file[256]={0};
    char key[256]={0};
    
    printf("enter file name:\n");
    fgets(file,256,stdin);
    printf("enter key:\n");
    fgets(key,256,stdin);
    file[strlen(file)-1]=0;
    key[strlen(key)-1]=0;
	kuz_init();

    encryptFile(file,key);
    // decryptFile(file,key);
    // encryptFile("./test.txt","Кузнечик");
    // decryptFile("./crypted.txt","Кузнечик");
    printf("processing is ok! Best regards!\n");
    printf("by Maksimov Denis\n");
    getc(stdin);
    return 0;
}



