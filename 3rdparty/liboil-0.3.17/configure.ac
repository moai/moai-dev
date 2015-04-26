AC_PREREQ([2.58])
AC_INIT(liboil,0.3.17)

AS_NANO(LIBOIL_CVS=no,LIBOIL_CVS=yes)

AC_CANONICAL_HOST([])
AM_INIT_AUTOMAKE(1.6)

AM_MAINTAINER_MODE

#LIBOIL_MAJORMINOR=0.3
#AC_SUBST(LIBOIL_MAJORMINOR)

AM_CONFIG_HEADER(config.h)

dnl CURRENT, REVISION, AGE
dnl - library source changed -> increment REVISION
dnl - interfaces added/removed/changed -> increment CURRENT, REVISION = 0
dnl - interfaces added -> increment AGE
dnl - interfaces removed -> AGE = 0
LIBOIL_LIBVERSION="3:0:3"
AC_SUBST(LIBOIL_LIBVERSION)
AC_LIBTOOL_WIN32_DLL
AC_LIBTOOL_TAGS([])
AC_PROG_LIBTOOL
AM_PROG_AS

AC_CONFIG_SRCDIR([liboil/liboil.h])
AC_CONFIG_MACRO_DIR([m4])

PKG_CHECK_MODULES(GLIB, glib-2.0, HAVE_GLIB=yes, HAVE_GLIB=no)
AC_SUBST(GLIB_LIBS)
AC_SUBST(GLIB_CFLAGS)
AC_ARG_ENABLE(glib,
AC_HELP_STRING([--disable-glib],[disable usage of glib]),
[case "${enableval}" in
  yes) HAVE_GLIB=yes ;;
  no) HAVE_GLIB=no ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --disable-glib) ;;
esac])
AM_CONDITIONAL(HAVE_GLIB, test "x$HAVE_GLIB" = "xyes")

AC_ARG_ENABLE(prototype-checking,
  AC_HELP_STRING([--enable-prototype-checking],[compile with strict prototype checking]),
  enable_proto_check=$enableval,enable_proto_check=no)
if test "x$enable_proto_check" = xyes ; then
  AC_DEFINE(LIBOIL_STRICT_PROTOTYPES,1,[Defined if prototype checking enabled])
fi

AC_ARG_WITH(alternate-optimization,
  AC_HELP_STRING([--with-alternate-optimization=CFLAGS],[compile with alternate optimization flags]),
  [enable_alt_opt=yes; LIBOIL_OPT_CFLAGS=$withval],enable_alt_opt=no)
if test "x$enable_alt_opt" = xyes ; then
  LIBOIL_OPT_CFLAGS="$LIBOIL_OPT_CFLAGS -D'OIL_OPT_MANGLE(a)=a\#\#_alt' -D'OIL_OPT_SUFFIX=\"_alt\"'"
fi
AM_CONDITIONAL(USE_ALT_OPT, test "x$enable_alt_opt" = xyes)
AC_SUBST(LIBOIL_OPT_CFLAGS)

AC_ARG_ENABLE(broken-implementations,
  AC_HELP_STRING([--enable-broken-implementations],[compile with broken implementations]),
  enable_broken_implementations=$enableval,enable_broken_implementations=no)
if test "x$enable_broken_implementations" = xyes ; then
  AC_DEFINE(ENABLE_BROKEN_IMPLS, 1, [Define if compiling broken implementations])
fi

#AC_ARG_ENABLE(new-abi,
#  AC_HELP_STRING([--enable-new-abi],[enable experimental 0.4 ABI]),
#  enable_new_abi=$enableval,enable_new_abi=no)
if test "x$enable_new_abi" = xyes ; then
  AC_DEFINE(ENABLE_NEW_ABI, 1, [Define if compiling new ABI])
fi
AM_CONDITIONAL(USE_NEW_ABI, test "x$enable_new_abi" = xyes)
if test "x$enable_new_abi" = xyes ; then
  LIBOIL_MAJORMINOR=0.4
  LIBOIL_OLD_MAJORMINOR=0.3
