#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e
	
	# check for command line switches
	usage="usage: build.sh -p <package> [-i thumb | arm] [-a all | armeabi | armeabi-v7a] [-l appPlatform]"
	package_name=
	arm_mode=arm
	arm_arch=armeabi-v7a
	app_platform=android-10
	
	while [ $# -gt 0 ];	do
	    case "$1" in
			-p)  package_name="$2"; shift;;
	        -i)  arm_mode="$2"; shift;;
	        -a)  arm_arch="$2"; shift;;
			-l)  app_platform="$2"; shift;;
			-*)
		    	echo >&2 \
		    		$usage
		    	exit 1;;
			*)  break;;	# terminate while loop
	    esac
	    shift
	done

	if [ x"$package_name" = x ]; then
		echo $usage
		exit 1
	fi

	if [ x"$arm_mode" != xarm ] && [ x"$arm_mode" != xthumb ]; then
		echo $usage
		exit 1		
	fi

	if [ x"$arm_arch" != xarmeabi ] && [ x"$arm_arch" != xarmeabi-v7a ] && [ x"$arm_arch" != xall ]; then
		echo $usage
		exit 1		
	elif [ x"$arm_arch" = xall ]; then
		arm_arch="armeabi armeabi-v7a"
	fi

	# echo message about what we are doing
	echo "Building libmoai.so for $package_name, $arm_mode, $arm_arch, $app_platform"

	# create package underscored value
	package_underscored=$( echo $package_name | sed 's/\./_/g' )

	# update moai.cpp with proper package name
	pushd jni/src > /dev/null
		cp -f moai.cpp packaged-moai.cpp
		sed -i.backup s%@PACKAGE_UNDERSCORED@%"$package_underscored"%g packaged-moai.cpp
		rm -f packaged-moai.cpp.backup
	popd > /dev/null

	# set app platform for library
	pushd jni > /dev/null
		cp -f AppPlatform.mk AppPlatformDefined.mk
		sed -i.backup s%@APP_PLATFORM@%"$app_platform"%g AppPlatformDefined.mk
		rm -f AppPlatformDefined.mk.backup
	popd > /dev/null
	
	# if selected, configure compiling for thumb
	pushd jni > /dev/null
		cp -f ArmMode.mk ArmModeDefined.mk
		sed -i.backup s%@ARM_MODE@%"$arm_mode"%g ArmModeDefined.mk
		sed -i.backup s%@ARM_ARCH@%"$arm_arch"%g ArmModeDefined.mk
		rm -f ArmModeDefined.mk.backup
	popd > /dev/null
	
	# build libcrypto
	pushd jni/crypto > /dev/null
		bash build.sh
	popd > /dev/null
	
	# build libmoai
	pushd jni > /dev/null
		ndk-build
	popd > /dev/null

	# remove ArmModeDefined.mk file
	rm -f jni/ArmModeDefined.mk
	rm -f jni/AppPlatformDefined.mk
		
	# remove packaged-moai.cpp so it doesn't accidentaly get edited
	rm -f jni/src/packaged-moai.cpp
	
	# create text file that shows the package libmoai.so was built with (this time)
	rm -f libs/armeabi/package.txt
	echo "$package_name" > libs/package.txt
	echo "$arm_mode" >> libs/package.txt
	echo "$arm_arch" >> libs/package.txt
	echo "$app_platform" >> libs/package.txt
