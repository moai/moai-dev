/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
  these functions are copied from 
  http://svn.xiph.org/trunk/vp32/CoreLibs/CDXV/Vp31/Common/mac/OptFunctionsPPC.c
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/simdpack/simdpack.h>

OIL_DECLARE_CLASS (recon8x8_intra);
OIL_DECLARE_CLASS (recon8x8_inter);
OIL_DECLARE_CLASS (recon8x8_inter2);

static const __attribute__ ((aligned(8),used)) uint64_t V128 = 0x8080808080808080LL;

static void
                    /*       r3,            r4,         r5 */
recon8x8_intra_altivec (uint8_t *dest, int16_t *change, int ds)
{
    asm {
		//trying cache hints
		lis			r6,0x0108
		or			r6,r6,r5
		dstst		r3,r6,0

		vspltish	v1,7

		vspltish	v8,1
		xor			r6,r6,r6
		
		lvx			v0,r4,r6				//get 8 shorts					
		vslh		v8,v8,v1				//now have 128
		addi		r6,r6,16
		
		lvx			v1,r4,r6				//get 8 shorts					
		vaddshs		v0,v0,v8				//+=128
		addi		r6,r6,16

		lvx			v2,r4,r6				//get 8 shorts					
		vaddshs		v1,v1,v8				//+=128
		addi		r6,r6,16
		vpkshus		v0,v0,v0				//convert to bytes

		lvx			v3,r4,r6				//get 8 shorts					
		vaddshs		v2,v2,v8				//+=128
		addi		r6,r6,16
		vpkshus		v1,v1,v1				//convert to bytes

		lvx			v4,r4,r6				//get 8 shorts					
		vaddshs		v3,v3,v8				//+=128
		addi		r6,r6,16
		vpkshus		v2,v2,v2				//convert to bytes

		lvx			v5,r4,r6				//get 8 shorts					
		vaddshs		v4,v4,v8				//+=128
		addi		r6,r6,16
		vpkshus		v3,v3,v3				//convert to bytes

		lvx			v6,r4,r6				//get 8 shorts					
		vaddshs		v5,v5,v8				//+=128
		addi		r6,r6,16
		vpkshus		v4,v4,v4				//convert to bytes

		lvx			v7,r4,r6				//get 8 shorts	
		xor			r6,r6,r6				
		vaddshs		v6,v6,v8				//+=128
		vpkshus		v5,v5,v5				//convert to bytes

		lvsr		v9,r3,r6				//load alignment vector for stores
		vaddshs		v7,v7,v8				//+=128
		vpkshus		v6,v6,v6				//convert to bytes

		vpkshus		v7,v7,v7				//convert to bytes

		li			r7,4
		vperm		v0,v0,v0,v9

		stvewx		v0,r3,r6
		add			r6,r6,r5

		lvsr		v9,r3,r6				//load alignment vector for stores

		stvewx		v0,r3,r7
		add			r7,r7,r5
		vperm		v1,v1,v1,v9

		stvewx		v1,r3,r6
		add			r6,r6,r5

		lvsr		v9,r3,r6				//load alignment vector for stores

		stvewx		v1,r3,r7
		add			r7,r7,r5
		vperm		v2,v2,v2,v9

		stvewx		v2,r3,r6
		add			r6,r6,r5

		lvsr		v9,r3,r6				//load alignment vector for stores

		stvewx		v2,r3,r7
		add			r7,r7,r5
		vperm		v3,v3,v3,v9

		stvewx		v3,r3,r6
		add			r6,r6,r5

		lvsr		v9,r3,r6				//load alignment vector for stores

		stvewx		v3,r3,r7
		add			r7,r7,r5
		vperm		v4,v4,v4,v9

		stvewx		v4,r3,r6
		add			r6,r6,r5

		lvsr		v9,r3,r6				//load alignment vector for stores

		stvewx		v4,r3,r7
		add			r7,r7,r5
		vperm		v5,v5,v5,v9

		stvewx		v5,r3,r6
		add			r6,r6,r5

		lvsr		v9,r3,r6				//load alignment vector for stores

		stvewx		v5,r3,r7
		add			r7,r7,r5
		vperm		v6,v6,v6,v9

		stvewx		v6,r3,r6
		add			r6,r6,r5

		lvsr		v9,r3,r6				//load alignment vector for stores

		stvewx		v6,r3,r7
		add			r7,r7,r5
		vperm		v7,v7,v7,v9

		stvewx		v7,r3,r6

		stvewx		v7,r3,r7
    }
}

