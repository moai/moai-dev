//------------------------------------------------------------------
// file:  vec_memset.S
//    AltiVec enabled version of memset and bzero and cacheable_memzero
//------------------------------------------------------------------

//------------------------------------------------------------------
//	Copyright Motorola, Inc. 2002
//	ALL RIGHTS RESERVED
//
//	You are hereby granted a copyright license to use, modify, and 
//	distribute the SOFTWARE so long as this entire notice is retained 
//	without alteration in any modified and/or redistributed versions, 
//	and that such modified versions are clearly identified as such.  
//	No licenses are granted by implication, estoppel or otherwise under 
//	any patents or trademarks of Motorola, Inc.
//
//	The SOFTWARE is provided on an "AS IS" basis and without warranty.  
//	To the maximum extent permitted by applicable law, MOTOROLA DISCLAIMS 
//	ALL WARRANTIES WHETHER EXPRESS OR IMPLIED, INCLUDING IMPLIED 
//	WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
//	PURPOSE AND ANY WARRANTY AGAINST INFRINGEMENT WITH 
//	REGARD TO THE SOFTWARE (INCLUDING ANY MODIFIED VERSIONS 
//	THEREOF) AND ANY ACCOMPANYING WRITTEN MATERIALS. 
//
//	To the maximum extent permitted by applicable law, IN NO EVENT SHALL 
//	MOTOROLA BE LIABLE FOR ANY DAMAGES WHATSOEVER 
//	(INCLUDING WITHOUT LIMITATION, DAMAGES FOR LOSS OF 
//	BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS 
//	INFORMATION, OR OTHER PECUNIARY LOSS) ARISING OF THE USE OR 
//	INABILITY TO USE THE SOFTWARE.   Motorola assumes no responsibility 
//	for the maintenance and support of the SOFTWARE.
//------------------------------------------------------------------

//------------------------------------------------------------------
// extern void *memset( void *ptr, int val, size_t len );
//   Copies val into each of len characters beginning at ptr.
//                                       - Harbison&Steele 4th ed
//    (despite val being an int, this memset assumes it is never
//     more than a byte.  That seems to be correct from all the
//     memset functions I've seen but I don't know if ANSI allows
//     anthing longer.     Chuck Corley  12/21/02) 
// Returns:
//  void * ptr
//------------------------------------------------------------------

//------------------------------------------------------------------
// extern void * bzero( char *ptr, int len);   
//   Copies 0 into each of len characters at ptr.
//                                       - Harbison&Steele 4th ed
// Returns:
//  void * ptr
//------------------------------------------------------------------

// Revision History:
//    Rev 0.0	Original                        Chuck Corley	02/09/03
//              Could benefit from changes added to memcpy
//    Rev 0.1	Revised per memcpy Rev 0.30     Chuck Corley	05/01/03
//
//  This is beta quality code; users are encouraged to make it faster.
//  ASSUMPTIONS:
//     Code is highly likely to be in the cache; data is not (streaming data)
//     Zero fill could be quite likely.
//     Moving fill byte from GPR to VR as below faster than stw->lvebx via stack

#define VRSV 256	//	VRSAVE spr
// Don't use vectors for BC <= MIN_VEC. Works only if MIN_VEC >= 16 bytes.
#define MIN_VEC 16

// Register useage
#define Rt r0	// 	r0 when used as a temporary register	

#define DST r3	// 	entering: dest pointer; exiting: same dest pointer

#define FILL r4	// 	entering: fill char then fill word

#define BC r5	//	entering: Byte_Count then remaining Byte_Count

#define DBC r6//	dst + byte count

#define BK r7	//  	BC - 1 +/- (n*16)

#define Fsh r8	//	fill byte shifted right one nibble

#define DM1 r9//	dst -1 for byte-by-byte backwards initially
#define D r9	//	(dst+16)[0:27] - dst[28:31]
#define DNX r9	//	(dst+n*16)[28:31]
#define BL r9	//	second byte_kount index pointer

