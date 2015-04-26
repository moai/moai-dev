
if USE_ALT_OPT
opt_libs = libcopy_opt.la
else
opt_libs = 
endif

noinst_LTLIBRARIES = libcopy.la $(opt_libs)

c_sources = \
	copy.c \
	copy8x8.c \
	splat_ref.c \
	trans8x8_c.c

libcopy_la_SOURCES = \
	$(c_sources) \
	$(powerpc_sources) \
	$(i386_sources) \
	$(amd64_sources)
libcopy_la_LIBADD = $(opt_libs)

libcopy_la_CFLAGS = $(LIBOIL_CFLAGS)

libcopy_opt_la_SOURCES = $(c_sources)
libcopy_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)


