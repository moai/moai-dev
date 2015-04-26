//------------------------------------------------------------------
// file:  vec_memcpy.S
//    AltiVec enabled version of memcpy and bcopy
//------------------------------------------------------------------

//------------------------------------------------------------------
//	Copyright Motorola, Inc. 2003
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
// extern  void * memcpy(void *dst, const void *src, size_t len);
// Returns:
//  void *dst
//------------------------------------------------------------------

//------------------------------------------------------------------
// extern void * memmove( void *dst, const void *src, size_t len );
//   Copies len characters from src to dst and returns the value of
//   dst.  Works correctly for overlapping memory regions.
//               - Harbison&Steele 4th ed (corrected as to return)
// Returns:
//  void *dst
//------------------------------------------------------------------

//------------------------------------------------------------------
// extern  void * bcopy(const void *src, void *dst,  size_t len);
// Returns:
//  void *dst
//------------------------------------------------------------------

// memcpy and memmove are combined into one entry point here because of
// the similarity of operation and need to create fool-proof code.
// The following conditions determine what is "fool proof":
//
// if:                                          then single entry:
// (DST-SRC)<0 && (SRC-DST)>=BC && BC>MIN_VEC    will b to v_memcpy
// (DST-SRC)<0 && (SRC-DST)< BC && BC>MIN_VEC    must b to v_memcpy
// (DST-SRC)<0                  && BC<MIN_VEC    copy fwd byte-by-byte
// (DST-SRC)==0                 || BC==0         will just return
// (DST-SRC)>0                  && BC<MIN_VEC    copy bkwd byte-by-byte
// (DST-SRC)>0 && (DST-SRC)< BC && BC>MIN_VEC    must b to v_memmove
// (DST-SRC)>0 && (SRC-DST)>=BC && BC>MIN_VEC    will b to v_memmove

// If you call memmove (or vec_memmove) and |DST-SRC|>=BC,
// this code will branch to v_memcpy anyway for maximum performance.

// Revision History:
//    Rev 0.0	Original                          Chuck Corley	02/03/03
//              Can still add dst, 128B loop, and aligned option
//    Rev 0.01  Fixed JY's seg-fault violation              CJC 02/17/03
//    Rev 0.1   Added 128B loop and dst; cndtnlzd dcbz      CJC 02/18/03
//              (Creating separate path for QW aligned didn't help much)
//    Rev 0.11  Small code schdling; chngd dst for memmove  CJC 02/23/03
//    Rev 0.20  Eliminated alternate entry and cleanup      CJC 02/27/03                   
//    Rev 0.21  Inproved loop branch targets for v_mempcy   CJC 03/01/03                   
//    Rev 0.22  Experimented with dst (sent to H.)          CJC 03/02/03                   
//    Rev 0.23  Substituted dcba for dcbz (sent to JY)      CJC 03/08/03                   
//    Rev 0.24  Use two dst streams                         CJC 03/12/03
//    Rev 0.25  Fix for all compilers, cleanup, and release with
//              libmotovec.a rev 0.10                       CJC 03/14/03
//    Rev 0.30  Fix for pre-empted destination (SNDF-DS)    CJC 04/02/03                   
//
//  Between Rev 0.25 and 0.30 the code was revised to store elements of
//  source at destination when first and/or last vector are less than 16
//  bytes. Areviewer at SNDF observed that loading the destination vector
//  for merging exposed the "uninvolved" destination bytes to incoherency 
//  if an interrupt pre-empted this routine and modified the "uninvolved"
//  destination vector(s) while held in register for merging.  It seems
//  like a low possibility but this revision is no longer subject to that
//  possibility.  (It is also slightly faster than Rev 0.25.)
//  This is beta quality code; users are encouraged to make it faster.
//  ASSUMPTIONS:
//     Code is highly likely to be in the cache; data is not (streaming data)

#define VRSV 256	//	VRSAVE spr
// Don't use vectors for BC <= MIN_VEC. Works only if MIN_VEC >= 16 bytes.
#define MIN_VEC 16
// Don't use Big_loop in v_memcpy for |dst-src|<= minimum overlap.
#define MIN_OVL 128

// Register useage
#define Rt r0	// 	r0 when used as a temporary register	

#define DST r3	// 	entering: dst pointer; exiting: same dst pointer

#define SRC r4	// 	entering: src ptr; then end of src range index (SRC+BC) in memmove

#define BC r5	//	entering: Byte_Count

#define PCS r6	//  	save for partial checksum entering

#define DMS r7	//      dst - src initially
#define BK r7	//  	BC - 1 +/- (n*16)

// Codewarrior will put an unwelcome space as "lbzu	r0,1(r7 )"
// if you don't put the comment right after the r7.  CJC 030314
#define SM1 r8//	src -1 for byte-by-byte forwards initially
#define S r8	//	src[28:31]
#define SMD r8	//      src[0:27]-dst[0:27]
#define STR r8	//	data stream touch block & stride info for Big_loop

#define DM1 r9//	dst -1 for byte-by-byte forwards initially
#define D r9	//	dst[28:31]
#define DNX r9	//	(dst+n*16)[28:31]
#define BL r9	//	second byte_kount index pointer

