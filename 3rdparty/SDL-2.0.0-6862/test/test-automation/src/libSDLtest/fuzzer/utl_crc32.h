#ifndef _utl_crc32_h
#define _utl_crc32_h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern    "C" {
#endif

/* ----------- Includes -------------- */

#include <stdlib.h>

/* ------------ Definitions --------- */

/* Definition shared by all CRC routines */

#ifndef CrcUint32
 #define CrcUint32	unsigned int
#endif
#ifndef CrcUint8
 #define CrcUint8	unsigned char
#endif

#ifdef ORIGINAL_METHOD
 #define CRC32_POLY 0x04c11db7 	 /* AUTODIN II, Ethernet, & FDDI */
#else
 #define CRC32_POLY 0xEDB88320   /* Perl String::CRC32 compatible */
#endif

/* Data structure for CRC32 (checksum) computation */

  typedef struct {
   CrcUint32    crc32_table[256]; /* CRC table */
  } CRC32_CTX;

/* ---------- Function Prototypes ------------- */

#ifdef WIN32
#ifdef BUILD_DLL
#define DLLINTERFACE __declspec(dllexport)
#else
#define DLLINTERFACE __declspec(dllimport)
#endif
#else
#define DLLINTERFACE
#endif

/* 
 * utl_crc32Init: initialize the CRC context
 *
 * Parameters:
 *
 *   crcContext		pointer to context variable
 *
 * Return value:
 *
 *   0          OK
 *  -1          error
 *
 * Note: The function initializes the crc table required for crc calcs.
 */
  DLLINTERFACE int utl_crc32Init(CRC32_CTX * crcContext);


/*
 * utl_crc32Calc: calculate a crc32 from a data block
 * 
 * Parameters:
 *
 *   crcContext		pointer to context variable
 *   inBuf              input buffer to checksum
 *   inLen              length of input buffer
 *   crc32              pointer to Uint32 to store the final CRC into
 *
 * Return value:
 *
 *   0        OK
 *  -1        error
 *
*/

  DLLINTERFACE int utl_crc32Calc(CRC32_CTX * crcContext, CrcUint8 *inBuf, CrcUint32 inLen, CrcUint32 *crc32);

/* Same routine broken down into three steps */

 DLLINTERFACE int utl_crc32CalcStart(CRC32_CTX * crcContext, CrcUint32 *crc32);
 DLLINTERFACE int utl_crc32CalcEnd(CRC32_CTX * crcContext, CrcUint32 *crc32);
 DLLINTERFACE int utl_crc32CalcBuffer(CRC32_CTX * crcContext, CrcUint8 *inBuf, CrcUint32 inLen, CrcUint32 *crc32);


/*
 * utl_crc32Done: clean up CRC context
 *
 * Parameters:
 *
 *   crcContext		pointer to context variable
 *
 * Return value:
 *
 *   0            OK
 *  -1            error
 *
*/

  DLLINTERFACE int utl_crc32Done(CRC32_CTX * crcContext);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
};
#endif

#endif /* _utl_crc32_h */
