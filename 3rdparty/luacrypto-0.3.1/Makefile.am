ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src \
	  doc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = luacrypto.pc

test: all
	$(MAKE) -C tests

clean-local:
	$(MAKE) -C tests clean
	
