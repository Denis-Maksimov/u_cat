/*********************
*  Кольцевой буффер  *
**********************/
// #include "../inc/u_stddef.h"
#include <u_stdlib/u_stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
void hexdump(void *mem, unsigned int len);
#define MAX_LEN_MSG 32

typedef struct 
{
    s8 buffer[MAX_LEN_MSG];
    s16 end_pos_at_end;
    s16 start_pos;
}u_ring;


void push_data(u_ring* ring, u8 c){
    if(ring->end_pos_at_end==0) {
        ring->end_pos_at_end=MAX_LEN_MSG;
        ring->buffer[MAX_LEN_MSG-ring->end_pos_at_end]=c;
        return;
    }
    
    ring->buffer[MAX_LEN_MSG-ring->end_pos_at_end]=c;
    ring->end_pos_at_end--;
    if(ring->end_pos_at_end<1) {ring->end_pos_at_end=MAX_LEN_MSG;}
    // (ring->end_pos_at_end<=1)?(ring->end_pos_at_end=MAX_LEN_MSG):(ring->end_pos_at_end--);
}

void write_data(u_ring* ring, u8* src, u16 n){
    for (size_t i = 0; i < n; i++)
    {
        push_data(ring, src[i]);
        // printf("push %d\n",i);
    }
    return;
    
}

u8 pop_data(u_ring* ring){
    s8 rv;
    if(ring->start_pos==(MAX_LEN_MSG-ring->end_pos_at_end)) 
    return 0;
    if (ring->start_pos<MAX_LEN_MSG){
      rv = ring->buffer[ring->start_pos];
      ring->start_pos++;
      if(ring->start_pos==MAX_LEN_MSG)ring->start_pos=0;
      return rv;
    }
    else{
        rv =ring->buffer[ring->start_pos];
        ring->start_pos=0;
      return rv;
      }

}

void read_data(u8* dest, u16 n, u_ring* ring){
    for (size_t i = 0; i < n; i++)
    {
       dest[i]=pop_data(ring);
    }
    
}


int main() {

    u_ring ring={
        .buffer="0123456789abcdef0123456789abcdef",
        .end_pos_at_end=25,
        .start_pos=1
    };
    u8 data[MAX_LEN_MSG];
    read_data(data,MAX_LEN_MSG,&ring);
    hexdump(data, MAX_LEN_MSG);
    hexdump(ring.buffer, MAX_LEN_MSG);

    write_data(&ring,"Message of fortune!",20);
    read_data(data,MAX_LEN_MSG,&ring);
    hexdump(data, MAX_LEN_MSG);
    hexdump(ring.buffer, MAX_LEN_MSG);
    
    write_data(&ring,"Double kill!",13);
    read_data(data,MAX_LEN_MSG,&ring);
    hexdump(data, MAX_LEN_MSG);
    // hexdump(ring.buffer, MAX_LEN_MSG);

    write_data(&ring,"Double kill!",13);
    read_data(data,MAX_LEN_MSG,&ring);
    hexdump(data, MAX_LEN_MSG);
    // hexdump(ring.buffer, MAX_LEN_MSG);

    write_data(&ring,"Double kill!",13);
    read_data(data,MAX_LEN_MSG,&ring);
    hexdump(data, MAX_LEN_MSG);
    // hexdump(ring.buffer, MAX_LEN_MSG);

    write_data(&ring,"Double kill!",13);
    read_data(data,MAX_LEN_MSG,&ring);
    hexdump(data, MAX_LEN_MSG);
    hexdump(ring.buffer, MAX_LEN_MSG);
    return 0;
}


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
                if(i % HEXDUMP_COLS == 0)
                {
                        printf("0x%04x: ", i);
                }

                /* print hex data */
                if(i < len)
                {
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
                                if(j >= len) /* end of block, not really printing */
                                {
                                        putchar(' ');
                                }
                                else if(isprint((((char*)mem)[j] & 0x7F))) /* printable char */
                                {
                                        putchar(0xFF & ((char*)mem)[j]);
                                }
                                else /* other char */
                                {
                                        putchar('.');
                                }
                        }
                        putchar('\n');
                }
        }
}