#define SBC r10//	src + byte count initially then src[28:31]
#define BLK r10	//      temporary data stream touch block & stride info
#define DR r10	//	(dst+16)[0:27]
#define QW r10	//  	number of quad words (vectors)

#define DBC r11//	dst + byte count initially
#define BLL r11	//      temporary data stream touch block & stride info
#define SBK r11	//	(src+byte_count-1)
#define SBR r11	//	(src+byte_count-1)[0:27]
#define DBK r11	//	(dst+byte_count-1) then (dst+byte_count-1)[28:31]
#define BIG r11	//	QW/8 or 128 byte loop count
#define SP8 r11	//      SRC + n*128 (8 QWs) for data streaming after first call

#define RSV r12	//  	storage for VRSAVE register if used

#define VS0   v0	//  	src vector for permuting

#define VS1   v1	//  	src vector for permuting

#define VP3   v2	// 	d - s permute register

#define VPS0  v3	// 	permuted source vector to store

#define VPS1  v4	//  	2nd permuted source vector to store

#define VPS2  v5	//      additional permuted src in Big loop

#define VS2   v6	//  	src vector for permuting
#define VPS3  v6	//      additional permuted src in Big loop

#define VS3   v7	//      additional src load in Big loop
#define VPS4  v7	//      additional permuted src in Big loop

#define VS4   v8	//      additional src load in Big loop
#define VPS5  v8	//      additional permuted src in Big loop

#define VS5   v9	//      additional src load in Big loop
#define VPS6  v9	//      additional permuted src in Big loop

#define VS6   v10	//      additional src load in Big loop
#define VPS7  v10	//      additional permuted src in Big loop

#define VS7   v11	//      additional src load in Big loop

// Conditionalize the use of dcba.  It will help if the data is
// not in cache and hurt if it is.  Generally, except for small
// benchmarks repeated many times, we assume data is not in cache
// (data streaming) and using dcbz is a performance boost.
#ifndef NO_DCBA
#if defined(__GNUC__) || defined(__MWERKS__) || defined(_DIAB_TOOL)
 // gcc and codewarrior and diab don't assemble dcba
#define DCBK .long 0x7c033dec
// dcba r3,r7    or    dcba DST,BK
#define DCBL .long 0x7c034dec
// dcba r3,r9     or    dcba DST,BL
#else
#ifdef __ghs__
.macro DCBK
.long 0x7c033dec
.endm
.macro DCBL
.long 0x7c034dec
.endm
#else
#define DCBK dcba DST,BK
#define DCBL dcba DST,BL
#endif  // __ghs__
#endif  // __GNUC__ or __MWERKS__
#else
#define DCBK nop
#define DCBL nop
#endif  // NO_DCBA

// Conditionalize the use of dst (data stream touch).  It will help
// if the data is not in cache and hurt if it is (though not as badly
// as dcbz).  Generally, except for small benchmarks repeated many times,
// we assume data is not in cache (data streaming) and using dst is a
// performance boost.
#ifndef NO_DST
#define STRM_B dst	SBC,BLL,0
#define STRM_F dst	SRC,BLK,0
#define STRM_1 dst	SP8,STR,1

#else
#define STRM_B	nop
#define STRM_F	nop
#define STRM_1	nop
#endif

//  Condition register use
//      cr0[0:2] = (dst-src==0)? return: ((dst-src>0)? copy_bkwd, copy_fwd;);
// then cr0[0:2] = (dst[28:31]-src[28:31]<0)? "shifting left", "shifting right";
//      cr1[0,2] = (BC == 0)? 1 : 0; (nothing to move)
// then cr1[2]   = (DST[28:31] == 0)? 1 : 0;  (D0 left justified)
// then cr1[2]   = ((DBK = DST+BC-1)[28:31] = 0xF)? 1 : 0; (DN right justified)
//      cr5[0,2] = (|DST-SRC|<=MIN_OVL)?1:0;  (Overlap too small for Big loop?)
//      cr6[1,2] = (DST-SRC>=BC)?1:0;  (Okay for v_memmove to copy forward?)
// then cr6[2]   = (QW == 0)? 1 : 0; (Any full vectors to move?)
// then cr6[1]   = (QW > 4)? 1 : 0; (>4 vectors to move?)
// then cr6[3]   = (third store[27] == 1)? 1: 0; (cache line alignment)
// then cr6[3]   = (last store[27] == 1)? 1: 0; (last store odd?)
//      cr7[2]   = (BC>MIN_VEC)?1:0;  (BC big enough to warrant vectors)
// then cr7[0:3] = (DST+16)[0:27]-DST  (How many bytes (iff <16) in first vector?)
// then cr7[1]   = (QW > 14)? 1 : 0; (>14 vectors to move?)
// then cr7[0:3] = (DST+BC)[0:27]  (How many bytes (iff <16) in last vector?)

	.text
#ifdef __MWERKS__
	.align	32
#else
	.align	5
#endif

#ifdef LIBMOTOVEC
	.globl	memmove     
memmove:
	nop			// IU1 Compilers forget first label
	.globl	memcpy     
memcpy:
#else
	.globl	vec_memmove     
vec_memmove:
	nop			// IU1 Only way I know to preserve both labels
	.globl	_vec_memcpy     
