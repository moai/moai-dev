
if USE_ALT_OPT
opt_libs = libmd5_opt.la
else
opt_libs =
endif

noinst_LTLIBRARIES = libmd5.la $(opt_libs)

noinst_HEADERS = md5.h

c_sources = md5.c

libmd5_la_SOURCES = \
	$(c_sources) $(i386_sources) $(powerpc_sources) $(amd64_sources)
libmd5_la_LIBADD = $(opt_libs)
libmd5_la_CFLAGS = $(LIBOIL_CFLAGS)

libmd5_opt_la_SOURCES = $(c_sources)
libmd5_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)

