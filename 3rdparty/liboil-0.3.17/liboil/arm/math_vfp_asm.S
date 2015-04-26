/*
 * Copyright (c) 2007
 *	Josep Torra <josep@fluendo.com>.  All rights reserved.
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

#if defined(__VFP_FP__) && !defined(__SOFTFP__)
/* 
** compile with -mcpu=arm1136j-s -mfpu=vfp -mfloat-abi=softfp
**
** void vfp_add_f32 (float *d, const float *s1, const float *s2, int n);
** void vfp_add_f64 (double *d, const double *s1, const double *s2, int n);
** void vfp_divide_f32 (float *d, const float *s1, const float *s2, int n);
** void vfp_divide_f64 (double *d, const double *s1, const double *s2, int n);
** void vfp_multiply_f32 (float *d, const float *s1, const float *s2, int n);
** void vfp_multiply_f64 (double *d, const double *s1, const double *s2, int n);
** void vfp_subtract_f32 (float *d, const float *s1, const float *s2, int n);
** void vfp_subtract_f64 (double *d, const double *s1, const double *s2, int n);
**
** d:   $r0     |   s1: $r1     | s2:  $r2     |   n:  $r3     |
**
*/

#define UNROLL_F32_TEMPLATE(fname,finst) \
  .global vfp_ ## fname ## ;                                                  \
  vfp_ ## fname ## :                                                          \
    stmdb         sp!, {fp, lr};            /* save registers to stack */     \
    ands          ip, r3, #7;               /* ip = n % 8 */                  \
    beq           vfp_ ## fname ## _unroll; /* if ip == 0 goto prep_loop2 */  \
  vfp_ ## fname ## _loop1:                                                    \
    fldmias       r1!, {s0};                                                  \
    fldmias       r2!, {s1};                                                  \
    ## finst ##s  s2, s0, s1;                                                 \
    fstmias       r0!, {s2};                                                  \
    subs          ip, ip, #1;                                                 \
    bne           vfp_ ## fname ## _loop1;                                    \
  vfp_ ## fname ## _unroll:                 /* unroll by 8 */                 \
    movs          ip, r3, lsr #3;           /* ip = n / 8 */                  \
    beq           vfp_ ## fname ## _end;    /* if ip == 0 goto finish */      \
    fmrx          lr, fpscr;                /* read fpscr register into arm */\
    mov           fp, #7;                                                     \
    orr           fp, lr, fp, lsl #16;      /* set vector lenght to 8 */      \
    fmxr          fpscr, fp;                                                  \
  vfp_ ## fname ## _loop2:                                                    \
    fldmias       r1!, {s8, s9, s10, s11, s12, s13, s14, s15};                \
    fldmias       r2!, {s16, s17, s18, s19, s20, s21, s22, s23};              \
    ## finst ##s  s24, s8, s16;                                               \
    fstmias       r0!, {s24, s25, s26, s27, s28, s29, s30, s31};              \
    subs          ip, ip, #1;                                                 \
    bne           vfp_ ## fname ## _loop2;                                    \
    fmxr          fpscr, lr;                /* restore original fpscr */      \
  vfp_ ## fname ## _end:                                                      \
    ldmia         sp!, {fp, pc};        /* recovering from stack and return */   

#define UNROLL_F64_TEMPLATE(fname,finst) \
  .global vfp_ ## fname ## ;                                                  \
  vfp_ ## fname ## :                                                          \
    stmdb         sp!, {fp, lr};            /* save registers to stack */     \
    ands          ip, r3, #3;               /* ip = n % 3 */                  \
    beq           vfp_ ## fname ## _unroll; /* if ip == 0 goto prep_loop2 */  \
  vfp_ ## fname ## _loop1:                                                    \
    fldmiad       r1!, {d0};                                                  \
    fldmiad       r2!, {d1};                                                  \
    ## finst ##d  d2, d0, d1;                                                 \
    fstmiad       r0!, {d2};                                                  \
    subs          ip, ip, #1;                                                 \
    bne           vfp_ ## fname ## _loop1;                                    \
  vfp_ ## fname ## _unroll:                 /* unroll by 4 */                 \
    movs          ip, r3, lsr #2;           /* ip = n / 4 */                  \
    beq           vfp_ ## fname ## _end;    /* if ip == 0 goto finish */      \
    fmrx          lr, fpscr;                /* read fpscr register into arm */\
    mov           fp, #3;                                                     \
    orr           fp, lr, fp, lsl #16;      /* set vector lenght to 8 */      \
    fmxr          fpscr, fp;                                                  \
  vfp_ ## fname ## _loop2:                                                    \
    fldmiad       r1!, {d4, d5, d6, d7};                                      \
    fldmiad       r2!, {d8, d9, d10, d11};                                    \
    ## finst ##d  d12, d4, d8;                                                \
    fstmiad       r0!, {d12, d13, d14, d15};                                  \
    subs          ip, ip, #1;                                                 \
    bne           vfp_ ## fname ## _loop2;                                    \
    fmxr          fpscr, lr;                /* restore original fpscr */      \
  vfp_ ## fname ## _end:                                                      \
    ldmia         sp!, {fp, pc};        /* recovering from stack and return */   

