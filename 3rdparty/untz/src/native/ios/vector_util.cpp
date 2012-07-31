//
//  vector_util.cpp
//  Part of UNTZ
//
//  Created by Zach Saul (zach@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "vector_util.h"
#include "TargetConditionals.h"

#include "math.h"
#define TARGET_IPHONE_SIMULATOR 1
void vector_fmul_vfp(float *dst, const float *src, int len)
{
#if TARGET_IPHONE_SIMULATOR
    for (int i=0;i<len;i++)
    {
        dst[i] *= src[i];
    }
#else
    int tmp;
    __asm__ __volatile__(
						 "fmrx %[tmp], fpscr\n\t"
						 "orr %[tmp], %[tmp], #(3 << 16)\n\t" /* set vector size to 4 */
						 "fmxr fpscr, %[tmp]\n\t"
						 
						 "fldmias %[src1]!, {s0-s3}\n\t"
						 "fldmias %[src2]!, {s8-s11}\n\t"
						 "fldmias %[src1]!, {s4-s7}\n\t"
						 "fldmias %[src2]!, {s12-s15}\n\t"
						 "fmuls s8, s0, s8\n\t"
						 "1:\n\t"
						 "subs %[len], %[len], #16\n\t"
						 "fmuls s12, s4, s12\n\t"
						 "fldmiasge %[src1]!, {s16-s19}\n\t"
						 "fldmiasge %[src2]!, {s24-s27}\n\t"
						 "fldmiasge %[src1]!, {s20-s23}\n\t"
						 "fldmiasge %[src2]!, {s28-s31}\n\t"
						 "fmulsge s24, s16, s24\n\t"
						 "fstmias %[dst]!, {s8-s11}\n\t"
						 "fstmias %[dst]!, {s12-s15}\n\t"
						 "fmulsge s28, s20, s28\n\t"
						 "fldmiasgt %[src1]!, {s0-s3}\n\t"
						 "fldmiasgt %[src2]!, {s8-s11}\n\t"
						 "fldmiasgt %[src1]!, {s4-s7}\n\t"
						 "fldmiasgt %[src2]!, {s12-s15}\n\t"
						 "fmulsge s8, s0, s8\n\t"
						 "fstmiasge %[dst]!, {s24-s27}\n\t"
						 "fstmiasge %[dst]!, {s28-s31}\n\t"
						 "bgt 1b\n\t"
						 
						 "bic %[tmp], %[tmp], #(7 << 16)\n\t" /* set vector size back to 1 */
						 "fmxr fpscr, %[tmp]\n\t"
						 : [dst] "+&r" (dst), [src1] "+&r" (dst), [src2] "+&r" (src), [len] "+&r" (len), [tmp] "=&r" (tmp)
						 :
						 : "s0",  "s1",  "s2",  "s3",  "s4",  "s5",  "s6",  "s7",
						 "s8",  "s9",  "s10", "s11", "s12", "s13", "s14", "s15",
						 "s16", "s17", "s18", "s19", "s20", "s21", "s22", "s23",
						 "s24", "s25", "s26", "s27", "s28", "s29", "s30", "s31",
						 "cc", "memory");
#endif
}

