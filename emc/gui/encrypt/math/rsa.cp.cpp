#include<stdio.h>
#include<stdlib.h>
#include<string.h>

extern "C" {

#include<openssl/crypto.h>
#include<openssl/ossl_typ.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#include<openssl/bn.h>

}

#define OPENSSLKEY "/home/mhc/meng.key"
#define PUBLICKEY "/home/mhc/public.key"
#define BUFFSIZE 1024

char* my_encrypt(char *str,char *path_key);//加密
char* my_decrypt(char *str,char *path_key);//解密

int main(void){

    char *source="A";
    char *ptr_en,*ptr_de;

    printf("source is    :%s\n",source);
    ptr_en=my_encrypt(source,PUBLICKEY);

    printf("after encrypt:\n");
    int len=strlen(ptr_en);
    for(int i=0;i<len;i++) {
        printf("%x",ptr_en[i]);
    }
    printf("\n");

    ptr_de=my_decrypt(ptr_en,OPENSSLKEY);
    printf("after decrypt:%s\n",ptr_de);
    if(ptr_en!=NULL){
        free(ptr_en);
    }
    if(ptr_de!=NULL){
        free(ptr_de);
    }
    return 0;
}


char *my_encrypt(char *str,char *path_key) {
    char *p_en;
    RSA *p_rsa;
    FILE *file;
    int flen,rsa_len;
    if((file=fopen(path_key,"r"))==NULL){
        perror("open key file error");
        return NULL;
    }
    if((p_rsa=PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL))==NULL){
    //if((p_rsa=PEM_read_RSAPublicKey(file,NULL,NULL,NULL))==NULL){   换成这句死活通不过，无论是否将公钥分离源文件
        ERR_print_errors_fp(stdout);
        return NULL;
    }

    //struct bignum_st *nk,*dk,*ek;
    //nk = (bignum_st*)p_rsa->n;
    //ek = (bignum_st*)p_rsa->e;
    //dk = (bignum_st*)p_rsa->d;

    //int a= nk->top;
    //int b= ek->top;
    //int c= nk->dmax;

    flen=strlen(str);
    rsa_len=RSA_size(p_rsa);
    p_en=(char *)malloc(rsa_len+1);
    memset(p_en,0,rsa_len+1);
    if(RSA_public_encrypt(rsa_len,(unsigned char *)str,(unsigned char*)p_en,p_rsa,RSA_NO_PADDING)<0){
        return NULL;
    }
    /*
    struct bignum_st
        {
        BN_ULONG *d;	 Pointer to an array of 'BN_BITS2' bit chunks.
        int top;	 Index of last used d +1.
        // The next are internal book keeping for bn_expand.
        int dmax;	 Size of the d array.
        int neg;	 one if the number is negative
        int flags;
        };
    */


    RSA_free(p_rsa);
    fclose(file);
    return p_en;
}
char *my_decrypt(char *str,char *path_key){

    char *p_de;
    RSA *p_rsa;
    FILE *file;
    int rsa_len;

    if((file=fopen(path_key,"r"))==NULL){
        perror("open key file error");
        return NULL;
    }
    if((p_rsa=PEM_read_RSAPrivateKey(file,NULL,NULL,NULL))==NULL){
        ERR_print_errors_fp(stdout);
        return NULL;
    }

    //struct bignum_st *nk,*dk,*ek;
    //nk = (bignum_st*)p_rsa->n;
    //ek = (bignum_st*)p_rsa->e;
    //dk = (bignum_st*)p_rsa->d;
    //int a= nk->top;
    //int b= dk->top;
    //int c= nk->dmax;


    rsa_len=RSA_size(p_rsa);
    p_de=(char *)malloc(rsa_len+1);
    memset(p_de,0,rsa_len+1);
    if(RSA_private_decrypt(rsa_len,(unsigned char *)str,(unsigned char*)p_de,p_rsa,RSA_NO_PADDING)<0){
        return NULL;
    }
    RSA_free(p_rsa);
    fclose(file);
    return p_de;
}
