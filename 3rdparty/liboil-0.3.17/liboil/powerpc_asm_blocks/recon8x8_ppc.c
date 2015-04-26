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

#ifdef ENABLE_BROKEN_IMPLS
static void
                    /*       r3,            r4,         r5 */
recon8x8_intra_ppc (uint8_t *dest, int16_t *change, int ds)
{
	asm
	{
		lwz		r0,0(r3)				;//preload cache
		mr		r12,r4
		
		addi	r12,r12,128				;//end ptr 
		
doLoop1:
		lha		r7,0(r4)
		
		lha		r8,2(r4)
		addi	r7,r7,128
	
		lha		r9,4(r4)
		addi	r8,r8,128		
		andi.	r0,r7,0xff00
		beq+		L1
		
		srawi	r0,r7,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r7,r0,0xff   			;//now have 00 or ff

L1:			
		lha		r10,6(r4)
		addi	r9,r9,128
		andi.	r0,r8,0xff00	
		beq+		L2	
	
		srawi	r0,r8,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r8,r0,0xff   			;//now have 00 or ff

L2:			
		lha		r31,8(r4)
		addi 	r10,r10,128
		andi.	r0,r9,0xff00
		beq+		L3
		
		srawi	r0,r9,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r9,r0,0xff   			;//now have 00 or ff

L3:					
		lha		r30,10(r4)
		andi.	r0,r10,0xff00
		beq+		L4
		
		srawi	r0,r10,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r10,r0,0xff   			;//now have 00 or ff

L4:
		lha		r29,12(r4)
		insrwi	r10,r7,8,0
		addi	r31,r31,128
		
		lwz		r27,0(r3)				;//preload cache with dest
		addi 	r30,r30,128
		andi.	r0,r31,0xff00
		beq+		L5
		
		srawi	r0,r31,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r31,r0,0xff   			;//now have 00 or ff

L5:
		lha		r28,14(r4)
		addi 	r29,r29,128
		andi.	r0,r30,0xff00
		beq+		L6
		
		srawi	r0,r30,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r30,r0,0xff   			;//now have 00 or ff
		
L6:		
		addi 	r28,r28,128
		andi.	r0,r29,0xff00
		beq+		L7
		
		srawi	r0,r29,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r29,r0,0xff   			;//now have 00 or ff
		
L7:		
		insrwi	r10,r8,8,8
		andi.	r0,r28,0xff00
		beq+		L8
		
		srawi	r0,r28,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r28,r0,0xff   			;//now have 00 or ff
		
L8:		
		insrwi	r10,r9,8,16
		insrwi	r28,r31,8,0
		
		stw		r10,0(r3)
		insrwi	r28,r30,8,8
		addi 	r4,r4,16		
		
		cmpw	r4,r12
		insrwi	r28,r29,8,16

		stw		r28,4(r3)
		add 	r3,r3,r5 				;//add in stride
		bne		doLoop1

	}	
}

OIL_DEFINE_IMPL_FULL (recon8x8_intra_ppc, recon8x8_intra, OIL_IMPL_FLAG_ASM);
#endif

