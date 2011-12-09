#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	# build Moai
	pushd libmoai > /dev/null
	bash build.sh -p com.ziplinegames.wolftoss -l android-10
	popd > /dev/null
	
	# remove old host
	#rmdir wolftoss-host
	
	# make the host
	bash make-host.sh -p com.ziplinegames.wolftoss -l android-10
	
	# rename host folder
	#mv untitled-host wolftoss-host
	
	# first run of host
	pushd untitled-host > /dev/null
	bash run-host.sh
	popd > /dev/null
	
	# copy settings
	cp settings/*.sh untitled-host
	
	# copy icons
	cp settings/*.png untitled-host/res
	
	# run host again
	pushd untitled-host > /dev/null
	bash run-host.sh
	popd > /dev/null
	