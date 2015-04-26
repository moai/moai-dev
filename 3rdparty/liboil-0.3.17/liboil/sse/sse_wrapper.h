#ifndef __SSE_WRAPPER_H__
#define __SSE_WRAPPER_H__

/* A massive hack to work around gcc (and mono) alignment bugs.  This
 * realigns the stack to 16 bytes when calling a wrapped function. */

#ifdef USE_SSE_WRAPPER
#define OIL_SSE_WRAPPER(func) \
static void func () __attribute__ ((used)); \
static void func ## _wrapper (void) \
{ \
  __asm__ __volatile__ ("\n" \
      "  subl $0x20, %%esp\n" \
      "  andl $0xfffffff0, %%esp\n" \
      "  movdqu 0x08(%%ebp), %%xmm0\n" \
      "  movdqa %%xmm0, 0x00(%%esp)\n" \
      "  movdqu 0x18(%%ebp), %%xmm0\n" \
      "  movdqa %%xmm0, 0x10(%%esp)\n" \
      "  call *%[f]\n" \
      "  movl %%ebp, %%esp\n" \
      : \
      : [f] "r" (func) \
      : "xmm0"); \
}

#define OIL_DEFINE_IMPL_FULL_WRAPPER(func,klass,flags) \
OIL_SSE_WRAPPER(func) \
OIL_DEFINE_IMPL_FULL(func ## _wrapper, klass, flags)
#else
#define OIL_DEFINE_IMPL_FULL_WRAPPER(func,klass,flags) \
OIL_DEFINE_IMPL_FULL(func, klass, flags)
#endif

#endif

