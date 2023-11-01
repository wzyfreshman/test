#ifndef EPP_H
#define EPP_H


int readChipId(unsigned char *chipid);
int writeEncryptInfo(unsigned char *info,unsigned short addr,
                     int size,int num);
void readEncryptInfo(unsigned short addrin,unsigned char *store,  int count) ;
int initEpp(unsigned char chipid[8]);
int LKT4200_reset(unsigned char rstinfo[8]);
int readTime(unsigned char *time);

int writeRealData(unsigned short addrin,unsigned char *data,int size) ;
void readRealData(unsigned short addrin,unsigned char *data,int size);

int LK_ReadAddr(unsigned char addr,\
                unsigned char *ResBuf,\
                int *LenOfRes) ;
#endif // EPP_H
