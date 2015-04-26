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

/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggTheora SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE Theora SOURCE CODE IS COPYRIGHT (C) 2002-2003                *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

  function:
  last mod: $Id: fdct8x8theora_altivec.c,v 1.1 2005-11-03 07:00:23 ds Exp $

 ********************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/liboilfuncs.h>
#include <liboil/dct/dct.h>
#include <math.h>

extern vector signed short idctConst;
extern vector unsigned char vPerm1;
extern vector unsigned char vPerm2;
extern vector unsigned char vPerm3;

OIL_DECLARE_CLASS(fdct8x8theora);

static void
fdct8x8theora_altivec(int16_t *src, int16_t *dest)
{
    (void) src;
    (void) dest;
    
    asm
    {
	    lwz         r10,vPerm1
	    xor         r7,r7,r7
		
	    lwz         r8,vPerm2

	    lwz         r9,vPerm3

	    lvx			v30,r10,r7
												
	    lvx			v31,r8,r7

	    lvx			v29,r9,r7


	    lwz         r9,idctConst
	    xor			r7,r7,r7
	   	xor			r8,r8,r8
	   	
//trying cache hints
//		lis			r8,0x1001				    //Block Size = 16, Block Count = 1, Block Stride = 0
//		dstst		r5,r8,0	  
//		dst 		r4,r8,1			
//		dst	    	r3,r8,2	
		
		lvx			v8,r9,r7
		xor			r8,r8,r8
			
		lvx			v10,r3,r8                   //row 0
		vsplth		v0,v8,0										
		addi		r8,r8,16

		lvx			v11,r3,r8                   //row 1
		vsplth		v1,v8,1										
		addi		r8,r8,16

		lvx			v12,r3,r8                   //row 2
		vsplth		v2,v8,2										
		addi		r8,r8,16

		lvx			v13,r3,r8                   //row 3
		vsplth		v3,v8,3										
		addi		r8,r8,16

		lvx			v14,r3,r8                   //row 4
		vsplth		v4,v8,4										
		addi		r8,r8,16

		lvx			v15,r3,r8                   //row 5
		vsplth		v5,v8,5										
		addi		r8,r8,16

		lvx			v16,r3,r8                   //row 6
		vsplth		v6,v8,6										
		addi		r8,r8,16

		lvx			v17,r3,r8                   //row 7
		vsplth		v7,v8,7		

                                                // on entry
                                                //00 01 02 03 04 05 06 07
                                        		//10 11 12 13 14 15 16 17
                                        		//20 21 22 23 24 25 26 27
                                        		//30 31 32 33 34 35 36 37
                                        		//40 41 42 43 44 45 46 47
                                        		//50 51 52 53 54 55 56 57 
                                        		//60 61 62 63 64 65 66 67
                                        		//70 71 72 73 74 75 76 77
//start of transpose
		vmrghh		v18,v10,v11					
		vmrglh		v19,v10,v11					
		vmrghh		v20,v12,v13					
		vmrglh		v21,v12,v13					
		vmrghh		v22,v14,v15					
		vmrglh		v23,v14,v15					
		vmrghh		v24,v16,v17					
		vmrglh		v25,v16,v17					
		
		vmrghw		v8,v18,v20					
		vmrghw		v9,v22,v24					
		vmrghw		v26,v19,v21					
		vmrghw		v27,v23,v25					
		vmrglw		v18,v18,v20					
		vmrglw		v22,v22,v24					
		vmrglw		v19,v19,v21					
		vmrglw		v23,v23,v25					
		
		vperm		v10,v8,v9,v30				//00 10 20 30 40 50 60 70
		vperm		v11,v8,v9,v31				//01 11 21 31 41 51 61 71
		vperm		v12,v18,v22,v30				//02 12 22 32 42 52 62 72
		vperm		v13,v18,v22,v31				//03 13 23 33 43 53 63 73
		vperm		v20,v26,v27,v30				//04 14 24 34 44 54 64 74
		vperm		v21,v26,v27,v31				//05 15 25 35 45 55 65 75 
		vperm		v22,v19,v23,v30				//06 16 26 36 46 56 66 76
		vperm		v23,v19,v23,v31				//07 17 27 37 47 57 67 77
//end of transpose		

//~~~~~~~~~~ start cut here
        vsubuhm     v14,v10,v23                 //id07
        vsubuhm     v15,v11,v12                 //id12
        vsubuhm     v16,v13,v20                 //id34
        vsubuhm     v17,v21,v22                 //id56

        vadduhm     v10,v10,v23                 //is07
        vadduhm     v11,v11,v12                 //is12
        vadduhm     v12,v13,v20                 //is34
        vadduhm     v13,v21,v22                 //is56

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// some precalulations
        vspltish    v28,15
        vadduhm     v18,v10,v12                 //is0734
        vadduhm     v19,v11,v13                 //is1256

        vsubuhm     v20,v11,v13                 //(is12 - is56)
        vmulesh     v22,v20,v4
        vmulosh     v23,v20,v4
        vperm       v8,v22,v23,v29              //(c4s4 * (is12 - is56)) - (is12 - is56)
        vadduhm     v8,v8,v20                   //c4s4 * (is12 - is56)
        vsrh        v20,v20,v28                 //get sign bit
        vadduhm     v8,v8,v20                   //add in sign bit aka icommon_product1
        
        vadduhm     v20,v15,v17                 //(id12 + id56)
        vmulesh     v22,v20,v4
        vmulosh     v23,v20,v4
        vperm       v9,v22,v23,v29              //(c4s4 * (is12 + is56)) - (is12 + is56)
        vadduhm     v9,v9,v20                   //c4s4 * (is12 + is56)
        vsrh        v20,v20,v28                 //get sign bit
        vadduhm     v9,v9,v20                   //add in sign bit aka icommon_product2

        vsubuhm     v20,v15,v17                  //irot_input_x = id12 - id56
        vsubuhm     v21,v10,v12                 //irot_input_y = is07 - is34
        
        vadduhm     v22,v14,v8                  //irot_input_x = icommon_product1 + id07
        vadduhm     v23,v16,v9                  //irot_input_y = icommon_product2 + id34
        vxor        v24,v24,v24
        vsubuhm     v23,v24,v23                 //irot_input_y = -(icommon_product2 + id34)

        vsubuhm     v24,v14,v8                  //irot_input_x = id07 - icommon_product1
        vsubuhm     v25,v16,v9                  //irot_input_y = id34 - icommon_product2

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ip[0], ip[4]
        vadduhm     v8,v18,v19                  //(is0734 + is1256)
        vsubuhm     v9,v18,v19                  //(is0734 - is1256)

        vmulesh     v18,v8,v4
        vmulosh     v19,v8,v4
        vperm       v18,v18,v19,v29             //(c4s4 * (is0734 + is1256)) - (is0734 + is1256)
        vadduhm     v18,v18,v8                  //(c4s4 * (is0734 + is1256))
        vsrh        v8,v8,v28
        vadduhm     v10,v18,v8                  //add in sign bit aka ip[0]
        
        vmulesh     v18,v9,v4
        vmulosh     v19,v9,v4
        vperm       v18,v18,v19,v29             //(c4s4 * (is0734 + is1256)) - (is0734 + is1256)
        vadduhm     v18,v18,v9                  //(c4s4 * (is0734 + is1256))
        vsrh        v9,v9,v28
        vadduhm     v14,v18,v9                  //add in sign bit aka ip[4]
        
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ip[2], ip[6]
        vmulesh     v18,v20,v6
        vmulosh     v19,v20,v6
        vperm       v18,v18,v19,v29             //(c6s2 * (irot_input_x))
        vsrh        v8,v20,v28
        vadduhm     v8,v18,v8                   //add in sign bit aka temp1
        vmulesh     v18,v21,v2
        vmulosh     v19,v21,v2
        vperm       v18,v18,v19,v29             //(c2s6 * (irot_input_y)) - irot_input_y
        vadduhm     v18,v18,v21                 //(c2s6 * (irot_input_y)) 
        vsrh        v9,v21,v28
        vadduhm     v9,v18,v9                   //add in sign bit aka temp2
        vadduhm     v12,v8,v9                   //ip[2]
        
        vmulesh     v18,v21,v6
        vmulosh     v19,v21,v6
        vperm       v18,v18,v19,v29             //(c6s2 * (irot_input_y))
        vsrh        v8,v21,v28
        vadduhm     v8,v18,v8                   //add in sign bit aka temp1
        vmulesh     v18,v20,v2
        vmulosh     v19,v20,v2
        vperm       v18,v18,v19,v29             //(c2s6 * (irot_input_x)) - irot_input_x
        vadduhm     v18,v18,v20                 //(c2s6 * (irot_input_x)) 
        vsrh        v9,v20,v28
        vadduhm     v9,v18,v9                   //add in sign bit aka temp2
        vsubuhm     v16,v8,v9                   //ip[6]

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ip[1], ip[7]
        vmulesh     v18,v22,v1
        vmulosh     v19,v22,v1
        vperm       v18,v18,v19,v29             //(c1s7 * (irot_input_x)) - irot_input_x
        vadduhm     v18,v18,v22                 //(c1s7 * (irot_input_x))
        vsrh        v8,v22,v28
        vadduhm     v8,v18,v8                   //add in sign bit aka temp1
        vmulesh     v18,v23,v7
        vmulosh     v19,v23,v7
        vperm       v18,v18,v19,v29             //(c7s1 * (irot_input_y))
        vsrh        v9,v23,v28
        vadduhm     v9,v18,v9                   //add in sign bit aka temp2
        vsubuhm     v11,v8,v9                   //ip[1]

        vmulesh     v18,v22,v7
        vmulosh     v19,v22,v7
        vperm       v18,v18,v19,v29             //(c7s1 * (irot_input_x))
        vsrh        v8,v22,v28
        vadduhm     v8,v18,v8                   //add in sign bit aka temp1
        vmulesh     v18,v23,v1
        vmulosh     v19,v23,v1
        vperm       v18,v18,v19,v29             //(c1s7 * (irot_input_y)) - irot_input_y
        vadduhm     v18,v18,v23                 //(c1s7 * (irot_input_7))
        vsrh        v9,v23,v28
        vadduhm     v9,v18,v9                   //add in sign bit aka temp2
        vadduhm     v17,v8,v9                   //ip[7]

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ip[3], ip[5]
        vmulesh     v18,v24,v3
        vmulosh     v19,v24,v3
        vperm       v18,v18,v19,v29             //(c3s5 * (irot_input_x)) - irot_input_x
        vadduhm     v18,v18,v24                 //(c3s5 * (irot_input_x))
        vsrh        v8,v24,v28
        vadduhm     v8,v18,v8                   //add in sign bit aka temp1
        vmulesh     v18,v25,v5
        vmulosh     v19,v25,v5
        vperm       v18,v18,v19,v29             //(c5s3 * (irot_input_y)) - irot_input_y
        vadduhm     v18,v18,v25                 //(c5s3 * (irot_input_y))
        vsrh        v9,v25,v28
        vadduhm     v9,v18,v9                   //add in sign bit aka temp2
        vsubuhm     v13,v8,v9                   //ip[3]

        vmulesh     v18,v24,v5
        vmulosh     v19,v24,v5
        vperm       v18,v18,v19,v29             //(c5s3 * (irot_input_x)) - irot_input_x
        vadduhm     v18,v18,v24                 //(c5s3 * (irot_input_x))
        vsrh        v8,v24,v28
        vadduhm     v8,v18,v8                   //add in sign bit aka temp1
        vmulesh     v18,v25,v3
        vmulosh     v19,v25,v3
        vperm       v18,v18,v19,v29             //(c3s5 * (irot_input_y)) - irot_input_y
        vadduhm     v18,v18,v25                 //(c3s5 * (irot_input_y))
        vsrh        v9,v25,v28
        vadduhm     v9,v18,v9                   //add in sign bit aka temp2
        vadduhm     v15,v8,v9                   //ip[5]
//~~~~~~~~~~ end cut here

//~~~~~~~~~~~~~~~ transpose back
//start of transpose
		vmrghh		v18,v10,v11					//00 01 10 11 20 21 30 31 
		vmrglh		v19,v10,v11					//40 41 50 51 60 61 70 71
		vmrghh		v20,v12,v13					//02 03 12 13 22 23 32 33
		vmrglh		v21,v12,v13					//42 43 52 53 62 63 72 73
		vmrghh		v22,v14,v15					//04 05 14 15 24 25 34 35
		vmrglh		v23,v14,v15					//44 45 54 55 64 65 74 75
		vmrghh		v24,v16,v17					//06 07 16 17 26 27 36 37
		vmrglh		v25,v16,v17					//46 47 56 57 66 67 76 77
		
		vmrghw		v8,v18,v20					//00 01 02 03 10 11 12 13
		vmrghw		v9,v22,v24					//04 05 06 07 14 15 16 17
		vmrghw		v26,v19,v21					//40 41 42 43 50 51 52 53
		vmrghw		v27,v23,v25					//44 45 46 47 54 55 56 57
		vmrglw		v18,v18,v20					//20 21 22 23 30 31 32 33
		vmrglw		v22,v22,v24					//24 25 26 27 34 35 36 37
		vmrglw		v19,v19,v21					//60 61 62 63 70 71 72 73
		vmrglw		v23,v23,v25					//64 65 66 67 74 75 76 77
		
		vperm		v10,v8,v9,v30				//00 01 02 03 04 05 06 07
		vperm		v11,v8,v9,v31				//10 11 12 13 14 15 16 17
		vperm		v12,v18,v22,v30				//20 21 22 23 24 25 26 27
		vperm		v13,v18,v22,v31				//30 31 32 33 34 35 36 37
		vperm		v20,v26,v27,v30				//40 41 42 43 44 45 46 47
		vperm		v21,v26,v27,v31				//50 51 52 53 54 55 56 57 
		vperm		v22,v19,v23,v30				//60 61 62 63 64 65 66 67
		vperm		v23,v19,v23,v31				//70 71 72 73 74 75 76 77
//end of transpose	
//~~~~~~~~~~ start cut here
        vsubuhm     v14,v10,v23                 //id07
        vsubuhm     v15,v11,v12                 //id12
        vsubuhm     v16,v13,v20                 //id34
        vsubuhm     v17,v21,v22                 //id56

        vadduhm     v10,v10,v23                 //is07
        vadduhm     v11,v11,v12                 //is12
        vadduhm     v12,v13,v20                 //is34
        vadduhm     v13,v21,v22                 //is56

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// some precalulations
        vspltish    v28,15
        vadduhm     v18,v10,v12                 //is0734
        vadduhm     v19,v11,v13                 //is1256

        vsubuhm     v20,v11,v13                 //(is12 - is56)
        vmulesh     v22,v20,v4
        vmulosh     v23,v20,v4
        vperm       v8,v22,v23,v29              //(c4s4 * (is12 - is56)) - (is12 - is56)
        vadduhm     v8,v8,v20                   //c4s4 * (is12 - is56)
        vsrh        v20,v20,v28                 //get sign bit
        vadduhm     v8,v8,v20                   //add in sign bit aka icommon_product1
        
        vadduhm     v20,v15,v17                 //(id12 + id56)
        vmulesh     v22,v20,v4
        vmulosh     v23,v20,v4
        vperm       v9,v22,v23,v29              //(c4s4 * (is12 + is56)) - (is12 + is56)
        vadduhm     v9,v9,v20                   //c4s4 * (is12 + is56)
        vsrh        v20,v20,v28                 //get sign bit
        vadduhm     v9,v9,v20                   //add in sign bit aka icommon_product2

        vsubuhm     v20,v15,v17                  //irot_input_x = id12 - id56
        vsubuhm     v21,v10,v12                 //irot_input_y = is07 - is34
        
        vadduhm     v22,v14,v8                  //irot_input_x = icommon_product1 + id07
        vadduhm     v23,v16,v9                  //irot_input_y = icommon_product2 + id34
        vxor        v24,v24,v24
        vsubuhm     v23,v24,v23                 //irot_input_y = -(icommon_product2 + id34)

        vsubuhm     v24,v14,v8                  //irot_input_x = id07 - icommon_product1
        vsubuhm     v25,v16,v9                  //irot_input_y = id34 - icommon_product2

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ip[0], ip[4]
        vadduhm     v8,v18,v19                  //(is0734 + is1256)
        vsubuhm     v9,v18,v19                  //(is0734 - is1256)

        vmulesh     v18,v8,v4
        vmulosh     v19,v8,v4
        vperm       v18,v18,v19,v29             //(c4s4 * (is0734 + is1256)) - (is0734 + is1256)
        vadduhm     v18,v18,v8                  //(c4s4 * (is0734 + is1256))
        vsrh        v8,v8,v28
        vadduhm     v10,v18,v8                  //add in sign bit aka ip[0]
        
        vmulesh     v18,v9,v4
        vmulosh     v19,v9,v4
        vperm       v18,v18,v19,v29             //(c4s4 * (is0734 + is1256)) - (is0734 + is1256)
        vadduhm     v18,v18,v9                  //(c4s4 * (is0734 + is1256))
        vsrh        v9,v9,v28
        vadduhm     v14,v18,v9                  //add in sign bit aka ip[4]
        
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ip[2], ip[6]
        vmulesh     v18,v20,v6
        vmulosh     v19,v20,v6
        vperm       v18,v18,v19,v29             //(c6s2 * (irot_input_x))
        vsrh        v8,v20,v28
        vadduhm     v8,v18,v8                   //add in sign bit aka temp1
        vmulesh     v18,v21,v2
        vmulosh     v19,v21,v2
        vperm       v18,v18,v19,v29             //(c2s6 * (irot_input_y)) - irot_input_y
        vadduhm     v18,v18,v21                 //(c2s6 * (irot_input_y)) 
        vsrh        v9,v21,v28
        vadduhm     v9,v18,v9                   //add in sign bit aka temp2
        vadduhm     v12,v8,v9                   //ip[2]
        
        vmulesh     v18,v21,v6
        vmulosh     v19,v21,v6
        vperm       v18,v18,v19,v29             //(c6s2 * (irot_input_y))
        vsrh        v8,v21,v28
        vadduhm     v8,v18,v8                   //add in sign bit aka temp1
        vmulesh     v18,v20,v2
        vmulosh     v19,v20,v2
        vperm       v18,v18,v19,v29             //(c2s6 * (irot_input_x)) - irot_input_x
        vadduhm     v18,v18,v20                 //(c2s6 * (irot_input_x)) 
        vsrh        v9,v20,v28
        vadduhm     v9,v18,v9                   //add in sign bit aka temp2
        vsubuhm     v16,v8,v9                   //ip[6]

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ip[1], ip[7]
        vmulesh     v18,v22,v1
        vmulosh     v19,v22,v1
        vperm       v18,v18,v19,v29             //(c1s7 * (irot_input_x)) - irot_input_x
        vadduhm     v18,v18,v22                 //(c1s7 * (irot_input_x))
        vsrh        v8,v22,v28
        vadduhm     v8,v18,v8                   //add in sign bit aka temp1
        vmulesh     v18,v23,v7
        vmulosh     v19,v23,v7
        vperm       v18,v18,v19,v29             //(c7s1 * (irot_input_y))
        vsrh        v9,v23,v28
        vadduhm     v9,v18,v9                   //add in sign bit aka temp2
        vsubuhm     v11,v8,v9                   //ip[1]

        vmulesh     v18,v22,v7
        vmulosh     v19,v22,v7
        vperm       v18,v18,v19,v29             //(c7s1 * (irot_input_x))
        vsrh        v8,v22,v28
        vadduhm     v8,v18,v8                   //add in sign bit aka temp1
        vmulesh     v18,v23,v1
        vmulosh     v19,v23,v1
        vperm       v18,v18,v19,v29             //(c1s7 * (irot_input_y)) - irot_input_y
        vadduhm     v18,v18,v23                 //(c1s7 * (irot_input_7))
        vsrh        v9,v23,v28
        vadduhm     v9,v18,v9                   //add in sign bit aka temp2
        vadduhm     v17,v8,v9                   //ip[7]

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ip[3], ip[5]
        vmulesh     v18,v24,v3
        vmulosh     v19,v24,v3
        vperm       v18,v18,v19,v29             //(c3s5 * (irot_input_x)) - irot_input_x
        vadduhm     v18,v18,v24                 //(c3s5 * (irot_input_x))
        vsrh        v8,v24,v28
        vadduhm     v8,v18,v8                   //add in sign bit aka temp1
        vmulesh     v18,v25,v5
        vmulosh     v19,v25,v5
        vperm       v18,v18,v19,v29             //(c5s3 * (irot_input_y)) - irot_input_y
        vadduhm     v18,v18,v25                 //(c5s3 * (irot_input_y))
        vsrh        v9,v25,v28
        vadduhm     v9,v18,v9                   //add in sign bit aka temp2
        vsubuhm     v13,v8,v9                   //ip[3]

        vmulesh     v18,v24,v5
        vmulosh     v19,v24,v5
        vperm       v18,v18,v19,v29             //(c5s3 * (irot_input_x)) - irot_input_x
        vadduhm     v18,v18,v24                 //(c5s3 * (irot_input_x))
        vsrh        v8,v24,v28
        vadduhm     v8,v18,v8                   //add in sign bit aka temp1
        vmulesh     v18,v25,v3
        vmulosh     v19,v25,v3
        vperm       v18,v18,v19,v29             //(c3s5 * (irot_input_y)) - irot_input_y
        vadduhm     v18,v18,v25                 //(c3s5 * (irot_input_y))
        vsrh        v9,v25,v28
        vadduhm     v9,v18,v9                   //add in sign bit aka temp2
        vadduhm     v15,v8,v9                   //ip[5]
//~~~~~~~~~~ end cut here

//~~~~~~~~~~ write to destination
		xor			r8,r8,r8
		
		stvx		v10,r4,r8
		addi		r8,r8,16

		stvx		v11,r4,r8
		addi		r8,r8,16

		stvx		v12,r4,r8
		addi		r8,r8,16

		stvx		v13,r4,r8
		addi		r8,r8,16

		stvx		v14,r4,r8
		addi		r8,r8,16

		stvx		v15,r4,r8
		addi		r8,r8,16

		stvx		v16,r4,r8
		addi		r8,r8,16

		stvx		v17,r4,r8
    }
    
}
OIL_DEFINE_IMPL_REF (fdct8x8theora_altivec, fdct8x8theora, OIL_IMPL_FLAG_ALTIVEC);