_vec_memcpy:
#endif
	subf.	DMS,SRC,DST	// IU1 Compute dst-src difference
	cmpi	cr1,0,BC,0	// IU1 Eliminate zero byte count moves
	cmpi	cr7,0,BC,MIN_VEC	// IU1 Check for minimum byte count

	addi	SM1,SRC,-1	// IU1 Pre-bias and duplicate src for fwd
	addi	DM1,DST,-1	// IU1 Pre-bias and duplicate destination
	add	SBC,SRC,BC	// IU1 Pre-bias and duplicate src for bkwd
	beqlr			// return if DST = SRC

	add	DBC,DST,BC	// IU1 Pre-bias and duplicate destination
	subf	Rt,DST,SRC	// IU1 Form |DST-SRC| if DST-SRC<0
	beqlr	cr1		// return if BC = 0

	bgt	Cpy_bkwd	// b if DST-SRC>0 (have to copy backward)
	cmpi	cr5,0,Rt,MIN_OVL	// IU1 (|DST-SRC|>128)?1:0; for v_memcpy
	bgt	cr7,v_memcpy	// b if BC>MIN_VEC (okay to copy vectors fwd)

// Copy byte-by-byte forwards if DST-SRC<0 and BC<=MIN_VEC	
	mtctr	BC		// i=BC; do ...;i--; while (i>0)
Byte_cpy_fwd:
	lbzu	Rt,1(SM1)	// LSU * ++(DST-1) = * ++(SRC-1)
	stbu	Rt,1(DM1)	// LSU
	bdnz	Byte_cpy_fwd

	blr
	nop			// IU1 Improve next label as branch target	
Cpy_bkwd:
	cmpi	cr5,0,DMS,MIN_OVL	// IU1 ((DST-SRC)>128)?1:0; for v_memcpy
	cmp	cr6,0,DMS,BC	// IU1 cr6[1,2]=(DST-SRC>=BC)?1:0;
	bgt	cr7,v_memmove	// b if BC>MIN_VEC (copy vectors bkwd)
// Copy byte-by-byte backwards if DST-SRC>0 and BC<=MIN_VEC
	mtctr	BC		// i=BC; do ...;i--; while (i>0)
Byte_cpy_bwd:
	lbzu	Rt,-1(SBC)	// LSU * --(DST+BC) = * --(SRC+BC)
	stbu	Rt,-1(DBC)	// LSU Store it
	bdnz	Byte_cpy_bwd
	blr
	
#ifdef __MWERKS__
	.align	16
#else
	.align	4
#endif

v_memmove:
// Byte count < MIN_VEC bytes will have been copied by scalar code above,
// so this will not deal with small block moves < MIN_VEC.

// For systems using VRSAVE, define VRSAVE=1 when compiling.  For systems
// that don't, make sure VRSAVE is undefined.
#ifdef VRSAVE
	mfspr	RSV,VRSV	// IU2 Get current VRSAVE contents
#endif
	rlwinm	S,SRC,0,28,31	// IU1 Save src address bits s[28:31]
	rlwinm	D,DST,0,28,31	// IU1 D = dst[28:31]
	bge	cr6,MC_entry	// b to v_memcpy if DST-SRC>=BC (fwd copy OK)

#ifdef VRSAVE
	oris	Rt,RSV,0xfff0	// IU1 Or in registers used by this routine
#endif	
	lis	BLL,0x010c	// IU1 Stream 12 blocks of 16 bytes
	subf.	SMD,D,S		// IU1 if S-D<0 essentially shifting right

#ifdef VRSAVE
	mtspr	VRSV,Rt		// IU2 Save in VRSAVE before first vec op
#endif
	lvsr	VP3,0,DMS	// LSU Permute vector for dst - src shft right
	ori	BLL,BLL,0xffe0	// IU1 Stream stride -32B

	STRM_B			// LSU Start data stream at SRC+BC
	addi	SBK,SBC,-1	// IU1 Address of last src byte
	bgt	Rt_shft		// Bytes from upper vector = (s-d>0)?s-d:16+s-d;
	addi	SMD,SMD,16	// IU1 Save 16-(d-s)
Rt_shft:

	rlwinm	SBR,SBK,0,0,27	// IU1 (SRC+BC-1)[0:27]
	addi	BK,BC,-1	// IU1 Initialize byte index

	subf	Rt,SBR,SBC	// IU1 How many bytes in first source?
	add	DBK,DST,BK	// IU1 Address of last dst byte
	addi	DR,DST,16	// IU1 Address of second dst vector

	subf.	SMD,Rt,SMD	// IU1 if bytes in 1st src>Bytes in 1st permute
	rlwinm	Rt,DBK,0,28,31	// IU1 (DST+BC-1)[28:31]
	rlwinm	DR,DR,0,0,27	// IU1 (DST+16)[0:27]

// If there are more useful bytes in the upper vector of a permute pair than we
// will get in the first permute, the first loaded vector needs to be in the
// lower half of the permute pair.  The upper half is a don't care then.
	blt	Get_bytes_rt	// b if shifting left (D-S>=0)

	lvx	VS1,SRC,BK	// LSU Get SN load started
