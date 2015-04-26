dnl as-version.m4 0.1.0

dnl autostars m4 macro for versioning (modified)

dnl Thomas Vander Stichele <thomas at apestaart dot org>
dnl David Schleef <ds@schleef.org>

dnl $Id: as-nano.m4,v 1.2 2007-03-16 23:30:02 ds Exp $

dnl AS_VERSION(ACTION-IF-NO-NANO, [ACTION-IF-NANO])

AC_DEFUN([AS_NANO],
[
  AC_MSG_CHECKING(nano version)

  NANO=$(echo AC_PACKAGE_VERSION | sed ['s/[0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*\.*//'])
  AC_SUBST(NANO)

  if test x"$NANO" = x || test "x$NANO" = "x0" ; then
    AC_MSG_RESULT([0 (release)])
    NANO=0
    ifelse([$1], , :, [$1])
  else
    AC_MSG_RESULT($NANO)
    ifelse([$2], , :, [$2])
  fi
])