#define DR r10	//	(dst+16)[0:27]
#define QW r10	//  	number of cache lines

#define DBK r11	//	(dst+byte_count-1) then (dst+byte_count-1)[28:31]

#define RSV r12	//  	storage for VRSAVE register if used

//  Condition register use (not including temporary cr0)
//      cr0[2]   = (FILL==0)?
//      cr1[0,2] = (BC == 0)? 1 : 0; (nothing to move)
// then cr1[2]   = (DST[28:31] == 0)? 1 : 0;  (D0 left justified)
// then cr1[2]   = ((DBK = DST+BC-1)[28:31] = 0xF)? 1 : 0; (DN right justified)
//      cr6[2]   = (QW == 0)? 1 : 0;
// then cr6[1]   = (QW > 4)? 1 : 0; (>4 vectors to move?)
// then cr6[3]   = (third store[27] == 1)? 1: 0; (cache line alignment)
// then cr6[3]   = (last store[27] == 1)? 1: 0; (last store odd?)
//      cr7[2]   = (BC>MIN_VEC)?1:0;  (BC big enough to warrant vectors)
// then cr7[0:3] = (DST+16)[0:27]-DST  (How many bytes (iff <16) in first vector?)
// then cr7[0:3] = (DST+BC)[0:27]  (How many bytes (iff <16) in last vector?)

// Conditionalize the use of dcba.  It will help if the data is
// not in cache and hurt if it is.  Generally, except for small
// benchmarks repeated many times, we assume data is not in cache
// (data streaming) and using dcba is a performance boost.
// We use dcba which will noop to non-cacheable memory rather than
// dcbz which will cause an aligment exception.
#ifndef NO_DCBA
#if defined(__GNUC__) || defined(__MWERKS__) || defined(_DIAB_TOOL)
 // gcc and codewarrior and diab don't assemble dcba
#define DCBK .long 0x7c033dec
// dcba r3,r7    or    dcba DST,BK
#else
#ifdef __ghs__
.macro DCBK
.long 0x7c033dec
.endm
#else
#define DCBK dcba DST,BK
#endif  // __ghs__
#endif  // __GNUC__ or __MWERKS__
#else
#define DCBK nop
#endif  // NO_DCBA

	.text
#ifdef __MWERKS__
	.align	32
#else
	.align	5
#endif

#ifdef LIBMOTOVEC
	.globl	memset     
memset:
#else
	.globl	_vec_memset     
_vec_memset:
#endif

	cmpi	cr7,0,BC,MIN_VEC	// IU1 Check for minimum byte count
	cmpi	cr1,0,BC,0	// IU1 Eliminate zero byte count
	rlwinm.	Fsh,FILL,28,28,3 // IU1 Is fill byte zero? and shift

	addi	DM1,DST,-1	// IU1 Pre-bias and duplicate destination
	addi	DR,DST,16	// IU1 Address of second dst vector
	add	DBC,DST,BC	// IU1 Address of last dst byte + 1
	bgt	cr7,v_memset	// b if BC>MIN_VEC

	mtctr	BC		// for (i=1;i<=BC;i++)
	beqlr	cr1		// return if BC = 0
Byte_set:
	stbu	FILL,1(DM1)	// LSU * ++(DST-1) = FILL
	bdnz	Byte_set

	blr

v_memset:
// Byte count < MIN_VEC bytes will have been set by scalar code above,
// so this will not deal with small block sets < MIN_VEC.

// For systems using VRSAVE, define VRSAV=1 when compiling.  For systems
// that don't, make sure VRSAVE is undefined.
#ifdef VRSAVE
	mfspr	RSV,VRSV	// IU2 Get current VRSAVE contents
#endif
	rlwinm	DR,DR,0,0,27	// IU1 (DST+16)[0:27]
	addi	DBK,DBC,-1	// IU1 Address of last dst byte

#ifdef VRSAVE
	oris	Rt,RSV,0xe000	// IU1 Or in registers used by this routine
#endif
	subf	D,DST,DR	// IU1 How many bytes in first destination?
	li	BK,0		// IU1 Initialize byte kount index