// Comments numbering source and destination assume single path through the
// code executing each instruction once.  For vec_memmove, an example would
// be the call memmove(BASE+0x0F, BASE+0x2F, 82). N = 6 in that case.
	addi	SRC,SRC,-16	// IU1 Decrement src base (to keep BK useful)

Get_bytes_rt:	// Come here to get VS0 & Don't care what VS1 is	
	lvx	VS0,SRC,BK	// LSU Get SN-1 (SN if D-S<0) in lower vector
	subf	QW,DR,DBK	// IU1 Bytes of full vectors to move (-16)
	cmpi	cr7,0,Rt,0xF	// IU1 Is Dn right justified?

	cmpi	cr1,0,D,0	// IU1 Is D0 left justified?
	rlwinm	QW,QW,28,4,31	// IU1 Quad words remaining
	add	Rt,DST,BC	// IU1 Refresh the value of DST+BC

	cmpi	cr6,0,QW,0	// IU1 Any full vectors to move?
	vperm	VPS0,VS0,VS1,VP3	// VPU Align SN-1 and SN to DN
	vor	VS1,VS0,VS0	// VIU1 Move lower vector to upper
	beq	cr7,Rt_just	// b if DN is right justified

	mtcrf	0x01,Rt		// IU2 Put final vector byte count in cr7
	rlwinm	DBK,DBK,0,0,27	// IU1 Address of first byte of final vector
	li	D,0		// IU1 Initialize an index pointer
	bnl	cr7,Only_1W_bkwd	// b if there was only one or zero words to store

	stvewx	VPS0,DBK,D	// LSU store word 1 of two or three
	addi	D,D,4		// IU1 increment index

	stvewx	VPS0,DBK,D	// LSU store word 2 of two or three
	addi	D,D,4		// IU1 increment index
Only_1W_bkwd:
	bng	cr7,Only_2W_bkwd	// b if there were only two or zero words to store

	stvewx	VPS0,DBK,D	// LSU store word 3 of three if necessary
	addi	D,D,4		// IU1 increment index
Only_2W_bkwd:
	bne	cr7,Only_B_bkwd	// b if there are no half words to store

	stvehx	VPS0,DBK,D	// LSU store one halfword if necessary
	addi	D,D,2		// IU1 increment index
Only_B_bkwd:
	bns	cr7,All_done_bkwd	// b if there are no bytes to store

	stvebx	VPS0,DBK,D	// LSU store one byte if necessary
	b	All_done_bkwd

Rt_just:	
	stvx	VPS0,DST,BK	// LSU Store 16 bytes at DN
All_done_bkwd:
	addi	BK,BK,-16	// IU1 Decrement destination byte count

	ble	cr6,Last_load	// b if no Quad words to do
	mtctr	QW		// IU2 for (i=0;i<=QW;i++)-execution serializng
	cmpi	cr6,0,QW,4	// IU1 Check QW>4
QW_loop:
	lvx	VS0,SRC,BK	// LSU Get SN-2 (or SN-1 if ADJ==0)

	vperm	VPS0,VS0,VS1,VP3	// VPU Align SN-2 and SN-1 to DN-1
	vor	VS1,VS0,VS0	// VIU1 Move lower vector to upper

	stvx	VPS0,DST,BK	// LSU Store 16 bytes at DN-1
	addi	BK,BK,-16	// IU1 Decrement byte kount
	bdnzf	25,QW_loop	// b if 4 or less quad words to do

	add	DNX,DST,BK	// IU1 address of next store (DST+BC-1-16)
	bgt	cr6,GT_4QW	// b if >4 quad words left

Last_load:	// if D-S>=0, next load will be from same address as last
	blt	No_ld_bkwd	// b if shifting right (S-D>=0)
	addi	SRC,SRC,16	// IU1 recorrect source if it was decremented
No_ld_bkwd:				
	lvx	VS0,0,SRC	// LSU Get last source SN-6 (guaranteed S0)
// Current 16 bytes is the last; we're done.
	dss	0		// Data stream stop
	vperm	VPS0,VS0,VS1,VP3	// VPU Align SN-6 and SN-5 to DN-6
	subfic	D,DST,16	// IU1 How many bytes in first destination?
	beq	cr1,Lt_just	// b if last destination is left justified

	mtcrf	0x01,D		// IU2 Put byte count remaining in cr7
	li	D,0		// IU1 Initialize index pointer
	bns	cr7,No_B_bkwd	// b if only even number of bytes to store

	stvebx	VPS0,DST,D	// LSU store first byte at DST+0
	addi	D,D,1		// IU1 increment index
No_B_bkwd:
	bne	cr7,No_H_bkwd	// b if only words to store
	stvehx	VPS0,DST,D	// LSU store halfword at DST+0/1
	addi	D,D,2		// IU1 increment index

No_H_bkwd:
	bng	cr7,No_W1_bkwd	// b if exactly zero or two words to store
	stvewx	VPS0,DST,D	// LSU store word 1 of one or three
	addi	D,D,4		// IU1 increment index

