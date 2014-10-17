#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e

osx_schemes=( "libmoai-osx" "libmoai-osx-3rdparty" "libmoai-osx-fmod-ex" "libmoai-osx-luaext" "libmoai-osx-untz" "libmoai-osx-zlcore" )
osx_sdks=( "macosx" )

ios_schemes=( "libmoai-ios" "libmoai-ios-3rdparty" "libmoai-ios-facebook" "libmoai-ios-fmod-ex" "libmoai-ios-luaext" "libmoai-ios-tapjoy" "libmoai-ios-untz" "libmoai-ios-zlcore" )
ios_sdks=( "iphoneos" "iphonesimulator" )

usage="usage: $0 [-j <jobName>] [-c Debug|Release|all] [-p osx|ios|all]"
job="moai"
configurations="all"
platforms="all"

while [ $# -gt 0 ];	do
    case "$1" in
		-j)  job="$2"; shift;;
		-c)  configurations="$2"; shift;;
		-p)  platforms="$2"; shift;;
		-*)
	    	echo >&2 \
	    		$usage
	    	exit 1;;
		*)  break;;
    esac
    shift
done

if ! [[ $job =~ ^[a-zA-Z0-9_\-]+$ ]]; then
	echo -e "*** Illegal job name specified: $job..."
	echo -e "    > Job names may only contain letters, numbers, dashes and underscores"
	echo
	exit 1
fi

if [ x"$configurations" != xDebug ] && [ x"$configurations" != xRelease ] && [ x"$configurations" != xall ]; then
	echo $usage
	exit 1
elif [ x"$configurations" = xall ]; then
	configurations="Debug Release"
fi

if [ x"$platforms" != xosx ] && [ x"$platforms" != xios ] && [ x"$platforms" != xall ]; then
	echo $usage
	exit 1
elif [ x"$platforms" = xall ]; then
	platforms="osx ios"
fi

for platform in $platforms; do

	schemes=
	sdks=
	if [ x"$platform" = xosx ]; then
		schemes="${osx_schemes[@]}"
		sdks="${osx_sdks[@]}"
	elif [ x"$platform" = xios ]; then
		schemes="${ios_schemes[@]}"
		sdks="${ios_sdks[@]}"
	fi

	for config in $configurations; do
		for sdk in $sdks; do		
			for scheme in $schemes; do
				echo "Cleaning libmoai/$scheme/$sdk for $config"
				xcodebuild -configuration $config -workspace libmoai.xcodeproj/project.xcworkspace -scheme $scheme -sdk $sdk clean CONFIGURATION_BUILD_DIR=/tmp/$job/$platform/$scheme/$sdk/$config
				echo "Done"
			done
		done
		
		rm -rf "/tmp/$job/$platform/$config"
	done
done