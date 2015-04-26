
EXTRA_DIST = generate_math.pl

if USE_ALT_OPT
opt_libs = libmath_opt1.la
else
opt_libs = 
endif
noinst_LTLIBRARIES = libmath.la $(opt_libs)

libmath_la_SOURCES = math.c ag_math.c
libmath_la_LIBADD = $(opt_libs)
libmath_la_CFLAGS = $(LIBOIL_CFLAGS)

libmath_opt1_la_SOURCES = $(c_sources)
libmath_opt1_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)


ag_math.c: generate_math.pl
	$(srcdir)/generate_math.pl >ag_math.c