No_W1_bkwd:
	bnl	cr7,No_W2_bkwd	// b if there was only one word to store
	stvewx	VPS0,DST,D	// LSU store word 1 of two or 2 of three
	addi	D,D,4		// IU1 increment index

	stvewx	VPS0,DST,D	// LSU store word 2 of two or 3 of three
	b	No_W2_bkwd

Lt_just:
	stvx	VPS0,0,DST	// LSU Store 16 bytes at final dst addr D0
No_W2_bkwd:
#ifdef VRSAVE
	mtspr	VRSV,RSV	// IU1 Restore VRSAVE	
#endif
	blr			// Return destination address from entry

GT_4QW:	// Do once if next store is to even half of cache line, else twice

	lvx	VS0,SRC,BK	// LSU Get SN-3 (or SN-2)
	mtcrf	0x02,DNX	// IU2 cr6[3]=((DST+BC-1)[27]==1)?1:0;
	
	vperm	VPS0,VS0,VS1,VP3	// VPU Align SN-3 and SN-2 to Dn-2
	vor	VS1,VS0,VS0	// VIU1 Move lower vector to upper
	addi	DNX,DNX,-16	// IU1 Prepare to update cr6 next loop

	stvx	VPS0,DST,BK	// LSU Store 16 bytes at DN-2
	vor	VS3,VS0,VS0	// VIU Make a copy of lower vector
	addi	BK,BK,-16	// IU1 Decrement byte count by 16
	bdnzt	27,GT_4QW	// b if next store is to upper (odd) half of CL
// At this point next store will be to even address.

	lis	STR,0x102	// IU1 Stream 2 blocks of 16 bytes
	mtcrf	0x02,DST	// IU2 cr6[3]=(DST[27]==1)?1:0; (DST odd?)
	addi	BL,BK,-16	// IU1 Create an alternate byte count - 16

	ori	STR,STR,0xffe0	// IU1 Stream stride -32B
	addi	SP8,SRC,-64	// IU1 Starting address for data stream touch
	bso	cr6,B32_bkwd	// b if DST[27] == 1; i.e, final store is odd

	bdnz	B32_bkwd	// decrement counter for last odd QW store
B32_bkwd:	// Should be at least 2 stores remaining and next 2 are cache aligned
	lvx	VS2,SRC,BK	// LSU Get SN-4 (or SN-3)
	addi	SP8,SP8,-32	// IU1 Next starting address for data stream touch

	lvx	VS1,SRC,BL	// LSU Get SN-5 (or SN-4)
	vperm	VPS0,VS2,VS3,VP3	// VPU Align SN-4 and SN-3 to DN-3

	STRM_1			// LSU Stream 64 byte blocks ahead of loads

	DCBL			// LSU allocate next cache line

	vperm	VPS1,VS1,VS2,VP3	// VPU Align SN-5 and SN-4 to DN-4
	vor	VS3,VS1,VS1	// VIU1 Move SN-5 to SN-3

	stvx	VPS0,DST,BK	// LSU Store 16 bytes at DN-3
	addi	BK,BL,-16	// IU1 Decrement byte count
	bdz	Nxt_loc_bkwd	// always decrement and branch to next instr		

Nxt_loc_bkwd:
	stvx	VPS1,DST,BL	// LSU Store 16 bytes at DN-4
	addi	BL,BK,-16	// IU1 Decrement alternate byte count
	bdnz	B32_bkwd	// b if there are at least two more QWs to do

	bns	cr6,One_odd_QW	// b if there was one more odd QW to store
	b	Last_load

// Come here with two more loads and two stores to do
One_odd_QW:
	lvx	VS1,SRC,BK	// LSU Get SN-6 (or SN-5)

	vperm	VPS1,VS1,VS3,VP3	// VPU Align SN-6 and SN-5 to DN-5

	stvx	VPS1,DST,BK	// LSU Store 16 bytes at DN-5

	b	Last_load

// End of memmove in AltiVec

#ifdef __MWERKS__
	.align	16
#else
	.align	4
#endif
v_memcpy:
// Byte count < MIN_VEC bytes will have been copied by scalar code above,
// so this will not deal with small block moves < MIN_VEC.

#ifdef VRSAVE
	mfspr	RSV,VRSV	// IU2 Get current VRSAVE contents
#endif
	rlwinm	S,SRC,0,28,31	// IU1 Save src address bits s[28:31]
	rlwinm	D,DST,0,28,31	// IU1 D = dst[28:31]

MC_entry:	// enter here from memmove if DST-SRC>=BC; this should be faster
#ifdef VRSAVE
	oris	Rt,RSV,0xfff0	// IU1 Or in registers used by this routine
#endif	
	lis	BLK,0x010c	// IU1 Stream 12 blocks of 16 bytes

	subf.	S,S,D		// IU1 if D-S<0 essentially shifting left

#ifdef VRSAVE
	mtspr	VRSV,Rt		// IU2 Save in VRSAVE before first vec op
#endif
	lvsr	VP3,0,DMS	// LSU Permute vector for dst - src shft right
	ori	BLK,BLK,32	// IU1 Stream stride 32B

	STRM_F			// LSU Start data stream 0 at SRC
	addi	DR,DST,16	// IU1 Address of second dst vector
	addi	DBK,DBC,-1	// IU1 Address of last dst byte