else 
  LIBOIL_MAJORMINOR=0.3
  LIBOIL_OLD_MAJORMINOR=0.2
fi
AC_SUBST(LIBOIL_MAJORMINOR)
AC_SUBST(LIBOIL_OLD_MAJORMINOR)

##################################################
# Check for gtk-doc.
##################################################

if test "x$cross_compiling" = "xyes" ; then
  enable_gtk_doc=no
fi
GTK_DOC_CHECK([1.0])


AS_GCC_INLINE_ASSEMBLY([HAVE_GCC_ASM=yes], [HAVE_GCC_ASM=no])
if test x$HAVE_GCC_ASM = xyes ; then
  AC_DEFINE(HAVE_GCC_ASM, 1,
    [Define if compiler supports gcc inline assembly])
fi
AM_CONDITIONAL(HAVE_GCC_ASM, test "x$HAVE_GCC_ASM" = "xyes")

AS_HOST_DEFINES()

AS_GCC_ASM_POWERPC_FPU([HAVE_GCC_ASM_POWERPC_FPU=yes],
  [HAVE_GCC_ASM_POWERPC_FPU=no])
if test x$HAVE_GCC_ASM_POWERPC_FPU = xyes ; then
  AC_DEFINE(HAVE_GCC_ASM_POWERPC_FPU, 1,
    [Define if compiler supports PowerPC FPU instructions])
fi
AM_CONDITIONAL(HAVE_GCC_ASM_POWERPC_FPU,
  test "x$HAVE_GCC_ASM_POWERPC" = "xyes")

AC_C_BIGENDIAN
AS_UNALIGNED_ACCESS

AC_LTDL_SYMBOL_USCORE
if test x${lt_cv_sys_symbol_underscore} = xyes ; then
  AC_DEFINE(HAVE_SYMBOL_UNDERSCORE, 1, [Define if C symbols have leading underscore])
fi

AX_CREATE_STDINT_H([liboil/liboil-stdint.h])

AC_FUNC_MMAP()
AC_CHECK_LIB(m, rint,
  AC_DEFINE(HAVE_RINT, 1, [Define if rint() is available]))
AC_CHECK_LIB(m, rintf,
  AC_DEFINE(HAVE_RINTF, 1, [Define if rintf() is available]))
AC_CHECK_LIB(m, lrint,
  AC_DEFINE(HAVE_LRINT, 1, [Define if lrint() is available]))
AC_CHECK_LIB(m, lrintf,
  AC_DEFINE(HAVE_LRINTF, 1, [Define if lrintf() is available]))
AC_CHECK_LIB(m, sincos,
  AC_DEFINE(HAVE_SINCOS, 1, [Define if sincos() is available]))

AC_CHECK_HEADER(ieee754.h,
  AC_DEFINE(HAVE_IEEE754_H, 1, [Define if ieee754.h exists]))

AC_CHECK_HEADERS([inttypes.h])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([unistd.h])

AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([sigaction])
AC_CHECK_FUNCS([sigsetjmp])

AC_CHECK_LIBM
AC_SUBST(LIBM)

AC_CHECK_LIB(rt, clock_gettime,
   AC_DEFINE(HAVE_CLOCK_GETTIME, 1, [Defined if we have clock_gettime()])
   LIBRT=-lrt
   )
AC_SUBST(LIBRT)

AC_CACHE_CHECK(for monotonic clocks,
    oil_cv_monotonic_clock,AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include <time.h>
#include <unistd.h>
int main() {
#if !(defined(_POSIX_MONOTONIC_CLOCK) && _POSIX_MONOTONIC_CLOCK >= 0 && defined(CLOCK_MONOTONIC))
        #error No monotonic clock
#endif
        return 0;
}
]])],oil_cv_monotonic_clock=yes,oil_cv_monotonic_clock=no))
if test "$oil_cv_monotonic_clock" = "yes"; then
  AC_DEFINE(HAVE_MONOTONIC_CLOCK,1,[Defined if we have a monotonic clock])
fi

