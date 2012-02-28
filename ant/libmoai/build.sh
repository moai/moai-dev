#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e
	
	# check for command line switches
	usage="usage: $0 -p <package> [-v] [-i thumb | arm] [-a all | armeabi | armeabi-v7a] [-l appPlatform] [--disable-tapjoy]"
	verbose=
	package_name=
	arm_mode=arm
	arm_arch=armeabi-v7a
	app_platform=android-10
	tapjoy_flags=
	
	while [ $# -gt 0 ];	do
	    case "$1" in
			-v)  verbose="V=1";;
			-p)  package_name="$2"; shift;;
	        -i)  arm_mode="$2"; shift;;
	        -a)  arm_arch="$2"; shift;;
			-l)  app_platform="$2"; shift;;
			--disable-tapjoy)  tapjoy_flags="-DDISABLE_TAPJOY";;
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

	# if libmoai already exists, find out which package it was build for
	if [ -f libs/package.txt ]; then
		existing_package_name=$( sed -n '1p' libs/package.txt )
		existing_arm_mode=$( sed -n '2p' libs/package.txt )
		existing_arm_arch=$( sed -n '3p' libs/package.txt )
		existing_app_platform=$( sed -n '4p' libs/package.txt )
		existing_tapjoy_flags=$( sed -n '5p' libs/package.txt )
	fi

	should_clean=false

	if [ x"$existing_package_name" != x"$package_name" ]; then
		should_clean=false
	fi

	if [ x"$existing_arm_mode" != x"$arm_mode" ]; then
		should_clean=true
	fi

	if [ x"$existing_arm_arch" != x"$arm_arch" ]; then
		should_clean=true
	fi

	if [ x"$existing_app_platform" != x"$app_platform" ]; then
		should_clean=true
	fi

	if [ x"$existing_tapjoy_flags" != x"$tapjoy_flags" ]; then
		should_clean=true
	fi
	
	if [ x"$should_clean" = xtrue ]; then
		./clean.sh
	fi

	# echo message about what we are doing
	echo "Building libmoai.so for $package_name, $arm_mode, $arm_arch, $app_platform"

	if [ x"$tapjoy_flags" != x ]; then
		echo "Tapjoy will be disabled"
	fi 

	# create package underscored value
	package_underscored=$( echo $package_name | sed 's/\./_/g' )

	# update moai.cpp with proper package name
	pushd jni/src > /dev/null
		cp -f moai.cpp packaged-moai.cpp
		sed -i.backup s%@PACKAGE_UNDERSCORED@%"$package_underscored"%g packaged-moai.cpp
		rm -f packaged-moai.cpp.backup
	popd > /dev/null

	pushd jni > /dev/null
		cp -f AppPlatform.mk AppPlatformDefined.mk
		sed -i.backup s%@APP_PLATFORM@%"$app_platform"%g AppPlatformDefined.mk
		rm -f AppPlatformDefined.mk.backup
	popd > /dev/null
	
	pushd jni > /dev/null
		cp -f ArmMode.mk ArmModeDefined.mk
		sed -i.backup s%@ARM_MODE@%"$arm_mode"%g ArmModeDefined.mk
		sed -i.backup s%@ARM_ARCH@%"$arm_arch"%g ArmModeDefined.mk
		rm -f ArmModeDefined.mk.backup
	popd > /dev/null

	pushd jni > /dev/null
		cp -f OptionalComponents.mk OptionalComponentsDefined.mk
		sed -i.backup s%@DISABLE_TAPJOY@%"$tapjoy_flags"%g OptionalComponentsDefined.mk
		rm -f OptionalComponentsDefined.mk.backup
	popd > /dev/null
	
	# build libcrypto
	pushd jni/crypto > /dev/null
		bash build.sh
	popd > /dev/null
	
	# build libmoai
	pushd jni > /dev/null
		ndk-build $verbose
	popd > /dev/null

	# remove temp files
	rm -f jni/ArmModeDefined.mk
	rm -f jni/AppPlatformDefined.mk
	rm -f jni/OptionalComponentsDefined.mk
		
	# remove packaged-moai.cpp
	rm -f jni/src/packaged-moai.cpp
	
	# create text file that shows the settings libmoai.so was built with (this time)
	rm -f libs/armeabi/package.txt
	echo "$package_name" > libs/package.txt
	echo "$arm_mode" >> libs/package.txt
	echo "$arm_arch" >> libs/package.txt
	echo "$app_platform" >> libs/package.txt
	echo "$tapjoy_flags" >> libs/package.txt
