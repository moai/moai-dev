
noinst_PROGRAMS = test_functable1

noinst_LTLIBRARIES = libaudioresample.la
#noinst_PROGRAMS = audioresample_test

libaudioresample_la_SOURCES = functable.c resample.c
libaudioresample_la_CFLAGS = $(LIBOIL_CFLAGS) $(GLIB_CFLAGS)

noinst_HEADERS = functable.h resample.h


#audioresample_test_SOURCES = test.c
#audioresample_test_CFLAGS = $(LIBOIL_CFLAGS)
#audioresample_test_LDADD = libaudioresample.la $(LIBOIL_LIBS)

test_functable1_SOURCES = test_functable1.c
test_functable1_CFLAGS = $(LIBOIL_CFLAGS)
test_functable1_LDADD = libaudioresample.la $(LIBOIL_LIBS)

