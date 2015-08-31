#include <stdlib.h>   
#include <string.h>   
#include <sha256.h>   
  

unsigned char cleanse_ctr = 0;   
#define HASH_LONG SHA_LONG   
   
#define Sigma0(x)   (ROTATE((x),30) ^ ROTATE((x),19) ^ ROTATE((x),10))   
#define Sigma1(x)   (ROTATE((x),26) ^ ROTATE((x),21) ^ ROTATE((x),7))   
#define sigma0(x)   (ROTATE((x),25) ^ ROTATE((x),14) ^ ((x)>>3))   
#define sigma1(x)   (ROTATE((x),15) ^ ROTATE((x),13) ^ ((x)>>10))   
  
  
#if 1  
  
#   define HOST_c2l(c,l)    ({ unsigned int r=*((const unsigned int *)(c)); \   
                   asm ("bswapl %0":"=r"(r):"0"(r));    \   
                   (c)+=4; (l)=r;           })   
#   define HOST_l2c(l,c)    ({ unsigned int r=(l);          \   
                   asm ("bswapl %0":"=r"(r):"0"(r));    \   
                   *((unsigned int *)(c))=r; (c)+=4; r; })  
  
#define ROTATE(a,n) ({ register unsigned int ret;   \  
                asm (           \  
                "roll %1,%0"        \  
                : "=r"(ret)     \  
                : "I"(n), "0"(a)    \  
                : "cc");        \  
               ret;             \  
            })  
#endif   
#define HASH_MAKE_STRING(c,s)   do {    \   
    unsigned long ll;       \   
    unsigned int  n;        \   
    switch ((c)->md_len)     \   
    {           \   
        case SHA256_DIGEST_LENGTH:  \   
        for (n=0;n<SHA256_DIGEST_LENGTH/4;n++)   \   
        {   ll=(c)->h[n]; HOST_l2c(ll,(s));   }  \   
        break;          \   
        default:            \   
        if ((c)->md_len > SHA256_DIGEST_LENGTH)   \   
            return 0;               \   
        for (n=0;n<(c)->md_len/4;n++)     \   
        {   ll=(c)->h[n]; HOST_l2c(ll,(s));   }  \   
        break;          \   
    }               \   
    } while (0)   
   
#define Ch(x,y,z)   (((x) & (y)) ^ ((~(x)) & (z)))   
#define Maj(x,y,z)  (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))   
static const SHA_LONG K256[64] = {   
    0x428a2f98UL,0x71374491UL,0xb5c0fbcfUL,0xe9b5dba5UL,   
    0x3956c25bUL,0x59f111f1UL,0x923f82a4UL,0xab1c5ed5UL,   
    0xd807aa98UL,0x12835b01UL,0x243185beUL,0x550c7dc3UL,   
    0x72be5d74UL,0x80deb1feUL,0x9bdc06a7UL,0xc19bf174UL,   
    0xe49b69c1UL,0xefbe4786UL,0x0fc19dc6UL,0x240ca1ccUL,   
    0x2de92c6fUL,0x4a7484aaUL,0x5cb0a9dcUL,0x76f988daUL,   
    0x983e5152UL,0xa831c66dUL,0xb00327c8UL,0xbf597fc7UL,   
    0xc6e00bf3UL,0xd5a79147UL,0x06ca6351UL,0x14292967UL,   
    0x27b70a85UL,0x2e1b2138UL,0x4d2c6dfcUL,0x53380d13UL,   
    0x650a7354UL,0x766a0abbUL,0x81c2c92eUL,0x92722c85UL,   
    0xa2bfe8a1UL,0xa81a664bUL,0xc24b8b70UL,0xc76c51a3UL,   
    0xd192e819UL,0xd6990624UL,0xf40e3585UL,0x106aa070UL,   
    0x19a4c116UL,0x1e376c08UL,0x2748774cUL,0x34b0bcb5UL,   
    0x391c0cb3UL,0x4ed8aa4aUL,0x5b9cca4fUL,0x682e6ff3UL,   
    0x748f82eeUL,0x78a5636fUL,0x84c87814UL,0x8cc70208UL,   
    0x90befffaUL,0xa4506cebUL,0xbef9a3f7UL,0xc67178f2UL };   
   
