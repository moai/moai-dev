/* 

 A "32-bit Multiply with carry: random number generator. 
 Has a list of recommended multipliers.  Very fast and good.
                         
 multiply-with-carry generator" x(n) = a*x(n-1) + carry mod 2^32.
 period" (a*2^31)-1
 
*/

#ifndef _utl_random_h
#define _utl_random_h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern    "C" {
#endif

/* ------- Includes ---------- */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* ------- Definitions ------- */

/*
 * Macros that return random number in a specific format. See utl_random()
 * below for details. Float values are in the range [0.0-1.0].
 */
#define utl_randomInt(c)		((int)utl_random(c))
#define utl_randomFloat(c)	((double)utl_random(c)/(unsigned long)0xffffffff)

  typedef struct {
    unsigned int a;
    unsigned int x;
    unsigned int c;
    unsigned int ah;
    unsigned int al;
  } RND_CTX;

/* ---- Function Prototypes -------- */

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
 * utl_randomInit: Initialize random number generator with two integers. 
 *
 * Paramaters:
 *
 *   rndContext     pointer to context structure
 *   xi, ci         integers that define the random sequence
 *
 * Return value:
 *
 *   none
 *
 * Note: The random sequence of numbers returned by utl_random() is the
 *       same for the same two integers and has a period of 2^31.
 *
 */
  DLLINTERFACE void utl_randomInit(RND_CTX * rndContext, unsigned int xi,
				  unsigned int ci);

/* 
 * utl_randomInitTime: Initialize random number generator with the time 
 *
 * Parameters:
 *
 *   rndContext     pointer to context structure
 *
 * Return value:
 *
 *   none
 *
 */
  DLLINTERFACE void utl_randomInitTime(RND_CTX * rndContext);


/* 
 * utl_random: Returns random numbers 
 *
 * Parameters:
 *
 *   rndContext     pointer to context structure
 *
 * Return value:
 *
 *   random number (32bit unsigned integer)
 *
 * Note: utl_randomInit() or utl_randomInitTime() must have been called
 *       before using this function.
 *
 */
  DLLINTERFACE unsigned int utl_random(RND_CTX * rndContext);



/* Ends C function definitions when using C++ */
#ifdef __cplusplus
};
#endif

#endif /* _utl_random_h */