void vector_fmul1_vfp(float *dst, const float *src, float *mult, int len)
{
#if TARGET_IPHONE_SIMULATOR
    for (int sample=0;sample<len;sample++)
        dst[sample] = src[sample]*(*mult);           
#else
    
    __asm__ __volatile__(						 
						 "fldmias %[mult], {s8}\n\t"
						 "fldmias %[src]!, {s0-s3}\n\t"
						 "fldmias %[src]!, {s4-s7}\n\t"
						 "fmuls s0, s0, s8\n\t"
						 "fmuls s1, s1, s8\n\t"
						 "fmuls s2, s2, s8\n\t"
						 "fmuls s3, s3, s8\n\t"
						 "1:\n\t"
						 "subs %[len], %[len], #16\n\t"
						 "fmuls s4, s4, s8\n\t"
						 "fmuls s5, s5, s8\n\t"
						 "fmuls s6, s6, s8\n\t"
						 "fmuls s7, s7, s8\n\t"
						 "fldmiasge %[src]!, {s16-s19}\n\t"
						 "fldmiasge %[src]!, {s20-s23}\n\t"
						 "fmulsge s16, s16, s8\n\t"
						 "fmulsge s17, s17, s8\n\t"
						 "fmulsge s18, s18, s8\n\t"
						 "fmulsge s19, s19, s8\n\t"
						 "fstmias %[dst]!, {s0-s3}\n\t"
						 "fstmias %[dst]!, {s4-s7}\n\t"
						 "fmulsge s20, s20, s8\n\t"
						 "fmulsge s21, s21, s8\n\t"
						 "fmulsge s22, s22, s8\n\t"
						 "fmulsge s23, s23, s8\n\t"
						 "fldmiasgt %[src]!, {s0-s3}\n\t"
						 "fldmiasgt %[src]!, {s4-s7}\n\t"
						 "fmulsgt s0, s0, s8\n\t"
						 "fmulsgt s1, s1, s8\n\t"
						 "fmulsgt s2, s2, s8\n\t"
						 "fmulsgt s3, s3, s8\n\t"
						 "fstmiasge %[dst]!, {s16-s19}\n\t"
						 "fstmiasge %[dst]!, {s20-s23}\n\t"
						 "bgt 1b\n\t"
						 
						 : [dst] "+&r" (dst), [src] "+&r" (src), [mult] "+&r" (mult), [len] "+&r" (len)
						 :
						 : "s0",  "s1",  "s2",  "s3",  "s4",  "s5",  "s6",  "s7",
						 "s8",  "s9",  "s10", "s11", "s12", "s13", "s14", "s15",
						 "s16", "s17", "s18", "s19", "s20", "s21", "s22", "s23",
						 "s24", "s25", "s26", "s27", "s28", "s29", "s30", "s31",
						 "cc", "memory");
#endif
}


void vector_fmac1_vfp(float *dst, const float *src, float *mult, int len) //this += (a*b)
{
#if TARGET_IPHONE_SIMULATOR
    for (int sample=0;sample<len;sample++)
        dst[sample] += src[sample]*(*mult);
    return;
#else
    {
        __asm__ __volatile__(						 
                             "fldmias %[mult], {s16}\n\t"
                             "fldmias %[src1]!, {s0-s3}\n\t"
                             "fldmias %[src2]!, {s8-s11}\n\t"
                             "fmacs s0, s8, s16\n\t"
                             "fmacs s1, s9, s16\n\t"
                             "fldmias %[src1]!, {s4-s7}\n\t"
                             "fldmias %[src2]!, {s12-s15}\n\t"						 
                             "fmacs s2, s10, s16\n\t"
                             "fmacs s3, s11, s16\n\t"
                             "1:\n\t"
                             "subs %[len], %[len], #8\n\t"
                             "fstmias %[dst]!, {s0-s3}\n\t"
                             "fmacs s4, s12, s16\n\t"
                             "fmacs s5, s13, s16\n\t"
                             "fldmiasgt %[src1]!, {s0-s3}\n\t"
                             "fldmiasgt %[src2]!, {s8-s11}\n\t"
                             "fmacs s6, s14, s16\n\t"
                             "fmacs s7, s15, s16\n\t"
                             "fstmias %[dst]!, {s4-s7}\n\t"
                             "fmacsgt s0, s8, s16\n\t"
                             "fmacsgt s1, s9, s16\n\t"
                             "fldmiasgt %[src1]!, {s4-s7}\n\t"
                             "fldmiasgt %[src2]!, {s12-s15}\n\t"						 
                             "fmacsgt s2, s10, s16\n\t"
                             "fmacsgt s3, s11, s16\n\t"
                             "bgt 1b\n\t"
                             
                             : [dst] "+&r" (dst), [src1] "+&r" (dst), [src2] "+&r" (src), [mult] "+&r" (mult), [len] "+&r" (len)
                             :
                             : "s0",  "s1",  "s2",  "s3",  "s4",  "s5",  "s6",  "s7",
                             "s8",  "s9",  "s10", "s11", "s12", "s13", "s14", "s15",
                             "s16", "s17", "s18", "s19", "s20", "s21", "s22", "s23",
                             "s24", "s25", "s26", "s27", "s28", "s29", "s30", "s31",
                             "cc", "memory");
    }

#endif
}

