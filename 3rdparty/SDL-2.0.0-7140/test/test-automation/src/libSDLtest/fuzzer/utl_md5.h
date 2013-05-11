
/*
 ***********************************************************************
 ** utl_md5.h -- header file for implementation of MD5                  **
 ** RSA Data Security, Inc. MD5 Message-Digest Algorithm              **
 ** Created: 2/17/90 RLR                                              **
 ** Revised: 12/27/90 SRD,AJ,BSK,JT Reference C version               **
 ** Revised (for MD5): RLR 4/27/91                                    **
 **   -- G modified to have y&~z instead of y&z                       **
 **   -- FF, GG, HH modified to add in last register done             **
 **   -- Access pattern: round 2 works mod 5, round 3 works mod 3     **
 **   -- distinct additive constant for each step                     **
 **   -- round 4 added, working mod 7                                 **
 ***********************************************************************
*/

/*
 ***********************************************************************
 **  Message-digest routines:                                         **
 **  To form the message digest for a message M                       **
 **    (1) Initialize a context buffer mdContext using MD5Init        **
 **    (2) Call MD5Update on mdContext and M                          **
 **    (3) Call MD5Final on mdContext                                 **
 **  The message digest is now in mdContext->digest[0...15]           **
 ***********************************************************************
*/

#ifndef _utl_md5_h
#define _utl_md5_h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern    "C" {
#endif

/* ------------ Definitions --------- */

/* typedef a 32-bit type */
  typedef unsigned long int MD5UINT4;

/* Data structure for MD5 (Message-Digest) computation */
  typedef struct {
    MD5UINT4  i[2];		/* number of _bits_ handled mod 2^64 */
    MD5UINT4  buf[4];		/* scratch buffer */
    unsigned char in[64];	/* input buffer */
    unsigned char digest[16];	/* actual digest after MD5Final call */
  } MD5_CTX;

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
 * utl_md5Init: initialize the context
 *
 * Parameters:
 *
 *   mdContext		pointer to context variable
 *
 * Return value:
 *
 *   none
 *
 * Note: The function initializes the message-digest context
 *       mdContext. Call before each new use of the context - 
 *       all fields are set to zero.
 */
  DLLINTERFACE void utl_md5Init(MD5_CTX * mdContext);


/*
 * utl_md5update: update digest from variable length data
 * 
 * Parameters:
 *
 *   mdContext       pointer to context variable
 *   inBuf           pointer to data array/string
 *   inLen           length of data array/string
 *
 * Return value:
 *
 *   none
 *
 * Note: The function updates the message-digest context to account 
 *       for the presence of each of the characters inBuf[0..inLen-1]
 *       in the message whose digest is being computed.
*/

  DLLINTERFACE void utl_md5Update(MD5_CTX * mdContext, unsigned char *inBuf,
				 unsigned int inLen);


/*
 * utl_md5Final: complete digest computation
 *
 * Parameters:
 *
 *   mdContext		pointer to context variable
 *
 * Return value:
 *
 *   none
 *
 * Note: The function terminates the message-digest computation and
 *       ends with the desired message digest in mdContext.digest[0..15].
 *       Always call before using the digest[] variable.
*/

  DLLINTERFACE void utl_md5Final(MD5_CTX * mdContext);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
};
#endif

#endif /* _utl_md5_h */
