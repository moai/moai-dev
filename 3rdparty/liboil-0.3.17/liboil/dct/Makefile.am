
if USE_ALT_OPT
opt_libs = libdct_opt.la
else
opt_libs = 
endif

noinst_LTLIBRARIES = libdct.la $(opt_libs)

noinst_HEADERS = \
	dct.h

c_sources = \
	dct12_f32.c \
	dct36_f32.c \
	fdct8_f64.c \
	fdct8x8_f64.c \
	fdct8x8s_s16.c \
	fdct8x8theora.c \
	idct8_f64.c \
	idct8x8_c.c \
	imdct32_f32.c \
	idct8x8theora_ref.c

libdct_la_SOURCES = \
	$(c_sources) \
	$(i386_sources) \
	$(amd64_sources)

libdct_la_CFLAGS = $(LIBOIL_CFLAGS)
libdct_la_LIBADD = $(opt_libs)

libdct_opt_la_SOURCES = $(c_sources)
libdct_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)