#ifdef VRSAVE
	mtspr	VRSV,Rt	// IU2 Save in VRSAVE before first vec op
#endif
	vxor	v0,v0,v0	// VIU Clear v0
	subf	QW,DR,DBK	// IU1 Bytes of full vectors to move (-16)
	cmpi	cr1,0,D,16	// IU1 Is D0 left justified?
	beq+	enter_bzero	// b if FILL==0

	lvsl	v0,0,Fsh	// LSU Move upper nibble to byte 0 of VR
	vspltisb	v1,4	// VPU Splat 0x4 to every byte

	lvsl	v2,0,FILL	// LSU Move lower nibble to byte 0 of VR

	vslb	v0,v0,v1	// VIU Move upper nibble to VR[0:3]

	vor	v0,v0,v2	// VIU Form FILL byte in VR[0:7]

	vspltb	v0,v0,0		// VPU Splat the fill byte to all bytes
enter_bzero:
	mtcrf	0x01,D		// IU2 Put bytes in 1st dst in cr7
	rlwinm	QW,QW,28,4,31	// IU1 Quad words remaining
	beq	cr1,Left_just	// b if D0 is left justified

	bns	cr7,No_B_fwd	// b if only even number of bytes to store

	stvebx	v0,DST,BK	// LSU store first byte at DST+0
	addi	BK,BK,1		// IU1 increment index
No_B_fwd:
	bne	cr7,No_H_fwd	// b if only words to store

	stvehx	v0,DST,BK	// LSU store halfword at DST+0/1
	addi	BK,BK,2		// IU1 increment index
No_H_fwd:
	bng	cr7,No_W1_fwd	// b if exactly zero or two words to store

	stvewx	v0,DST,BK	// LSU store word 1 of one or three
	addi	BK,BK,4		// IU1 increment index

No_W1_fwd:
	bnl	cr7,No_W2_fwd	// b if there was only one word to store
	stvewx	v0,DST,BK	// LSU store word 1 of two or 2 of three
	addi	BK,BK,4		// IU1 increment index

	stvewx	v0,DST,BK	// LSU store word 2 of two or 3 of three
	b	No_W2_fwd

Left_just:	
	stvx	v0,0,DST	// LSU Store 16 bytes at D0
No_W2_fwd:
	rlwinm	Rt,DBK,0,28,31	// IU1 (DBK = DST+BC-1)[28:31]
	cmpi	cr6,0,QW,0	// IU1 Any full vectors to move?

	li	BK,16		// IU1 Re-initialize byte kount index
	cmpi	cr1,0,Rt,0xF	// IU1 Is DN right justified?
	ble	cr6,Last_QW	// b if no Quad words to do

	mtctr	QW		// IU2 for (i=0;i<=QW;i++)
	cmpi	cr6,0,QW,4	// IU1 Check QW>4

QW_loop:
	stvx	v0,DST,BK	// LSU Store 16 fill bytes
	addi	BK,BK,16	// IU1 Increment byte kount index
	bdnzf	25,QW_loop	// b if 4 or less quad words to do

	add	DNX,DST,BK	// IU1 address of next store (DST+32 if QW>4)
	addi	QW,QW,-1	// IU1 One more QW stored by now
	bgt	cr6,GT_4QW_fwd	// b if >4 quad words left

Last_QW:	// Next vector is the last; we're done.
	mtcrf	0x01,DBC	// IU2 Put final vector byte count in cr7

	beq	cr1,Rt_just_fwd	// b if last destination is right justified

	rlwinm	DBK,DBK,0,0,27	// IU1 Round to QW addr of last byte
	li	BL,0		// IU1 Initialize index pointer
	bnl	cr7,Only_1W_fwd	// b if there was only one or zero words to store

	stvewx	v0,DBK,BL	// LSU store word 1 of two or three
	addi	BL,BL,4		// IU1 increment index

	stvewx	v0,DBK,BL	// LSU store word 2 of two or three
	addi	BL,BL,4		// IU1 increment index
