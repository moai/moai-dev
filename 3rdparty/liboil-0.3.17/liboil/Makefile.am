
pkgincludedir = $(includedir)/liboil-@LIBOIL_MAJORMINOR@/liboil

DIST_SUBDIRS = amd64 3dnow c colorspace conv copy dct fb i386 i386_amd64 jpeg math md5 mmx motovec powerpc powerpc_asm_blocks ref simdpack sse utf8 deprecated arm
SUBDIRS = c colorspace conv copy dct jpeg math md5 ref simdpack utf8 deprecated

EXTRA_DIST = README

libs =

if HAVE_I386
if HAVE_GCC_ASM
SUBDIRS += i386 i386_amd64
libs += i386/libi386.la i386_amd64/libi386_amd64.la
endif
endif

if HAVE_MMX_INTRINSICS
SUBDIRS += mmx
libs += mmx/libmmx.la
endif
if HAVE_SSE2_INTRINSICS
SUBDIRS += fb sse
libs += fb/libfb.la sse/libsse.la
endif
if HAVE_3DNOW_INTRINSICS
#subdir_i386 += 3dnow
#libs_i386 += 3dnow/lib3dnow.la
endif

if HAVE_AMD64
if HAVE_GCC_ASM
SUBDIRS += amd64 i386_amd64
libs += amd64/libamd64.la i386_amd64/libi386_amd64.la
endif
endif

if HAVE_POWERPC
if HAVE_GCC_ASM
SUBDIRS += powerpc motovec
libs += powerpc/libpowerpc.la motovec/libmotovec.la

# motovec doesn't compile on snow leopard, and isn't useful elsewhere
#SUBDIRS += motovec
#libs += motovec/libmotovec.la
if HAVE_ASM_BLOCKS
SUBDIRS += powerpc_asm_blocks
libs += powerpc/libpowerpc_asm_blocks.la
endif
endif
endif

if HAVE_ARM
if HAVE_GCC_ASM
SUBDIRS += arm
libs += arm/libarm.la 
endif
endif


lib_LTLIBRARIES = liboil-@LIBOIL_MAJORMINOR@.la
if USE_NEW_ABI
lib_LTLIBRARIES += liboil-@LIBOIL_OLD_MAJORMINOR@.la
endif

buildnoinstdir = $(libdir)
buildnoinst_LTLIBRARIES = liboiltmp1.la

noinst_LTLIBRARIES = liboilfunctions.la

noinst_PROGRAMS = build_prototypes build_prototypes_doc build_marshal \
	build_class_decls build_trampolines build_prototypes_04

pkginclude_HEADERS = liboil.h liboilfunction.h liboildebug.h liboilfuncs.h \
	liboiltypes.h liboilcpu.h liboilprototype.h liboilparameter.h \
	liboilrandom.h liboilgcc.h liboiltest.h liboilprofile.h \
	liboilinternal.h liboilclasses.h liboilfault.h liboilutils.h

nodist_pkginclude_HEADERS = liboil-stdint.h

if ENABLE_GTK_DOC
nodist_noinst_HEADERS = liboilfuncs-doc.h
else
nodist_noinst_HEADERS = 
endif

CLEANFILES = liboilarray.c liboilfuncs-doc.h
DISTCLEANFILES = liboil-stdint.h

liboilfunctions_la_SOURCES = \
	null.c
liboilfunctions_la_CFLAGS = $(LIBOIL_CFLAGS)
liboilfunctions_la_LIBADD = \
	c/lib_c.la \
	colorspace/libcolorspace.la \
	copy/libcopy.la \
	dct/libdct.la \
	$(libs) \
	jpeg/libjpeg.la \
	math/libmath.la \
	md5/libmd5.la \
	ref/libref.la \
	simdpack/libsimdpack.la \
	utf8/libutf8.la
if USE_NEW_ABI
else
liboilfunctions_la_LIBADD += \
	conv/libconv.la \
	deprecated/libdeprecated.la
endif
liboilfunctions_la_LDFLAGS = \
	-no-undefined

liboiltmp1_la_SOURCES = \
	liboiltmp.c
liboiltmp1_la_LDFLAGS = \
	-no-undefined \
	-export-symbols-regex 'oil_'
liboiltmp1_la_LIBADD = \
	liboilfunctions.la \
	$(LIBM) $(LIBRT)

liboil_@LIBOIL_MAJORMINOR@_la_SOURCES = \
	liboil.h \
	liboilcolorspace.h \
	liboilcpu.c \
	liboildebug.c \
	liboildebug.h \
	liboilfault.c \
	liboilfault.h \
	liboilfuncs.h \
	liboilfunction.c \
	liboilfunction.h \
	liboilmarshal.c \
	liboilprofile.c \
	liboilprofile.h \
	liboilprototype.c \
	liboilrandom.c \
	liboiltest.c \
	liboiltest.h \
	liboiltypes.h \
	liboilutils.c \
	liboilutils.h

if HAVE_I386
liboil_@LIBOIL_MAJORMINOR@_la_SOURCES += liboilcpu-x86.c
else
if HAVE_AMD64
liboil_@LIBOIL_MAJORMINOR@_la_SOURCES += liboilcpu-x86.c
else
if HAVE_POWERPC
liboil_@LIBOIL_MAJORMINOR@_la_SOURCES += liboilcpu-powerpc.c
else
if HAVE_ARM
liboil_@LIBOIL_MAJORMINOR@_la_SOURCES += liboilcpu-arm.c
else
liboil_@LIBOIL_MAJORMINOR@_la_SOURCES += liboilcpu-misc.c
endif
endif
endif
endif

nodist_liboil_@LIBOIL_MAJORMINOR@_la_SOURCES = \
	liboilarray.c

