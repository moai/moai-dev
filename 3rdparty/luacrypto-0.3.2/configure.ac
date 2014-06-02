AC_PREREQ([2.68])
AC_INIT([luacrypto], [0.3.1], [https://github.com/mkottman/luacrypto/issues])
AC_CONFIG_SRCDIR([src/lcrypto.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall])
LT_INIT([disable-static])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PATH_PROG([PKGCONFIG], [pkg-config])
AS_IF([test "x$PKGCONFIG" = "xno"],
      [AC_MSG_ERROR([pkg-config not found!])])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stddef.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

# pkgconfig
PKG_CHECK_MODULES([OPENSSL], [openssl])
PKG_CHECK_MODULES([LUA], [lua])

# lua libdir
LUALIBDIR="`$PKGCONFIG --variable=libdir lua`"

# dest of headers
CRYPTOINC="${includedir}/${PACKAGE_NAME}"

# substitute variables
AC_SUBST([AM_CPPFLAGS], ["$OPENSSL_CFLAGS $LUA_CFLAGS"])
AC_SUBST([OPENSSL_LIBS])
AC_SUBST([LUA_LIBS])
AC_SUBST([LUALIBDIR])
AC_SUBST([CRYPTOINC])

AC_CONFIG_FILES([Makefile src/Makefile doc/Makefile tests/Makefile luacrypto.pc])
AC_OUTPUT

echo
echo luacrypto.................................... : Version $VERSION
echo Prefix....................................... : $prefix
echo Lua libdir \(installdest\)..................... : $LUALIBDIR
echo htmldir...................................... : $htmldir
echo docdir....................................... : $docdir
echo