.align 2
UNROLL_F32_TEMPLATE(add_f32,fadd);
UNROLL_F64_TEMPLATE(add_f64,fadd);

UNROLL_F32_TEMPLATE(divide_f32,fdiv);
UNROLL_F64_TEMPLATE(divide_f64,fdiv);

UNROLL_F32_TEMPLATE(multiply_f32,fmul);
UNROLL_F64_TEMPLATE(multiply_f64,fmul);

UNROLL_F32_TEMPLATE(subtract_f32,fsub);
UNROLL_F64_TEMPLATE(subtract_f64,fsub);

#undef UNROLL_F32_TEMPLATE
#undef UNROLL_F64_TEMPLATE

/* 
**
** void vfp_scalaradd_f32_ns (float *d, const float *s1, const float *s2_1, int n);
** void vfp_scalaradd_f64_ns (double *d, const double *s1, const double *s2_1, int n);
** void vfp_scalarmultiply_f32_ns (float *d, const float *s1, const float *s2_1, int n);
** void vfp_scalarmultiply_f64_ns (double *d, const double *s1, const double *s2_1, int n);
**
** d:   $r0     |   s1: $r1     | s2_1:  $r2   |   n:  $r3     |
**
*/
#define UNROLL_F32_TEMPLATE(fname,finst) \
  .global vfp_ ## fname ## ;                                                  \
  vfp_ ## fname ## :                                                          \
    stmdb         sp!, {fp, lr};            /* save registers to stack */     \
    fldmias       r2, {s1};                 /* load scalar value */           \
    ands          ip, r3, #7;               /* ip = n % 8 */                  \
    beq           vfp_ ## fname ## _unroll; /* if ip == 0 goto prep_loop2 */  \
  vfp_ ## fname ## _loop1:                                                    \
    fldmias       r1!, {s0};                                                  \
    ## finst ##s  s2, s0, s1;                                                 \
    fstmias       r0!, {s2};                                                  \
    subs          ip, ip, #1;                                                 \
    bne           vfp_ ## fname ## _loop1;                                    \
  vfp_ ## fname ## _unroll:                 /* unroll by 8 */                 \
    movs          ip, r3, lsr #3;           /* ip = n / 8 */                  \
    beq           vfp_ ## fname ## _end;    /* if ip == 0 goto finish */      \
    fmrx          lr, fpscr;                /* read fpscr register into arm */\
    mov           fp, #7;                                                     \
    orr           fp, lr, fp, lsl #16;      /* set vector lenght to 8 */      \
    fmxr          fpscr, fp;                                                  \
  vfp_ ## fname ## _loop2:                                                    \
    fldmias       r1!, {s8, s9, s10, s11, s12, s13, s14, s15};                \
    ## finst ##s  s24, s8, s1;                                                \
    fstmias       r0!, {s24, s25, s26, s27, s28, s29, s30, s31};              \
    subs          ip, ip, #1;                                                 \
    bne           vfp_ ## fname ## _loop2;                                    \
    fmxr          fpscr, lr;                /* restore original fpscr */      \
  vfp_ ## fname ## _end:                                                      \
    ldmia         sp!, {fp, pc};        /* recovering from stack and return */   

#define UNROLL_F64_TEMPLATE(fname,finst) \
  .global vfp_ ## fname ## ;                                                  \
  vfp_ ## fname ## :                                                          \
    stmdb         sp!, {fp, lr};            /* save registers to stack */     \
    fldmiad       r2, {d1};                 /* load scalar value */           \
    ands          ip, r3, #3;               /* ip = n % 3 */                  \
    beq           vfp_ ## fname ## _unroll; /* if ip == 0 goto prep_loop2 */  \
  vfp_ ## fname ## _loop1:                                                    \
    fldmiad       r1!, {d0};                                                  \
    ## finst ##d  d2, d0, d1;                                                 \
    fstmiad       r0!, {d2};                                                  \
    subs          ip, ip, #1;                                                 \
    bne           vfp_ ## fname ## _loop1;                                    \
  vfp_ ## fname ## _unroll:                 /* unroll by 4 */                 \
    movs          ip, r3, lsr #2;           /* ip = n / 4 */                  \
    beq           vfp_ ## fname ## _end;    /* if ip == 0 goto finish */      \
    fmrx          lr, fpscr;                /* read fpscr register into arm */\
    mov           fp, #3;                                                     \
    orr           fp, lr, fp, lsl #16;      /* set vector lenght to 4 */      \
    fmxr          fpscr, fp;                                                  \
  vfp_ ## fname ## _loop2:                                                    \
    fldmiad       r1!, {d4, d5, d6, d7};                                      \
    ## finst ##d  d12, d4, d1;                                                \
    fstmiad       r0!, {d12, d13, d14, d15};                                  \
    subs          ip, ip, #1;                                                 \
    bne           vfp_ ## fname ## _loop2;                                    \
    fmxr          fpscr, lr;                /* restore original fpscr */      \
  vfp_ ## fname ## _end:                                                      \
    ldmia         sp!, {fp, pc};        /* recovering from stack and return */   

