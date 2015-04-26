
EXTRA_DIST = example.c


if HAVE_GLIB
noinst_PROGRAMS = uberopt
else
noinst_PROGRAMS = 
endif

uberopt_SOURCES = uberopt.c
uberopt_CFLAGS = $(LIBOIL_CFLAGS) $(GLIB_CFLAGS)
uberopt_LDADD = $(LIBOIL_LIBS) $(GLIB_LIBS)