// If D-S<0 we are "kinda" shifting left with the right shift permute vector
// loaded to VP3 and we need both S0 and S1 to permute.  If D-S>=0 then the
// first loaded vector needs to be in the upper half of the permute pair and
// the lower half is a don't care then.
	bge	Ld_bytes_rt	// b if shifting right (D-S>=0)

	lvx	VS0,0,SRC	// LSU Get S0 load started
// Comments numbering source and destination assume single path through the
// code executing each instruction once.  For vec_memcpy, an example would
// be the call memcpy(BASE+0x1E, BASE+0x1F, 259). N = 16 in that case.
	addi	SRC,SRC,16	// IU1 Increment src base (to keep BK useful)

Ld_bytes_rt:	// Come here to get VS1 & Don't care what VS0 is	
	lvx	VS1,0,SRC	// LSU Get S1 (or S0 if D-S>=0) in upper vector
	rlwinm	DR,DR,0,0,27	// IU1 (DST+16)[0:27]
	cmpi	cr1,0,D,0	// IU1 Is D0 left justified?

	subf	Rt,DST,DR	// IU1 How many bytes in first destination?
	subf	QW,DR,DBK	// IU1 Bytes of full vectors to move (-16)
	li	BK,0		// IU1 Initialize byte kount index

	mtcrf	0x01,Rt		// IU2 Put bytes in 1st dst in cr7
	rlwinm	QW,QW,28,4,31	// IU1 Quad words remaining
	vperm	VPS0,VS0,VS1,VP3	// VPU Align S0 and S1 to D0

	vor	VS0,VS1,VS1	// VIU1 Move upper vector to lower
	beq	cr1,Left_just	// b if D0 is left justified

	bns	cr7,No_B_fwd	// b if only even number of bytes to store

	stvebx	VPS0,DST,BK	// LSU store first byte at DST+0
	addi	BK,BK,1		// IU1 increment index
No_B_fwd:
	bne	cr7,No_H_fwd	// b if only words to store

	stvehx	VPS0,DST,BK	// LSU store halfword at DST+0/1
	addi	BK,BK,2		// IU1 increment index
No_H_fwd:
	bng	cr7,No_W1_fwd	// b if exactly zero or two words to store

	stvewx	VPS0,DST,BK	// LSU store word 1 of one or three
	addi	BK,BK,4		// IU1 increment index

No_W1_fwd:
	bnl	cr7,No_W2_fwd	// b if there was only one word to store
	stvewx	VPS0,DST,BK	// LSU store word 1 of two or 2 of three
	addi	BK,BK,4		// IU1 increment index

	stvewx	VPS0,DST,BK	// LSU store word 2 of two or 3 of three
	b	No_W2_fwd

Left_just:	
	stvx	VPS0,0,DST	// LSU Store 16 bytes at D0
No_W2_fwd:
	rlwinm	Rt,DBK,0,28,31	// IU1 (DBK = DST+BC-1)[28:31]
	cmpi	cr6,0,QW,0	// IU1 Any full vectors to move?

	li	BK,16		// IU1 Re-initialize byte kount index
	cmpi	cr1,0,Rt,0xF	// IU1 Is DN right justified?
	cmpi	cr7,0,QW,14	// IU1 Check QW>14
	ble	cr6,Last_ld_fwd	// b if no Quad words to do

	mtctr	QW		// IU2 for (i=0;i<=QW;i++)
	cmpi	cr6,0,QW,4	// IU1 Check QW>4
QW_fwd_loop:
	lvx	VS1,SRC,BK	// LSU Get S2 (or S1)

	vperm	VPS0,VS0,VS1,VP3	// VPU Align S1 and S2 to D1
	vor	VS0,VS1,VS1	// VIU1 Move upper vector to lower

	stvx	VPS0,DST,BK	// LSU Store 16 bytes at D1(+n*16 where n<4)
	addi	BK,BK,16	// IU1 Increment byte kount index
	bdnzf	25,QW_fwd_loop	// b if 4 or less quad words to do

	add	DNX,DST,BK	// IU1 address of next store (DST+32 if QW>4)
	addi	QW,QW,-1	// IU1 One more QW stored by now
	bgt	cr6,GT_4QW_fwd	// b if >4 quad words left

Last_ld_fwd:	// Next 16 bytes is the last; we're done.
	add	DBC,DST,BC	// IU1 Recompute address of last dst byte + 1
	add	SBC,SRC,BC	// IU1 Recompute address of last src byte + 1
	bge	No_ld_fwd	// b if shifting right (D-S>=0)

	addi	SBC,SBC,-16	// IU1 if D-S>=0 we didn't add 16 to src
No_ld_fwd:
	mtcrf	0x01,DBC	// IU2 Put final vector byte count in cr7
	addi	DBK,DBC,-1	// IU1 Recompute address of last dst byte
	addi	Rt,SBC,-1	// IU1 Recompute address of last src byte

// If D-S<0 we have already loaded all the source vectors.
// If D-S>=0 then the first loaded vector went to the upper half of the permute
// pair and we need one more vector.  (This may be a duplicate.)

	lvx	VS1,0,Rt	// LSU Get last source S14 (guaranteed SN)

#ifndef NO_DST				
	dss	0		// Data stream 0 stop

	dss	1		// Data stream 1 stop