AS_COMPILER_FLAG(-Wall, LIBOIL_CFLAGS="$LIBOIL_CFLAGS -Wall")
if test "x$LIBOIL_CVS" = "xyes"
then
  AS_COMPILER_FLAG(-Werror, LIBOIL_CFLAGS="$LIBOIL_CFLAGS -Werror")
fi
if test x$HAVE_GCC_ASM = xyes -a x$HAVE_POWERPC = xyes ; then
  AS_COMPILER_FLAG(["-Wa,-maltivec"],
    [LIBOIL_CFLAGS="$LIBOIL_CFLAGS -Wa,-maltivec"],
    true)
  AS_COMPILER_FLAG(["-Wa,-mregnames"],
    [LIBOIL_CFLAGS="$LIBOIL_CFLAGS -Wa,-mregnames"],
    true)
  AS_COMPILER_FLAG(["-force_cpusubtype_ALL"],
    [LIBOIL_CFLAGS="$LIBOIL_CFLAGS -force_cpusubtype_ALL"],
    true)
  AS_COMPILER_FLAG(["-fno-tree-vectorize"],
    [LIBOIL_CFLAGS="$LIBOIL_CFLAGS -fno-tree-vectorize"],
    true)
fi

AC_ARG_ENABLE(vfp,
  AC_HELP_STRING([--enable-vfp],[compile with Vector Floating-point unit support]),
  enable_vfp=$enableval,enable_vfp=yes)
if test "x$enable-vfp" = xyes -a x$HAVE_GCC_ASM = xyes -a x$HAVE_ARM = xyes; then
  AS_COMPILER_FLAG(["-Wa,-mfpu=vfp"],
    [VFP_CFLAGS="$VFP_CFLAGS -mfpu=vfp"],
    true)
  #AS_COMPILER_FLAG(["-Wa,-mfloat-abi=softfp"],
  #  [VFP_CFLAGS="$VFP_CFLAGS -mfloat-abi=softfp"],
  #  true)
fi
AC_SUBST(VFP_CFLAGS)

#AS_COMPILER_FLAG(["-fasm-blocks"], HAVE_ASM_BLOCKS=yes, HAVE_ASM_BLOCKS=no)
HAVE_ASM_BLOCKS=no
if test "x$HAVE_ASM_BLOCKS" = "xyes"
then
  LIBOIL_CFLAGS="$LIBOIL_CFLAGS -fasm-blocks"
fi
AM_CONDITIONAL(HAVE_ASM_BLOCKS, test "x$HAVE_ASM_BLOCKS" = "xyes")

#if test "x$HAVE_I386" = "xyes"
#then
#  # I'd write a test for this, but as of 4.1.2, gcc is still broken
#  AC_DEFINE(USE_SSE_WRAPPER, 1, [Defined if SSE functions need stack alignment wrappers])
#fi

AS_MMX_INTRINSICS(MMX_CFLAGS, HAVE_MMX_INTRINSICS=yes, HAVE_MMX_INTRINSICS=no)
AS_SSE_INTRINSICS(SSE_CFLAGS, HAVE_SSE_INTRINSICS=yes, HAVE_SSE_INTRINSICS=no)
AS_SSE2_INTRINSICS(SSE2_CFLAGS, HAVE_SSE2_INTRINSICS=yes, HAVE_SSE2_INTRINSICS=no)
AS_SSE3_INTRINSICS(SSE3_CFLAGS, HAVE_SSE3_INTRINSICS=yes, HAVE_SSE3_INTRINSICS=no)
AS_3DNOW_INTRINSICS(_3DNOW_CFLAGS, HAVE_3DNOW_INTRINSICS=yes, HAVE_3DNOW_INTRINSICS=no)
AS_3DNOWEXT_INTRINSICS(_3DNOWEXT_CFLAGS, HAVE_3DNOWEXT_INTRINSICS=yes, HAVE_3DNOWEXT_INTRINSICS=no)
AS_ALTIVEC_INTRINSICS(ALTIVEC_CFLAGS, HAVE_ALTIVEC_INTRINSICS=yes, HAVE_ALTIVEC_INTRINSICS=no)