Only_1W_fwd:
	bng	cr7,Only_2W_fwd	// b if there were only two or zero words to store

	stvewx	v0,DBK,BL	// LSU store word 3 of three if necessary
	addi	BL,BL,4		// IU1 increment index
Only_2W_fwd:
	bne	cr7,Only_B_fwd	// b if there are no half words to store

	stvehx	v0,DBK,BL	// LSU store one halfword if necessary
	addi	BL,BL,2		// IU1 increment index
Only_B_fwd:
	bns	cr7,All_done_fwd	// b if there are no bytes to store

	stvebx	v0,DBK,BL	// LSU store one byte if necessary
	b	All_done_fwd

Rt_just_fwd:

	stvx	v0,DST,BK	// LSU Store 16 bytes at D14
All_done_fwd:
#ifdef VRSAVE
	mtspr	VRSV,RSV	// IU1 Restore VRSAVE	
#endif
	blr			// Return destination address from entry

#ifdef __MWERKS__
	.align	16
#else
	.align	4
#endif
GT_4QW_fwd:	// Do once if nxt st is to odd half of cache line, else twice

	addi	QW,QW,-1	// IU1 Keeping track of QWs stored
	mtcrf	0x02,DNX	// IU2 cr6[3]=((DST+32)[27]==1)?1:0;
	addi	DNX,DNX,16	// IU1 Update cr6 for next loop

	stvx	v0,DST,BK	// LSU Store 16 bytes at D2
	addi	BK,BK,16	// IU1 Increment byte count by 16
	bdnzf	27,GT_4QW_fwd	// b if next store is to lower (even) half of CL

	mtcrf	0x02,DBK	// IU2 cr6[3]=((last store)[27]==1)?1:0; (odd?)

	bns	cr6,B32_fwd	// b if DST[27] == 0; i.e, final store is even

// We need the ctr register to reflect an even byte count before entering
// the next block - faster to decrement than to reload.
	bdnz	B32_fwd		// decrement counter for last QW store odd

B32_fwd:	// Should be at least 2 stores remaining and next 2 are cache aligned
	DCBK			// LSU then Kill instead of RWITM

	stvx	v0,DST,BK	// LSU Store 16 bytes at D11
	addi	BK,BK,16	// IU1 Increment byte count
	bdz	Nxt_loc_fwd	// always decrement and branch to next instr		

Nxt_loc_fwd:
	stvx	v0,DST,BK	// LSU Store 16 bytes at D12
	addi	BK,BK,16	// IU1 Increment byte count
	bdnz	B32_fwd		// b if there are at least two more QWs to do

	bso	cr6,One_even_QW	// b if there is one even and one odd QW to store
	b	Last_QW		// b if last store is to even address

// Come here with two more loads and two stores to do
One_even_QW:
	stvx	v0,DST,BK	// LSU Store 16 bytes at D13
	addi	BK,BK,16	// IU1 Increment byte count

	b	Last_QW

// End of memset in AltiVec

#define BCz r4		// in bzero r4 enters with byte count

#ifdef __MWERKS__
	.align	32
#else
	.align	5
#endif

#ifdef LIBMOTOVEC
	.globl	bzero     
bzero:
#else
	.globl	vec_bzero     
vec_bzero:
#endif

	mr	BC,BCz		// IU1 arg[2] is BC here, not FILL
	li	FILL,0		// IU1 for bzero FILL=0
#ifdef LIBMOTOVEC
	b	memset     
#else
	b	_vec_memset     
#endif

// cacheable_memzero will employ dcbz to clear 32 bytes at a time
// of cacheable memory. Like bzero, second entering argument will be BC.
// Using this for non-cacheable memory will generate an alignment exception.

	.text
#ifdef __MWERKS__
	.align	32
#else
	.align	5
#endif

#ifdef LIBMOTOVEC
	.globl	cacheable_memzero     
cacheable_memzero:
#else
	.globl	vec_cacheable_memzero     
