
if USE_ALT_OPT
opt_libs = libcolorspace_opt1.la
else
opt_libs = 
endif
noinst_LTLIBRARIES = libcolorspace.la $(opt_libs)

c_sources = \
	composite.c

libcolorspace_la_SOURCES = 	\
	$(c_sources) \
	$(powerpc_sources) \
	$(i386_sources) \
	$(amd64_sources)
libcolorspace_la_LIBADD = $(opt_libs)
libcolorspace_la_CFLAGS = $(LIBOIL_CFLAGS)

libcolorspace_opt1_la_SOURCES = $(c_sources)
libcolorspace_opt1_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)

