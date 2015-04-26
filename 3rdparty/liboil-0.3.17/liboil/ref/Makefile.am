
if USE_ALT_OPT
opt_libs = libref_opt1.la
else
opt_libs =
endif

noinst_LTLIBRARIES = libref.la $(opt_libs)

c_sources = \
	addc.c \
	argb_paint.c \
	ayuv2argb.c \
	clamp.c \
	composite.c \
	convert.c \
	copy.c \
	copy8x8.c \
	diff8x8.c \
	diffsquaresum_f64.c \
	diffsquaresum_f32.c \
	error8x8.c \
	mas.c \
	math.c \
	mix_u8.c \
	mt19937ar.c \
	mult8x8_s16.c \
	multsum.c \
	recon8x8.c \
	resample.c \
	rgb.c \
	rowcolsad8x8.c \
	sad8x8.c \
	sad8x8_broken.c \
	sad8x8avg.c \
	sincos_f64.c \
	splat.c \
	squaresum_f32.c \
	squaresum_f64.c \
	sum.c \
	sum_f64.c \
	swab.c \
	trans8x8.c \
	yuv.c \
	wavelet.c

libref_la_SOURCES = \
	$(c_sources)
libref_la_LIBADD = \
	$(opt_libs)
libref_la_CFLAGS = $(LIBOIL_CFLAGS)

libref_opt1_la_SOURCES = $(c_sources)
libref_opt1_la_CFLAGS = $(LIBOIL_CFLAGS) \
	$(LIBOIL_OPT_CFLAGS)

