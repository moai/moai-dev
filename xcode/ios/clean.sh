#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e

usage="usage: $0 -j <jobName>"
job="default"

while [ $# -gt 0 ];	do
    case "$1" in
		-j)  job="$2"; shift;;
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

xcodebuild -configuration Debug -workspace MoaiSample.xcodeproj/project.xcworkspace -scheme moai -sdk iphonesimulator clean CONFIGURATION_BUILD_DIR=/tmp/ios/$job/MoaiSample/moai/iphonesimulator/Debug
xcodebuild -configuration Release -workspace MoaiSample.xcodeproj/project.xcworkspace -scheme moai -sdk iphonesimulator clean CONFIGURATION_BUILD_DIR=/tmp/ios/$job/MoaiSample/moai/iphonesimulator/Release

xcodebuild -configuration Debug -workspace MoaiSample.xcodeproj/project.xcworkspace -scheme moai -sdk iphoneos clean CONFIGURATION_BUILD_DIR=/tmp/ios/$job/MoaiSample/moai/iphoneos/Debug
xcodebuild -configuration Release -workspace MoaiSample.xcodeproj/project.xcworkspace -scheme moai -sdk iphoneos clean CONFIGURATION_BUILD_DIR=/tmp/ios/$job/MoaiSample/moai/iphoneos/Release