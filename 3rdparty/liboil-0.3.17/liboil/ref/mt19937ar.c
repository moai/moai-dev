/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <liboil/liboilfunction.h>

/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */


OIL_DEFINE_CLASS(mt19937, "uint32_t *d_624, uint32_t *i_624");
#if 0
OIL_DEFINE_CLASS(mt19937x8, "uint32_t *d_624x8, uint32_t *i_624x8");
#endif

/* mag01[x] = x * MATRIX_A  for x=0,1 */
static const uint32_t mag01[2]={0x0UL, MATRIX_A};

static void
mt19937_ref (uint32_t *d, uint32_t *mt)
{
  uint32_t y;
  int kk;

  for (kk=0;kk<N-M;kk++) {
      y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
      mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
  }
  for (;kk<N-1;kk++) {
      y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
      mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
  }
  y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
  mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

  for(kk=0;kk<N;kk++){
    y = mt[kk];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    d[kk] = y;
  }
}
OIL_DEFINE_IMPL_REF (mt19937_ref, mt19937);



#if 0
/* There's no point in doing this in parallel, since the above class
 * is handled by MMX quite well */
static void
mt19937x8_ref (uint32_t *d, uint32_t *mt)
{
  uint32_t y;
  int kk;
  int i;

  for(i=0;i<8;i++){
    for (kk=0;kk<N-M;kk++) {
        y = (mt[kk*8+i]&UPPER_MASK)|(mt[(kk+1)*8+i]&LOWER_MASK);
        mt[kk*8+i] = mt[(kk+M)*8+i] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    for (;kk<N-1;kk++) {
        y = (mt[kk*8+i]&UPPER_MASK)|(mt[(kk+1)*8+i]&LOWER_MASK);
        mt[kk*8+i] = mt[(kk+(M-N))*8+i] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    y = (mt[(N-1)*8+i]&UPPER_MASK)|(mt[0*8+i]&LOWER_MASK);
    mt[(N-1)*8+i] = mt[(M-1)*8+i] ^ (y >> 1) ^ mag01[y & 0x1UL];

    for(kk=0;kk<N;kk++){
      y = mt[kk*8 + i];

      /* Tempering */
      y ^= (y >> 11);
      y ^= (y << 7) & 0x9d2c5680UL;
      y ^= (y << 15) & 0xefc60000UL;
      y ^= (y >> 18);

      d[kk*8 + i] = y;
    }
  }
}
OIL_DEFINE_IMPL_REF (mt19937x8_ref, mt19937x8);
#endif

