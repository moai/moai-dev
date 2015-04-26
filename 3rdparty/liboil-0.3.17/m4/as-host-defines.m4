dnl as-host-defines.m4 0.1.0

dnl autostars m4 macro for generating defines for various host architectures

dnl David Schleef <ds@schleef.org>

dnl $Id: as-host-defines.m4,v 1.3 2007-08-22 20:55:24 uid2631 Exp $

dnl AS_HOST_DEFINES()

AC_DEFUN([AS_HOST_DEFINES],
[
  case "x${host_cpu}" in
    xi?86 | k?)
      HAVE_I386=yes
      AC_DEFINE(HAVE_I386, 1, [Defined if host is i386])
      ;;
    xx86_64|xamd64)
      HAVE_AMD64=yes
      AC_DEFINE(HAVE_AMD64, 1, [Defined if host is amd64])
      ;;
    xpowerpc|xppc|xpowerpc64|xppc64)
      HAVE_POWERPC=yes
      AC_DEFINE(HAVE_POWERPC, 1, [Defined if host is powerpc])
      ;;
    xarm)
      HAVE_ARM=yes
      AC_DEFINE(HAVE_ARM, 1, [Defined if host is arm])
      ;;
  esac

AM_CONDITIONAL(HAVE_I386, test "x$HAVE_I386" = "xyes")
AM_CONDITIONAL(HAVE_AMD64, test "x$HAVE_AMD64" = "xyes")
AM_CONDITIONAL(HAVE_POWERPC, test "x$HAVE_POWERPC" = "xyes")
AM_CONDITIONAL(HAVE_ARM, test "x$HAVE_ARM" = "xyes")

  case "${host_os}" in
    mingw*)
      HAVE_OS_WIN32=yes
      AC_DEFINE(HAVE_OS_WIN32, 1, [Defined if host OS is MS Windows])
      ;;
    linux*)
      HAVE_OS_LINUX=yes
      AC_DEFINE(HAVE_OS_LINUX, 1, [Defined if host OS is linux])
      ;;
  esac

AM_CONDITIONAL(HAVE_OS_WIN32, test "$HAVE_WIN32" = "yes")
AM_CONDITIONAL(HAVE_OS_LINUX, test "$HAVE_LINUX" = "yes")

])

