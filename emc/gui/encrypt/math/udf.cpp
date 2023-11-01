#include "udf.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//static unsigned char infokey[8] = {'e','n','h','i','R','W','F','#'};

//unsigned char __CURRENT_CUSTOMER[8] = {'1','2','3','4','r','t','y','6'};

static unsigned char _sysnum[4];
static unsigned char _vernum[4];
static unsigned char _info[4];


void setSysNum(unsigned char sysnum[4]) {
    for(int i = 0;i<4;i++)
        _sysnum[i] = sysnum[i];
}
void setVerNum(unsigned char vernum[4]) {
    for(int i = 0;i<4;i++)
        _vernum[i] = vernum[i];
}
void setInfoNum(unsigned char infonum[4]) {
    for(int i = 0;i<4;i++)
        _info[i] = infonum[i];
}


int charToInt2(unsigned char *from) {

    int tmp = 0;
    int tmp1 = (int)(*from);
    int tmp2 = (int)(*(from+1));
    //printf("in char 2 int %d %d\n",tmp1,tmp2);

    tmp|= (tmp1<<8);
    tmp|= (tmp2);

    return tmp;

}

static int charToInt3(unsigned char *from) {

    int tmp = 0;
    int tmp1 = (int)(*from);
    int tmp2 = (int)(*(from+1));
    int tmp3 = (int)(*(from+2));

    tmp|= (tmp1<<16);
    tmp|= (tmp2<<8);
    tmp|= (tmp3);

    return tmp;

}

int  charToInt4(unsigned char *from) {

    int tmp = 0;
    int tmp1 = (int)(*from);
    int tmp2 = (int)(*(from+1));
    int tmp3 = (int)(*(from+2));
    int tmp4 = (int)(*(from+3));

    tmp|= (tmp1<<24);
    tmp|= (tmp2<<16);
    tmp|= (tmp3<<8);
    tmp|= (tmp4);

    return tmp;

}

void intToChar4(unsigned int from, unsigned char *to) {


    *to = (unsigned char)(from>>24);
    *(to+1) = (unsigned char)(from>>16);
    *(to+2) = (unsigned char)(from>>8);
    *(to+3) = (unsigned char)(from);

}

 void intToChar2(unsigned int from, unsigned char *to) {


    *to = (unsigned char)(from>>8);
    *(to+1) = (unsigned char)(from);

}

void infoEncryPt(unsigned char encryinfo[8] , unsigned char random) {


    unsigned char tmp;
    unsigned char sum_ver_info[4];


    for(int i = 0;i<4;i++) {
        sum_ver_info[i] = (_sysnum[i]^(_vernum[i]+1));
    }

    tmp = _info[0];
    _info[0] = _info[3]<<4;
    _info[0] |= _info[3]>>4;
    _info[3] = tmp<<4;
    _info[3] |= tmp>>4;


    unsigned int combinfo = charToInt4(_info);
    unsigned int combver = charToInt4(_vernum);
    unsigned int add = combinfo + combver;

    intToChar4(add,_info);

    for(int i = 0;i<4;i++) {
        encryinfo[i] = (_info[i]^(_sysnum[i]));
    }

    //for(int i = 0;i<4;i++)
    //    printf("info[%d] = %x",i,_info[i]);
    //printf("\n");
    //for(int i = 0;i<4;i++)
    //    printf("sum_ver_info[%d] = %x",i,sum_ver_info[i]);
    //printf("\n");

    encryinfo[4] = _sysnum[0]^_vernum[3];

    for(int i = 0;i<5;i++)
        encryinfo[i] ^= random;

    encryinfo[5] = random;

    unsigned char forHash[6+8];
    for(int i =0;i<6;i++)
        forHash[i] = encryinfo[i];
    for(int i =0;i<4;i++)
        forHash[i+6] = _sysnum[i];
    for(int i =0;i<4;i++)
        forHash[i+10] = _vernum[i];

    calculateCrc16(forHash,&encryinfo[6],14);




}

bool infoDecryPt(unsigned char encryinfo[8],unsigned char _info[4]) {

    unsigned char random;
    unsigned char tmp;
    //unsigned char sum_ver_info[4];
    unsigned char aftercrc[2];

    unsigned char forHash[6+8];
    for(int i =0;i<6;i++)
        forHash[i] = encryinfo[i];
    for(int i =0;i<4;i++)
        forHash[i+6] = _sysnum[i];
    for(int i =0;i<4;i++)
        forHash[i+10] = _vernum[i];
    calculateCrc16(forHash,aftercrc,14);


    if(aftercrc[0]!=encryinfo[6] || aftercrc[1]!=encryinfo[7]) {
        return false;
    }

    random = encryinfo[5];
    for(int i = 0;i<5;i++)
        encryinfo[i] ^= random;


    tmp = _sysnum[0]^_vernum[3];
    if(tmp != encryinfo[4]) {
        return false;
    }


    for(int i = 0;i<4;i++)
        encryinfo[i] ^= _sysnum[i];

    unsigned int combinfo = charToInt4(encryinfo);
    unsigned int combver = charToInt4(_vernum);
    unsigned int sub = combinfo - combver;

    intToChar4(sub,_info);
    tmp = _info[0];
    _info[0] = _info[3]<<4;
    _info[0] |= _info[3]>>4;
    _info[3] = tmp<<4;
    _info[3] |= tmp>>4;

    return true;


}


static unsigned char verkey[8] = {'w','2','d','v','q','W','F','#'};

void verKeyEncryPt(unsigned char in[4],unsigned char out[4] ) {
    for(int i = 0;i<4;i++) {
        out[i] = (in[i]^verkey[i]);
    }

}

void verKeyDecryPt(unsigned char in[4],unsigned char out[4] ) {

    for(int i = 0;i<4;i++) {
        out[i] = (in[i]^verkey[i]);
    }
}



void toBaseWhatever(unsigned int num, char to[] ,int rad) {

    char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;
    char buf[66];   /* enough space for any 64-bit in base 2 */

    /* bounds check for radix */
    if (rad < 2 || rad > 36)
        return ;
    /* if num is zero */
    if (!num)
        return ;

    buf[65] = '\0';
    i = 65;

    if (num > 0) {  /* if positive... */
        while (num) { /* until num is 0... */
            /* go left 1 digit, divide by radix, and set digit to remainder */
            buf[--i] = digits[num % rad];
            num /= rad;
        }
    } else {    /* same for negative, but negate the modulus and prefix a '-' */
        while (num) {
            buf[--i] = digits[-(num % rad)];
            num /= rad;
        }
        buf[--i] = '-';
    }

    strcpy(to,buf+i);

}

unsigned int base36ToDecimal(char from[]) {

    //static char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned int tmp = 0;
    int i=0;
    int len = strlen(from);
    if(len == 0) return 0;
    int pownum = len-1;
    int powunder = 0;

    while(from[i] != '\0') {

        if( from[i] >= '0' && from[i] <= '9' ) {
            powunder = from[i] - '0';
        } else if( from[i] >= 'A' && from[i] <= 'Z' ) {
            powunder = from[i] - 'A';
            powunder += 10;
        } else if( from[i] >= 'a' && from[i] <= 'z' ) {
            powunder = from[i] - 'a';
            powunder += 10;
        } else {
            powunder = 0;
        }

        tmp += ( pow(36,pownum) * powunder);
        pownum --;
        i++;

    }
    return tmp;


}





