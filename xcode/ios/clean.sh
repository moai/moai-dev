#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e

usage="usage: $0 [-j <jobName>] [-c Debug|Release|All]"
job="default"
configuration="All"

while [ $# -gt 0 ];	do
    case "$1" in
		-j)  job="$2"; shift;;
		-c)  configuration="$2"; shift;;
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

if [ x"$configuration" != xDebug ] && [ x"$configuration" != xRelease ] && [ x"$configuration" != xAll ]; then
	echo $usage
	exit 1
elif [ x"$configuration" = xAll ]; then
	configuration="Debug Release"
fi

for config in $configuration; do
	echo "Cleaning MoaiSample/moai for $config"
	xcodebuild -configuration $configuration -workspace MoaiSample.xcodeproj/project.xcworkspace -scheme moai -sdk iphonesimulator clean CONFIGURATION_BUILD_DIR=/tmp/ios/$job/MoaiSample/moai/iphonesimulator/$configuration
	xcodebuild -configuration $configuration -workspace MoaiSample.xcodeproj/project.xcworkspace -scheme moai -sdk iphoneos clean CONFIGURATION_BUILD_DIR=/tmp/ios/$job/MoaiSample/moai/iphoneos/$configuration
	echo "Done"
done