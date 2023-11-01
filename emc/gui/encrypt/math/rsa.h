#ifndef RSA_H
#define RSA_H

#define OPENSSLKEY "/home/mhc/meng.key"
#define PUBLICKEY "/home/mhc/public.key"
#define BUFFSIZE 1024

char* my_encrypt(char *str,char *path_key);//加密
char* my_decrypt(char *str,char *path_key);//解密


#endif // RSA_H
