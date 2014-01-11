#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e

   APP_NAME='Moai App'
   APP_ID='com.getmoai.moaiapp'
   APP_VERSION='1.0'


   # check for command line switches
	usage="usage: $0  \
        [--use-untz true | false] [--disable-adcolony] [--disable-billing] \
        [--disable-chartboost] [--disable-crittercism] [--disable-facebook] [--disable-push] [--disable-tapjoy] \
        [--disable-twitter] [--simulator] [--release]"
	
	use_untz="true"
	
	adcolony_flags=
	billing_flags=
	chartboost_flags=
	crittercism_flags=
	facebook_flags=
	push_flags=
	tapjoy_flags=
	twitter_flags=
	buildtype_flags="Debug"
    windows_flags=
	simulator="false"

	while [ $# -gt 1 ];	do
	    case "$1" in
			--use-untz)  use_untz="$2"; shift;;
			--disable-adcolony)  adcolony_flags="-DDISABLE_ADCOLONY";;
			--disable-billing)  billing_flags="-DDISABLE_BILLING";;
			--disable-chartboost)  chartboost_flags="-DDISABLE_CHARTBOOST";;
			--disable-crittercism)  crittercism_flags="-DDISABLE_CRITTERCISM";;
			--disable-facebook)  facebook_flags="-DDISABLE_FACEBOOK";;
			--disable-push)  push_flags="-DDISABLE_NOTIFICATIONS";;
			--disable-tapjoy)  tapjoy_flags="-DDISABLE_TAPJOY";;
			--disable-twitter)  twitter_flags="-DDISABLE_TWITTER";;
			--release) buildtype_flags="Release";;
			--simulator) simulator="true";;
			-*)
		    	echo >&2 \
		    		$usage
		    	exit 1;;
			*)  break;;	# terminate while loop
	    esac
	    shift
	done

LUASRC=$(ruby -e 'puts File.expand_path(ARGV.first)' "$1")

if [ ! -f "${LUASRC}/main.lua" ]; then
  echo "Could not find main.lua in specified lua source directory [${LUASRC}]"
  exit 1
fi




	if [ x"$use_untz" != xtrue ] && [ x"$use_untz" != xfalse ]; then
		echo $usage
		exit 1		
	fi

#get some required variables
XCODEPATH=$(xcode-select --print-path)

if [ x"$simulator" == xtrue ]; then
PLATFORM_PATH=${XCODEPATH}/Platforms/iPhoneSimulator.platform/Developer
PLATFORM=SIMULATOR
else
PLATFORM_PATH=${XCODEPATH}/Platforms/iPhone.platform/Developer
PLATFORM=OS
fi

SIGN_IDENTITY='iPhone Developer'



	# echo message about what we are doing
	echo "Building moai.app via CMAKE"
    
    disabled_ext=
        
    
    
	if [ x"$use_untz" != xtrue ]; then
		echo "UNTZ will be disabled"
		untz_param='-DMOAI_UNTZ=0'
	else
		untz_param='-DMOAI_UNTZ=1'
	fi 

	if [ x"$adcolony_flags" != x ]; then
		echo "AdColony will be disabled"
		disabled_ext="$disabled_extADCOLONY;"
	fi 

	if [ x"$billing_flags" != x ]; then
		echo "Billing will be disabled"
		disabled_ext="$disabled_extBILLING;"
	fi 

	if [ x"$chartboost_flags" != x ]; then
		echo "ChartBoost will be disabled"
		disabled_ext="$disabled_extCHARTBOOST;"
	fi 

	if [ x"$crittercism_flags" != x ]; then
		echo "Crittercism will be disabled"
		disabled_ext="$disabled_extCRITTERCISM;"
	fi 

	if [ x"$facebook_flags" != x ]; then
		echo "Facebook will be disabled"
		disabled_ext="$disabled_extFACEBOOK;"
	fi 

	if [ x"$push_flags" != x ]; then
		echo "Push Notifications will be disabled"
		disabled_ext="$disabled_extNOTIFICATIONS;"
	fi 

	if [ x"$tapjoy_flags" != x ]; then
		echo "Tapjoy will be disabled"
		disabled_ext="$disabled_extTAPJOY;"
	fi 
    
    if [ x"$twitter_flags" != x ]; then
		echo "Twitter will be disabled"
		disabled_ext="$disabled_extTWITTER;"
	fi 

build_dir=${PWD}

	 cd cmake
	 rm -rf build
	 mkdir build
	 cd build

     echo "Building resource list from ${LUASRC}"
     ruby ../host-ios/build_resources.rb "${LUASRC}"

     echo "Creating xcode project"

      #create our makefiles
      cmake -DDISABLED_EXT="$disabled_ext" -DMOAI_BOX2D=1 \
     -DMOAI_CHIPMUNK=1 -DMOAI_CURL=1 -DMOAI_CRYPTO=1 -DMOAI_EXPAT=1 -DMOAI_FREETYPE=1 \
     -DMOAI_HTTP_CLIENT=1 -DMOAI_JSON=1 -DMOAI_JPG=1 -DMOAI_LUAEXT=1 \
     -DMOAI_MONGOOSE=1 -DMOAI_OGG=1 -DMOAI_OPENSSL=1 -DMOAI_SQLITE3=1 \
     -DMOAI_TINYXML=1 -DMOAI_PNG=1 -DMOAI_SFMT=1 -DMOAI_VORBIS=1 $untz_param \
     -DBUILD_IOS=true \
     -DCMAKE_TOOLCHAIN_FILE="${PWD}/../host-ios/iOS.cmake" \
     -DIOS_PLATFORM=${PLATFORM}  \
     -DCMAKE_IOS_DEVELOPER_ROOT="${PLATFORM_PATH}" \
     -DSIGN_IDENTITY="${SIGN_IDENTITY}" \
     -DAPP_NAME="${APP_NAME}" \
     -DAPP_ID="${APP_ID}" \
     -DAPP_VERSION="${APP_VERSION}" \
     -DCMAKE_BUILD_TYPE=$buildtype_flags \
     -G "Xcode" \
      ../


      #build them
	  xcodebuild -target moai

    cd ${build_dir}

