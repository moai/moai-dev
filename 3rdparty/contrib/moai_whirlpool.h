
#ifndef PORTABLE_C__
#define PORTABLE_C__

#include <limits.h>

/* Definition of minimum-width integer types
 *
 * wp_u8   -> unsigned integer type, at least 8 bits, equivalent to unsigned char
 * wp_u16  -> unsigned integer type, at least 16 bits
 * wp_u32  -> unsigned integer type, at least 32 bits
 *
 * wp_s8, wp_s16, wp_s32  -> signed counterparts of wp_u8, wp_u16, wp_u32
 *
 * Always use macro's T8(), T16() or T32() to obtain exact-width results,
 * i.e., to specify the size of the result of each expression.
 */

#if UINT_MAX >= 4294967295UL
	#define ONE32   0xffffffffU
#else
	#define ONE32   0xffffffffUL
#endif

#define ONE8    0xffU
#define ONE16   0xffffU

#define T8(x)   ((x) & ONE8)
#define T16(x)  ((x) & ONE16)
#define T32(x)  ((x) & ONE32)

#ifdef _MSC_VER
	#define LL(v)   (v##ui64)
	#define ONE64   LL(0xffffffffffffffff)
#else  /* !_MSC_VER */
	#define LL(v)   (v##ULL)
	#define ONE64   LL(0xffffffffffffffff)
#endif /* ?_MSC_VER */

#define T64(x)  ((x) & ONE64)
#define ROTR64(v, n)   (((v) >> (n)) | T64((v) << (64 - (n))))
/*
 * Note: the test is used to detect native 64-bit architectures;
 * if the unsigned long is strictly greater than 32-bit, it is
 * assumed to be at least 64-bit. This will not work correctly
 * on (old) 36-bit architectures (PDP-11 for instance).
 *
 * On non-64-bit architectures, "long long" is used.
 */

/*
 * U8TO32_BIG(c) returns the 32-bit value stored in big-endian convention
 * in the unsigned char array pointed to by c.
 */
#define U8TO32_BIG(c)  (((wp_u32)T8(*(c)) << 24) | ((wp_u32)T8(*((c) + 1)) << 16) | ((wp_u32)T8(*((c) + 2)) << 8) | ((wp_u32)T8(*((c) + 3))))

/*
 * U8TO32_LITTLE(c) returns the 32-bit value stored in little-endian convention
 * in the unsigned char array pointed to by c.
 */
#define U8TO32_LITTLE(c)  (((wp_u32)T8(*(c))) | ((wp_u32)T8(*((c) + 1)) << 8) | (wp_u32)T8(*((c) + 2)) << 16) | ((wp_u32)T8(*((c) + 3)) << 24))

/*
 * U8TO32_BIG(c, v) stores the 32-bit-value v in big-endian convention
 * into the unsigned char array pointed to by c.
 */
#define U32TO8_BIG(c, v)    do { wp_u32 x = (v); wp_u8 *d = (c); d[0] = T8(x >> 24); d[1] = T8(x >> 16); d[2] = T8(x >> 8); d[3] = T8(x); } while (0)

/*
 * U8TO32_LITTLE(c, v) stores the 32-bit-value v in little-endian convention
 * into the unsigned char array pointed to by c.
 */
#define U32TO8_LITTLE(c, v)    do { wp_u32 x = (v); wp_u8 *d = (c); d[0] = T8(x); d[1] = T8(x >> 8); d[2] = T8(x >> 16); d[3] = T8(x >> 24); } while (0)

/*
 * ROTL32(v, n) returns the value of the 32-bit unsigned value v after
 * a rotation of n bits to the left. It might be replaced by the appropriate
 * architecture-specific macro.
 *
 * It evaluates v and n twice.
 *
 * The compiler might emit a warning if n is the constant 0. The result
 * is undefined if n is greater than 31.
 */
#define ROTL32(v, n)   (T32((v) << (n)) | ((v) >> (32 - (n))))

/*
 * Whirlpool-specific definitions.
 */

#define DIGESTBYTES 64
#define DIGESTBITS  (8*DIGESTBYTES) /* 512 */

#define WBLOCKBYTES 64
#define WBLOCKBITS  (8*WBLOCKBYTES) /* 512 */

#define LENGTHBYTES 32
#define LENGTHBITS  (8*LENGTHBYTES) /* 256 */

typedef unsigned char wp_u8;
typedef unsigned short wp_u16;
typedef unsigned long wp_u32;
typedef unsigned long long wp_u64;
typedef signed short wp_s16;
typedef signed long wp_s32;
typedef signed long long wp_s64;

typedef struct Whirlpool {
	wp_u8  bitLength[LENGTHBYTES];	/* global number of hashed bits (256-bit counter) */
	wp_u8  buffer[WBLOCKBYTES];		/* buffer of data to hash */
	int bufferBits;					/* current number of bits on the buffer */
	int bufferPos;					/* current (possibly incomplete) byte slot on the buffer */
	wp_u64 hash[DIGESTBYTES/8];		/* the hashing state */
} Whirlpool;

void MOAIWhirlpool_Add(const unsigned char * const source, unsigned long sourceBits, struct Whirlpool * const structpointer);
void MOAIWhirlpool_Finalize(struct Whirlpool * const structpointer, unsigned char * const result);
void MOAIWhirlpool_Init(struct Whirlpool * const structpointer);

#endif   /* PORTABLE_C__ */
