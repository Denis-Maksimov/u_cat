#ifndef CRC16_H
#define CRC16_H

//для нахождения ошибок
extern unsigned short crc16(unsigned char * pcBlock, unsigned short len);
extern unsigned long crc32(unsigned long crc32, unsigned char * buf, ssize_t cnt);

//для хэш-таблиц
extern unsigned int PJWHash (void *data, size_t n, size_t table_size);



#endif // !CRC16_H