#ifndef PEDANTIC   
# if defined(__GNUC__) && __GNUC__>=2 && !defined(OPENSSL_NO_ASM) && !defined(OPENSSL_NO_INLINE_ASM)   
#  if ((defined(__i386) || defined(__i386__)) && !defined(I386_ONLY)) || \   
      (defined(__x86_64) || defined(__x86_64__))   
    /*  
     * This gives ~30-40% performance improvement in SHA-256 compiled  
     * with gcc [on P4]. Well, first macro to be frank. We can pull  
     * this trick on x86* platforms only, because these CPUs can fetch  
     * unaligned data without raising an exception.  
     */   
#   define HOST_c2l(c,l)    ({ unsigned int r=*((const unsigned int *)(c)); \   
                   asm ("bswapl %0":"=r"(r):"0"(r));    \   
                   (c)+=4; (l)=r;           })   
#   define HOST_l2c(l,c)    ({ unsigned int r=(l);          \   
                   asm ("bswapl %0":"=r"(r):"0"(r));    \   
                   *((unsigned int *)(c))=r; (c)+=4; r; })   
#  endif   
# endif   
#endif   
  
   
   
#define HOST_p_c2l(c,l,n)   {                   \   
            switch (n) {                    \   
            case 0: l =((unsigned long)(*((c)++)))<<24;   \   
            case 1: l|=((unsigned long)(*((c)++)))<<16;   \   
            case 2: l|=((unsigned long)(*((c)++)))<< 8;   \   
            case 3: l|=((unsigned long)(*((c)++)));     \   
                } }   
               
#ifndef HOST_c2l   
#define HOST_c2l(c,l)   (l =(((unsigned long)(*((c)++)))<<24),        \   
             l|=(((unsigned long)(*((c)++)))<<16),        \   
             l|=(((unsigned long)(*((c)++)))<< 8),        \   
             l|=(((unsigned long)(*((c)++)))    ),      \   
             l)   
#endif   
   
#define ROUND_00_15(i,a,b,c,d,e,f,g,h)      do {    \   
    T1 += h + Sigma1(e) + Ch(e,f,g) + K256[i];  \   
    h = Sigma0(a) + Maj(a,b,c);         \   
    d += T1;    h += T1;        } while (0)   
   
#define ROUND_16_63(i,a,b,c,d,e,f,g,h,X)    do {    \   
    s0 = X[(i+1)&0x0f]; s0 = sigma0(s0);    \   
    s1 = X[(i+14)&0x0f];    s1 = sigma1(s1);    \   
    T1 = X[(i)&0x0f] += s0 + s1 + X[(i+9)&0x0f];    \   
    ROUND_00_15(i,a,b,c,d,e,f,g,h);     } while (0)   
//#define   ROTATE(a,n) (_lrotr(a,n))   
//#define ROTATE(a,n)   __rol(a,n)   
#undef ROTATE   
#ifndef PEDANTIC   
# if defined(_MSC_VER) || defined(__ICC)   
#  define ROTATE(a,n)   _lrotl(a,n)   
# elif defined(__MWERKS__)   
#  if defined(__POWERPC__)   
#   define ROTATE(a,n)  __rlwinm(a,n,0,31)   
#  elif defined(__MC68K__)   
    /* Motorola specific tweak. <appro@fy.chalmers.se> */   
#   define ROTATE(a,n)  ( n<24 ? __rol(a,n) : __ror(a,32-n) )   
#  else   
#   define ROTATE(a,n)  __rol(a,n)   
#  endif   
# elif defined(__GNUC__) && __GNUC__>=2 && !defined(OPENSSL_NO_ASM) && !defined(OPENSSL_NO_INLINE_ASM)   
  /*  
   * Some GNU C inline assembler templates. Note that these are  
   * rotates by *constant* number of bits! But that's exactly  
   * what we need here...  
   *                    <appro@fy.chalmers.se>  
   */   
#  if defined(__i386) || defined(__i386__) || defined(__x86_64) || defined(__x86_64__)   
#   define ROTATE(a,n)  ({ register unsigned int ret;   \   
                asm (           \   
                "roll %1,%0"        \   
                : "=r"(ret)     \   
                : "I"(n), "0"(a)    \   
                : "cc");        \   
               ret;             \   
            })   
#  elif defined(__powerpc) || defined(__ppc__) || defined(__powerpc64__)   
#   define ROTATE(a,n)  ({ register unsigned int ret;   \   
                asm (           \   
                "rlwinm %0,%1,%2,0,31"  \   
                : "=r"(ret)     \   
                : "r"(a), "I"(n));  \   
               ret;             \   
            })   
