#ifndef UDF_H
#define UDF_H

#include "customer.h"

void intToChar4(unsigned int from, unsigned char *to);
int  charToInt4(unsigned char *from);
void intToChar2(unsigned int from, unsigned char *to);
int  charToInt2(unsigned char *from);

void setSysNum(unsigned char sysnum[4]);
void setVerNum(unsigned char vernum[4]);
void setInfoNum(unsigned char vernum[4]);
void infoEncryPt(unsigned char encryinfo[8] , unsigned char random) ;
bool infoDecryPt(unsigned char encryinfo[8],unsigned char out[4]);

unsigned int base36ToDecimal(char from[]);
void toBaseWhatever(unsigned int num, char to[] ,int rad);

#endif // UDF_H