if USE_NEW_ABI
liboil_@LIBOIL_MAJORMINOR@_la_SOURCES += \
	liboiltrampolines.c
endif

liboil_@LIBOIL_MAJORMINOR@_la_LIBADD = \
	liboilfunctions.la \
	$(LIBM) \
	$(LIBRT)
liboil_@LIBOIL_MAJORMINOR@_la_CFLAGS = $(LIBOIL_CFLAGS)
liboil_@LIBOIL_MAJORMINOR@_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LIBOIL_LIBVERSION) \
	-export-symbols-regex '^oil_'

# This is required to use 'make -j2'. Automake doesn't seem to notice
# that one of the dependencies is in this directory.
build_prototypes_DEPENDENCIES = liboil-$(LIBOIL_MAJORMINOR).la
build_prototypes_SOURCES = build_prototypes.c
build_prototypes_CFLAGS = $(LIBOIL_CFLAGS)
build_prototypes_LDADD = $(LIBOIL_LIBS)

build_prototypes_04_DEPENDENCIES = liboil-$(LIBOIL_MAJORMINOR).la
build_prototypes_04_SOURCES = build_prototypes_04.c
build_prototypes_04_CFLAGS = $(LIBOIL_CFLAGS)
build_prototypes_04_LDADD = $(LIBOIL_LIBS)

build_class_decls_DEPENDENCIES = liboil-$(LIBOIL_MAJORMINOR).la
build_class_decls_SOURCES = build_class_decls.c
build_class_decls_CFLAGS = $(LIBOIL_CFLAGS)
build_class_decls_LDADD = $(LIBOIL_LIBS)

build_prototypes_doc_DEPENDENCIES = liboil-$(LIBOIL_MAJORMINOR).la
build_prototypes_doc_SOURCES = build_prototypes_doc.c
build_prototypes_doc_CFLAGS = $(LIBOIL_CFLAGS)
build_prototypes_doc_LDADD = $(LIBOIL_LIBS)

build_marshal_DEPENDENCIES = liboil-$(LIBOIL_MAJORMINOR).la
build_marshal_SOURCES = build_marshal.c
build_marshal_CFLAGS = $(LIBOIL_CFLAGS)
build_marshal_LDADD = $(LIBOIL_LIBS)

build_trampolines_DEPENDENCIES = liboil-$(LIBOIL_MAJORMINOR).la
build_trampolines_SOURCES = build_trampolines.c
build_trampolines_CFLAGS = $(LIBOIL_CFLAGS)
build_trampolines_LDADD = $(LIBOIL_LIBS)

liboilarray.c: liboiltmp1.la Makefile
	echo '/* This file is autogenerated.  Do not edit */' >liboilarray.c.tmp
	echo >>liboilarray.c.tmp
	echo '#include <liboil/liboilfunction.h>' >>liboilarray.c.tmp
	echo >>liboilarray.c.tmp
	grep '^_oil_function_class_' .libs/liboiltmp1.exp | \
	  sed -e 's/ DATA$$//' -e 's/ .*//' -e 's/.*/extern OilFunctionClass &;/' >>liboilarray.c.tmp
	echo >>liboilarray.c.tmp
	echo 'OilFunctionClass *_oil_function_class_array[] = {' >>liboilarray.c.tmp
	grep '^_oil_function_class_' .libs/liboiltmp1.exp | \
	  sed -e 's/ .*//' -e 's/.*/  \&&,/' >>liboilarray.c.tmp
	echo '  NULL' >>liboilarray.c.tmp
	echo '};' >>liboilarray.c.tmp
	echo >>liboilarray.c.tmp
	grep '^_oil_function_impl_' .libs/liboiltmp1.exp | \
	  sed -e 's/ .*//' -e 's/.*/extern OilFunctionImpl &;/' >>liboilarray.c.tmp
	echo >>liboilarray.c.tmp
	echo 'OilFunctionImpl *_oil_function_impl_array[] = {' >>liboilarray.c.tmp
	grep '^_oil_function_impl_' .libs/liboiltmp1.exp | \
	  sed -e 's/ .*//' -e 's/.*/  \&&,/' >>liboilarray.c.tmp
	echo '  NULL' >>liboilarray.c.tmp
	echo '};' >>liboilarray.c.tmp
	echo >>liboilarray.c.tmp
	cmp liboilarray.c.tmp liboilarray.c || mv liboilarray.c.tmp liboilarray.c

liboilfuncs-doc.h: build_prototypes_doc$(EXEEXT)
	./build_prototypes_doc$(EXEEXT) >liboilfuncs-doc.h

update: build_prototypes$(EXEEXT) build_marshal$(EXEEXT) build_prototypes_doc$(EXEEXT) build_class_decls$(EXEEXT) build_trampolines$(EXTEXT) build_prototypes_04$(EXTEXT)
	./build_prototypes$(EXEEXT) >liboilfuncs.h
	./build_marshal$(EXEEXT) >liboilmarshal.c
	./build_prototypes_doc$(EXEEXT) >liboilfuncs-doc.h
	./build_class_decls$(EXEEXT) >liboilclasses.h
	./build_trampolines$(EXTEXT) >liboiltrampolines.c
	./build_prototypes_04$(EXTEXT) >liboilfuncs-04.h

install-buildnoinstLTLIBRARIES:

if USE_NEW_ABI
liboil_@LIBOIL_OLD_MAJORMINOR@_la_SOURCES =
liboil_@LIBOIL_OLD_MAJORMINOR@_la_LIBADD = \
	deprecated/libdeprecated.la \
	liboil-@LIBOIL_MAJORMINOR@.la
liboil_@LIBOIL_OLD_MAJORMINOR@_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LIBOIL_LIBVERSION) \
	-export-symbols-regex '^oil_'
endif

