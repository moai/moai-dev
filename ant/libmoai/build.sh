#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e
	
	# check for command line switches
	usage="usage: $0 [-v] [-i thumb | arm] [-a all | armeabi | armeabi-v7a] [-l appPlatform] [--use-fmod true | false] [--use-untz true | false] [--disable-adcolony] [--disable-billing] [--disable-chartboost] [--disable-crittercism] [--disable-facebook] [--disable-push] [--disable-tapjoy]"
	verbose=
	arm_mode="arm"
	arm_arch="armeabi-v7a"
	app_platform="android-10"
	use_fmod="false"
	use_untz="true"
	adcolony_flags=
	billing_flags=
	chartboost_flags=
	crittercism_flags=
	facebook_flags=
	push_flags=
	tapjoy_flags=
	
	while [ $# -gt 0 ];	do
	    case "$1" in
			-v)  verbose="V=1";;
	        -i)  arm_mode="$2"; shift;;
	        -a)  arm_arch="$2"; shift;;
			-l)  app_platform="$2"; shift;;
			--use-fmod)  use_fmod="$2"; shift;;
			--use-untz)  use_untz="$2"; shift;;
			--disable-adcolony)  adcolony_flags="-DDISABLE_ADCOLONY";;
			--disable-billing)  billing_flags="-DDISABLE_BILLING";;
			--disable-chartboost)  chartboost_flags="-DDISABLE_CHARTBOOST";;
			--disable-crittercism)  crittercism_flags="-DDISABLE_CRITTERCISM";;
			--disable-facebook)  facebook_flags="-DDISABLE_FACEBOOK";;
			--disable-push)  push_flags="-DDISABLE_NOTIFICATIONS";;
			--disable-tapjoy)  tapjoy_flags="-DDISABLE_TAPJOY";;
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

	# TODO: Validate app_platform

	if [ x"$use_fmod" != xtrue ] && [ x"$use_fmod" != xfalse ]; then
		echo $usage
		exit 1		
	fi

	if [ x"$use_untz" != xtrue ] && [ x"$use_untz" != xfalse ]; then
		echo $usage
		exit 1		
	fi

	if [ x"$use_fmod" == xtrue ] && [ x"$FMOD_ANDROID_SDK_ROOT" == x ]; then
		echo "*** The FMOD SDK is not redistributed with the Moai SDK. Please download the FMOD EX"
		echo "*** Programmers API SDK from http://fmod.org and install it. Then ensure that the"
		echo "*** FMOD_ANDROID_SDK_ROOT environment variable is set and points to the root of the"
		echo "*** FMOD SDK installation; e.g., /FMOD/Android"
		exit 1		
	fi

	should_clean=false

	# if libmoai already exists, find out which package it was build for
	if [ -f libs/package.txt ]; then
		existing_arm_mode=$( sed -n '1p' libs/package.txt )
		existing_arm_arch=$( sed -n '2p' libs/package.txt )
		existing_app_platform=$( sed -n '3p' libs/package.txt )
		existing_use_fmod=$( sed -n '4p' libs/package.txt )
		existing_use_untz=$( sed -n '5p' libs/package.txt )
		existing_adcolony_flags=$( sed -n '6p' libs/package.txt )
		existing_billing_flags=$( sed -n '7p' libs/package.txt )
		existing_chartboost_flags=$( sed -n '8p' libs/package.txt )
		existing_crittercism_flags=$( sed -n '9p' libs/package.txt )
		existing_facebook_flags=$( sed -n '10p' libs/package.txt )
		existing_push_flags=$( sed -n '11p' libs/package.txt )
		existing_tapjoy_flags=$( sed -n '12p' libs/package.txt )

		if [ x"$existing_arm_mode" != x"$arm_mode" ]; then
			should_clean=true
		fi

		if [ x"$existing_arm_arch" != x"$arm_arch" ]; then
			should_clean=true
		fi

		if [ x"$existing_app_platform" != x"$app_platform" ]; then
			should_clean=true
		fi

		if [ x"$existing_use_fmod" != x"$use_fmod" ]; then
			should_clean=true
		fi

		if [ x"$existing_use_untz" != x"$use_untz" ]; then
			should_clean=true
		fi

		if [ x"$existing_adcolony_flags" != x"$adcolony_flags" ]; then
			should_clean=true
		fi

		if [ x"$existing_billing_flags" != x"$billing_flags" ]; then
			should_clean=true
		fi

		if [ x"$existing_chartboost_flags" != x"$chartboost_flags" ]; then
			should_clean=true
		fi

		if [ x"$existing_crittercism_flags" != x"$crittercism_flags" ]; then
			should_clean=true
		fi

		if [ x"$existing_facebook_flags" != x"$facebook_flags" ]; then
			should_clean=true
		fi

		if [ x"$existing_push_flags" != x"$push_flags" ]; then
			should_clean=true
		fi

		if [ x"$existing_tapjoy_flags" != x"$tapjoy_flags" ]; then
			should_clean=true
		fi
	fi
	
	if [ x"$should_clean" = xtrue ]; then
		./clean.sh
	fi

	# echo message about what we are doing
	echo "Building libmoai.so for $arm_mode, $arm_arch, $app_platform"

	if [ x"$use_fmod" != xtrue ]; then
		echo "FMOD will be disabled"
	fi 

	if [ x"$use_untz" != xtrue ]; then
		echo "UNTZ will be disabled"
	fi 

	if [ x"$adcolony_flags" != x ]; then
		echo "AdColony will be disabled"
	fi 

	if [ x"$billing_flags" != x ]; then
		echo "Billing will be disabled"
	fi 

	if [ x"$chartboost_flags" != x ]; then
		echo "ChartBoost will be disabled"
	fi 

	if [ x"$crittercism_flags" != x ]; then
		echo "Crittercism will be disabled"
	fi 

	if [ x"$facebook_flags" != x ]; then
		echo "Facebook will be disabled"
	fi 

	if [ x"$push_flags" != x ]; then
		echo "Push Notifications will be disabled"
	fi 

	if [ x"$tapjoy_flags" != x ]; then
		echo "Tapjoy will be disabled"
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
		sed -i.backup s%@DISABLE_BILLING@%"$billing_flags"%g OptionalComponentsDefined.mk
		sed -i.backup s%@DISABLE_CHARTBOOST@%"$chartboost_flags"%g OptionalComponentsDefined.mk
		sed -i.backup s%@DISABLE_CRITTERCISM@%"$crittercism_flags"%g OptionalComponentsDefined.mk
		sed -i.backup s%@DISABLE_FACEBOOK@%"$facebook_flags"%g OptionalComponentsDefined.mk
		sed -i.backup s%@DISABLE_NOTIFICATIONS@%"$push_flags"%g OptionalComponentsDefined.mk
		sed -i.backup s%@DISABLE_TAPJOY@%"$tapjoy_flags"%g OptionalComponentsDefined.mk
		sed -i.backup s%@USE_FMOD@%"$use_fmod"%g OptionalComponentsDefined.mk
		sed -i.backup s%@USE_UNTZ@%"$use_untz"%g OptionalComponentsDefined.mk
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
	rm -f libs/package.txt
	echo "$arm_mode" >> libs/package.txt
	echo "$arm_arch" >> libs/package.txt
	echo "$app_platform" >> libs/package.txt
	echo "$use_fmod" >> libs/package.txt
	echo "$use_untz" >> libs/package.txt
	echo "$adcolony_flags" >> libs/package.txt
	echo "$billing_flags" >> libs/package.txt
	echo "$chartboost_flags" >> libs/package.txt
	echo "$crittercism_flags" >> libs/package.txt
	echo "$facebook_flags" >> libs/package.txt
	echo "$push_flags" >> libs/package.txt
	echo "$tapjoy_flags" >> libs/package.txt