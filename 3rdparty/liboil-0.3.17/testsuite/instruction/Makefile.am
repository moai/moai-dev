
EXTRA_DIST = check-instructions.pl

check_PROGRAMS = list-impls

AM_LDFLAGS = $(LIBOIL_LIBS)
AM_CFLAGS = $(LIBOIL_CFLAGS)
	

report: list-impls $(srcdir)/check-instructions.pl $(top_builddir)/liboil/.libs/liboiltmp1.so
	$(srcdir)/check-instructions.pl $(top_builddir)/liboil/.libs/liboiltmp1.so >report


CLEANFILES = report