#ifdef ENABLE_BROKEN_IMPLS
static void          /*      r3,            r4,           r5,         r6 */
recon8x8_inter_ppc (uint8_t *dest, uint8_t *src, int16_t *change, int dss)
{
	asm
	{
		mr		r26,r4
		mr		r4,r5					;//same reg usage as intra
			
		lwz		r0,0(r3)				;//preload cache
		mr		r12,r4
		
		addi	r12,r12,128				;//end ptr 
		mr		r5,r6					;//same reg usage as intra
		
doLoop1:
		lha		r7,0(r4)
		
		lbz		r25,0(r26)
		
		lha		r8,2(r4)
		add 	r7,r7,r25
	
		lbz		r25,1(r26)

		lha		r9,4(r4)
		add		r8,r8,r25		
		andi.	r0,r7,0xff00
		beq+		L1
		
		srawi	r0,r7,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r7,r0,0xff   			;//now have 00 or ff

L1:			
		lbz		r25,2(r26)

		lha		r10,6(r4)
		add 	r9,r9,r25
		andi.	r0,r8,0xff00	
		beq+		L2	
	
		srawi	r0,r8,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r8,r0,0xff   			;//now have 00 or ff

L2:			
		lbz		r25,3(r26)

		lha		r31,8(r4)
		add  	r10,r10,r25
		andi.	r0,r9,0xff00
		beq+		L3
		
		srawi	r0,r9,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r9,r0,0xff   			;//now have 00 or ff

L3:					
		lha		r30,10(r4)
		andi.	r0,r10,0xff00
		beq+		L4
		
		srawi	r0,r10,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r10,r0,0xff   			;//now have 00 or ff

L4:
		lbz		r25,4(r26)


		lha		r29,12(r4)
		insrwi	r10,r7,8,0
		add 	r31,r31,r25

		lbz		r25,5(r26)
		
		lwz		r27,0(r3)				;//preload cache with dest
		add  	r30,r30,r25
		andi.	r0,r31,0xff00
		beq+		L5
		
		srawi	r0,r31,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r31,r0,0xff   			;//now have 00 or ff

L5:
		lbz		r25,6(r26)

		lha		r28,14(r4)
		add  	r29,r29,r25
		andi.	r0,r30,0xff00
		beq+		L6
		
		srawi	r0,r30,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r30,r0,0xff   			;//now have 00 or ff
		
L6:		
		lbz		r25,7(r26)
		add		r26,r26,r5

		add  	r28,r28,r25
		andi.	r0,r29,0xff00
		beq+		L7
		
		srawi	r0,r29,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r29,r0,0xff   			;//now have 00 or ff
		
L7:		
		insrwi	r10,r8,8,8
		andi.	r0,r28,0xff00
		beq+		L8
		
		srawi	r0,r28,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r28,r0,0xff   			;//now have 00 or ff
		
L8:		
		insrwi	r10,r9,8,16
		insrwi	r28,r31,8,0
		
		stw		r10,0(r3)
		insrwi	r28,r30,8,8
		addi 	r4,r4,16		
		
		cmpw	r4,r12
		insrwi	r28,r29,8,16

		stw		r28,4(r3)
		add 	r3,r3,r5 				;//add in stride
		bne		doLoop1

	}
}
OIL_DEFINE_IMPL_FULL (recon8x8_inter_ppc, recon8x8_inter, OIL_IMPL_FLAG_ASM);
#endif