#  endif   
# endif   
#endif /* PEDANTIC */   
   
#ifndef ROTATE   
#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))   
#endif   
   
#if 0   
static void sha256_block (SHA256_CTX *ctx, const void *in, size_t num, int host)   
    {   
    unsigned long a,b,c,d,e,f,g,h,s0,s1,T1,T2;   
    SHA_LONG    X[16];   
    int i;   
    const unsigned char *data=in;   
   
            while (num--) {   
   
    a = ctx->h[0];   b = ctx->h[1];   c = ctx->h[2];   d = ctx->h[3];   
    e = ctx->h[4];   f = ctx->h[5];   g = ctx->h[6];   h = ctx->h[7];   
   
    if (host)   
        {   
        const SHA_LONG *W=(const SHA_LONG *)data;   
   
        for (i=0;i<16;i++)   
            {   
            T1 = X[i] = W[i];   
            T1 += h + Sigma1(e) + Ch(e,f,g) + K256[i];   
            T2 = Sigma0(a) + Maj(a,b,c);   
            h = g;  g = f;  f = e;  e = d + T1;   
            d = c;  c = b;  b = a;  a = T1 + T2;   
            }   
   
        data += SHA256_CBLOCK;   
        }   
    else   
        {   
        SHA_LONG l;   
   
        for (i=0;i<16;i++)   
            {   
            HOST_c2l(data,l); T1 = X[i] = l;   
            T1 += h + Sigma1(e) + Ch(e,f,g) + K256[i];   
            T2 = Sigma0(a) + Maj(a,b,c);   
            h = g;  g = f;  f = e;  e = d + T1;   
            d = c;  c = b;  b = a;  a = T1 + T2;   
            }   
        }   
   
    for (;i<64;i++)   
        {   
        s0 = X[(i+1)&0x0f]; s0 = sigma0(s0);   
        s1 = X[(i+14)&0x0f];    s1 = sigma1(s1);   
   
        T1 = X[i&0xf] += s0 + s1 + X[(i+9)&0xf];   
        T1 += h + Sigma1(e) + Ch(e,f,g) + K256[i];   
        T2 = Sigma0(a) + Maj(a,b,c);   
        h = g;  g = f;  f = e;  e = d + T1;   
        d = c;  c = b;  b = a;  a = T1 + T2;   
        }   
   
    ctx->h[0] += a;  ctx->h[1] += b;  ctx->h[2] += c;  ctx->h[3] += d;   
    ctx->h[4] += e;  ctx->h[5] += f;  ctx->h[6] += g;  ctx->h[7] += h;   
   
            }   
}   
   
   
#else    
    static void sha256_block (SHA256_CTX *ctx, const void *in, size_t num, int host)   
    {   
    unsigned int a,b,c,d,e,f,g,h,s0,s1,T1;   
    SHA_LONG    X[16];   
    int i;   
    const unsigned char *data=in;   
   
            while (num--) {   
   
    a = ctx->h[0];   b = ctx->h[1];   c = ctx->h[2];   d = ctx->h[3];   
    e = ctx->h[4];   f = ctx->h[5];   g = ctx->h[6];   h = ctx->h[7];   
   
    if (host)   
        {   
        const SHA_LONG *W=(const SHA_LONG *)data;   
   
        T1 = X[0] = W[0];   ROUND_00_15(0,a,b,c,d,e,f,g,h);   
        T1 = X[1] = W[1];   ROUND_00_15(1,h,a,b,c,d,e,f,g);   
        T1 = X[2] = W[2];   ROUND_00_15(2,g,h,a,b,c,d,e,f);   
        T1 = X[3] = W[3];   ROUND_00_15(3,f,g,h,a,b,c,d,e);   
        T1 = X[4] = W[4];   ROUND_00_15(4,e,f,g,h,a,b,c,d);   
        T1 = X[5] = W[5];   ROUND_00_15(5,d,e,f,g,h,a,b,c);   
        T1 = X[6] = W[6];   ROUND_00_15(6,c,d,e,f,g,h,a,b);   
        T1 = X[7] = W[7];   ROUND_00_15(7,b,c,d,e,f,g,h,a);   
        T1 = X[8] = W[8];   ROUND_00_15(8,a,b,c,d,e,f,g,h);   
        T1 = X[9] = W[9];   ROUND_00_15(9,h,a,b,c,d,e,f,g);   
        T1 = X[10] = W[10]; ROUND_00_15(10,g,h,a,b,c,d,e,f);   
        T1 = X[11] = W[11]; ROUND_00_15(11,f,g,h,a,b,c,d,e);   
        T1 = X[12] = W[12]; ROUND_00_15(12,e,f,g,h,a,b,c,d);   
        T1 = X[13] = W[13]; ROUND_00_15(13,d,e,f,g,h,a,b,c);   
        T1 = X[14] = W[14]; ROUND_00_15(14,c,d,e,f,g,h,a,b);   
        T1 = X[15] = W[15]; ROUND_00_15(15,b,c,d,e,f,g,h,a);   
   
        data += SHA256_CBLOCK;   
        }   
    else   
        {   
        SHA_LONG l;   
   
        HOST_c2l(data,l); T1 = X[0] = l;  ROUND_00_15(0,a,b,c,d,e,f,g,h);   
        HOST_c2l(data,l); T1 = X[1] = l;  ROUND_00_15(1,h,a,b,c,d,e,f,g);   
        HOST_c2l(data,l); T1 = X[2] = l;  ROUND_00_15(2,g,h,a,b,c,d,e,f);   
        HOST_c2l(data,l); T1 = X[3] = l;  ROUND_00_15(3,f,g,h,a,b,c,d,e);   
        HOST_c2l(data,l); T1 = X[4] = l;  ROUND_00_15(4,e,f,g,h,a,b,c,d);   
        HOST_c2l(data,l); T1 = X[5] = l;  ROUND_00_15(5,d,e,f,g,h,a,b,c);   
        HOST_c2l(data,l); T1 = X[6] = l;  ROUND_00_15(6,c,d,e,f,g,h,a,b);   
        HOST_c2l(data,l); T1 = X[7] = l;  ROUND_00_15(7,b,c,d,e,f,g,h,a);   
        HOST_c2l(data,l); T1 = X[8] = l;  ROUND_00_15(8,a,b,c,d,e,f,g,h);   
        HOST_c2l(data,l); T1 = X[9] = l;  ROUND_00_15(9,h,a,b,c,d,e,f,g);   
        HOST_c2l(data,l); T1 = X[10] = l; ROUND_00_15(10,g,h,a,b,c,d,e,f);   
        HOST_c2l(data,l); T1 = X[11] = l; ROUND_00_15(11,f,g,h,a,b,c,d,e);   
        HOST_c2l(data,l); T1 = X[12] = l; ROUND_00_15(12,e,f,g,h,a,b,c,d);   
        HOST_c2l(data,l); T1 = X[13] = l; ROUND_00_15(13,d,e,f,g,h,a,b,c);   
        HOST_c2l(data,l); T1 = X[14] = l; ROUND_00_15(14,c,d,e,f,g,h,a,b);   
        HOST_c2l(data,l); T1 = X[15] = l; ROUND_00_15(15,b,c,d,e,f,g,h,a);   
        }   
   
    for (i=16;i<64;i+=8)   
        {   
        ROUND_16_63(i+0,a,b,c,d,e,f,g,h,X);   
        ROUND_16_63(i+1,h,a,b,c,d,e,f,g,X);   
        ROUND_16_63(i+2,g,h,a,b,c,d,e,f,X);   
        ROUND_16_63(i+3,f,g,h,a,b,c,d,e,X);   
        ROUND_16_63(i+4,e,f,g,h,a,b,c,d,X);   
        ROUND_16_63(i+5,d,e,f,g,h,a,b,c,X);   
        ROUND_16_63(i+6,c,d,e,f,g,h,a,b,X);   
        ROUND_16_63(i+7,b,c,d,e,f,g,h,a,X);   
        }   
   
    ctx->h[0] += a;  ctx->h[1] += b;  ctx->h[2] += c;  ctx->h[3] += d;   
    ctx->h[4] += e;  ctx->h[5] += f;  ctx->h[6] += g;  ctx->h[7] += h;   
   
            }   
    }   