AM_CONDITIONAL(HAVE_MMX_INTRINSICS, test "x$HAVE_MMX_INTRINSICS" = "xyes")
AM_CONDITIONAL(HAVE_SSE_INTRINSICS, test "x$HAVE_SSE_INTRINSICS" = "xyes")
AM_CONDITIONAL(HAVE_SSE2_INTRINSICS, test "x$HAVE_SSE2_INTRINSICS" = "xyes")
AM_CONDITIONAL(HAVE_SSE3_INTRINSICS, test "x$HAVE_SSE3_INTRINSICS" = "xyes")
AM_CONDITIONAL(HAVE_3DNOW_INTRINSICS, test "x$HAVE_3DNOW_INTRINSICS" = "xyes")
AM_CONDITIONAL(HAVE_3DNOWEXT_INTRINSICS, test "x$HAVE_3DNOWEXT_INTRINSICS" = "xyes")
AM_CONDITIONAL(HAVE_ALTIVEC_INTRINSICS, test "x$HAVE_ALTIVEC_INTRINSICS" = "xyes")

AC_SUBST(MMX_CFLAGS)
AC_SUBST(SSE_CFLAGS)
AC_SUBST(SSE2_CFLAGS)
AC_SUBST(SSE3_CFLAGS)
AC_SUBST(_3DNOW_CFLAGS)
AC_SUBST(_3DNOWEXT_CFLAGS)
AC_SUBST(ALTIVEC_CFLAGS)

dnl binutils-2.18 required for this, but is too new for FreeBSD/Solaris
AS_GCC_INLINE_ASM_INSTRUCTION([pmaddubsw %%mm0, %%mm0], [HAVE_SSSE3_ASM=yes], [HAVE_SSSE3_ASM=no])
if test "x$HAVE_SSSE3_ASM" = xyes ; then
  AC_DEFINE(HAVE_SSSE3_ASM, 1, [Defined if compiler/assembler can handle SSSE3 instructions])
fi

AC_DEFINE(OIL_EXPORTS, 1, [Defined for compiling internal code])

LIBOIL_CFLAGS="$LIBOIL_CFLAGS -D_BSD_SOURCE -D_GNU_SOURCE -I\$(top_srcdir) -DOIL_ENABLE_UNSTABLE_API"
AC_SUBST(LIBOIL_CFLAGS)

LIBOIL_LIBS="\$(top_builddir)/liboil/liboil-$LIBOIL_MAJORMINOR.la $LIBM $LIBRT"
AC_SUBST(LIBOIL_LIBS)

pkgconfigdir="\$(libdir)/pkgconfig"
AC_SUBST(pkgconfigdir)

AC_CONFIG_FILES([
Makefile 
doc/Makefile
liboil/Makefile
liboil/amd64/Makefile
liboil/3dnow/Makefile
liboil/c/Makefile
liboil/colorspace/Makefile
liboil/conv/Makefile
liboil/copy/Makefile
liboil/dct/Makefile
liboil/deprecated/Makefile
liboil/fb/Makefile
liboil/i386/Makefile
liboil/i386_amd64/Makefile
liboil/jpeg/Makefile
liboil/math/Makefile
liboil/md5/Makefile
liboil/mmx/Makefile
liboil/motovec/Makefile
liboil/powerpc/Makefile
liboil/powerpc_asm_blocks/Makefile
liboil/ref/Makefile
liboil/simdpack/Makefile
liboil/sse/Makefile
liboil/utf8/Makefile
liboil/arm/Makefile
testsuite/Makefile
testsuite/instruction/Makefile
examples/Makefile
examples/audioresample/Makefile
examples/huffman/Makefile
examples/jpeg/Makefile
examples/md5/Makefile
examples/taylor/Makefile
examples/uberopt/Makefile
examples/videoscale/Makefile
examples/work/Makefile
liboil-uninstalled.pc
liboil.pc
])
AC_OUTPUT