#endif
	vperm	VPS0,VS0,VS1,VP3	// VPU Align S13 and S14 to D14
	beq	cr1,Rt_just_fwd	// b if last destination is right justified

	rlwinm	DBK,DBK,0,0,27	// IU1 Round to QW addr of last byte
	li	D,0		// IU1 Initialize index pointer
	bnl	cr7,Only_1W_fwd	// b if there was only one or zero words to store

	stvewx	VPS0,DBK,D	// LSU store word 1 of two or three
	addi	D,D,4		// IU1 increment index

	stvewx	VPS0,DBK,D	// LSU store word 2 of two or three
	addi	D,D,4		// IU1 increment index
Only_1W_fwd:
	bng	cr7,Only_2W_fwd	// b if there were only two or zero words to store

	stvewx	VPS0,DBK,D	// LSU store word 3 of three if necessary
	addi	D,D,4		// IU1 increment index
Only_2W_fwd:
	bne	cr7,Only_B_fwd	// b if there are no half words to store

	stvehx	VPS0,DBK,D	// LSU store one halfword if necessary
	addi	D,D,2		// IU1 increment index
Only_B_fwd:
	bns	cr7,All_done_fwd	// b if there are no bytes to store

	stvebx	VPS0,DBK,D	// LSU store one byte if necessary
	b	All_done_fwd

Rt_just_fwd:

	stvx	VPS0,DST,BK	// LSU Store 16 bytes at D14
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

	lvx	VS1,SRC,BK	// LSU Get S3 (or S2)
	addi	QW,QW,-1	// IU1 Keeping track of QWs stored
	mtcrf	0x02,DNX	// IU2 cr6[3]=((DST+32)[27]==1)?1:0;
	
	addi	DNX,DNX,16	// IU1 Update cr6 for next loop
	addi	Rt,QW,-2	// IU1 Insure at least 2 QW left after big loop

	vperm	VPS0,VS0,VS1,VP3	// VPU Align S2 and S3 to D2
	vor	VS0,VS1,VS1	// VIU1 Move upper vector to lower

	stvx	VPS0,DST,BK	// LSU Store 16 bytes at D2
	addi	BK,BK,16	// IU1 Increment byte count by 16
	bdnzf	27,GT_4QW_fwd	// b if next store is to lower (even) half of CL
// At this point next store will be to even address.

	mtcrf	0x02,DBK	// IU2 cr6[3]=((last store)[27]==1)?1:0; (odd?)
	lis	STR,0x104	// IU1 Stream 4 blocks of 16 bytes
	addi	BL,BK,16	// IU1 Create an alternate byte kount + 32

	ori	STR,STR,32	// IU1 Stream stride 32B
#ifndef NO_BIG_LOOP
	rlwinm	BIG,Rt,29,3,31	// IU1 QW/8 big loops to do

	rlwinm	Rt,Rt,0,0,28	// IU1 How many QWs will be done in big loop
	bgt	cr7,Big_loop	// b if QW > 14
#endif
No_big_loop:
// We need the ctr register to reflect an even byte count before entering
// the next block - faster to decrement than to reload.

	addi	SP8,SRC,256	// IU1 Starting address for data stream touch
	xoris	STR,STR,0x6	// IU1 Reset stream to 2 blocks of 16 bytes
	bns	cr6,B32_fwd	// b if DST[27] == 0; i.e, final store is even

	bdnz	B32_fwd		// decrement counter for last QW store odd

B32_fwd:	// Should be at least 2 stores remaining and next 2 are cache aligned
	lvx	VS1,SRC,BK	// LSU Get S12
	addi	SP8,SP8,32	// IU1 Next starting address for data stream touch

	lvx	VS2,SRC,BL	// LSU Get S13
	vperm	VPS1,VS0,VS1,VP3	// VPU Align S11 and S12 to D11

	STRM_1			// LSU Stream 64 byte blocks ahead of loads

	DCBK			// LSU then Kill instead of RWITM

	vperm	VPS0,VS1,VS2,VP3	// VPU Align S12 and S13 to D12
	vor	VS0,VS2,VS2	// VIU1 Move S13 to S11

	stvx	VPS1,DST,BK	// LSU Store 16 bytes at D11
	addi	BK,BL,16	// IU1 Increment byte count
	bdz	Nxt_loc_fwd	// always decrement and branch to next instr		

Nxt_loc_fwd:
	stvx	VPS0,DST,BL	// LSU Store 16 bytes at D12
	addi	BL,BK,16	// IU1 Increment alternate byte count
	bdnz	B32_fwd		// b if there are at least two more QWs to do

	bso	cr6,One_even_QW	// b if there is one even and one odd QW to store
	b	Last_ld_fwd	// b if last store is to even address

// Come here with two more loads and two stores to do
One_even_QW:
	lvx	VS1,SRC,BK	// LSU Get S14 (or S13 if if D-S>=0)

	vperm	VPS0,VS0,VS1,VP3	// VPU Align S13 and S14 to D13
	vor	VS0,VS1,VS1	// VIU1 Move upper vector to lower

	stvx	VPS0,DST,BK	// LSU Store 16 bytes at D13
	addi	BK,BK,16	// IU1 Increment byte count

	b	Last_ld_fwd