#endif   
   
#define Xupdate(a,ix,ia,ib,ic,id)   (ix=(a)=(ia^ib^ic^id))   
   
#define K_00_19 0x5a827999UL   
#define K_20_39 0x6ed9eba1UL   
#define K_40_59 0x8f1bbcdcUL   
#define K_60_79 0xca62c1d6UL   
   
   
#define BODY_00_15(i,a,b,c,d,e,f,xi) \   
    (f)=xi+(e)+K_00_19+ROTATE((a),5)+F_00_19((b),(c),(d)); \   
    (b)=ROTATE((b),30);   
   
#define BODY_16_19(i,a,b,c,d,e,f,xi,xa,xb,xc,xd) \   
    Xupdate(f,xi,xa,xb,xc,xd); \   
    (f)+=(e)+K_00_19+ROTATE((a),5)+F_00_19((b),(c),(d)); \   
    (b)=ROTATE((b),30);   
   
#define BODY_20_31(i,a,b,c,d,e,f,xi,xa,xb,xc,xd) \   
    Xupdate(f,xi,xa,xb,xc,xd); \   
    (f)+=(e)+K_20_39+ROTATE((a),5)+F_20_39((b),(c),(d)); \   
    (b)=ROTATE((b),30);   
   
#define BODY_32_39(i,a,b,c,d,e,f,xa,xb,xc,xd) \   
    Xupdate(f,xa,xa,xb,xc,xd); \   
    (f)+=(e)+K_20_39+ROTATE((a),5)+F_20_39((b),(c),(d)); \   
    (b)=ROTATE((b),30);   
   