vec_cacheable_memzero:
#endif

	mr	BC,BCz		// IU1 arg[2] is BC here, not FILL
	li	FILL,0		// IU1 for bzero FILL=0
	cmpi	cr7,0,BC,MIN_VEC	// IU1 Check for minimum byte count

	cmpi	cr1,0,BC,0	// IU1 Eliminate zero byte count

	addi	DM1,DST,-1	// IU1 Pre-bias and duplicate destination
	addi	DR,DST,16	// IU1 Address of second dst vector
	add	DBC,DST,BC	// IU1 Address of last dst byte + 1
	bgt	cr7,c_v_memset	// b if BC>MIN_VEC

	mtctr	BC		// for (i=1;i<=BC;i++)
	beqlr	cr1		// return if BC = 0
c_Byte_set:
	stbu	FILL,1(DM1)	// LSU * ++(DST-1) = FILL
	bdnz	c_Byte_set

	blr

c_v_memset:
// Byte count < MIN_VEC bytes will have been set by scalar code above,
// so this will not deal with small block sets < MIN_VEC.

// For systems using VRSAVE, define VRSAV=1 when compiling.  For systems
// that don't, make sure VRSAVE is undefined.
#ifdef VRSAVE
	mfspr	RSV,VRSV	// IU2 Get current VRSAVE contents
#endif
	rlwinm	DR,DR,0,0,27	// IU1 (DST+16)[0:27]
	addi	DBK,DBC,-1	// IU1 Address of last dst byte

#ifdef VRSAVE
	oris	Rt,RSV,0x8000	// IU1 Or in registers used by this routine
#endif
	subf	D,DST,DR	// IU1 How many bytes in first destination?
	li	BK,0		// IU1 Initialize byte kount index

#ifdef VRSAVE
	mtspr	VRSV,Rt	// IU2 Save in VRSAVE before first vec op
#endif
	vxor	v0,v0,v0	// VIU Clear v0
	subf	QW,DR,DBK	// IU1 Bytes of full vectors to move (-16)
	cmpi	cr1,0,D,16	// IU1 Is D0 left justified?

	mtcrf	0x01,D		// IU2 Put bytes in 1st dst in cr7
	rlwinm	QW,QW,28,4,31	// IU1 Quad words remaining
	beq	cr1,c_Left_just	// b if D0 is left justified

	bns	cr7,c_No_B_fwd	// b if only even number of bytes to store

	stvebx	v0,DST,BK	// LSU store first byte at DST+0
	addi	BK,BK,1		// IU1 increment index
c_No_B_fwd:
	bne	cr7,c_No_H_fwd	// b if only words to store

	stvehx	v0,DST,BK	// LSU store halfword at DST+0/1
	addi	BK,BK,2		// IU1 increment index
c_No_H_fwd:
	bng	cr7,c_No_W1_fwd	// b if exactly zero or two words to store

	stvewx	v0,DST,BK	// LSU store word 1 of one or three
	addi	BK,BK,4		// IU1 increment index

c_No_W1_fwd:
	bnl	cr7,c_No_W2_fwd	// b if there was only one word to store
	stvewx	v0,DST,BK	// LSU store word 1 of two or 2 of three
	addi	BK,BK,4		// IU1 increment index

	stvewx	v0,DST,BK	// LSU store word 2 of two or 3 of three
	b	c_No_W2_fwd

c_Left_just:	
	stvx	v0,0,DST	// LSU Store 16 bytes at D0
c_No_W2_fwd:
	rlwinm	Rt,DBK,0,28,31	// IU1 (DBK = DST+BC-1)[28:31]
	cmpi	cr6,0,QW,0	// IU1 Any full vectors to move?

	li	BK,16		// IU1 Re-initialize byte kount index
	cmpi	cr1,0,Rt,0xF	// IU1 Is DN right justified?
	ble	cr6,c_Last_QW	// b if no Quad words to do

	mtctr	QW		// IU2 for (i=0;i<=QW;i++)
	cmpi	cr6,0,QW,4	// IU1 Check QW>4

