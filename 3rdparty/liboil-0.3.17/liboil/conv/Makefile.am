
if USE_ALT_OPT
opt_libs = libconv_opt.la
else
opt_libs =
endif
noinst_LTLIBRARIES = libconv.la $(opt_libs)

c_sources = \
	conv_c.c \
	conv_bitstuff.c \
	conv_misc.c

libconv_opt_la_SOURCES = $(c_sources)
libconv_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)

noinst_HEADERS = conv.h

libconv_la_SOURCES = \
	$(c_sources)
libconv_la_CFLAGS = $(LIBOIL_CFLAGS)
libconv_la_LIBADD = $(opt_libs)