#define BODY_40_59(i,a,b,c,d,e,f,xa,xb,xc,xd) \   
    Xupdate(f,xa,xa,xb,xc,xd); \   
    (f)+=(e)+K_40_59+ROTATE((a),5)+F_40_59((b),(c),(d)); \   
    (b)=ROTATE((b),30);   
   
#define BODY_60_79(i,a,b,c,d,e,f,xa,xb,xc,xd) \   
    Xupdate(f,xa,xa,xb,xc,xd); \   
    (f)=xa+(e)+K_60_79+ROTATE((a),5)+F_60_79((b),(c),(d)); \   
    (b)=ROTATE((b),30);   
   
#define HOST_p_c2l_p(c,l,sc,len) {                  \   
            switch (sc) {                   \   
            case 0: l =((unsigned long)(*((c)++)))<<24;   \   
                if (--len == 0) break;          \   
            case 1: l|=((unsigned long)(*((c)++)))<<16;   \   
                if (--len == 0) break;          \   
            case 2: l|=((unsigned long)(*((c)++)))<< 8;   \   
                } }   
/* NOTE the pointer is not incremented at the end of this */   
#define HOST_c2l_p(c,l,n)   {                   \   
            l=0; (c)+=n;                    \   
            switch (n) {                    \   
            case 3: l =((unsigned long)(*(--(c))))<< 8;   \   
            case 2: l|=((unsigned long)(*(--(c))))<<16;   \   
            case 1: l|=((unsigned long)(*(--(c))))<<24;   \   
                } }   
   
#ifndef HOST_l2c   
#define HOST_l2c(l,c)   (*((c)++)=(unsigned char)(((l)>>24)&0xff),    \   
             *((c)++)=(unsigned char)(((l)>>16)&0xff),    \   
             *((c)++)=(unsigned char)(((l)>> 8)&0xff),    \   
             *((c)++)=(unsigned char)(((l)    )&0xff),  \   
             l)   
#endif   
   
int SHA256_Init (SHA256_CTX *c)   
{   
    c->h[0]=0x6a09e667UL;    c->h[1]=0xbb67ae85UL;   
    c->h[2]=0x3c6ef372UL;    c->h[3]=0xa54ff53aUL;   
    c->h[4]=0x510e527fUL;    c->h[5]=0x9b05688cUL;   
    c->h[6]=0x1f83d9abUL;    c->h[7]=0x5be0cd19UL;   
    c->Nl=0; c->Nh=0;   
    c->num=0;    c->md_len=SHA256_DIGEST_LENGTH;   
    return 1;   
}   
   
