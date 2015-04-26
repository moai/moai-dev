/*
 * Copyright (c) 2007
 *	Josep Torra.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <liboil/liboilclasses.h>
#include <liboil/liboilfunction.h>

#if defined(__VFP_FP__) && !defined(__SOFTFP__)

extern void vfp_add_f32 (float *d, const float *s1, const float *s2, int n);
extern void vfp_add_f64 (double *d, const double *s1, const double *s2, int n);
extern void vfp_divide_f32 (float *d, const float *s1, const float *s2, int n);
extern void vfp_divide_f64 (double *d, const double *s1, const double *s2, int n);
extern void vfp_multiply_f32 (float *d, const float *s1, const float *s2, int n);
extern void vfp_multiply_f64 (double *d, const double *s1, const double *s2, int n);
extern void vfp_subtract_f32 (float *d, const float *s1, const float *s2, int n);
extern void vfp_subtract_f64 (double *d, const double *s1, const double *s2, int n);

extern void vfp_scalaradd_f32_ns (float *d, const float *s1, const float *s2_1, int n);
extern void vfp_scalaradd_f64_ns (double *d, const double *s1, const double *s2_1, int n);
extern void vfp_scalarmultiply_f32_ns (float *d, const float *s1, const float *s2_1, int n);
extern void vfp_scalarmultiply_f64_ns (double *d, const double *s1, const double *s2_1, int n);

extern void vfp_abs_f32_f32_ns(float *d, const float *s, int n);
extern void vfp_abs_f64_f64_ns(double *d, const double *s, int n);
extern void vfp_negative_f32(float *d, const float *s, int n);
extern void vfp_negative_f64(double *d, const double *s, int n);

OIL_DEFINE_IMPL_FULL (vfp_add_f32, add_f32, OIL_IMPL_FLAG_VFP);
OIL_DEFINE_IMPL_FULL (vfp_add_f64, add_f64, OIL_IMPL_FLAG_VFP);

OIL_DEFINE_IMPL_FULL (vfp_divide_f32, divide_f32, OIL_IMPL_FLAG_VFP);
OIL_DEFINE_IMPL_FULL (vfp_divide_f64, divide_f64, OIL_IMPL_FLAG_VFP);

OIL_DEFINE_IMPL_FULL (vfp_multiply_f32, multiply_f32, OIL_IMPL_FLAG_VFP);
OIL_DEFINE_IMPL_FULL (vfp_multiply_f64, multiply_f64, OIL_IMPL_FLAG_VFP);

OIL_DEFINE_IMPL_FULL (vfp_subtract_f32, subtract_f32, OIL_IMPL_FLAG_VFP);
OIL_DEFINE_IMPL_FULL (vfp_subtract_f64, subtract_f64, OIL_IMPL_FLAG_VFP);

OIL_DEFINE_IMPL_FULL (vfp_scalaradd_f32_ns, scalaradd_f32_ns, OIL_IMPL_FLAG_VFP);
//OIL_DEFINE_IMPL_FULL (vfp_scalaradd_f64_ns, scalaradd_f64_ns, OIL_IMPL_FLAG_VFP);

OIL_DEFINE_IMPL_FULL (vfp_scalarmultiply_f32_ns, scalarmultiply_f32_ns, OIL_IMPL_FLAG_VFP);
OIL_DEFINE_IMPL_FULL (vfp_scalarmultiply_f64_ns, scalarmultiply_f64_ns, OIL_IMPL_FLAG_VFP);

//OIL_DEFINE_IMPL_FULL (vfp_abs_f32_f32_ns, abs_f32_f32_ns, OIL_IMPL_FLAG_VFP);
//OIL_DEFINE_IMPL_FULL (vfp_abs_f64_f64_ns, abs_f64_f64_ns, OIL_IMPL_FLAG_VFP);

OIL_DEFINE_IMPL_FULL (vfp_negative_f32, negative_f32, OIL_IMPL_FLAG_VFP);
//OIL_DEFINE_IMPL_FULL (vfp_negative_f64, negative_f64, OIL_IMPL_FLAG_VFP);

#endif
