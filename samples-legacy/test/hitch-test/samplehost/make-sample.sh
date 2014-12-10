#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	# build libmoai
	pushd ../libmoai > /dev/null
	./build.sh -p com.ziplinegames.wolftoss -l android-10
	popd > /dev/null
	
	# make the android host
	pushd .. > /dev/null
	./make-host.sh -p com.ziplinegames.wolftoss -l android-10
	popd > /dev/null
	
	# copy settings
	cp settings*.sh ../untitled-host
	
	# copy icons
	cp icon*.png ../untitled-host/res
	
	# run host 
	pushd ../untitled-host > /dev/null
	./run-host.sh
	popd > /dev/null
	