#ifdef ENABLE_BROKEN_IMPLS
static void          /*      r3,             r4,       r5,             r6,         r7 */
recon8x8_inter2_ppc (uint8_t *dest, uint8_t *s1, uint8_t *s2, int16_t *change, int dsss)
{
	asm
	{
		mr		r26,r4
		mr		r4,r6					;//same reg usage as intra
			
		lwz		r0,0(r3)				;//preload cache
		mr		r25,r5	
		mr		r12,r4
		
		addi	r12,r12,128				;//end ptr 
		mr		r5,r7					;//same reg usage as intra
		
		li		r24,0x0101
		//li		r23,0xfefe
		li		r23,-258
		
		insrwi	r23,r23,16,0			;//0xfefefefe
		insrwi	r24,r24,16,0			;//0x01010101
		
doLoop1:
		lwz		r22,0(r26)				;//get 4 ref pels		

		lwz		r21,0(r25)				;//get 4 src pels
		
		lha		r7,0(r4)
		and		r20,r22,r21

		lha		r8,2(r4)
		and		r21,r21,r23				;//mask low bits
		and		r22,r22,r23				;//mask low bits
	
		srwi	r21,r21,1
		srwi	r22,r22,1
		
		and		r20,r20,r24				;//save low bits
		add		r21,r21,r22
		
		lwz		r22,4(r26)				;//get 4 ref pels		
//		or		r20,r21,r20				;//add in hot fudge		
		add		r20,r21,r20				;//add in hot fudge		

//xor r20,r20,r20

		lwz		r21,4(r25)				;//get 4 src pels
		rlwinm	r19,r20,8,24,31
		rlwinm	r18,r20,16,24,31		

		add 	r7,r7,r19
		
		lha		r9,4(r4)
		add		r8,r8,r18		
		andi.	r0,r7,0xff00
		beq+		L1
		
		srawi	r0,r7,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r7,r0,0xff   			;//now have 00 or ff

L1:			
		rlwinm	r19,r20,24,24,31
		rlwinm	r18,r20,0,24,31		

		lha		r10,6(r4)
		add 	r9,r9,r19
		andi.	r0,r8,0xff00	
		beq+		L2	
	
		srawi	r0,r8,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r8,r0,0xff   			;//now have 00 or ff

L2:			
		lha		r31,8(r4)
		add  	r10,r10,r18
		andi.	r0,r9,0xff00
		beq+		L3
		
		srawi	r0,r9,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r9,r0,0xff   			;//now have 00 or ff

L3:					
		lha		r30,10(r4)
		andi.	r0,r10,0xff00
		beq+		L4
		
		srawi	r0,r10,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r10,r0,0xff   			;//now have 00 or ff

L4:
		lha		r29,12(r4)
		insrwi	r10,r7,8,0
		and		r20,r22,r21

		and		r21,r21,r23				;//mask low bits
		and		r22,r22,r23				;//mask low bits
	
		srwi	r21,r21,1
		srwi	r22,r22,1
		
		and		r20,r20,r24				;//save low bits
		add		r21,r21,r22
		
//		or		r20,r21,r20				;//add in hot fudge		
		add		r20,r21,r20				;//add in hot fudge		
	
		rlwinm	r19,r20,8,24,31
		rlwinm	r18,r20,16,24,31		
	
	
		add 	r31,r31,r19

//xor r20,r20,r20

		lwz		r27,0(r3)				;//preload cache with dest
		add  	r30,r30,r18
		andi.	r0,r31,0xff00
		beq+		L5
		
		srawi	r0,r31,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r31,r0,0xff   			;//now have 00 or ff

L5:
		rlwinm	r19,r20,24,24,31
		rlwinm	r18,r20,0,24,31		

		lha		r28,14(r4)
		add  	r29,r29,r19
		andi.	r0,r30,0xff00
		beq+		L6
		
		srawi	r0,r30,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r30,r0,0xff   			;//now have 00 or ff
		
L6:		
		add		r26,r26,r5				;//add stride to ref pels
		add		r25,r25,r5				;//add stride to src pels

		add  	r28,r28,r18
		andi.	r0,r29,0xff00
		beq+		L7
		
		srawi	r0,r29,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r29,r0,0xff   			;//now have 00 or ff
		
L7:		
		insrwi	r10,r8,8,8
		andi.	r0,r28,0xff00
		beq+		L8
		
		srawi	r0,r28,15				;//generate ff or 00
		
		xori	r0,r0,0xff				;//flip the bits
		
		andi.	r28,r0,0xff   			;//now have 00 or ff
		
L8:		
		insrwi	r10,r9,8,16
		insrwi	r28,r31,8,0
		
		stw		r10,0(r3)
		insrwi	r28,r30,8,8
		addi 	r4,r4,16		
		
		cmpw	r4,r12
		insrwi	r28,r29,8,16

		stw		r28,4(r3)
		add 	r3,r3,r5 				;//add in stride
		bne		doLoop1

	}
}

OIL_DEFINE_IMPL_FULL (recon8x8_inter2_ppc, recon8x8_inter2, OIL_IMPL_FLAG_ASM);
#endif