c_QW_loop:
	stvx	v0,DST,BK	// LSU Store 16 fill bytes
	addi	BK,BK,16	// IU1 Increment byte kount index
	bdnzf	25,c_QW_loop	// b if 4 or less quad words to do

	add	DNX,DST,BK	// IU1 address of next store (DST+32 if QW>4)
	addi	QW,QW,-1	// IU1 One more QW stored by now
	bgt	cr6,c_GT_4QW_fwd	// b if >4 quad words left

c_Last_QW:	// Next vector is the last; we're done.
	mtcrf	0x01,DBC	// IU2 Put final vector byte count in cr7

	beq	cr1,c_Rt_just_fwd	// b if last destination is right justified

	rlwinm	DBK,DBK,0,0,27	// IU1 Round to QW addr of last byte
	li	BL,0		// IU1 Initialize index pointer
	bnl	cr7,c_Only_1W_fwd	// b if there was only one or zero words to store

	stvewx	v0,DBK,BL	// LSU store word 1 of two or three
	addi	BL,BL,4		// IU1 increment index

	stvewx	v0,DBK,BL	// LSU store word 2 of two or three
	addi	BL,BL,4		// IU1 increment index
c_Only_1W_fwd:
	bng	cr7,Only_2W_fwd	// b if there were only two or zero words to store

	stvewx	v0,DBK,BL	// LSU store word 3 of three if necessary
	addi	BL,BL,4		// IU1 increment index
c_Only_2W_fwd:
	bne	cr7,c_Only_B_fwd	// b if there are no half words to store

	stvehx	v0,DBK,BL	// LSU store one halfword if necessary
	addi	BL,BL,2		// IU1 increment index
c_Only_B_fwd:
	bns	cr7,c_All_done_fwd	// b if there are no bytes to store

	stvebx	v0,DBK,BL	// LSU store one byte if necessary
	b	c_All_done_fwd

c_Rt_just_fwd:

	stvx	v0,DST,BK	// LSU Store 16 bytes at D14
c_All_done_fwd:
#ifdef VRSAVE
	mtspr	VRSV,RSV	// IU1 Restore VRSAVE	
#endif
	blr			// Return destination address from entry

#ifdef __MWERKS__
	.align	16
#else
	.align	4
#endif
c_GT_4QW_fwd:	// Do once if nxt st is to odd half of cache line, else twice

	addi	QW,QW,-1	// IU1 Keeping track of QWs stored
	mtcrf	0x02,DNX	// IU2 cr6[3]=((DST+32)[27]==1)?1:0;
	addi	DNX,DNX,16	// IU1 Update cr6 for next loop

	stvx	v0,DST,BK	// LSU Store 16 bytes at D2
	addi	BK,BK,16	// IU1 Increment byte count by 16
	bdnzf	27,c_GT_4QW_fwd	// b if next store is to lower (even) half of CL

	mtcrf	0x02,DBK	// IU2 cr6[3]=((last store)[27]==1)?1:0; (odd?)

	bns	cr6,c_B32_fwd	// b if DST[27] == 0; i.e, final store is even

// We need the ctr register to reflect an even byte count before entering
// the next block - faster to decrement than to reload.
	bdnz	B32_fwd		// decrement counter for last QW store odd

c_B32_fwd:	// Should be at least 2 stores remaining and next 2 are cache aligned
	dcbz	DST,BK		// LSU zero whole cache line
	bdz	c_Nxt_loc_fwd	// always decrement and branch to next instr		

c_Nxt_loc_fwd:
	addi	BK,BK,32	// IU1 Increment byte count
	bdnz	B32_fwd		// b if there are at least two more QWs to do

	bso	cr6,c_One_even_QW	// b if there is one even and one odd QW to store
	b	c_Last_QW		// b if last store is to even address

// Come here with two more loads and two stores to do
c_One_even_QW:
	stvx	v0,DST,BK	// LSU Store 16 bytes at D13
	addi	BK,BK,16	// IU1 Increment byte count

	b	c_Last_QW

// End of cacheable_memzero in AltiVec
