
if USE_ALT_OPT
opt_libs = lib_c_opt1.la
else
opt_libs =
endif

EXTRA_DIST = generate_clamp.pl

noinst_LTLIBRARIES = lib_c.la $(opt_libs)

c_sources = \
	ag_clamp.c \
	composite.c \
	copy.c \
	swab.c \
	wavelet.c

lib_c_la_SOURCES = \
        $(c_sources)
lib_c_la_LIBADD = \
        $(opt_libs)
lib_c_la_CFLAGS = $(LIBOIL_CFLAGS)

lib_c_opt1_la_SOURCES = $(c_sources)
lib_c_opt1_la_CFLAGS = $(LIBOIL_CFLAGS) \
        $(LIBOIL_OPT_CFLAGS)


ag_clamp.c: generate_clamp.pl
	$(srcdir)/generate_clamp.pl >ag_clamp.c

