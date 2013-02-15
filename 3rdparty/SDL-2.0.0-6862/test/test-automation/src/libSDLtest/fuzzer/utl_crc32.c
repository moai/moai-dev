
#include "utl_crc32.h"

int utl_crc32Init(CRC32_CTX *crcContext)
{
  int i,j;
  CrcUint32 c;

  /* Sanity check context pointer */
  if (crcContext==NULL) {
   return(-1);
  }  
 
  /*
   * Build auxiliary table for parallel byte-at-a-time CRC-32 
   */
#ifdef ORIGINAL_METHOD
  for (i = 0; i < 256; ++i) {
    for (c = i << 24, j = 8; j > 0; --j) {
      c = c & 0x80000000 ? (c << 1) ^ CRC32_POLY : (c << 1);
    }
    crcContext->crc32_table[i] = c;
  }
#else
  for (i=0; i<256; i++) {
   c = i;
   for (j=8; j>0; j--) {
    if (c & 1) {
     c = (c >> 1) ^ CRC32_POLY;
    } else {
     c >>= 1;
    }
   }
   crcContext->crc32_table[i] = c;
  }
#endif
  
  return(0);
}

/* Complete CRC32 calculation on a memory block */

int utl_crc32Calc(CRC32_CTX * crcContext, CrcUint8 *inBuf, CrcUint32 inLen, CrcUint32 *crc32)
{
  if (utl_crc32CalcStart(crcContext,crc32)) {
   return(-1);
  }
  if (utl_crc32CalcBuffer(crcContext, inBuf, inLen, crc32)) {
   return(-1);
  }
  if (utl_crc32CalcEnd(crcContext, crc32)) {
   return(-1);
  } 
  return(0);
}

/* Start crc calculation */

int utl_crc32CalcStart(CRC32_CTX * crcContext, CrcUint32 *crc32)
{
  /* Sanity check pointers */
  if (crcContext==NULL) {
   *crc32=0;
   return(-1);
  }  

  /*
   * Preload shift register, per CRC-32 spec 
   */
  *crc32 = 0xffffffff;

  return(0);
}

/* Finish crc calculation */

int utl_crc32CalcEnd(CRC32_CTX * crcContext, CrcUint32 *crc32)
{
  /*
   * Return complement, per CRC-32 spec 
   */
  *crc32 = (~(*crc32));

  return(0);
}

/* Include memory block in crc */

int utl_crc32CalcBuffer(CRC32_CTX * crcContext, CrcUint8 *inBuf, CrcUint32 inLen, CrcUint32 *crc32)
{
  CrcUint8    *p;
  register CrcUint32    crc;

  /* Sanity check pointers */
  if (crcContext==NULL) {
   *crc32=0;
   return(-1);
  }  


  /*
   * Calculate CRC from data 
   */
  crc = *crc32;
  for (p = inBuf; inLen > 0; ++p, --inLen) {
#ifdef ORIGINAL_METHOD  
    crc = (crc << 8) ^ crcContext->crc32_table[(crc >> 24) ^ *p];
#else
    crc = ((crc >> 8) & 0x00FFFFFF) ^ crcContext->crc32_table[ (crc ^ *p) & 0xFF ];
#endif         
  }  
  *crc32 = crc;
  
  return(0);
}

int utl_crc32Done(CRC32_CTX * crcContext)
{
  /* Sanity check context pointer */
  if (crcContext==NULL) {
   return(-1);
  }  

  return(0);
}
