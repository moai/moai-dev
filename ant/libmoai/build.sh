#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e
	
	# check for command line switches
	usage="usage: $0 [-v] [-i thumb | arm] [-a all | armeabi | armeabi-v7a] [-l appPlatform] [--disable-tapjoy] [--disable-google-push] [--disable-google-billing] [--disable-crittercism] [--disable-adcolony]"
	verbose=
	arm_mode=arm
	arm_arch=armeabi-v7a
	app_platform=android-10
	tapjoy_flags=
	google_push_flags=
	google_billing_flags=
	crittercism_flags=
	adcolony_flags=
	
	while [ $# -gt 0 ];	do
	    case "$1" in
			-v)  verbose="V=1";;
	        -i)  arm_mode="$2"; shift;;
	        -a)  arm_arch="$2"; shift;;
			-l)  app_platform="$2"; shift;;
			--disable-tapjoy)  tapjoy_flags="-DDISABLE_TAPJOY";;
			--disable-google-push)  google_push_flags="-DDISABLE_NOTIFICATIONS";;
			--disable-google-billing)  google_push_flags="-DDISABLE_BILLING";;
			--disable-crittercism)  crittercism_flags="-DDISABLE_CRITTERCISM";;
			--disable-adcolony)  adcolony_flags="-DDISABLE_ADCOLONY";;
			-*)
		    	echo >&2 \
		    		$usage
		    	exit 1;;
			*)  break;;	# terminate while loop
	    esac
	    shift
	done

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

	should_clean=false

	# if libmoai already exists, find out which package it was build for
	if [ -f libs/package.txt ]; then
		existing_arm_mode=$( sed -n '1p' libs/package.txt )
		existing_arm_arch=$( sed -n '2p' libs/package.txt )
		existing_app_platform=$( sed -n '3p' libs/package.txt )
		existing_tapjoy_flags=$( sed -n '4p' libs/package.txt )
		existing_google_push_flags=$( sed -n '5p' libs/package.txt )
		existing_google_billing_flags=$( sed -n '6p' libs/package.txt )
		existing_crittercism_flags=$( sed -n '7p' libs/package.txt )
		existing_adcolony_flags=$( sed -n '8p' libs/package.txt )

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

		if [ x"$existing_google_push_flags" != x"$google_push_flags" ]; then
			should_clean=true
		fi

		if [ x"$existing_google_billing_flags" != x"$google_billing_flags" ]; then
			should_clean=true
		fi

		if [ x"$existing_crittercism_flags" != x"$crittercism_flags" ]; then
			should_clean=true
		fi

		if [ x"$existing_adcolony_flags" != x"$adcolony_flags" ]; then
			should_clean=true
		fi
	fi
	
	if [ x"$should_clean" = xtrue ]; then
		./clean.sh
	fi

	# echo message about what we are doing
	echo "Building libmoai.so for $arm_mode, $arm_arch, $app_platform"

	if [ x"$tapjoy_flags" != x ]; then
		echo "Tapjoy will be disabled"
	fi 

	if [ x"$google_push_flags" != x ]; then
		echo "Google Push Notifications will be disabled"
	fi 

	if [ x"$google_billing_flags" != x ]; then
		echo "Google Billing will be disabled"
	fi 

	if [ x"$crittercism_flags" != x ]; then
		echo "Crittercism will be disabled"
	fi 

	if [ x"$adcolony_flags" != x ]; then
		echo "AdColony will be disabled"
	fi 

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
		sed -i.backup s%@DISABLE_ADCOLONY@%"$adcolony_flags"%g OptionalComponentsDefined.mk
		sed -i.backup s%@DISABLE_BILLING@%"$google_billing_flags"%g OptionalComponentsDefined.mk
		sed -i.backup s%@DISABLE_CRITTERCISM@%"$crittercism_flags"%g OptionalComponentsDefined.mk
		sed -i.backup s%@DISABLE_NOTIFICATIONS@%"$google_push_flags"%g OptionalComponentsDefined.mk
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
	echo "$arm_mode" >> libs/package.txt
	echo "$arm_arch" >> libs/package.txt
	echo "$app_platform" >> libs/package.txt
	echo "$tapjoy_flags" >> libs/package.txt
	echo "$google_push_flags" >> libs/package.txt
	echo "$google_billing_flags" >> libs/package.txt
	echo "$crittercism_flags" >> libs/package.txt
	echo "$adcolony_flags" >> libs/package.txt