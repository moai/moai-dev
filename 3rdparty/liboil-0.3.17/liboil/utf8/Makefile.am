
if USE_ALT_OPT
opt_libs = libutf8_opt.la
else
opt_libs =
endif

noinst_LTLIBRARIES = libutf8.la $(opt_libs)

noinst_HEADERS = utf8.h


libutf8_la_SOURCES = \
	utf8.c utf8_fast.c
libutf8_la_LIBADD = $(opt_libs)
libutf8_la_CFLAGS = $(LIBOIL_CFLAGS)

libutf8_opt_la_SOURCES = $(libutf8_la_SOURCES)
libutf8_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)

