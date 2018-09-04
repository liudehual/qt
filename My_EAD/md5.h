
#ifndef _MD5_H_
#define _MD5_H_

typedef unsigned long  UInt32;
typedef unsigned char  UInt8;

#ifdef __cplusplus
extern "C" {
#endif

/* MD5 context. */
typedef struct {
  UInt32 state[4];                                   /* state (ABCD) */
  UInt32 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

void MD5_Init(MD5_CTX *context);
void MD5_Update(MD5_CTX *context, unsigned char *input, unsigned int inputLen);
void MD5_Final(unsigned char digest[16], MD5_CTX *context);

#ifdef __cplusplus
}
#endif

#endif






