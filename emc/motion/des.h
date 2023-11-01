#ifndef _DES_H
#define _DES_H

#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
#define uint32 unsigned long int
#endif

typedef struct
{
    uint32 esk[32];     /* DES encryption subkeys */
    uint32 dsk[32];     /* DES decryption subkeys */
}
des_context;

typedef struct
{
    uint32 esk[96];     /* Triple-DES encryption subkeys */
    uint32 dsk[96];     /* Triple-DES decryption subkeys */
}
des3_context;

/*int  des_set_key( des_context *ctx, uint8 key[8] );
void des_encrypt( des_context *ctx, uint8 input[8], uint8 output[8] );
void des_decrypt( des_context *ctx, uint8 input[8], uint8 output[8] );

int  des3_set_2keys( des3_context *ctx, uint8 key1[8], uint8 key2[8] );
int  des3_set_3keys( des3_context *ctx, uint8 key1[8], uint8 key2[8],
                                        uint8 key3[8] );

void des3_encrypt( des3_context *ctx, uint8 input[8], uint8 output[8] );
void des3_decrypt( des3_context *ctx, uint8 input[8], uint8 output[8] );*/

int  hal_ppmc_1( des_context *ctx, uint8 key[8] );
void hal_ppmc_2( des_context *ctx, uint8 input[8], uint8 output[8] );
void hal_ppmc_3( des_context *ctx, uint8 input[8], uint8 output[8] );

int  hal_ppmc_4( des3_context *ctx, uint8 key1[8], uint8 key2[8] );
int  hal_ppmc_5( des3_context *ctx, uint8 key1[8], uint8 key2[8],
                                        uint8 key3[8] );

void hal_ppmc_6( des3_context *ctx, uint8 input[8], uint8 output[8] );
void hal_ppmc_7( des3_context *ctx, uint8 input[8], uint8 output[8] );

int sendLKcmd(unsigned char *CMD,\
              int LenOfCmd,\
              unsigned char *ResBuf,\
              int *LenOfRes);   //write the LK_Chip function
int LK_ReadAddr(unsigned char addr, \
                unsigned char *ResBuf, \
                int *LenOfRes) ;    //READ THE LK_CHIP ADDR DATA
#endif /* des.h */
