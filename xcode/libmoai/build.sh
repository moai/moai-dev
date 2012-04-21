#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e

osx_schemes=( "libmoai-osx" "libmoai-osx-3rdparty" "libmoai-osx-luaext" "libmoai-osx-untz" "libmoai-osx-zipfs" )
osx_sdks=( "macosx" )

ios_schemes=( "libmoai-ios" "libmoai-ios-3rdparty" "libmoai-ios-adcolony" "libmoai-ios-chartboost" "libmoai-ios-facebook" "libmoai-ios-luaext" "libmoai-ios-tapjoy" "libmoai-ios-untz" "libmoai-ios-zipfs" )
ios_sdks=( "iphoneos" "iphonesimulator" )

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
				xcodebuild -configuration $config -workspace libmoai.xcodeproj/project.xcworkspace -scheme $scheme -sdk $sdk build CONFIGURATION_BUILD_DIR=/tmp/$platform/$job/libmoai/$scheme/$sdk/$config
				echo "Done"
			done
		done
	done
done

exit 1

cd /tmp/osx/beta/libmoai

mkdir -p Release
cp ./libmoai-osx/macosx/Release/libmoai-osx.a ./Release/libmoai-osx.a
cp ./libmoai-osx-3rdparty/macosx/Release/libmoai-osx-3rdparty.a ./Release/libmoai-osx-3rdparty.a
cp ./libmoai-osx-luaext/macosx/Release/libmoai-osx-luaext.a ./Release/libmoai-osx-luaext.a
cp ./libmoai-osx-untz/macosx/Release/libmoai-osx-untz.a ./Release/libmoai-osx-untz.a
cp ./libmoai-osx-zipfs/macosx/Release/libmoai-osx-zipfs.a ./Release/libmoai-osx-zipfs.a

cd /tmp/ios/beta/libmoai

mkdir -p Release-universal
lipo -create -output ./Release-universal/libmoai-ios.a ./libmoai-ios/iphonesimulator/Release/libmoai-ios.a ./libmoai-ios/iphoneos/Release/libmoai-ios.a
lipo -create -output ./Release-universal/libmoai-ios-3rdparty.a ./libmoai-ios-3rdparty/iphonesimulator/Release/libmoai-ios-3rdparty.a ./libmoai-ios-3rdparty/iphoneos/Release/libmoai-ios-3rdparty.a
lipo -create -output ./Release-universal/libmoai-ios-adcolony.a ./libmoai-ios-adcolony/iphonesimulator/Release/libmoai-ios-adcolony.a ./libmoai-ios-adcolony/iphoneos/Release/libmoai-ios-adcolony.a
lipo -create -output ./Release-universal/libmoai-ios-chartboost.a ./libmoai-ios-chartboost/iphonesimulator/Release/libmoai-ios-chartboost.a ./libmoai-ios-chartboost/iphoneos/Release/libmoai-ios-chartboost.a
lipo -create -output ./Release-universal/libmoai-ios-facebook.a ./libmoai-ios-facebook/iphonesimulator/Release/libmoai-ios-facebook.a ./libmoai-ios-facebook/iphoneos/Release/libmoai-ios-facebook.a
lipo -create -output ./Release-universal/libmoai-ios-luaext.a ./libmoai-ios-luaext/iphonesimulator/Release/libmoai-ios-luaext.a ./libmoai-ios-luaext/iphoneos/Release/libmoai-ios-luaext.a
lipo -create -output ./Release-universal/libmoai-ios-tapjoy.a ./libmoai-ios-tapjoy/iphonesimulator/Release/libmoai-ios-tapjoy.a ./libmoai-ios-tapjoy/iphoneos/Release/libmoai-ios-tapjoy.a
lipo -create -output ./Release-universal/libmoai-ios-untz.a ./libmoai-ios-untz/iphonesimulator/Release/libmoai-ios-untz.a ./libmoai-ios-untz/iphoneos/Release/libmoai-ios-untz.a
lipo -create -output ./Release-universal/libmoai-ios-zipfs.a ./libmoai-ios-zipfs/iphonesimulator/Release/libmoai-ios-zipfs.a ./libmoai-ios-zipfs/iphoneos/Release/libmoai-ios-zipfs.a

mkdir -p Release-armv6
lipo -thin armv6 -output ./Release-armv6/libmoai-ios.a ./Release-universal/libmoai-ios.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-3rdparty.a ./Release-universal/libmoai-ios-3rdparty.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-adcolony.a ./Release-universal/libmoai-ios-adcolony.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-chartboost.a ./Release-universal/libmoai-ios-chartboost.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-facebook.a ./Release-universal/libmoai-ios-facebook.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-luaext.a ./Release-universal/libmoai-ios-luaext.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-tapjoy.a ./Release-universal/libmoai-ios-tapjoy.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-untz.a ./Release-universal/libmoai-ios-untz.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-zipfs.a ./Release-universal/libmoai-ios-zipfs.a

mkdir -p Release-armv7
lipo -thin armv7 -output ./Release-armv7/libmoai-ios.a ./Release-universal/libmoai-ios.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-3rdparty.a ./Release-universal/libmoai-ios-3rdparty.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-adcolony.a ./Release-universal/libmoai-ios-adcolony.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-chartboost.a ./Release-universal/libmoai-ios-chartboost.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-facebook.a ./Release-universal/libmoai-ios-facebook.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-luaext.a ./Release-universal/libmoai-ios-luaext.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-tapjoy.a ./Release-universal/libmoai-ios-tapjoy.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-untz.a ./Release-universal/libmoai-ios-untz.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-zipfs.a ./Release-universal/libmoai-ios-zipfs.a

mkdir -p Release-i386
lipo -thin i386 -output ./Release-i386/libmoai-ios.a ./Release-universal/libmoai-ios.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-3rdparty.a ./Release-universal/libmoai-ios-3rdparty.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-adcolony.a ./Release-universal/libmoai-ios-adcolony.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-chartboost.a ./Release-universal/libmoai-ios-chartboost.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-facebook.a ./Release-universal/libmoai-ios-facebook.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-luaext.a ./Release-universal/libmoai-ios-luaext.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-tapjoy.a ./Release-universal/libmoai-ios-tapjoy.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-untz.a ./Release-universal/libmoai-ios-untz.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-zipfs.a ./Release-universal/libmoai-ios-zipfs.a