UNROLL_F32_TEMPLATE(scalaradd_f32_ns,fadd);
UNROLL_F64_TEMPLATE(scalaradd_f64_ns,fadd);

UNROLL_F32_TEMPLATE(scalarmultiply_f32_ns,fmul);
UNROLL_F64_TEMPLATE(scalarmultiply_f64_ns,fmul);

#undef UNROLL_F32_TEMPLATE
#undef UNROLL_F64_TEMPLATE

/* 
**
** void vfp_abs_f32_f32_ns(float *d, const float *s, int n);
** void vfp_abs_f64_f64_ns(double *d, const double *s, int n);
** void vfp_negative_f32(float *d, const float *s, int n);
** void vfp_negative_f64(double *d, const double *s, int n);
**
** d:   $r0     |   s: $r1      |   n:  $r2     |
**
*/
#define UNROLL_F32_TEMPLATE(fname,finst) \
  .global vfp_ ## fname ## ;                                                  \
  vfp_ ## fname ## :                                                          \
    stmdb         sp!, {fp, lr};            /* save registers to stack */     \
    ands          ip, r2, #7;               /* ip = n % 8 */                  \
    beq           vfp_ ## fname ## _unroll; /* if ip == 0 goto prep_loop2 */  \
  vfp_ ## fname ## _loop1:                                                    \
    fldmias       r1!, {s0};                                                  \
    ## finst ##s  s2, s0;                                                     \
    fstmias       r0!, {s2};                                                  \
    subs          ip, ip, #1;                                                 \
    bne           vfp_ ## fname ## _loop1;                                    \
  vfp_ ## fname ## _unroll:                 /* unroll by 8 */                 \
    movs          ip, r2, lsr #3;           /* ip = n / 8 */                  \
    beq           vfp_ ## fname ## _end;    /* if ip == 0 goto finish */      \
    fmrx          lr, fpscr;                /* read fpscr register into arm */\
    mov           fp, #7;                                                     \
    orr           fp, lr, fp, lsl #16;      /* set vector lenght to 8 */      \
    fmxr          fpscr, fp;                                                  \
  vfp_ ## fname ## _loop2:                                                    \
    fldmias       r1!, {s8, s9, s10, s11, s12, s13, s14, s15};                \
    ## finst ##s  s24, s8;                                                    \
    fstmias       r0!, {s24, s25, s26, s27, s28, s29, s30, s31};              \
    subs          ip, ip, #1;                                                 \
    bne           vfp_ ## fname ## _loop2;                                    \
    fmxr          fpscr, lr;                /* restore original fpscr */      \
  vfp_ ## fname ## _end:                                                      \
    ldmia         sp!, {fp, pc};        /* recovering from stack and return */   

#define UNROLL_F64_TEMPLATE(fname,finst) \
  .global vfp_ ## fname ## ;                                                  \
  vfp_ ## fname ## :                                                          \
    stmdb         sp!, {fp, lr};            /* save registers to stack */     \
    ands          ip, r2, #3;               /* ip = n % 3 */                  \
    beq           vfp_ ## fname ## _unroll; /* if ip == 0 goto prep_loop2 */  \
  vfp_ ## fname ## _loop1:                                                    \
    fldmiad       r1!, {d0};                                                  \
    ## finst ##d  d2, d0;                                                     \
    fstmiad       r0!, {d2};                                                  \
    subs          ip, ip, #1;                                                 \
    bne           vfp_ ## fname ## _loop1;                                    \
  vfp_ ## fname ## _unroll:                 /* unroll by 4 */                 \
    movs          ip, r2, lsr #2;           /* ip = n / 4 */                  \
    beq           vfp_ ## fname ## _end;    /* if ip == 0 goto finish */      \
    fmrx          lr, fpscr;                /* read fpscr register into arm */\
    mov           fp, #3;                                                     \
    orr           fp, lr, fp, lsl #16;      /* set vector lenght to 4 */      \
    fmxr          fpscr, fp;                                                  \
  vfp_ ## fname ## _loop2:                                                    \
    fldmiad       r1!, {d4, d5, d6, d7};                                      \
    ## finst ##d  d12, d4;                                                    \
    fstmiad       r0!, {d12, d13, d14, d15};                                  \
    subs          ip, ip, #1;                                                 \
    bne           vfp_ ## fname ## _loop2;                                    \
    fmxr          fpscr, lr;                /* restore original fpscr */      \
  vfp_ ## fname ## _end:                                                      \
    ldmia         sp!, {fp, pc};        /* recovering from stack and return */   

UNROLL_F32_TEMPLATE(abs_f32_f32_ns,fabs);
UNROLL_F64_TEMPLATE(abs_f64_f64_ns,fabs);

UNROLL_F32_TEMPLATE(negative_f32,fneg);
UNROLL_F64_TEMPLATE(negative_f64,fneg);

#undef UNROLL_F32_TEMPLATE
#undef UNROLL_F64_TEMPLATE
#endif
