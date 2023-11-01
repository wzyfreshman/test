#ifndef HASH_H
#define HASH_H

#include <stdint.h>
//#define uint64_t unsigned long long

void calculateMd5(unsigned char* out,unsigned char *in,int size);
void calculateCrc32(unsigned char *in,unsigned char *out, unsigned int size);
void calculateCrc16(unsigned char *in,unsigned char *out,unsigned short size );
void calculateCrc64(unsigned char *in,unsigned char *out,unsigned int size );


#endif // HASH_H
