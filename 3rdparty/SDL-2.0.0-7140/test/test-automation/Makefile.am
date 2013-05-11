ACLOCAL_AMFLAGS = -I acinclude -I build-scripts

SUBDIRS = src/libSDLtest \
		  src/runner \
		  tests/testdummy \
		  tests/testrect \
		  tests/testplatform \
		  tests/testaudio \
		  tests/testsurface \
		  tests/testrwops \
		  tests/testrender \
		  tests/testvideo \
		  tests/testsyswm \
		  tests/testclipboard \
		  tests/testevents \
	  	  tests/testkeyboard 
		
install: install-runner install-tests
install-runner: 
	$(SHELL) build-scripts/install-runner.sh
install-tests:
	$(SHELL) build-scripts/install-tests.sh

distclean-local:
	$(SHELL) build-scripts/distclean.sh