int SHA256_Update (SHA256_CTX *c, const void *data_, size_t len)   
    {   
    const unsigned char *data=data_;   
    register HASH_LONG * p;   
    register HASH_LONG l;   
    size_t sw,sc,ew,ec;   
   
    if (len == 0) return 1;   
   
    l=(c->Nl+(((HASH_LONG)len)<<3))&0xffffffffUL;   
    /* 95-05-24 eay Fixed a bug with the overflow handling, thanks to  
     * Wei Dai <weidai@eskimo.com> for pointing it out. */   
    if (l < c->Nl) /* overflow */   
        c->Nh++;   
    c->Nh+=(len>>29);  /* might cause compiler warning on 16-bit */   
    c->Nl=l;   
   
    if (c->num != 0)   
        {   
        p=c->data;   
        sw=c->num>>2;   
        sc=c->num&0x03;   
   
        if ((c->num+len) >= SHA_CBLOCK)   
            {   
            l=p[sw]; HOST_p_c2l(data,l,sc); p[sw++]=l;   
            for (; sw<SHA_LBLOCK; sw++)   
                {   
                HOST_c2l(data,l); p[sw]=l;   
                }   
            sha256_block(c,p,1,1);   
            len-=(SHA256_CBLOCK-c->num);   
            c->num=0;   
            /* drop through and do the rest */   
            }   
        else   
            {   
            c->num+=(unsigned int)len;   
            if ((sc+len) < 4) /* ugly, add char's to a word */   
                {   
                l=p[sw]; HOST_p_c2l_p(data,l,sc,len); p[sw]=l;   
                }   
            else   
                {   
                ew=(c->num>>2);   
                ec=(c->num&0x03);   
                if (sc)   
                    l=p[sw];   
                HOST_p_c2l(data,l,sc);   
                p[sw++]=l;   
                for (; sw < ew; sw++)   
                    {   
                    HOST_c2l(data,l); p[sw]=l;   
                    }   
                if (ec)   
                    {   
                    HOST_c2l_p(data,l,ec); p[sw]=l;   
                    }   
                }   
            return 1;   
            }   
        }   
   
    sw=len/SHA_CBLOCK;   
    if (sw > 0)   
        {   
   
        if ((((size_t)data)%4) == 0)   
            {   
            /* data is properly aligned so that we can cast it: */   
            sha256_block(c,(const HASH_LONG *)data,sw,1);   
            sw*=SHA256_CBLOCK;   
            data+=sw;   
            len-=sw;   
            }   
        }   
   
    if (len!=0)   
        {   
        p = c->data;   
        c->num = len;   
        ew=len>>2;    /* words to copy */   
        ec=len&0x03;   
        for (; ew; ew--,p++)   
            {   
            HOST_c2l(data,l); *p=l;   
            }   
        HOST_c2l_p(data,l,ec);   
        *p=l;   
        }   
    return 1;   
    }   
   
   
   
   
   
int SHA256_Final (unsigned char *md, SHA256_CTX *c)   
{   
    register HASH_LONG *p;   
    register unsigned long l;   
    register int i,j;   
    static const unsigned char end[4]={0x80,0x00,0x00,0x00};   
    const unsigned char *cp=end;   
   
    /* c->num should definitly have room for at least one more byte. */   
    p=c->data;   
    i=c->num>>2;   
    j=c->num&0x03;   
   
   
   
    l = (j==0) ? 0 : p[i];   
   
    HOST_p_c2l(cp,l,j); p[i++]=l; /* i is the next 'undefined word' */   
   
    if (i>(SHA_LBLOCK-2)) /* save room for Nl and Nh */   
        {   
        if (i<SHA_LBLOCK) p[i]=0;   
        sha256_block (c,p,1,1);   
        i=0;   
        }   
    for (; i<(SHA_LBLOCK-2); i++)   
        p[i]=0;   
   
   
    p[SHA_LBLOCK-2]=c->Nh;   
    p[SHA_LBLOCK-1]=c->Nl;   
   
    sha256_block(c,p,1,1);   
   
    HASH_MAKE_STRING(c,md);   
   
    c->num=0;   
    /* clear stuff, HASH_BLOCK may be leaving some stuff on the stack  
     * but I'm not worried :-)  
    OPENSSL_cleanse((void *)c,sizeof(HASH_CTX));  
     */   
    return 1;   
}   
   
   
void OPENSSL_cleanse(void *ptr, size_t len)   
{   
    unsigned char *p = ptr;   
    size_t loop = len;   
    while(loop--)   
        {   
        *(p++) = cleanse_ctr;   
        cleanse_ctr += (17 + (unsigned char)((unsigned long)p & 0xF));   
        }   
    if(memchr(ptr, cleanse_ctr, len))   
        cleanse_ctr += 63;   
}   
   
unsigned char *SHA256(const unsigned char *d, size_t n, unsigned char *md)   
{   
    SHA256_CTX c;   
    static unsigned char m[SHA256_DIGEST_LENGTH];   
   
    if (md == NULL) md=m;   
    SHA256_Init(&c);   
    SHA256_Update(&c,d,n);   
    SHA256_Final(md,&c);   
    OPENSSL_cleanse(&c,sizeof(c));   
    return(md);   
}   
