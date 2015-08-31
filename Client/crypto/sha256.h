#ifndef HEADER_SHA256_H 
#define HEADER_SHA256_H 
 
#ifdef  __cplusplus 
extern "C" { 
#endif 
 
 
#if defined(OPENSSL_SYS_WIN16) || defined(__LP32__) 
#define SHA_LONG unsigned long 
#elif defined(OPENSSL_SYS_CRAY) || defined(__ILP64__) 
#define SHA_LONG unsigned long 
#define SHA_LONG_LOG2 3 
#else 
#define SHA_LONG unsigned int 
#endif 
 
#define SHA_LBLOCK	16 
#define SHA_CBLOCK	(SHA_LBLOCK*4)	/* SHA treats input data as a 
					 * contiguous array of 32 bit 
					 * wide big-endian values. */ 
#define SHA_LAST_BLOCK  (SHA_CBLOCK-8) 
#define SHA_DIGEST_LENGTH 20 
 
typedef struct SHAstate_st 
	{ 
	SHA_LONG h0,h1,h2,h3,h4; 
	SHA_LONG Nl,Nh; 
	SHA_LONG data[SHA_LBLOCK]; 
	unsigned int num; 
	} SHA_CTX; 
 
 
 
#define SHA256_CBLOCK	(SHA_LBLOCK*4)	/* SHA-256 treats input data as a 
					 * contiguous array of 32 bit 
					 * wide big-endian values. */ 
					  
#define SHA256_DIGEST_LENGTH	32 
 
typedef struct SHA256state_st 
	{ 
	SHA_LONG h[8]; 
	SHA_LONG Nl,Nh; 
	SHA_LONG data[SHA_LBLOCK]; 
	unsigned int num,md_len; 
	} SHA256_CTX; 
 
 
//int SHA256_Init(SHA256_CTX *c); 
//int SHA256_Update(SHA256_CTX *c, const void *data, size_t len); 
//int SHA256_Final(unsigned char *md, SHA256_CTX *c); 
unsigned char *SHA256(const unsigned char *d, size_t n,unsigned char *md); 
//void SHA256_Transform(SHA256_CTX *c, const unsigned char *data); 
#ifdef  __cplusplus 
} 
#endif 

#endif 
 
