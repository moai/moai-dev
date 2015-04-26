dnl as-gcc-inline-assembly.m4 0.1.0

dnl autostars m4 macro for detection of gcc inline assembly

dnl David Schleef <ds@schleef.org>

dnl $Id: as-gcc-inline-assembly.m4,v 1.5 2007-11-14 06:48:57 ds Exp $

dnl AS_COMPILER_FLAG(ACTION-IF-ACCEPTED, [ACTION-IF-NOT-ACCEPTED])
dnl Tries to compile with the given CFLAGS.
dnl Runs ACTION-IF-ACCEPTED if the compiler can compile with the flags,
dnl and ACTION-IF-NOT-ACCEPTED otherwise.

AC_DEFUN([AS_GCC_INLINE_ASSEMBLY],
[
  AC_MSG_CHECKING([if compiler supports gcc-style inline assembly])

  AC_TRY_COMPILE([], [
#ifndef __GNUC__
#error Not GCC
#endif
#ifdef __GNUC_MINOR__
#if (__GNUC__ * 1000 + __GNUC_MINOR__) < 3004
#error GCC before 3.4 has critical bugs compiling inline assembly
#endif
#endif
__asm__ (""::) ], [flag_ok=yes], [flag_ok=no])

  if test "X$flag_ok" = Xyes ; then
    $1
    true
  else
    $2
    true
  fi
  AC_MSG_RESULT([$flag_ok])
])


AC_DEFUN([AS_GCC_ASM_POWERPC_FPU],
[
  AC_MSG_CHECKING([if compiler supports FPU instructions on PowerPC])

  AC_TRY_COMPILE([], [double x = 0; __asm__ ("fadd 0,0,0"::"f" (x):"32") ], [flag_ok=yes], [flag_ok=no])

  if test "X$flag_ok" = Xyes ; then
    $1
    true
  else
    $2
    true
  fi
  AC_MSG_RESULT([$flag_ok])
])

AC_DEFUN([AS_GCC_INLINE_ASM_INSTRUCTION],
[
  AC_MSG_CHECKING([if compiler supports '$1' asm instruction])

  AC_TRY_COMPILE([], [
#ifndef __GNUC__
#error Not GCC
#endif
#ifdef __GNUC_MINOR__
#if (__GNUC__ * 1000 + __GNUC_MINOR__) < 3004
#error GCC before 3.4 has critical bugs compiling inline assembly
#endif
#endif
__asm__ ("$1"::) ], [flag_ok=yes], [flag_ok=no])

  if test "X$flag_ok" = Xyes ; then
    $2
    true
  else
    $3
    true
  fi
  AC_MSG_RESULT([$flag_ok])
])