#ifdef __MWERKS__
	.align	16
#else
	.align	4
#endif
Big_loop:
	subf	QW,Rt,QW	// IU1 Should be 2-7 QWs left after big loop
	blt	cr5,No_big_loop	// b back if |DST-SRC|<128; Big_loop won't work.
	mtctr	BIG		// IU2 loop for as many 128B loops as possible
	addi	SP8,SRC,256	// IU1 Starting address for data stream touch

Loop_of_128B:	// Come here with QW>=10 and next store even; VS0 last load
	lvx	VS1,SRC,BK	// LSU Get S4 (or S3 if D-S>=0)
	addi	BL,BK,32	// IU1 Increment Byte_Kount+16 by 32	
	addi	SP8,SP8,128	// IU1 increment address for data stream touch

	lvx	VS3,SRC,BL	// LSU Get S6 (or S5)
	addi	BL,BL,32	// IU1 Increment Byte_Kount+48 by 32	

	lvx	VS5,SRC,BL	// LSU Get S8 (or S7)
	addi	BL,BL,32	// IU1 Increment Byte_Kount+80 by 32	

	lvx	VS7,SRC,BL	// LSU Get S10 (or S9)
	addi	BL,BK,16	// IU1 Increment Byte_Kount+16 by 16	

	lvx	VS2,SRC,BL	// LSU Get S5 (or S4)
	addi	BL,BL,32	// IU1 Increment Byte_Kount+32 by 32	

	lvx	VS4,SRC,BL	// LSU Get S7 (or S6)
	addi	BL,BL,32	// IU1 Increment Byte_Kount+64 by 32	
	
	lvx	VS6,SRC,BL	// LSU Get S9 (or S8)
	addi	BL,BL,32	// IU1 Increment Byte_Kount+96 by 32	
	vperm	VPS0,VS0,VS1,VP3	// VPU

	lvx	VS0,SRC,BL	// LSU Get S11 (or S10)
	vperm	VPS1,VS1,VS2,VP3	// VPU

	STRM_1			// LSU Stream 4 32B blocks, stride 32B

	DCBK			// LSU then Kill instead of RWITM

	stvx	VPS0,DST,BK	// LSU Store D3
	addi	BK,BK,16	// IU1 Increment Byte_Kount+16 by 16	
	vperm	VPS2,VS2,VS3,VP3	// VPU

	stvx	VPS1,DST,BK	// LSU Store D4
	addi	BK,BK,16	// IU1 Increment Byte_Kount+32 by 16	
	vperm	VPS3,VS3,VS4,VP3	// VPU

	DCBK			// LSU then Kill instead of RWITM

	stvx	VPS2,DST,BK	// LSU Store D5
	addi	BK,BK,16	// IU1 Increment Byte_Kount+48 by 16	
	vperm	VPS4,VS4,VS5,VP3	// VPU

	stvx	VPS3,DST,BK	// LSU Store D6
	addi	BK,BK,16	// IU1 Increment Byte_Kount+64 by 16	
	vperm	VPS5,VS5,VS6,VP3	// VPU

	DCBK			// LSU then Kill instead of RWITM

	stvx	VPS4,DST,BK	// LSU Store D7
	addi	BK,BK,16	// IU1 Increment Byte_Kount+80 by 16	
	vperm	VPS6,VS6,VS7,VP3	// VPU

	stvx	VPS5,DST,BK	// LSU Store D8
	addi	BK,BK,16	// IU1 Increment Byte_Kount+96 by 16	
	vperm	VPS7,VS7,VS0,VP3	// VPU

	DCBK			// LSU then Kill instead of RWITM

	stvx	VPS6,DST,BK	// LSU Store D9
	addi	BK,BK,16	// IU1 Increment Byte_Kount+112 by 16	

	stvx	VPS7,DST,BK	// LSU Store D10
	addi	BK,BK,16	// IU1 Increment Byte_Kount+128 by 16	
	bdnz	Loop_of_128B	// b if ctr > 0 (QW/8 still > 0)

	mtctr	QW		// IU1 Restore QW remaining to counter
	addi	BL,BK,16	// IU1 Create an alternate byte kount + 16
	bns	cr6,B32_fwd	// b if DST[27] == 0; i.e, final store is even

	bdnz	B32_fwd		// b and decrement counter for last QW store odd
				// One of the above branches should have taken

// End of memcpy in AltiVec

// bcopy works like memcpy, but the source and destination operands are reversed.
// Following will just reverse the operands and branch to memcpy.

#ifdef LIBMOTOVEC
	.globl	bcopy     
bcopy:
#else
	.globl	vec_bcopy     
vec_bcopy:
#endif
	mr	Rt,DST		// temp storage for what is really source address (r3)
	mr	DST,SRC		// swap destination address to r3 to match memcpy dst
	mr	SRC,Rt		// Complete swap of destination and source for memcpy
#ifdef LIBMOTOVEC
	b	memcpy		// b to memcpy with correct args in r3 and r4	
#else
	b	_vec_memcpy	// b to vec_memcpy with correct args in r3 and r4	
#endif
// End of bcopy in AltiVec
