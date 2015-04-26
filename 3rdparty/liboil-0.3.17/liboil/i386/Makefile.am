
noinst_LTLIBRARIES = libi386.la

libi386_la_SOURCES = \
	argb_paint_i386.c \
	ayuv2argb_i386.c \
	composite_i386.c \
	copy8x8_i386.c \
	diff8x8_i386.c \
	error8x8_i386.c \
	md5_i386.c \
	mult8x8_i386.c \
	recon8x8_i386.c \
	rowcolsad8x8_i386.c \
	sad8x8avg_i386.c \
	splat_i386.c \
	trans8x8_i386.c \
	wavelet.c

if USE_NEW_ABI
else
libi386_la_SOURCES += \
	abs_i386.c \
	conv_sse.c \
	conv_3dnow.c
endif

libi386_la_CFLAGS = $(LIBOIL_CFLAGS)