void float_to_int16_vfp(short *dst, float *src, int len)
{
	//FLDM load float IA increment after S single precision 
	//FTO float to SI singed integer S single precision
	//SUBS length = length - 8 and updated flags used in GT instructions to follow
	//FMRRS transfers two floats to ARM registers from VFP registers
	//FLDMIAS same as above GT if result of SUBS greater than 0
	//SSAT saturate int (multiply it for the float to 16bit conversion with clipping)
	//PKHBT pack 16 bit ints into registers LSL option shifts one before packing
	//FTOSIS same as above GT if result of SUBS greater than 0
	//STM store multiple registers IA increment after
	//B branch GT if greater than 0
#if TARGET_IPHONE_SIMULATOR
    for (int i=0;i<len;i++)
    {
        float val=src[i];
        if (val>1.0)
           val=1.0;
        else if (val<-1.0)
            val=-1.0;
        dst[i] = val*32767;
    }
#else
	// ZMS:  This code may do the vector optimized float to int conversion.  It also might be susceptible to some kind of digital clipping (depending on what the ftosisgt instruction does).
	float top_val = 32767.0;
	float *top = &top_val;
        __asm__ __volatile__(
							 "fldmias %[top], {s24}\n\t" /* zach, load 32767 into s24 */
							 "fldmias %[src]!, {s16-s23}\n\t"
							 "fmuls s16, s16, s24\n\t" /*ZMS, multiply each value by 32767 */
							 "fmuls s17, s17, s24\n\t"
							 "fmuls s18, s18, s24\n\t"
							 "fmuls s19, s19, s24\n\t"
							 "fmuls s20, s20, s24\n\t"
							 "fmuls s21, s21, s24\n\t"
							 "fmuls s22, s22, s24\n\t"
							 "fmuls s23, s23, s24\n\t"
							 "ftosis s0, s16\n\t"
							 "ftosis s1, s17\n\t"
							 "ftosis s2, s18\n\t"
							 "ftosis s3, s19\n\t"
							 "ftosis s4, s20\n\t"
							 "ftosis s5, s21\n\t"
							 "ftosis s6, s22\n\t"
							 "ftosis s7, s23\n\t"
							 "1:\n\t"
							 "subs %[len], %[len], #8\n\t"
							 "fmrrs r3, r4, {s0, s1}\n\t"
							 "fmrrs r5, r6, {s2, s3}\n\t"
							 "fldmiasgt %[src]!, {s16-s23}\n\t"
							 "fmrrs r9, r8, {s4, s5}\n\t"
							 "fmrrs ip, lr, {s6, s7}\n\t"
							 "pkhbt r3, r3, r4, lsl #16\n\t"
							 "pkhbt r4, r5, r6, lsl #16\n\t"
							 "fmuls s16, s16, s24\n\t" /*ZMS, multiply each value by 32767 */
							 "fmuls s17, s17, s24\n\t"
							 "fmuls s18, s18, s24\n\t"
							 "fmuls s19, s19, s24\n\t"
							 "fmuls s20, s20, s24\n\t"
							 "fmuls s21, s21, s24\n\t"
							 "fmuls s22, s22, s24\n\t"
							 "fmuls s23, s23, s24\n\t"							 
							 "ftosisgt s0, s16\n\t"
							 "ftosisgt s1, s17\n\t"
							 "ftosisgt s2, s18\n\t"
							 "ftosisgt s3, s19\n\t"
							 "ftosisgt s4, s20\n\t"
							 "ftosisgt s5, s21\n\t"
							 "ftosisgt s6, s22\n\t"
							 "ftosisgt s7, s23\n\t"
							 "pkhbt r5, r9, r8, lsl #16\n\t"
							 "pkhbt r6, ip, lr, lsl #16\n\t"
							 "stmia %[dst]!, {r3-r6}\n\t"
							 "bgt 1b\n\t"
							 
							 : [dst] "+&r" (dst), [src] "+&r" (src), [len] "+&r" (len)
							 : [top] "r" (top)
							 : "s0",  "s1",  "s2",  "s3",  "s4",  "s5",  "s6",  "s7",
							 "s16", "s17", "s18", "s19", "s20", "s21", "s22", "s23", "s24",
							 "r3", "r4", "r5", "r6", "r9", "r8", "ip", "lr",
							 "cc", "memory");
#endif
}
void limit_float(float *srcdest,int numSamples)
{
    float twoOverPi = 2.0f / 3.14f;
    float gain = 2.0f;
    for (int i=0;i<numSamples;i++)
    {
        float inValue = srcdest[i];
        int inSign = 1-2*signbit(inValue);
        
        float absValue = fabs(inValue);
        
        //float powValue = powf(absValue, softness);
        //float atanValue = atanf(powValue);
        //float value = powf(atanValue*twoOverPi, oneOverSoftness);
        float value = atanf(absValue*gain) * twoOverPi;
        //        if (value >= 1.0f)
        //            NSLog(@"value = %f", value);
        srcdest[i] = inSign * value;
        
        /*
         if (left[i] > 1.0f)
         left[i] = 1.0f;
         else if (left[i] < -1.0f)
         left[i] = -1.0f;
         */
    }
}