OIL_DEFINE_IMPL_FULL (recon8x8_intra_altivec, recon8x8_intra, OIL_IMPL_FLAG_ALTIVEC);

static void          /*      r3,            r4,           r5,         r6 */
recon8x8_inter_altivec (uint8_t *dest, uint8_t *src, int16_t *change, int dss)
{
	asm
	{
		//trying cache hints
		lis			r7,0x0108
		or			r7,r7,r6
		dstst		r3,r7,0
		
		xor			r7,r7,r7
		li			r8,16
		
		lvsl		v8,r4,r7				//load alignment vector for refs
		vxor		v9,v9,v9
		
		lvx			v10,r4,r7				//get 8 refs
		add			r7,r7,r6					

		lvx			v0,r4,r8				//need another 16 bytes for misaligned data -- 0
		add			r8,r8,r6					

		lvx			v11,r4,r7				//get 8 refs
		vperm		v10,v10,v0,v8

		lvsl		v8,r4,r7				//load alignment vector for refs
		add			r7,r7,r6					

		lvx			v1,r4,r8				//need another 16 bytes for misaligned data -- 1
		add			r8,r8,r6					

		lvx			v12,r4,r7				//get 8 refs
		vperm		v11,v11,v1,v8

		lvsl		v8,r4,r7				//load alignment vector for refs
		add			r7,r7,r6					

		lvx			v2,r4,r8				//need another 16 bytes for misaligned data -- 2
		add			r8,r8,r6					

		lvx			v13,r4,r7				//get 8 refs
		vperm		v12,v12,v2,v8

		lvsl		v8,r4,r7				//load alignment vector for refs
		add			r7,r7,r6					

		lvx			v3,r4,r8				//need another 16 bytes for misaligned data -- 3
		add			r8,r8,r6					

		lvx			v14,r4,r7				//get 8 refs
		vperm		v13,v13,v3,v8

		lvsl		v8,r4,r7				//load alignment vector for refs
		add			r7,r7,r6					

		lvx			v4,r4,r8				//need another 16 bytes for misaligned data -- 4
		add			r8,r8,r6					

		lvx			v15,r4,r7				//get 8 refs
		vperm		v14,v14,v4,v8

		lvsl		v8,r4,r7				//load alignment vector for refs
		add			r7,r7,r6					

		lvx			v5,r4,r8				//need another 16 bytes for misaligned data -- 5
		add			r8,r8,r6					

		lvx			v16,r4,r7				//get 8 refs
		vperm		v15,v15,v5,v8

		lvsl		v8,r4,r7				//load alignment vector for refs
		add			r7,r7,r6					

		lvx			v6,r4,r8				//need another 16 bytes for misaligned data -- 6
		add			r8,r8,r6					

		lvx			v17,r4,r7				//get 8 refs
		vperm		v16,v16,v6,v8

		lvsl		v8,r4,r7				//load alignment vector for refs
		xor			r7,r7,r7

		lvx			v7,r4,r8				//need another 16 bytes for misaligned data -- 7
		add			r8,r8,r6					

		lvx			v0,r5,r7				//get 8 shorts 				
		vperm		v17,v17,v7,v8
		addi		r7,r7,16

		lvx			v1,r5,r7				//get 8 shorts 				
		vmrghb		v10,v9,v10				//unsigned byte -> unsigned half		
		addi		r7,r7,16

		lvx			v2,r5,r7				//get 8 shorts 				
		vmrghb		v11,v9,v11				//unsigned byte -> unsigned half		
		vaddshs		v0,v0,v10
		addi		r7,r7,16

		lvx			v3,r5,r7				//get 8 shorts 				
		vmrghb		v12,v9,v12				//unsigned byte -> unsigned half		
		vaddshs		v1,v1,v11
		addi		r7,r7,16

		lvx			v4,r5,r7				//get 8 shorts 				
		vmrghb		v13,v9,v13				//unsigned byte -> unsigned half		
		vaddshs		v2,v2,v12
		addi		r7,r7,16

		lvx			v5,r5,r7				//get 8 shorts 				
		vmrghb		v14,v9,v14				//unsigned byte -> unsigned half		
		vaddshs		v3,v3,v13
		addi		r7,r7,16

		lvx			v6,r5,r7				//get 8 shorts 				
		vmrghb		v15,v9,v15				//unsigned byte -> unsigned half		
		vaddshs		v4,v4,v14
		addi		r7,r7,16

		lvx			v7,r5,r7				//get 8 shorts 				
		vmrghb		v16,v9,v16				//unsigned byte -> unsigned half		
		vaddshs		v5,v5,v15
		
		vmrghb		v17,v9,v17				//unsigned byte -> unsigned half	
		vaddshs		v6,v6,v16
		
		vpkshus		v0,v0,v0				
		vaddshs		v7,v7,v17
			
		vpkshus		v1,v1,v1				
		xor			r7,r7,r7

		vpkshus		v2,v2,v2				

		vpkshus		v3,v3,v3				

		vpkshus		v4,v4,v4				

		vpkshus		v5,v5,v5				

		vpkshus		v6,v6,v6				

		lvsr		v9,r3,r7				//load alignment vector for stores
		vpkshus		v7,v7,v7

		li			r8,4
		vperm		v0,v0,v0,v9				//adjust for writes

		stvewx		v0,r3,r7
		add			r7,r7,r6	

		lvsr		v9,r3,r7				//load alignment vector for stores

		stvewx		v0,r3,r8
		add			r8,r8,r6	
		vperm		v1,v1,v1,v9

		stvewx		v1,r3,r7
		add			r7,r7,r6	

		lvsr		v9,r3,r7				//load alignment vector for stores

		stvewx		v1,r3,r8
		add			r8,r8,r6	
		vperm		v2,v2,v2,v9

		stvewx		v2,r3,r7
		add			r7,r7,r6	

		lvsr		v9,r3,r7				//load alignment vector for stores

		stvewx		v2,r3,r8
		add			r8,r8,r6	
		vperm		v3,v3,v3,v9

		stvewx		v3,r3,r7
		add			r7,r7,r6	

		lvsr		v9,r3,r7				//load alignment vector for stores

		stvewx		v3,r3,r8
		add			r8,r8,r6	
		vperm		v4,v4,v4,v9

		stvewx		v4,r3,r7
		add			r7,r7,r6	

		lvsr		v9,r3,r7				//load alignment vector for stores

		stvewx		v4,r3,r8
		add			r8,r8,r6	
		vperm		v5,v5,v5,v9

		stvewx		v5,r3,r7
		add			r7,r7,r6	

		lvsr		v9,r3,r7				//load alignment vector for stores

		stvewx		v5,r3,r8
		add			r8,r8,r6	
		vperm		v6,v6,v6,v9

		stvewx		v6,r3,r7
		add			r7,r7,r6	

		lvsr		v9,r3,r7				//load alignment vector for stores

		stvewx		v6,r3,r8
		add			r8,r8,r6	
		vperm		v7,v7,v7,v9

		stvewx		v7,r3,r7
						
		stvewx		v7,r3,r8
	}
}

