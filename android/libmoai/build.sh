#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e
	
	# check for package name provided on command line
	packageName=$1
	
	if [ "$packageName" = "" ]; then
		echo "No package name provided. Defaulting to \"com.getmoai.samples\""
		echo "To specify a different package name, pass it on the command line."
		packageName="com.getmoai.samples"
	fi

	# echo message about what we are doing
	echo "Building libmoai.so for package $packageName"

	# create package underscored value
	package_underscored=$( echo $packageName | sed 's/\./_/g' )

	# update moai.cpp with proper package name
	pushd jni/src > /dev/null
		cp -f moai.cpp packaged-moai.cpp
		sed -i.backup s%@PACKAGE_UNDERSCORED@%"$package_underscored"%g packaged-moai.cpp
		rm -f packaged-moai.cpp.backup
	popd > /dev/null
	
	# build libcrypto
	pushd jni/crypto > /dev/null
		bash build.sh
	popd > /dev/null
	
	# build libmoai
	pushd jni > /dev/null
		ndk-build
	popd > /dev/null
	
	# remove packaged-moai.cpp so it doesn't accidentaly get edited
	rm -f jni/src/packaged-moai.cpp
