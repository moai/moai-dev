#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e

	if [ $OSTYPE == cygwin ]; then

		pushd windows > /dev/null

		# build libcrypto-a, libcrypto-b, and libcrypto-c
		pushd jni > /dev/null
			ndk-build
		popd > /dev/null

		# combine libs into final libcrypto
		pushd obj/local/armeabi > /dev/null
			ar -x libcrypto-a.a
			ar -x libcrypto-b.a
			ar -x libcrypto-c.a
			ar -x libcrypto-d.a
			ar rcs libcrypto.a *.o
			ranlib libcrypto.a
		popd > /dev/null
	else
		
		pushd osx > /dev/null
		
		# build libcrypto
		pushd jni > /dev/null
			ndk-build
		popd > /dev/null
	fi

	# copy libcrypto to output directory
	OUTDIR=../libs/armeabi
	if [ ! -d $OUTDIR ]; then mkdir -p $OUTDIR; fi
	mv -f obj/local/armeabi/libcrypto.a $OUTDIR/libcrypto.a

	popd > /dev/null
