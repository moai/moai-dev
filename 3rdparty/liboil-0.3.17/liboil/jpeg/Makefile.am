
if USE_ALT_OPT
opt_libs = libjpeg_opt.la
else
opt_libs =
endif

noinst_LTLIBRARIES = libjpeg.la $(opt_libs)

noinst_HEADERS = jpeg.h

c_sources = \
	convert8x8_c.c \
	jpeg_rgb_decoder.c \
	quantize8x8_c.c \
	yuv2rgb_c.c \
	zigzag8x8_c.c

libjpeg_la_SOURCES = \
	$(c_sources) \
	$(powerpc_sources)
libjpeg_la_LIBADD = $(opt_libs)
libjpeg_la_CFLAGS = $(LIBOIL_CFLAGS)

libjpeg_opt_la_SOURCES = $(c_sources)
libjpeg_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)

