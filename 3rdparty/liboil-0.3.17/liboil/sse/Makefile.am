
noinst_LTLIBRARIES = libsse.la

libsse_la_SOURCES = \
	clamp_sse.c \
	composite_sse.c \
	composite_sse_2pix.c \
	composite_sse_4pix.c \
	copy_sse.c \
	math_sse.c \
	math_sse_unroll2.c \
	multsum_sse.c \
	sad8x8_sse.c \
	splat_sse.c \
	sse_wrapper.h

libsse_la_CFLAGS = $(SSE_CFLAGS) $(SSE2_CFLAGS) $(LIBOIL_CFLAGS)

