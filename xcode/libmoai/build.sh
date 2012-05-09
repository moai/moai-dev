#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e

osx_schemes=( "libmoai-osx" "libmoai-osx-3rdparty" "libmoai-osx-luaext" "libmoai-osx-untz" "libmoai-osx-zipfs" )
osx_sdks=( "macosx" )
osx_architectures=( "i386" )

ios_schemes=( "libmoai-ios" "libmoai-ios-3rdparty" "libmoai-ios-facebook" "libmoai-ios-luaext" "libmoai-ios-tapjoy" "libmoai-ios-untz" "libmoai-ios-zipfs" )
ios_sdks=( "iphoneos" "iphonesimulator" )
ios_architectures=( "i386" "armv6" "armv7" )

usage="usage: $0 [-j <jobName>] [-c Debug|Release|all] [-p osx|ios|all]"
job="default"
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
	architectures=
	if [ x"$platform" = xosx ]; then
		schemes="${osx_schemes[@]}"
		sdks="${osx_sdks[@]}"
		architectures="${osx_architectures[@]}"
	elif [ x"$platform" = xios ]; then
		schemes="${ios_schemes[@]}"
		sdks="${ios_sdks[@]}"
		architectures="${ios_architectures[@]}"
	fi

	for config in $configurations; do
		for sdk in $sdks; do		
			for scheme in $schemes; do
				echo "Building libmoai/$scheme/$sdk for $platform $config"
				xcodebuild -configuration $config -workspace libmoai.xcodeproj/project.xcworkspace -scheme $scheme -sdk $sdk build CONFIGURATION_BUILD_DIR=/tmp/$platform/$job/libmoai/$scheme/$sdk/$config
				echo "Done. Binaries available in /tmp/$platform/$job/libmoai/$scheme/$sdk/$config"
			done
		done
	done

	for config in $configurations; do
		rm -rf "/tmp/$platform/$job/libmoai/$config/universal"
		mkdir -p "/tmp/$platform/$job/libmoai/$config/universal"
		for scheme in $schemes; do
			libs=
			for sdk in $sdks; do
				libs="$libs /tmp/$platform/$job/libmoai/$scheme/$sdk/$config/$scheme.a"
			done
			lipo -create -output "/tmp/$platform/$job/libmoai/$config/universal/$scheme.a" $libs						
		done
	done

	for config in $configurations; do
		for arch in $architectures; do
			rm -rf "/tmp/$platform/$job/libmoai/$config/$arch"
			mkdir -p "/tmp/$platform/$job/libmoai/$config/$arch"
			for scheme in $schemes; do
				lipo -thin $arch -output "/tmp/$platform/$job/libmoai/$config/$arch/$scheme.a" "/tmp/$platform/$job/libmoai/$config/universal/$scheme.a"
			done
		done
	done
done