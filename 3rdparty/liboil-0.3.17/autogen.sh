#!/bin/sh

if [ $(whoami) = ds ] ; then
  confflags="--enable-gtk-doc"
else
  confflags="--enable-gtk-doc"
fi

autoreconf -i -f &&
  ./configure --enable-maintainer-mode --disable-static $confflags $@