OIL_DEFINE_IMPL_FULL (recon8x8_inter_altivec, recon8x8_inter, OIL_IMPL_FLAG_ALTIVEC);

static void          /*      r3,             r4,       r5,             r6,         r7 */
recon8x8_inter2_altivec (uint8_t *dest, uint8_t *s1, uint8_t *s2, int16_t *change, int dsss)
{
	asm
	{
		//trying cache hints
		lis			r8,0x0108
		or			r8,r8,r7
		dstst		r3,r8,0

		xor			r8,r8,r8
		li			r9,16
		
		lvsl		v8,r4,r8				//load alignment vector for RefPtr1
		vxor		v9,v9,v9
		
		lvx			v10,r4,r8				//get 8 RefPtr1 -- 0
		add			r8,r8,r7					

		lvx			v0,r4,r9				//need another 16 bytes for misaligned data -- 0
		add			r9,r9,r7					

		lvx			v11,r4,r8				//get 8 RefPtr1 -- 1
		vperm		v10,v10,v0,v8

		lvsl		v8,r4,r8				//load alignment vector for RefPtr1
		add			r8,r8,r7					

		lvx			v1,r4,r9				//need another 16 bytes for misaligned data -- 1
		vmrghb		v10,v9,v10				//unsigned byte -> unsigned half		
		add			r9,r9,r7					

		lvx			v12,r4,r8				//get 8 RefPtr1 -- 2
		vperm		v11,v11,v1,v8

		lvsl		v8,r4,r8				//load alignment vector for RefPtr1
		add			r8,r8,r7					

		lvx			v2,r4,r9				//need another 16 bytes for misaligned data -- 2
		vmrghb		v11,v9,v11				//unsigned byte -> unsigned half		
		add			r9,r9,r7					

		lvx			v13,r4,r8				//get 8 RefPtr1 -- 3
		vperm		v12,v12,v2,v8

		lvsl		v8,r4,r8				//load alignment vector for RefPtr1
		add			r8,r8,r7					

		lvx			v3,r4,r9				//need another 16 bytes for misaligned data -- 3
		vmrghb		v12,v9,v12				//unsigned byte -> unsigned half		
		add			r9,r9,r7					

		lvx			v14,r4,r8				//get 8 RefPtr1 -- 4
		vperm		v13,v13,v3,v8

		lvsl		v8,r4,r8				//load alignment vector for RefPtr1
		add			r8,r8,r7					

		lvx			v4,r4,r9				//need another 16 bytes for misaligned data -- 4
		vmrghb		v13,v9,v13				//unsigned byte -> unsigned half		
		add			r9,r9,r7					

		lvx			v15,r4,r8				//get 8 RefPtr1 -- 5
		vperm		v14,v14,v4,v8

		lvsl		v8,r4,r8				//load alignment vector for RefPtr1
		add			r8,r8,r7					

		lvx			v5,r4,r9				//need another 16 bytes for misaligned data -- 5
		vmrghb		v14,v9,v14				//unsigned byte -> unsigned half		
		add			r9,r9,r7					

		lvx			v16,r4,r8				//get 8 RefPtr1 -- 6
		vperm		v15,v15,v5,v8

		lvsl		v8,r4,r8				//load alignment vector for RefPtr1
		add			r8,r8,r7					

		lvx			v6,r4,r9				//need another 16 bytes for misaligned data -- 6
		vmrghb		v15,v9,v15				//unsigned byte -> unsigned half		
		add			r9,r9,r7					

		lvx			v17,r4,r8				//get 8 RefPtr1 -- 7
		vperm		v16,v16,v6,v8

		lvsl		v8,r4,r8				//load alignment vector for RefPtr1
		add			r8,r8,r7					

		lvx			v7,r4,r9				//need another 16 bytes for misaligned data -- 7
		vmrghb		v16,v9,v16				//unsigned byte -> unsigned half		
		add			r9,r9,r7					
//--------
		vperm		v17,v17,v7,v8
		xor			r8,r8,r8
		li			r9,16

		lvsl		v18,r5,r8				//load alignment vector for RefPtr2
		vmrghb		v17,v9,v17				//unsigned byte -> unsigned half		
		
		lvx			v20,r5,r8				//get 8 RefPtr2 -- 0
		add			r8,r8,r7					

		lvx			v0,r5,r9				//need another 16 bytes for misaligned data -- 0
		add			r9,r9,r7					

		lvx			v21,r5,r8				//get 8 RefPtr2 -- 1
		vperm		v20,v20,v0,v18

		lvsl		v18,r5,r8				//load alignment vector for RefPtr2
		add			r8,r8,r7					

		lvx			v1,r5,r9				//need another 16 bytes for misaligned data -- 1
		vmrghb		v20,v9,v20				//unsigned byte -> unsigned half		
		add			r9,r9,r7					

		lvx			v22,r5,r8				//get 8 RefPtr2 -- 2
		vperm		v21,v21,v1,v18

		lvsl		v18,r5,r8				//load alignment vector for RefPtr2
		add			r8,r8,r7					

		lvx			v2,r5,r9				//need another 16 bytes for misaligned data -- 2
		vmrghb		v21,v9,v21				//unsigned byte -> unsigned half	
		vadduhm		v10,v10,v20	
		add			r9,r9,r7					

		lvx			v23,r5,r8				//get 8 RefPtr2 -- 3
		vperm		v22,v22,v2,v18

		lvsl		v18,r5,r8				//load alignment vector for RefPtr2
		add			r8,r8,r7					

		lvx			v3,r5,r9				//need another 16 bytes for misaligned data -- 3
		vmrghb		v22,v9,v22				//unsigned byte -> unsigned half		
		vadduhm		v11,v11,v21	
		add			r9,r9,r7					

		lvx			v24,r5,r8				//get 8 RefPtr2 -- 4
		vperm		v23,v23,v3,v18

		lvsl		v18,r5,r8				//load alignment vector for RefPtr2
		add			r8,r8,r7					

		lvx			v4,r5,r9				//need another 16 bytes for misaligned data -- 4
		vmrghb		v23,v9,v23				//unsigned byte -> unsigned half		
		vadduhm		v12,v12,v22	
		add			r9,r9,r7					

		lvx			v25,r5,r8				//get 8 RefPtr2 -- 5
		vperm		v24,v24,v4,v18

		lvsl		v18,r5,r8				//load alignment vector for RefPtr2
		add			r8,r8,r7					

		lvx			v5,r5,r9				//need another 16 bytes for misaligned data -- 5
		vmrghb		v24,v9,v24				//unsigned byte -> unsigned half		
		vadduhm		v13,v13,v23	
		add			r9,r9,r7					

		lvx			v26,r5,r8				//get 8 RefPtr2 -- 6
		vperm		v25,v25,v5,v18

		lvsl		v18,r5,r8				//load alignment vector for RefPtr2
		add			r8,r8,r7					

		lvx			v6,r5,r9				//need another 16 bytes for misaligned data -- 6
		vmrghb		v25,v9,v25				//unsigned byte -> unsigned half		
		vadduhm		v14,v14,v24	
		add			r9,r9,r7					

		lvx			v27,r5,r8				//get 8 RefPtr2 -- 7
		vperm		v26,v26,v6,v18

		lvsl		v18,r5,r8				//load alignment vector for RefPtr2
		add			r8,r8,r7					

		lvx			v7,r5,r9				//need another 16 bytes for misaligned data -- 7
		vmrghb		v26,v9,v26				//unsigned byte -> unsigned half		
		vadduhm		v15,v15,v25	
		add			r9,r9,r7					

		vperm		v27,v27,v7,v18
		xor			r8,r8,r8

		vmrghb		v27,v9,v27				//unsigned byte -> unsigned half		
		vadduhm		v16,v16,v26	

		vadduhm		v17,v17,v27	
		vspltish	v8,1
//--------
		lvx			v0,r6,r8				//get 8 shorts 				
		vsrh		v10,v10,v8
		addi		r8,r8,16

		lvx			v1,r6,r8				//get 8 shorts 				
		vsrh		v11,v11,v8
		addi		r8,r8,16

		lvx			v2,r6,r8				//get 8 shorts 				
		vsrh		v12,v12,v8
		addi		r8,r8,16

		lvx			v3,r6,r8				//get 8 shorts 				
		vsrh		v13,v13,v8
		addi		r8,r8,16

		lvx			v4,r6,r8				//get 8 shorts 				
		vsrh		v14,v14,v8
		addi		r8,r8,16

		lvx			v5,r6,r8				//get 8 shorts 				
		vsrh		v15,v15,v8
		addi		r8,r8,16

		lvx			v6,r6,r8				//get 8 shorts 				
		vsrh		v16,v16,v8
		addi		r8,r8,16

		lvx			v7,r6,r8				//get 8 shorts 				
		vsrh		v17,v17,v8
		xor			r8,r8,r8
//--------
		lvsr		v9,r3,r8				//load alignment vector for stores
		vaddshs		v0,v0,v10

		vaddshs		v1,v1,v11
		vpkshus		v0,v0,v0				

		vaddshs		v2,v2,v12
		vpkshus		v1,v1,v1				

		vaddshs		v3,v3,v13
		vpkshus		v2,v2,v2				

		vaddshs		v4,v4,v14
		vpkshus		v3,v3,v3				

		vaddshs		v5,v5,v15
		vpkshus		v4,v4,v4				

		vaddshs		v6,v6,v16
		vpkshus		v5,v5,v5				

		vaddshs		v7,v7,v17
		vpkshus		v6,v6,v6				

		vpkshus		v7,v7,v7

		li			r9,4
		vperm		v0,v0,v0,v9				//adjust for writes

		stvewx		v0,r3,r8
		add			r8,r8,r7	

		lvsr		v9,r3,r8				//load alignment vector for stores

		stvewx		v0,r3,r9
		add			r9,r9,r7	
		vperm		v1,v1,v1,v9

		stvewx		v1,r3,r8
		add			r8,r8,r7	

		lvsr		v9,r3,r8				//load alignment vector for stores

		stvewx		v1,r3,r9
		add			r9,r9,r7	
		vperm		v2,v2,v2,v9

		stvewx		v2,r3,r8
		add			r8,r8,r7	

		lvsr		v9,r3,r8				//load alignment vector for stores

		stvewx		v2,r3,r9
		add			r9,r9,r7	
		vperm		v3,v3,v3,v9

		stvewx		v3,r3,r8
		add			r8,r8,r7	

		lvsr		v9,r3,r8				//load alignment vector for stores

		stvewx		v3,r3,r9
		add			r9,r9,r7	
		vperm		v4,v4,v4,v9

		stvewx		v4,r3,r8
		add			r8,r8,r7	

		lvsr		v9,r3,r8				//load alignment vector for stores

		stvewx		v4,r3,r9
		add			r9,r9,r7	
		vperm		v5,v5,v5,v9

		stvewx		v5,r3,r8
		add			r8,r8,r7	

		lvsr		v9,r3,r8				//load alignment vector for stores

		stvewx		v5,r3,r9
		add			r9,r9,r7	
		vperm		v6,v6,v6,v9

		stvewx		v6,r3,r8
		add			r8,r8,r7	

		lvsr		v9,r3,r8				//load alignment vector for stores

		stvewx		v6,r3,r9
		add			r9,r9,r7	
		vperm		v7,v7,v7,v9

		stvewx		v7,r3,r8

		stvewx		v7,r3,r9
	}
}

OIL_DEFINE_IMPL_FULL (recon8x8_inter2_altivec, recon8x8_inter2, OIL_IMPL_FLAG_ALTIVEC);
