//
//  vector_util.h
//  Part of UNTZ
//
//  Created by Zach Saul (zach@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef VECTORUTIL_H_
#define VECTORUTIL_H_

void vector_fmul_vfp(float *dst, const float *src, int len);
void vector_fmul1_vfp(float *dst, const float *src, float *mult, int len);
void vector_fmac1_vfp(float *dst, const float *src, float *mult, int len);
void float_to_int16_vfp(short *dst,  float *src, int len);
void limit_float(float *srcdest,int numSamples);

#endif